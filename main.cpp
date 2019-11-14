// [build] https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html#org0a77d3a
// [SIMD Tutorial] http://www.cs.uu.nl/docs/vakken/magr/2017-2018/files/SIMD%20Tutorial.pdf
// [SIMD Instruction List (SSE to AVX512)] https://www.officedaytime.com/simd512e/simd.html
/*
 * https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html
 * 21.3.1 CPU Time Inquiry
 * To get a process’ CPU time, you can use the clock function. This facility is declared in the header file time.h.
 * 
 * In typical usage, you call the clock function at the beginning and end of the interval you want to time, subtract the values, and then divide by CLOCKS_PER_SEC (the number of clock ticks per second) to get processor time, like this:
 * 
 * #include <time.h>
 * 
 * clock_t start, end;
 * double cpu_time_used;
 * 
 * start = clock();
 * end = clock();
 * cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 * Do not use a single CPU time as an amount of time; it doesn’t work that way. Either do a subtraction as shown above or query processor time directly. See [Processor Time](https://www.gnu.org/software/libc/manual/html_node/Processor-Time.html#Processor-Time).
 * 
 * Different computers and operating systems vary wildly in how they keep track of CPU time. It’s common for the internal processor clock to have a resolution somewhere between a hundredth and millionth of a second.
 * 
 * Macro: int CLOCKS_PER_SEC
 * The value of this macro is the number of clock ticks per second measured by the clock function. POSIX requires that this value be one million independent of the actual resolution.
 * 
 * Data Type: clock_t
 * This is the type of the value returned by the clock function. Values of type clock_t are numbers of clock ticks.
 * 
 * Function: clock_t clock (void)
 * Preliminary: | MT-Safe | AS-Safe | AC-Safe | See [POSIX Safety Concepts](https://www.gnu.org/software/libc/manual/html_node/POSIX-Safety-Concepts.html#POSIX-Safety-Concepts).
 * 
 * This function returns the calling process’ current CPU time. If the CPU time is not available or cannot be represented, clock returns the value (clock_t)(-1).
**/
/*
 * https://www.gnu.org/software/libc/manual/html_node/Processor-Time.html#Processor-Time
 * The times function returns information about a process’ consumption of processor time in a struct tms object, in addition to the process’ CPU time. See Time Basics. You should include the header file sys/times.h to use this facility.

 * Data Type: struct tms
 * The tms structure is used to return information about process times. It contains at least the following members:
 * 
 * clock_t tms_utime
 * This is the total processor time the calling process has used in executing the instructions of its program.
 * 
 * clock_t tms_stime
 * This is the processor time the system has used on behalf of the calling process.
 * 
 * clock_t tms_cutime
 * This is the sum of the tms_utime values and the tms_cutime values of all terminated child processes of the calling process, whose status has been reported to the parent process by wait or waitpid; see [Process Completion](https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html#Process-Completion). In other words, it represents the total processor time used in executing the instructions of all the terminated child processes of the calling process, excluding child processes which have not yet been reported by wait or waitpid.
 * 
 * clock_t tms_cstime
 * This is similar to tms_cutime, but represents the total processor time the system has used on behalf of all the terminated child processes of the calling process.
 * 
 * All of the times are given in numbers of clock ticks. Unlike CPU time, these are the actual amounts of time; not relative to any event. See [Creating a Process](https://www.gnu.org/software/libc/manual/html_node/Creating-a-Process.html#Creating-a-Process).
 * 
 * Macro: int CLK_TCK
 * This is an obsolete name for the number of clock ticks per second. Use sysconf (_SC_CLK_TCK) instead.
 * 
 * Function: clock_t times (struct tms *buffer)
 * Preliminary: | MT-Safe | AS-Safe | AC-Safe | See [POSIX Safety Concepts](https://www.gnu.org/software/libc/manual/html_node/POSIX-Safety-Concepts.html#POSIX-Safety-Concepts).
 * 
 * The times function stores the processor time information for the calling process in buffer.
 * 
 * The return value is the number of clock ticks since an arbitrary point in the past, e.g. since system start-up. times returns (clock_t)(-1) to indicate failure.
 * 
 * Portability Note: The clock function described in CPU Time is specified by the ISO C standard. The times function is a feature of POSIX.1. On GNU systems, the CPU time is defined to be equivalent to the sum of the tms_utime and tms_stime fields returned by times.
**/

#include <iostream>
#include <iomanip>

#include <unistd.h>
#include <sys/times.h>

#include "asm.h"

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#include <memory>
static std::string demangle(const char* name) {
    auto demangledName = abi::__cxa_demangle(name, 0, 0, nullptr);
    auto result = std::string(demangledName);
    free(demangledName);
    return result;
}
#define DEMANGLE(CLASS) demangle(CLASS)
#elif defined(_MSC_VER)
#define DEMANGLE(CLASS) CLASS
#endif

using ClockType = decltype(times(nullptr));

struct tms start, end;
ClockType clock_start, clock_end;

#define START_TIME() \
clock_start = times(&start);

#define END_TIME() \
clock_end = times(&end);

#define DELTA_TIME() \
(double(end.tms_utime - start.tms_utime) / sysconf(_SC_CLK_TCK)) 

template<std::size_t SIZE>
class byte_size: public byte_size<SIZE-1> {
    uint8_t _;
};

template<>
class byte_size<0> {};

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

template<typename T, std::size_t size>
typename std::enable_if<std::is_integral<T>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    for (decltype(size) i = 0u; i < size; ++i) {
        output_buffer[i] = source_buffer[i];
    }
    END_TIME();
    return DELTA_TIME();
}

class MEMCPY: byte_size<1> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MEMCPY>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    std::memcpy(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class COPY: byte_size<1> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, COPY>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    std::copy(source_buffer, source_buffer + size, output_buffer);
    END_TIME();
    return DELTA_TIME();
}

class MOVSB: byte_size<1> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MOVSB>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    asm_copy_u8(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class MOVSW: byte_size<2> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MOVSW>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    asm_copy_u16(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class MOVSD: byte_size<4> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MOVSD>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    asm_copy_u32(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class MOVSQ: byte_size<8> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MOVSQ>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
    START_TIME();
    asm_copy_u64(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class MOVSDQA: byte_size<16> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, MOVSDQA>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {    
    START_TIME();
    asm_copy_u128(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class VMOVDQU_256: byte_size<32> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, VMOVDQU_256>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {    
    START_TIME();
    asm_copy_u256(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
}

class VMOVDQU_512: byte_size<64> {};
template<typename T, std::size_t size>
typename std::enable_if<std::is_same<T, VMOVDQU_512>::value, decltype(DELTA_TIME())>::type
test(T* source_buffer, T* output_buffer) {
#if __AVX512F__ == 1
    START_TIME();
    asm_copy_u512(output_buffer, source_buffer, size);
    END_TIME();
    return DELTA_TIME();
#else
    return -1;
#endif   
}

#define TEST(T, SIZE, SOURCE, OUTPUT) \
test<T, (SIZE) / sizeof(T)>(reinterpret_cast<T*>(SOURCE), reinterpret_cast<T*>(OUTPUT))

constexpr static std::size_t ONE_GB = 1 << 30;
static uint8_t* source_buffer = new uint8_t[ONE_GB];
static uint8_t* output_buffer = new uint8_t[ONE_GB];

template<typename T, std::size_t count_epoch = 100u>
void benchmark() {
    using delta_t = decltype(TEST(T, ONE_GB, source_buffer, output_buffer));
    delta_t max {-100000}, min {100000}, delta {0};

    for (auto i = 0u; i < count_epoch; ++i) {
        auto timediff = TEST(T, ONE_GB, source_buffer, output_buffer);
        max = std::max(max, timediff);
        min = std::min(min, timediff);
        delta += timediff;
    }

    std::cout << "[" << count_epoch << "][(" << DEMANGLE(typeid(T).name()) << ")/" << sizeof(T) << "](max, min, average): (" << max << "s, " << min << "s, " << delta / count_epoch << "s)" << std::endl;
}

int main() {
    benchmark<U8>();
    benchmark<U16>();
    benchmark<U32>();
    benchmark<U64>();
    benchmark<MEMCPY>();
    benchmark<COPY>();
    benchmark<MOVSB>();
    benchmark<MOVSW>();
    benchmark<MOVSD>();
    benchmark<MOVSQ>();
    benchmark<MOVSDQA>();
    benchmark<VMOVDQU_256>();
    benchmark<VMOVDQU_512>();

    return 0;
}

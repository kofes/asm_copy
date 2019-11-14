#pragma once

#include <cstdint>
#include <cstring>

extern "C" void asm_copy_u8(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u16(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u32(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u64(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u128(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u256(const void* output, const void* source, size_t size);
extern "C" void asm_copy_u512(const void* output, const void* source, size_t size);
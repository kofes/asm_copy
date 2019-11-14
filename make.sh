nasm -felf64 asm_copy.asm && \
$1 \
-DNDEBUG -O3 \
-ffunction-sections -fdata-sections -flto \
-finline-functions \
asm_copy.o main.cpp \
-o main
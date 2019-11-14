# Results
## Environment
CPU: Intel Core i7-8750H @ 12x 2.208GHz
Kernel: x86_64 Linux 4.4.0-18362-Microsoft
OS: Ubuntu 18.04 bionic [Ubuntu on Windows 10]
## clang [6.0.0-1ubuntu2]/NASM[2.13.02]
```
[100][(unsigned char)/1](max, min, average): (0.45s, 0.1s, 0.1215s)
[100][(unsigned short)/2](max, min, average): (0.15s, 0.1s, 0.1166s)
[100][(unsigned int)/4](max, min, average): (0.18s, 0.1s, 0.1187s)
[100][(unsigned long)/8](max, min, average): (0.14s, 0.1s, 0.1146s)
[100][(MEMCPY)/1](max, min, average): (0.13s, 0.07s, 0.0892s)
[100][(COPY)/1](max, min, average): (0.11s, 0.07s, 0.0865s)
[100][(MOVSB)/1](max, min, average): (0.15s, 0.12s, 0.1322s)
[100][(MOVSW)/2](max, min, average): (0.18s, 0.12s, 0.1398s)
[100][(MOVSD)/4](max, min, average): (0.19s, 0.12s, 0.1391s)
[100][(MOVSQ)/8](max, min, average): (0.15s, 0.12s, 0.1328s)
[100][(MOVSDQA)/16](max, min, average): (0.14s, 0.1s, 0.1155s)
[100][(VMOVDQU_256)/32](max, min, average): (0.14s, 0.1s, 0.1204s)
[100][(VMOVDQU_512)/64](max, min, average): (-1s, -1s, -1s)
```
## gcc [7.4.0]/NASM[2.13.02]
```
[100][(unsigned char)/1](max, min, average): (0.48s, 0.1s, 0.1173s)
[100][(unsigned short)/2](max, min, average): (0.13s, 0.1s, 0.1136s)
[100][(unsigned int)/4](max, min, average): (0.13s, 0.1s, 0.1147s)
[100][(unsigned long)/8](max, min, average): (0.13s, 0.09s, 0.1137s)
[100][(MEMCPY)/1](max, min, average): (0.1s, 0.07s, 0.0846s)
[100][(COPY)/1](max, min, average): (0.1s, 0.07s, 0.0853s)
[100][(MOVSB)/1](max, min, average): (0.16s, 0.12s, 0.1372s)
[100][(MOVSW)/2](max, min, average): (0.18s, 0.12s, 0.1432s)
[100][(MOVSD)/4](max, min, average): (0.19s, 0.12s, 0.1372s)
[100][(MOVSQ)/8](max, min, average): (0.17s, 0.12s, 0.1395s)
[100][(MOVSDQA)/16](max, min, average): (0.13s, 0.1s, 0.1136s)
[100][(VMOVDQU_256)/32](max, min, average): (0.14s, 0.1s, 0.1189s)
[100][(VMOVDQU_512)/64](max, min, average): (-1s, -1s, -1s)
```
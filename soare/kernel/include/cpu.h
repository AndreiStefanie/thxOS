#pragma once
#include "defs.h"

// CPU EFLAGS register values
#define CPU_EFLAGS_CARRY       (1 << 0)
#define CPU_EFLAGS_PARITY      (1 << 2)
#define CPU_EFLAGS_ADJUST      (1 << 4)
#define CPU_EFLAGS_ZERO        (1 << 6)
#define CPU_EFLAGS_SIGN        (1 << 7)
#define CPU_EFLAGS_TRAP        (1 << 8)
#define CPU_EFLAGS_INTERRUPT   (1 << 9)
#define CPU_EFLAGS_DIRECTION   (1 << 10)
#define CPU_EFLAGS_OVERFLOW    (1 << 11)
#define CPU_EFLAGS_IOPL1       (1 << 12)
#define CPU_EFLAGS_IOPL0       (1 << 13)
#define CPU_EFLAGS_NESTED      (1 << 14)
#define CPU_EFLAGS_RESUME      (1 << 16)
#define CPU_EFLAGS_V8086       (1 << 17)
#define CPU_EFLAGS_ALIGNCHECK  (1 << 18)
#define CPU_EFLAGS_VINTERRUPT  (1 << 19)
#define CPU_EFLAGS_VPENDING    (1 << 20)
#define CPU_EFLAGS_CPUID       (1 << 21)

#pragma pack(push, 1)
struct registers
{
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
};
#pragma pack(pop)

typedef struct registers registers_t;

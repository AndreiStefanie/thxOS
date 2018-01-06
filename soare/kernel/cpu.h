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
	uint64 rax;
	uint64 rbx;
	uint64 rcx;
	uint64 rdx;
	uint64 rsi;
	uint64 rdi;
	uint64 rbp;
	uint64 r8;
	uint64 r9;
	uint64 r10;
	uint64 r11;
	uint64 r12;
	uint64 r13;
	uint64 r14;
	uint64 r15;
};
#pragma pack(pop)

typedef struct registers registers_t;

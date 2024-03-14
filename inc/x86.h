#ifndef JOS_INC_X86_H
#define JOS_INC_X86_H

#include <inc/types.h>

// https://gcc.gnu.org/onlinedocs/gcc/Inline.html
// GCC does not inline any functions when not optimizing unless you specify the ‘always_inline’ attribute for the function
// If you don't use __attribute__((always_inline)) then when gcc -O0 will have hard-to-catch problem,
// e.g. in backtrace, we are depending on ebp to get the eip return address
// if read_ebp became a non-inline function, then the eip return address that is pushed on stack when calling read_ebp may be overwritten once read_ebp returns
// and if we use this garbage eip return address and perform stab_binsearch, garbage in garbage out!

static inline __attribute__((always_inline)) void
breakpoint(void)
{
	asm volatile("int3");
}

// in  addr, data
// out data, addr
// a for data, d for addr


// https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
// below is equivalent to inb dx, eax
static inline __attribute__((always_inline)) uint8_t
inb(int port)
{
	uint8_t data;
	asm volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data; // no need to do data & (0xFF) because inb is to receive one byte and data is of type uint8_t which will truncate the upper bytes of eax
}

static inline __attribute__((always_inline)) void
insb(int port, void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\tinsb"
		     : "=D" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "memory", "cc");
}

static inline __attribute__((always_inline)) uint16_t
inw(int port)
{
	uint16_t data;
	asm volatile("inw %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline __attribute__((always_inline)) void
insw(int port, void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\tinsw"
		     : "=D" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "memory", "cc");
}

static inline __attribute__((always_inline)) uint32_t
inl(int port)
{
	uint32_t data;
	asm volatile("inl %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline __attribute__((always_inline)) void
insl(int port, void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\tinsl"
		     : "=D" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "memory", "cc");
}

static inline __attribute__((always_inline)) void
outb(int port, uint8_t data)
{
	asm volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

static inline __attribute__((always_inline)) void
outsb(int port, const void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\toutsb"
		     : "=S" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "cc");
}

static inline __attribute__((always_inline)) void
outw(int port, uint16_t data)
{
	asm volatile("outw %0,%w1" : : "a" (data), "d" (port));
}

static inline __attribute__((always_inline)) void
outsw(int port, const void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\toutsw"
		     : "=S" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "cc");
}

static inline __attribute__((always_inline)) void
outsl(int port, const void *addr, int cnt)
{
	asm volatile("cld\n\trepne\n\toutsl"
		     : "=S" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "cc");
}

static inline __attribute__((always_inline)) void
outl(int port, uint32_t data)
{
	asm volatile("outl %0,%w1" : : "a" (data), "d" (port));
}

static inline __attribute__((always_inline)) void
invlpg(void *addr)
{
	asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
}

static inline __attribute__((always_inline)) void
lidt(void *p)
{
	asm volatile("lidt (%0)" : : "r" (p));
}

static inline __attribute__((always_inline)) void
lgdt(void *p)
{
	asm volatile("lgdt (%0)" : : "r" (p));
}

static inline __attribute__((always_inline)) void
lldt(uint16_t sel)
{
	asm volatile("lldt %0" : : "r" (sel));
}

static inline __attribute__((always_inline)) void
ltr(uint16_t sel)
{
	asm volatile("ltr %0" : : "r" (sel));
}

static inline __attribute__((always_inline)) void
lcr0(uint32_t val)
{
	asm volatile("movl %0,%%cr0" : : "r" (val));
}

static inline __attribute__((always_inline)) uint32_t
rcr0(void)
{
	uint32_t val;
	asm volatile("movl %%cr0,%0" : "=r" (val));
	return val;
}

static inline __attribute__((always_inline)) uint32_t
rcr2(void)
{
	uint32_t val;
	asm volatile("movl %%cr2,%0" : "=r" (val));
	return val;
}

static inline __attribute__((always_inline)) void
lcr3(uint32_t val)
{
	asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline __attribute__((always_inline)) uint32_t
rcr3(void)
{
	uint32_t val;
	asm volatile("movl %%cr3,%0" : "=r" (val));
	return val;
}

static inline __attribute__((always_inline)) void
lcr4(uint32_t val)
{
	asm volatile("movl %0,%%cr4" : : "r" (val));
}

static inline __attribute__((always_inline)) uint32_t
rcr4(void)
{
	uint32_t cr4;
	asm volatile("movl %%cr4,%0" : "=r" (cr4));
	return cr4;
}

static inline __attribute__((always_inline)) void
tlbflush(void)
{
	uint32_t cr3;
	asm volatile("movl %%cr3,%0" : "=r" (cr3));
	asm volatile("movl %0,%%cr3" : : "r" (cr3));
}

static inline __attribute__((always_inline)) uint32_t
read_eflags(void)
{
	uint32_t eflags;
	asm volatile("pushfl; popl %0" : "=r" (eflags));
	return eflags;
}

static inline __attribute__((always_inline)) void
write_eflags(uint32_t eflags)
{
	asm volatile("pushl %0; popfl" : : "r" (eflags));
}

// static inline uint32_t
static inline __attribute__((always_inline)) uint32_t
read_ebp(void)
{
	uint32_t ebp;
	asm volatile("movl %%ebp,%0" : "=r" (ebp));
	return ebp;
}

static inline __attribute__((always_inline)) uint32_t
read_esp(void)
{
	uint32_t esp;
	asm volatile("movl %%esp,%0" : "=r" (esp));
	return esp;
}

static inline __attribute__((always_inline)) void
cpuid(uint32_t info, uint32_t *eaxp, uint32_t *ebxp, uint32_t *ecxp, uint32_t *edxp)
{
	uint32_t eax, ebx, ecx, edx;
	asm volatile("cpuid"
		     : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
		     : "a" (info));
	if (eaxp)
		*eaxp = eax;
	if (ebxp)
		*ebxp = ebx;
	if (ecxp)
		*ecxp = ecx;
	if (edxp)
		*edxp = edx;
}

static inline __attribute__((always_inline)) uint64_t
read_tsc(void)
{
	uint64_t tsc;
	asm volatile("rdtsc" : "=A" (tsc));
	return tsc;
}

static inline __attribute__((always_inline)) uint32_t
xchg(volatile uint32_t *addr, uint32_t newval)
{
	uint32_t result;

	// The + in "+m" denotes a read-modify-write operand.
	asm volatile("lock; xchgl %0, %1"
		     : "+m" (*addr), "=a" (result)
		     : "1" (newval)
		     : "cc");
	return result;
}

#endif /* !JOS_INC_X86_H */

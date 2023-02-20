// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/kdebug.h>

#define CMDBUF_SIZE	80	// enough for one VGA text line


struct Command {
	const char *name;
	const char *desc;
	// return -1 to force monitor to exit
	int (*func)(int argc, char** argv, struct Trapframe* tf);
};

static struct Command commands[] = {
	{ "help", "Display this list of commands", mon_help },
	{ "kerninfo", "Display information about the kernel", mon_kerninfo },
	{ "backtrace", "Display backtrace info", mon_backtrace}
};

/***** Implementations of basic kernel monitor commands *****/

int
mon_help(int argc, char **argv, struct Trapframe *tf)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(commands); i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].desc);
	return 0;
}

int
mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
{
	extern char _start[], entry[], etext[], edata[], end[];

	cprintf("Special kernel symbols:\n");
	cprintf("  _start                  %08x (phys)\n", _start);
	cprintf("  entry  %08x (virt)  %08x (phys)\n", entry, entry - KERNBASE);
	cprintf("  etext  %08x (virt)  %08x (phys)\n", etext, etext - KERNBASE);
	cprintf("  edata  %08x (virt)  %08x (phys)\n", edata, edata - KERNBASE);
	cprintf("  end    %08x (virt)  %08x (phys)\n", end, end - KERNBASE);
	cprintf("Kernel executable memory footprint: %dKB\n",
		ROUNDUP(end - entry, 1024) / 1024);
	return 0;
}

int
mon_backtrace(int argc, char **argv, struct Trapframe *tf)
{
	// Your code here.
	uint32_t *ebp;
	struct Eipdebuginfo info;
	// ebp is the content of register %ebp
	// ebp holds the base esp of current frame (esp when we first arrive at the called function)
	// if we dereference ebp then we get the ebp of previous frame
	ebp = (uint32_t *) read_ebp(); 
	
	cprintf("Stack backtrace:\n");
	// cprintf("--> Address growth\n");
	// cprintf("<-- Stack growth\n");
	// cprintf("| %-10s ", "ebp");
	// cprintf("| %-10s ", "eip");
	// cprintf("| %-10s ", "args[0]");
	// cprintf("| %-10s ", "args[1]");
	// cprintf("| %-10s ", "args[2]");
	// cprintf("| %-10s ", "args[3]");
	// cprintf("| %-10s ", "args[4]");
	// cprintf("|\n");
	while (ebp != NULL)
	{
		// cprintf("| %#8x ", ebp);
		// for (int i=1; i<=6; i++)
		// {
		// 	cprintf("| %0#8x ", *(ebp+i));
		// }
		// cprintf("|\n");

		cprintf("  ebp %08x  eip %08x  args %08x %08x %08x %08x %08x\n", ebp, ebp[1], ebp[2], ebp[3], ebp[4], ebp[5], ebp[6]);
		
		// ebp[1] is the return address eip
		if (debuginfo_eip(ebp[1], &info) == 0){
			cprintf("\t%s:%d: %.*s+%x\n", info.eip_file, info.eip_line, info.eip_fn_namelen, info.eip_fn_name, ebp[1] - info.eip_fn_addr);
		} else {
			cprintf("Error in mon_backtrace: eip %#8x doesn't exist in debuginfo.\n", ebp[1]);
		}
		// cprintf("\n");
		ebp = (uint32_t *) *ebp;
	}

	return 0;
}



/***** Kernel monitor command interpreter *****/

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

// buf is a line of command
static int
runcmd(char *buf, struct Trapframe *tf)
{
	int argc;
	char *argv[MAXARGS]; // array of char*
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1) {
		// gobble whitespace
		while (*buf && strchr(WHITESPACE, *buf)) // if current character is not null and the && is to check if current character is WHITESPACEs
			*buf++ = 0; // if so, replace whitespace with null so that we don't consider it again in the next while loop
		if (*buf == 0) // null
			break;

		// save and scan past next arg
		if (argc == MAXARGS-1) { // minus one due to zero-indexing
			cprintf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf)) // skip non-WHITESPACE characters since we already took it in the line above
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0) // e.g. when you press enter or all whitespaces
		return 0;
	for (i = 0; i < ARRAY_SIZE(commands); i++) {
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv, tf);
	}
	cprintf("Unknown command '%s'\n", argv[0]);
	return 0;
}

void
monitor(struct Trapframe *tf)
{
	char *buf;

	cprintf("Welcome to the JOS kernel monitor!\n");
	cprintf("Type 'help' for a list of commands.\n");


	while (1) {
		// buf is a copy of the static buf used in readline.c 
		// (copy of static char buf[BUFLEN];)
		buf = readline("K> ");
		if (buf != NULL)
			if (runcmd(buf, tf) < 0)
				break;
	}
}

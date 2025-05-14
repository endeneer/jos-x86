# Assembly
- Intel syntax `instr dest, source`
- AT&T  syntax `instr source, dest`
## Inline assembly
- https://gcc.gnu.org/onlinedocs/gcc/Constraints.html
- https://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html#Simple-Constraints
- https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints
- https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#x86-Operand-Modifiers

# Makefile
## Automatic variables
- https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
- $@ name of target
- $< name of first prereq
- $^ all prereq
## override directive
We can override via command arguments in terminal (e.g. make CFLAGS=something)
- https://www.gnu.org/software/make/manual/html_node/Overriding.html  
But this means inside Makefile, any ordinary assignment (including +=) will be ignored.
That's why we need override directive, so that even when we have a overriding variable through command argument, we can still `override +=` to append to that command argument. `override =` will take over command argument which render command argument ineffective, so usually we use override for += only.
- https://www.gnu.org/software/make/manual/html_node/Override-Directive.html
## Assignment
- `=` is expanded when being used
- `:=` is immediate assignment, once assigned, won't change even if the variable it derives from changes later
## About the `.vars.%` target
The following target in `GNUMakefile` is a trick to cause recompilation upon changes in compiler flags.  
The cmp expect file1 and file2, where file 2 is passed via pipe in this case.  
cmp return non-zero if there is difference, then the command after `||` is executed.
```
$(OBJDIR)/.vars.%: FORCE
	$(V)echo "$($*)" | cmp -s $@ || echo "$($*)" > $@
.PRECIOUS: $(OBJDIR)/.vars.%
.PHONY: FORCE
```
`%` is passed to `$*`.  
e.g. If the target is `obj/.vars.KERN_CFLAGS`, then `$*` is `KERN_CFLAGS`.  
Then `$($*)` becomes `$(KERN_CFLAGS)`.  

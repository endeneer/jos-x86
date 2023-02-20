# Review
## QEMU Installation:
[https://www.cnblogs.com/gatsby123/p/9746193.html](https://www.cnblogs.com/gatsby123/p/9746193.html)  
[https://www.jianshu.com/p/3d6a9df84056](https://www.jianshu.com/p/3d6a9df84056)  
To quit qemu: In terminal, ctrl+a, then x
## QEMU Monitor:
To enter/exit monitor: In terminal, ctrl+a, then c
## QEMU GDB:
In a terminal:
```
make qemu-nox-gdb
```
In another terminal:
```
make gdb
```
### Examine memory at address
```
xp 0xf0100031
```
## GDB
### Breakpoint
#### Insert breakpoint at address (notice the unfortunate asterisk)
```
b *address
```
### Examine memory
#### Content of register
```
x/i $eax
```
#### At address (note: GDB can only access QEMU's memory by virtual address)
```
x/i 0xf0100031
```
#### At location pointed by address
```
x/i *0xf0100031
```

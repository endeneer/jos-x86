# MIT 6.828 JOS (x86)
Based on 2018 class (2019 onwards is using RISC-V).

# First time setup
## QEMU
Instead of using Ubuntu 16.04 (as suggested by the class), I've ported the 6.828-qemu repo, so that it can be built on Ubuntu 22.04:  
https://github.com/endeneer/6.828-qemu/tree/Ubuntu_22.04.2_LTS

For 6.828-qemu build and installation, you may use the bash script I've written in script/install.sh.  
Below shows the errors that I encountered while porting to Ubuntu 22.04.2 and how I fixed them.
### Solutions for build/installation errors on Ubuntu 22.04.2 LTS 
- Error:  
```
ERROR: Python not found. Use --python=/path/to/python
```
- Solution:  
```
./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list="i386-softmmu x86_64-softmmu" --python=/usr/bin/python2
```

- Error:  
```
  LINK  qemu-ga
/usr/bin/ld: qga/commands-posix.o: in function `dev_major_minor':
/home/user/git/worktree/personal/jos-x86/qemu/qga/commands-posix.c:634: undefined reference to `major'
/usr/bin/ld: /home/user/git/worktree/personal/jos-x86/qemu/qga/commands-posix.c:635: undefined reference to `minor'
collect2: error: ld returned 1 exit status
make: *** [Makefile:288: qemu-ga] Error 1
```
- Solution:  
```
// qga/commands-posix.c
#include <sys/sysmacros.h>
```

- Error:  
```
qemu/include/hw/xen/xen_common.h:113:9: error: unknown type name ‘xc_evtchn’
  113 | typedef xc_evtchn *XenEvtchn;
      |         ^~~~~~~~~
/home/user/git/worktree/personal/jos-x86/qemu/include/hw/xen/xen_common.h:114:9: error: unknown type name ‘xc_gnttab’
  114 | typedef xc_gnttab *XenGnttab;
      |         ^~~~~~~~~
```
- Solution:  
```
// include/hw/xen/xen_common.h
#include <xenctrl_compat.h>
// and also use make CFLAGS="-DXC_WANT_COMPAT_EVTCHN_API -DXC_WANT_COMPAT_GNTTAB_API"
```

- Error:  
```
qemu/include/glib-compat.h:19:10: fatal error: glib.h: No such file or directory
   19 | #include <glib.h>
      |          ^~~~~~~~
```
- Solution:  
```
sudo apt install libglib2.0-dev
sudo ln -s /usr/include/glib-2.0/glib.h /usr/include/glib.h
```

- Error:  
```
In file included from /home/user/git/worktree/personal/jos-x86/qemu/include/glib-compat.h:19,
                 from /home/user/git/worktree/personal/jos-x86/qemu/include/qemu-common.h:43,
                 from /home/user/git/worktree/personal/jos-x86/qemu/include/hw/hw.h:5,
                 from hw/block/xen_disk.c:38:
/usr/include/glib.h:30:10: fatal error: glib/galloca.h: No such file or directory
   30 | #include <glib/galloca.h>
      |          ^~~~~~~~~~~~~~~~
```
- Solution:  
```
sudo ln -s /usr/include/glib-2.0/glib /usr/include/glib

```

- Error:  
```
In file included from /usr/include/glib/galloca.h:32,
                 from /usr/include/glib.h:30,
                 from /home/user/git/worktree/personal/jos-x86/qemu/include/glib-compat.h:19,
                 from /home/user/git/worktree/personal/jos-x86/qemu/include/qemu-common.h:43,
                 from /home/user/git/worktree/personal/jos-x86/qemu/include/hw/hw.h:5,
                 from hw/block/xen_disk.c:38:
/usr/include/glib/gtypes.h:32:10: fatal error: glibconfig.h: No such file or directory
   32 | #include <glibconfig.h>
      |          ^~~~~~~~~~~~~~
```
- Solution:  
```
sudo ln -s /usr/lib/x86_64-linux-gnu/glib-2.0/include/glibconfig.h /usr/include/glibconfig.h
```

- Error:  
```
/home/user/git/worktree/personal/jos-x86/qemu/hw/i386/../xenpv/xen_domainbuild.c:237:10: error: too many arguments to function ‘xc_domain_create’
  237 |     rc = xc_domain_create(xen_xc, ssidref, uuid, flags, &xen_domid);
      |          ^~~~~~~~~~~~~~~~
```
- Solution:  
```
// hw/xenpv/xen_domainbuild.c
#include <xen/domctl.h>
...
    struct xen_domctl_createdomain config = {
	    .ssidref = ssidref,
	    .flags = flags,
	    .handle = uuid,
    };
    /* rc = xc_domain_create(xen_xc, ssidref, uuid, flags, &xen_domid); */
    rc = xc_domain_create(xen_xc, &xen_domid, &config);
...
```
- Reference:  
https://github.com/Xilinx/xen/commit/54ed251dc7b85565820019102e533afcea814e16

- Error:  
```
/usr/bin/ld: ../hw/9pfs/virtio-9p.o: in function `stat_to_v9stat':
virtio-9p.c:(.text+0x1e7a): undefined reference to `minor'
/usr/bin/ld: virtio-9p.c:(.text+0x1e91): undefined reference to `major'
/usr/bin/ld: ../hw/9pfs/virtio-9p.o: in function `v9fs_create':
virtio-9p.c:(.text+0x5ba7): undefined reference to `makedev'
/usr/bin/ld: ../hw/9pfs/virtio-9p.o: in function `v9fs_mknod':
virtio-9p.c:(.text+0x7ab8): undefined reference to `makedev'
```
- Solution:  
```
// hw/9pfs/virtio-9p.c
#include <sys/sysmacros.h>
```

- Error:  
```
/bin/sh: 1: msgfmt: not found
make[1]: *** [Makefile:13: de_DE.mo] Error 127
```
- Solution:  
```
sudo apt install gettext
```


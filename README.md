## DEV ENV

SW: Centos 6.0 32bit, Kernel version 2.6.32-71.el6.i686, 
    glibc-2.12-1.7.el6.i686, gcc-4.4.4-13.el6.i686

HW: PENTIUM(R) Dual-Core CPU E5700 @ 3.00GHZ


## TREE

```
multi-threaded-interface
|-- DOCUMENT
|-- include		header files
|   |-- mti.h           interface functions
|   |-- mtiimpl.h       other functions
|   |-- mtithread.h     wrap pthread
|   `-- util.h          deal with queue
|-- INSTALL
|-- makefile
|-- objects.mk
|-- README
|-- sources.mk
|-- src                 source files
|   |-- mti.c
|   |-- mtiimpl.c
|   |-- mtithread.c
|   |-- subdir.mk
|   `-- util.c
`-- test                test files
    |-- helloworld.c    
    |-- onetomany.c
    |-- pi.c
    |-- README
    `-- sendmessage.c
```

## USAGE

Compile your file & link MTI as follow:

```
gcc yoursourcefile -o yourtargetfile -lmti
```


## CONTACT


double.620(at)163.com

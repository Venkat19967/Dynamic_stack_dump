Readme:


I have included the patch file, Make file, readme, test application. I have not included the source code and header file separately for the sys call implementation.


Instructions to run:
1. Do the patching
2. Copy the image to sd card
3. Enable CONFIG_DYNAMIC_DUMP_STACK — I did that manually in .config file
4. Go to the folder with the test.c and Makefile
5. make
6. The test app name is “test”
7. To run type — ./test


The syscall numbers are 
359 and 360 for insdump and rmdump respectively

For testing purpose, I had another syscall implemented — sys_hello — syscall number: 361. This will just print hello world.


Sys_open was giving me issues as the kprobe was getting called many times unless I do rmdump to remove the kprobe. My assumptions is that when the OS is running many files are continuously opening and closing. Hence kprobe is getting hit many times. I had a small discussion regarding this with professor regarding this.

For testing all the use cases I have tried it with my user defined kernel symbol which is a syscall by itself and as I had mentioned, it will print hello world.

The test application is more specifically catered to one particular symbol — sys_hello

The following is the sample output of my program:





%%%%%%%%%%%%%%%%[  234.566528]
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

Dumpmode > 0 Testing the syscall

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list
[  234.597674]
Inside >1
Current pid:322
Actual pid:322
Current tgid:322
Actual tgid:322
[  234.600048] CPU: 0 PID: 322 Comm: test Not tainted 3.19.8-yocto-standard #1
[  234.600048] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  234.600048]  ce626cf8 ce626cf8 cd735efc c1453a01 cd735f2c c1253058 c15b0428 00000142
[  234.600048]  00000142 00000142 00000142 c15aff2b 0000032c ce626cf8 cd5dda80 cd735f74
[  234.600048]  cd735f44 c10a27d2 ce626d00 cd735f74 cd5dda80 c1252fe1 cd735f5c c1027fa4
[  234.600048] Call Trace:
[  234.600048]  [<c1453a01>] dump_stack+0x16/0x18
[  234.600048]  [<c1253058>] handler_pre+0x58/0xd0
[  234.600048]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  234.600048]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  234.600048]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  234.600048]  [<c1252fe0>] ? reciprocal_value+0x70/0x70
[  234.600048]  [<c1002964>] do_int3+0x44/0xa0
[  234.600048]  [<c1457f53>] int3+0x33/0x40
[  234.600048]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  234.600048]  [<c1457404>] ? syscall_call+0x7/0x7
[  234.693898] Hello world
[  234.697752]
Inside the rmdump definition
Removing kprobe of this dumpid:0
Return value from rmdump:1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

Dumpmode>0 testing if the dumpstack is working a new process --- It will not!

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list

[  239.737653]
Inside >1
Current pid:323
Actual pid:322
Current tgid:323
Actual tgid:322
[  239.740049] CPU: 0 PID: 323 Comm: test Not tainted 3.19.8-yocto-standard #1
[  239.740049] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  239.740049]  ce626038 ce626038 cd437efc c1453a01 cd437f2c c1253058 c15b0428 00000143
[  239.740049]  00000142 00000143 00000142 c15aff2b cd437fb4 ce626038 cd4b6780 cd437f74
[  239.740049]  cd437f44 c10a27d2 ce626040 cd437f74 cd4b6780 c1252fe1 cd437f5c c1027fa4
[  239.740049] Call Trace:
[  239.740049]  [<c1453a01>] dump_stack+0x16/0x18
[  239.740049]  [<c1253058>] handler_pre+0x58/0xd0
[  239.740049]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  239.740049]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  239.740049]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  239.740049]  [<c1252fe0>] ? reciprocal_value+0x70/0x70
[  239.740049]  [<c1002964>] do_int3+0x44/0xa0
[  239.740049]  [<c1457f53>] int3+0x33/0x40
[  239.740049]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  239.740049]  [<c1020060>] ? __mtrr_type_lookup.part.1+0x1f0/0x360
[  239.740049]  [<c1457404>] ? syscall_call+0x7/0x7
[  239.840185] Hello world
Inside the child process
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%[  249.737930]
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

Dumpmode>0  this test will show that rmdump cannot be removed from the child process and it is removed from the parent

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list

[  249.773035]
Inside >1
Current pid:324
Actual pid:322
Current tgid:324
Actual tgid:322
[  249.780083] CPU: 0 PID: 324 Comm: test Not tainted 3.19.8-yocto-standard #1
[  249.780083] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  249.780083]  ce626b78 ce626b78 cd7d5f18 c1453a01 cd7d5f48 c1253058 c15b0428 00000144
[  249.780083]  00000142 00000144 00000142 c15aff2b 00000000 ce626b78 cd4b6780 cd7d5f74
[  249.780083]  cd7d5f60 c10a2772 ce626b80 00000246 ffffffff bff69ff4 cd7d5f6c c102853a
[  249.780083] Call Trace:
[  249.780083]  [<c1453a01>] dump_stack+0x16/0x18
[  249.780083]  [<c1253058>] handler_pre+0x58/0xd0
[  249.780083]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  249.780083]  [<c102853a>] optimized_callback+0x5a/0x70
[  249.780083]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  249.780083]  [<c1457404>] ? syscall_call+0x7/0x7
[  249.780083]
Inside >1
Current pid:324
Actual pid:322
Current tgid:324
Actual tgid:322
[  249.780083] CPU: 0 PID: 324 Comm: test Not tainted 3.19.8-yocto-standard #1
[  249.780083] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  249.780083]  ce626038 ce626038 cd7d5f18 c1453a01 cd7d5f48 c1253058 c15b0428 00000144
[  249.780083]  00000142 00000144 00000142 c15aff2b 00000000 ce626038 cd4b6780 cd7d5f74
[  249.780083]  cd7d5f60 c10a2772 ce626040 00000246 ffffffff bff69ff4 cd7d5f6c c102853a
[  249.780083] Call Trace:
[  249.780083]  [<c1453a01>] dump_stack+0x16/0x18
[  249.780083]  [<c1253058>] handler_pre+0x58/0xd0
[  249.780083]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  249.780083]  [<c102853a>] optimized_callback+0x5a/0x70
[  249.780083]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  249.780083]  [<c1457404>] ? syscall_call+0x7/0x7
[  249.929093] Hello world
[  249.940136]
Inside the rmdump definition
Cannot remove probe with dumpid:2Inside the child process
Return value from rmdump:-1
*********It does not get removed here hence exiting process will remove the kprobe ************

[  254.769874]
[  254.769874] Inside the rmdump definition
Removing kprobe of this dumpid:2
Return value from rmdump:1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

Dumpmode=0  this test will show that a forked process cannot do dumpstack

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list

[  259.811859]
Cannot do dump_stack()
Current pid:325
Actual pid:322
Current tgid:325
Actual tgid:322
Inside >1
Current pid:325
Actual pid:322
Current tgid:325
Actual tgid:322
[  259.820079] CPU: 0 PID: 325 Comm: test Not tainted 3.19.8-yocto-standard #1
[  259.820079] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  259.820079]  ce626038 ce626038 ce757f18 c1453a01 ce757f48 c1253058 c15b0428 00000145
[  259.820079]  00000142 00000145 00000142 c15aff2b 00000000 ce626038 cd4b6780 ce757f74
[  259.820079]  ce757f60 c10a2772 ce626040 00000246 ffffffff bff69ff4 ce757f6c c102853a
[  259.820079] Call Trace:
[  259.820079]  [<c1453a01>] dump_stack+0x16/0x18
[  259.820079]  [<c1253058>] handler_pre+0x58/0xd0
[  259.820079]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  259.820079]  [<c102853a>] optimized_callback+0x5a/0x70
[  259.820079]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  259.820079]  [<c1457404>] ? syscall_call+0x7/0x7
[  259.898074] Hello world
Inside the child process
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%[  269.810918]
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

Dumpmode=1  this test will show that a forked process cannot do dumpstack

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list

[  274.843609]
Inside 1
Current pid:326
Actual pid:322
Current tgid:322
Actual tgid:322
[  274.851799] CPU: 0 PID: 326 Comm: test Not tainted 3.19.8-yocto-standard #1
[  274.853491] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  274.853491]  ce626638 ce626638 cd5abf18 c1453a01 cd5abf48 c1253058 c15b0428 00000146
[  274.853491]  00000142 00000142 00000142 c15aff40 0000c350 ce626638 cd4b6780 cd5abf74
[  274.853491]  cd5abf60 c10a2772 ce626640 00000246 b77e9390 b77ed298 cd5abf6c c102853a
[  274.853491] Call Trace:
[  274.853491]  [<c1453a01>] dump_stack+0x16/0x18
[  274.853491]  [<c1253058>] handler_pre+0x58/0xd0
[  274.853491]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  274.853491]  [<c102853a>] optimized_callback+0x5a/0x70
[  274.853491]  [<c1070000>] ? unmask_threaded_irq+0x30/0x40
[  274.853491]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  274.853491]  [<c1457404>] ? syscall_call+0x7/0x7
[  274.853491]
Cannot do dump_stack()
Current pid:326
Actual pid:322
Current tgid:322
Actual tgid:322
Inside >1
Current pid:326
Actual pid:322
Current tgid:322
Actual tgid:322
[  274.853491] CPU: 0 PID: 326 Comm: test Not tainted 3.19.8-yocto-standard #1
[  274.853491] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  274.853491]  ce626038 ce626038 cd5abf18 c1453a01 cd5abf48 c1253058 c15b0428 00000146
[  274.853491]  00000142 00000142 00000142 c15aff2b 0000c350 ce626038 cd4b6780 cd5abf74
[  274.853491]  cd5abf60 c10a2772 ce626040 00000246 b77e9390 b77ed298 cd5abf6c c102853a
[  274.853491] Call Trace:
[  274.853491]  [<c1453a01>] dump_stack+0x16/0x18
[  274.853491]  [<c1253058>] handler_pre+0x58/0xd0
[  274.853491]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  274.853491]  [<c102853a>] optimized_callback+0x5a/0x70
[  274.853491]  [<c1070000>] ? unmask_threaded_irq+0x30/0x40
[  274.853491]  [<c1252fe1>] ? sys_hello+0x1/0x20
[  274.853491]  [<c1457404>] ? syscall_call+0x7/0x7
[  275.018519] Hello world
[  280.030669]
Inside the rmdump definition
Removing kprobe of this dumpid:4Inside the thread
Return value from rmdump:1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%
Inside the insdump definition%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

the new kprobe is added in the list

This is the final exiting process that will remove all the kprobes:
[  286.068548]
[  286.068548] Removing the follwing dumpid:5
[  286.074288]
[  286.074288] Removing the follwing dumpid:3
[  286.079919]
[  286.079919] Removing the follwing dumpid:1
root@quark:~#






For which ever use case the probes were not removed, the final exiting main program is removing the kprobe via exit function ( I have made the call to this in do_exit in exit.c)



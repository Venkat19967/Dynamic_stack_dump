# Dynamic stack dump in Linux


Instructions to run:
1. Copy the patch file in the kernel source
2. Run `patch -p1 < patch.patch` at kernel source
3. Enable `CONFIG_DYNAMIC_DUMP_STACK` — I did that manually in .config file
4. Build the kernel
5. Copy the image to sd-card
6. Insert the sd-card to the Galileo board
7. Go to the folder with the test.c and Makefile
8. `make`
9. The test app name is “test”
10. To run type — `./test`


The syscall numbers are: `359` and `360` for insdump and rmdump respectively
For testing purpose, I had another syscall implemented — `sys_hello` — syscall number: `361`. This will just print hello world.
For testing all the use cases I have tried it with my user defined kernel symbol which is a syscall by itself and as I had mentioned, it will print hello world.
The test application is more specifically catered to one particular symbol — `sys_hello`
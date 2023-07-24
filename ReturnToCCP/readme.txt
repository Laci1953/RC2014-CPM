How to avoid CP/M re-boot when exiting a HiTech C program

The answer is mycrtcpm.as, as a substitute for HiTech's crtcpm

It saves the CCP's SP, launches the C program, then returns the control to CCP, instead of rebooting CP/M

See test.c for an example (use maketest.sub to build mytest.com)

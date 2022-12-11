Updated on 11 December 2022

Improvement for the OPTIM.COM for RC2014's provided with 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module).

Enables OPTIM.COM to process larger files, compared to the original version.

Works ONLY for systems provided with both:

- 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module)
- CP/M from the folder PutSys 

Best results are obtained with RC2014's provided with 64MB CF, due to larger TPA.

The executable is OPTIMNEW.COM

Current settings are made for SC108

You may change settings in myalloc.as, to choose SC114, SC118 or Phillip Stevens memory module.

Use -n option to get optimization statistics, including upper RAM usage

Examples:

part21.c 48KB

D>optim1 -n part21.as test.as
15K, 2 iterations
102 Redundant labels
149 Jumps to jumps
145 Stack adjustments
291 Temporary labels
224 Unref'ed labels
238 Unreachable code
21 Jumps to .+1
53 Skips over jumps
56 Common code seq's
4 Ex (sp),hl's used
35 Redundant operations
192 Redundant loads/stores
26 Simplified addresses
1 Xor a's used
5 Redundant ex de,hl's
19 Code motions
47 KB used from the upper 64KB RAM

part31.c 40KB

D>optim1 -n part31.as test.as
15K, 3 iterations
163 Redundant labels
225 Jumps to jumps
151 Stack adjustments
197 Temporary labels
254 Unref'ed labels
163 Unreachable code
16 Jumps to .+1
48 Skips over jumps
69 Common code seq's
6 Ex (sp),hl's used
8 Redundant operations
161 Redundant loads/stores
15 Simplified addresses
16 Code motions
33 KB used from the upper 64KB RAM



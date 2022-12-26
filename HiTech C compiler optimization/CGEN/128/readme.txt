Works ONLY for systems provided with both:

- 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module)
- CP/M from the folder PutSys 

Best results are obtained with RC2014's provided with 64MB CF, due to larger TPA.

The gain is significant, because all the objects allocated by CGEN are allocated in the upper 64 KB RAM, allowing more space for the rest of malloc's

Therefore, larger C source files can be processed, compared with the "original" CGEN.COM

Current settings are made for SC108

You may change settings in alloc128.as, to choose SC114, SC118 or Phillip Stevens memory module.

Use -n option to get upper RAM usage

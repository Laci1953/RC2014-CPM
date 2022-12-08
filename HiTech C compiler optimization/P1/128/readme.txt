(updated on 8 December 2022)

Improvement for the P1.COM for RC2014's provided with 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module).

Enables P1.COM to process larger files, compared to the original version.

It uses the upper 64 KB RAM to store data (instead of using malloc).

Works ONLY for systems provided with both:

- 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module)
- CP/M from the folder PutSys 

Best results are obtained with RC2014's provided with 64MB CF, due to larger TPA.

The executable is P1v128.COM

Current settings are made for SC108

You may change settings in p1alloc.as, to choose SC114, SC118 or Phillip Stevens memory module.

Use -n option to get the statistics about how much of the upper RAM was used.

While executing P1, the possible syntax errors are displayed using ONLY an error number; use the attached file to identify the issue. 

The SOURCES folder contains the source files.

Many thanks to Mark Ogden for recreating the C sources for P1.COM and to Andrey Nikitin for publishing these files; I used these files as a starting point.

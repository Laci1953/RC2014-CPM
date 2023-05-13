This folder contains the resources needed to boot CP/M from EPROM

This might not be necessary when you have an RC2014 provided with SCM, but it's a must if you want to use CP/M in a configuration NOT provided with SCM.

Example: you have a module containing the Z80 processor, a serial module (SIO, ACIA or KIO) and a memory module (64/128/512KB) NOT containing the SCM EPROM.

In this case, you might consider using my boot code, in order to directly boot CP/M at power-up.

The boot code initializes the serial and loads CP/M from the CF.

The booter .hex files must be burned on the memory module's EEPROM

For any RC2014's configurations using:

- Steve Cousins SC108, SC114, SC118, SC150, SC152 or
- Phillip Stevens's 32KB ROM+128KB RAM memory module (MM) or
- Spencer Owen's 512KB ROM + 512KB RAM memory board (M512)

, provided with serial boards using SIO or KIO or ACIA and 64/128MB CF

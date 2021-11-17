This folder contains the resources needed to boot CP/M from EPROM
The .hex files must be burned on the module's EEPROM

512 = 512KB RAM + 512KB ROM Spencer Owen memory module
SC108 = SC108 (CPU + 32KB ROM + 128KB RAM) Steve Cousins module
MM = 32KB ROM + 128KB RAM Phillip Stevens memory module

CF64 = large TPA CP/M (BDOS loads at 0DA00H, used only for 64MB CF's) (see PutSys folder)
CF128 = "classic" CP/M (BDOS loads at 0D000H, used for 64MB or 128MB CF's)

For any combination of RC2014's with 64MB CF configurations using:

- Steve Cousins SC108 or
- Phillip Stevens's 32KB ROM+128KB RAM memory module (MM) or
- Spencer Owen's 512KB ROM + 512KB RAM memory board (M512)

, provided with serial boards using SIO or KIO or ACIA , you may now use an enhanced CP/M and burn a CP/M booter to start directly with CP/M from power-on.

This booter contains also code that allows to use the TE text editor on SC108 or MM, without SCM (the M512's TE does not have this constraint...)

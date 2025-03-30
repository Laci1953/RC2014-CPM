Digital I/O module based debugger

;	At start, leds will display 1111 0000, to show that you may enter a new address (or return)
;	Then, press button 7 to set a new address, leds will display 0001 0000, or
;		press button 0 to return
;	Then, set a new address, by feeding in the address nibbles, using buttons 0 to 3, 
;		(leds will display also the bit being fed-in) in the following order:
;			low byte low nibble, then press button 7, leds will display 0010 0000
;			low byte high nibble, then press button 7, leds will display 0100 0000
;			high byte low nibble, then press button 7, leds will display 1000 0000
;			high byte high nibble, then press button 7, leds will display 1111 0000
;	Then, 
;		press button 6 to display byte @address and wait next command, or
;		press button 5 to increment address & display byte @address and wait next command, or
;		press button 4 to decrement address & display byte @address and wait next command, or
;		press button 1 to set a breakpoint @address and return, or
;		press button 0 to return
;
;	After a breakpoint is reached, registers (AF,BC,DE,HL,AF',BC',DE',HL',IX,IY,SP,PC) are stored at 8H
;	leds will display 1111 0000, then you may enter a new address or return
;

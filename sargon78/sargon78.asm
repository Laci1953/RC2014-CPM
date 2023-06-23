;***********************************************************
;
;               SARGON
;
;       Sargon is a computer chess playing program designed
; and coded by Dan and Kathe Spracklen.  Copyright 1978. All
; rights reserved.  No part of this publication may be
; reproduced without the prior written permission.
;***********************************************************

    ORG 0100H               ; The standard location to start a CP/M program.
    
    jp DRIVER               ; Jump over all the tables to the start of the program.

;***********************************************************
; EQUATES
;***********************************************************
;
PAWN    EQU     1
KNIGHT  EQU     2
BISHOP  EQU     3
ROOK    EQU     4
QUEEN   EQU     5
KING    EQU     6
WHITE   EQU     0
BLACK   EQU     80H
BPAWN   EQU     BLACK+PAWN

;***********************************************************
; TABLES SECTION
;***********************************************************
START equ 0200H

        ORG     START+80H
TBASE   EQU     START+100H

;There are multiple tables used for fast table look ups
;that are declared relative to TBASE. In each case there
;is a table (say DIRECT) and one or more variables that
;index into the table (say INDX2). The table is declared
;as a relative offset from the TBASE like this;
;
;DIRECT = .-TBASE  ;In this . is the current location
;                  ;($ rather than . is used in most assemblers)
;
;The index variable is declared as;
;INDX2    .WORD TBASE
;
;TBASE itself is page aligned, for example TBASE = 100h
;Although 2 bytes are allocated for INDX2 the most significant
;never changes (so in our example it's 01h). If we want
;to index 5 bytes into DIRECT we set the low byte of INDX2
;to 5 (now INDX2 = 105h) and load IDX2 into an index
;register. The following sequence loads register C with
;the 5th byte of the DIRECT table (Z80 mnemonics)
;        LD      A,5
;        LD      [INDX2],A
;        LD      IY,INDX2
;        LD      C,[IY+DIRECT]
;
;It's a bit like the little known C trick where array[5]
;can also be written as 5[array].
;
;The Z80 indexed addressing mode uses a signed 8 bit
;displacement offset (here DIRECT) in the range -128
;to 127. Sargon needs most of this range, which explains
;why DIRECT is allocated 80h bytes after start and 80h
;bytes *before* TBASE, this arrangement sets the DIRECT
;displacement to be -80h bytes (-128 bytes). After the 24
;byte DIRECT table comes the DPOINT table. So the DPOINT
;displacement is -128 + 24 = -104. The final tables have
;positive displacements.
;
;The negative displacements are not necessary in X86 where
;the equivalent mov reg,[di+offset] indexed addressing
;is not limited to 8 bit offsets, so in the X86 port we
;put the first table DIRECT at the same address as TBASE,
;a more natural arrangement I am sure you'll agree.
;
;In general it seems Sargon doesn't want memory allocated
;in the first page of memory, so we start TBASE at 100h not
;at 0h. One reason is that Sargon extensively uses a trick
;to test for a NULL pointer; it tests whether the hi byte of
;a pointer == 0 considers this as a equivalent to testing
;whether the whole pointer == 0 (works as long as pointers
;never point to page 0).
;
;Also there is an apparent bug in Sargon, such that MLPTRJ
;is left at 0 for the root node and the MLVAL for that root
;node is therefore written to memory at offset 5 from 0 (so
;in page 0). It's a bit wasteful to waste a whole 256 byte
;page for this, but it is compatible with the goal of making
;as few changes as possible to the inner heart of Sargon.
;In the X86 port we lock the uninitialised MLPTRJ bug down
;so MLPTRJ is always set to zero and rendering the bug
;harmless (search for MLPTRJ to find the relevant code).

;**********************************************************
; DIRECT  --  Direction Table.  Used to determine the dir-
;             ection of movement of each piece.
;***********************************************************
DIRECT  EQU     080H ; -128 ; $-TBASE             ;  -128
        DB      +09,+11,-11,-09
        DB      +10,-10,+01,-01
        DB      -21,-12,+08,+19
        DB      +21,+12,-08,-19
        DB      +10,+10,+11,+09
        DB      -10,-10,-11,-09
;***********************************************************
; DPOINT  --  Direction Table Pointer. Used to determine
;             where to begin in the direction table for any
;             given piece.
;***********************************************************
DPOINT  EQU     098H ; -104 ; $-TBASE             ; -104
        DB      20,16,8,0,4,0,0

;***********************************************************
; DCOUNT  --  Direction Table Counter. Used to determine
;             the number of directions of movement for any
;             given piece.
;***********************************************************
DCOUNT  EQU     09FH ; -97 ; $-TBASE             ; -97
        DB      4,4,8,4,4,8,8

;***********************************************************
; PVALUE  --  Point Value. Gives the point value of each
;             piece, or the worth of each piece.
;***********************************************************
PVALUE  EQU     0A5H ; -91 ; $-TBASE-1           ; -91
        DB      1,3,3,5,9,10

;***********************************************************
; PIECES  --  The initial arrangement of the first rank of
;             pieces on the board. Use to set up the board
;             for the start of the game.
;***********************************************************
PIECES  EQU    0ACH ;  -84 ; $-TBASE             ; -84
        DB      4,2,3,5,6,3,2,4

;***********************************************************
; BOARD   --  Board Array.  Used to hold the current position
;             of the board during play. The board itself
;             looks like:
;             FFFFFFFFFFFFFFFFFFFF
;             FFFFFFFFFFFFFFFFFFFF
;             FF0402030506030204FF
;             FF0101010101010101FF
;             FF0000000000000000FF
;             FF0000000000000000FF
;             FF0000000000000060FF
;             FF0000000000000000FF
;             FF8181818181818181FF
;             FF8482838586838284FF
;             FFFFFFFFFFFFFFFFFFFF
;             FFFFFFFFFFFFFFFFFFFF
;             The values of FF form the border of the
;             board, and are used to indicate when a piece
;             moves off the board. The individual bits of
;             the other bytes in the board array are as
;             follows:
;             Bit 7 -- Color of the piece
;                     1 -- Black
;                     0 -- White
;             Bit 6 -- Not used
;             Bit 5 -- Not used
;             Bit 4 --Castle flag for Kings only
;             Bit 3 -- Piece has moved flag
;             Bits 2-0 Piece type
;                     1 -- Pawn
;                     2 -- Knight
;                     3 -- Bishop
;                     4 -- Rook
;                     5 -- Queen
;                     6 -- King
;                     7 -- Not used     (actually this is "mated-king")
;                     0 -- Empty Square
;***********************************************************
BOARD   EQU     0B4H ; -76 ; $-TBASE             ; -76
BOARDA:  DS      120

;***********************************************************
; ATKLIST -- Attack List. A two part array, the first
;            half for white and the second half for black.
;            It is used to hold the attackers of any given
;            square in the order of their value.
;
; WACT   --  White Attack Count. This is the first
;            byte of the array and tells how many pieces are
;            in the white portion of the attack list.
;
; BACT   --  Black Attack Count. This is the eighth byte of
;            the array and does the same for black.
;***********************************************************
ATKLST:  DW      0,0,0,0,0,0,0
WACT    EQU     ATKLST
BACT    EQU     ATKLST+7

;***********************************************************
; PLIST   --  Pinned Piece Array. This is a two part array.
;             PLISTA contains the pinned piece position.
;             PLISTD contains the direction from the pinned
;             piece to the attacker.
;***********************************************************
PLIST   EQU     $-TBASE-1
PLISTD  EQU     PLIST+10
PLISTA:  DW      0,0,0,0,0,0,0,0,0,0

;***********************************************************
; POSK    --  Position of Kings. A two byte area, the first
;             byte of which hold the position of the white
;             king and the second holding the position of
;             the black king.
;
; POSQ    --  Position of Queens. Like POSK,but for queens.
;***********************************************************
POSK:    DB      24,95
POSQ:    DB      14,94
        DB      -1

;***********************************************************
; SCORE   --  Score Array. Used during Alpha-Beta pruning to
;             hold the scores at each ply. It includes two
;             "dummy" entries for ply -1 and ply 0.
;***********************************************************
SCORE:   DW      0,0,0,0,0,0     ;Z80 max 6 ply

;***********************************************************
; STACK   --  Contains the stack for the program.
;***********************************************************
        ORG     START+2FFH
STACK:

;***********************************************************
; TABLE INDICES SECTION
;
; M1-M4   --  Working indices used to index into
;             the board array.
;
; T1-T3   --  Working indices used to index into Direction
;             Count, Direction Value, and Piece Value tables.
;
; INDX1   --  General working indices. Used for various
; INDX2       purposes.
;
; NPINS   --  Number of Pins. Count and pointer into the
;             pinned piece list.
;
; MLPTRI  --  Pointer into the ply table which tells
;             which pair of pointers are in current use.
;
; MLPTRJ  --  Pointer into the move list to the move that is
;             currently being processed.
;
; SCRIX   --  Score Index. Pointer to the score table for
;             the ply being examined.
;
; BESTM   --  Pointer into the move list for the move that
;             is currently considered the best by the
;             Alpha-Beta pruning process.
;
; MLLST   --  Pointer to the previous move placed in the move
;             list. Used during generation of the move list.
;
; MLNXT   --  Pointer to the next available space in the move
;             list.
;
;***********************************************************
        ORG     START+0
M1:      DW      TBASE
M2:      DW      TBASE
M3:      DW      TBASE
M4:      DW      TBASE
T1:      DW      TBASE
T2:      DW      TBASE
T3:      DW      TBASE
INDX1:   DW      TBASE
INDX2:   DW      TBASE
NPINS:   DW      TBASE
MLPTRI:  DW      PLYIX
MLPTRJ:  DW      0
SCRIX:   DW      0
BESTM:   DW      0
MLLST:   DW      0
MLNXT:   DW      MLIST

;***********************************************************
; VARIABLES SECTION
;
; KOLOR   --  Indicates computer's color. White is 0, and
;             Black is 80H.
;
; COLOR   --  Indicates color of the side with the move.
;
; P1-P3   --  Working area to hold the contents of the board
;             array for a given square.
;
; PMATE   --  The move number at which a checkmate is
;             discovered during look ahead.
;
; MOVENO  --  Current move number.
;
; PLYMAX  --  Maximum depth of search using Alpha-Beta
;             pruning.
;
; NPLY    --  Current ply number during Alpha-Beta
;             pruning.
;
; CKFLG   --  A non-zero value indicates the king is in check.
;
; MATEF   --  A zero value indicates no legal moves.
;
; VALM    --  The score of the current move being examined.
;
; BRDC    --  A measure of mobility equal to the total number
;             of squares white can move to minus the number
;             black can move to.
;
; PTSL    --  The maximum number of points which could be lost
;             through an exchange by the player not on the
;             move.
;
; PTSW1   --  The maximum number of points which could be won
;             through an exchange by the player not on the
;             move.
;
; PTSW2   --  The second highest number of points which could
;             be won through a different exchange by the player
;             not on the move.
;
; MTRL    --  A measure of the difference in material
;             currently on the board. It is the total value of
;             the white pieces minus the total value of the
;             black pieces.
;
; BC0     --  The value of board control(BRDC) at ply 0.
;
; MV0     --  The value of material(MTRL) at ply 0.
;
; PTSCK   --  A non-zero value indicates that the piece has
;             just moved itself into a losing exchange of
;             material.
;
; BMOVES  --  Our very tiny book of openings. Determines
;             the first move for the computer.
;
;***********************************************************
KOLOR:   DB      0
COLOR:   DB      0
P1:      DB      0
P2:      DB      0
P3:      DB      0
PMATE:   DB      0
MOVENO:  DB      0
PLYMAX:  DB      2
NPLY:    DB      0
CKFLG:   DB      0
MATEF:   DB      0
VALM:    DB      0
BRDC:    DB      0
PTSL:    DB      0
PTSW1:   DB      0
PTSW2:   DB      0
MTRL:    DB      0
BC0:     DB      0
MV0:     DB      0
PTSCK:   DB      0
BMOVES:  DB      35,55,10H
        DB      34,54,10H
        DB      85,65,10H
        DB      84,64,10H

;***********************************************************
; MOVE LIST SECTION
;
; MLIST   --  A 2048 byte storage area for generated moves.
;             This area must be large enough to hold all
;             the moves for a single leg of the move tree.
;
; MLEND   --  The address of the last available location
;             in the move list.
;
; MLPTR   --  The Move List is a linked list of individual
;             moves each of which is 6 bytes in length. The
;             move list pointer(MLPTR) is the link field
;             within a move.
;
; MLFRP   --  The field in the move entry which gives the
;             board position from which the piece is moving.
;
; MLTOP   --  The field in the move entry which gives the
;             board position to which the piece is moving.
;
; MLFLG   --  A field in the move entry which contains flag
;             information. The meaning of each bit is as
;             follows:
;             Bit 7  --  The color of any captured piece
;                        0 -- White
;                        1 -- Black
;             Bit 6  --  Double move flag (set for castling and
;                        en passant pawn captures)
;             Bit 5  --  Pawn Promotion flag; set when pawn
;                        promotes.
;             Bit 4  --  When set, this flag indicates that
;                        this is the first move for the
;                        piece on the move.
;             Bit 3  --  This flag is set is there is a piece
;                        captured, and that piece has moved at
;                        least once.
;             Bits 2-0   Describe the captured piece.  A
;                        zero value indicates no capture.
;
; MLVAL   --  The field in the move entry which contains the
;             score assigned to the move.
;
;***********************************************************
        ORG     START+300H
MLIST:   DS      2048
MLEND   EQU     MLIST+2040
MLPTR   EQU     0
MLFRP   EQU     2
MLTOP   EQU     3
MLFLG   EQU     4
MLVAL   EQU     5

;***********************************************************

;***********************************************************
; PLYIX   --  Ply Table. Contains pairs of pointers, a pair
;             for each ply. The first pointer points to the
;             top of the list of possible moves at that ply.
;             The second pointer points to which move in the
;             list is the one currently being considered.
;***********************************************************
PLYIX:   DW      0,0,0,0,0,0,0,0,0,0
        DW      0,0,0,0,0,0,0,0,0,0

;**********************************************************
; PROGRAM CODE SECTION
;**********************************************************

;**********************************************************
; BOARD SETUP ROUTINE
;***********************************************************
; FUNCTION:   To initialize the board array, setting the
;             pieces in their initial positions for the
;             start of the game.
;
; CALLED BY:  DRIVER
;
; CALLS:      None
;
; ARGUMENTS:  None
;***********************************************************
INITBD: LD      b,120           ; Pre-fill board with -1's
        LD      hl,BOARDA
back01: LD      (hl),-1
        INC     hl
        DJNZ    back01
        LD      b,8
        LD      ix,BOARDA
IB2:    LD      a,(ix-8)        ; Fill non-border squares
        LD      (ix+21),a       ; White pieces
        SET     7,a             ; Change to black
        LD      (ix+91),a       ; Black pieces
        LD      (ix+31),PAWN    ; White Pawns
        LD      (ix+81),BPAWN   ; Black Pawns
        LD      (ix+41),0       ; Empty squares
        LD      (ix+51),0
        LD      (ix+61),0
        LD      (ix+71),0
        INC     ix
        DJNZ    IB2
        LD      ix,POSK         ; Init King/Queen position list
        LD      (ix+0),25
        LD      (ix+1),95
        LD      (ix+2),24
        LD      (ix+3),94
        RET

;***********************************************************
; PATH ROUTINE
;***********************************************************
; FUNCTION:   To generate a single possible move for a given
;             piece along its current path of motion including:

;                Fetching the contents of the board at the new
;                position, and setting a flag describing the
;                contents:
;                          0  --  New position is empty
;                          1  --  Encountered a piece of the
;                                 opposite color
;                          2  --  Encountered a piece of the
;                                 same color
;                          3  --  New position is off the
;                                 board
;
; CALLED BY:  MPIECE
;             ATTACK
;             PINFND
;
; CALLS:      None
;
; ARGUMENTS:  Direction from the direction array giving the
;             constant to be added for the new position.
;***********************************************************

PATH:   LD      hl,M2           ; Get previous position
        LD      a,(hl)
        ADD     a,c             ; Add direction constant
        LD      (hl),a          ; Save new position
        LD      ix,(M2)         ; Load board index
        LD      a,(ix+BOARD)    ; Get contents of board
        CP      -1              ; In border area ?
        JR      Z,PA2           ; Yes - jump
        LD      (P2),a          ; Save piece
        AND     7               ; Clear flags
        LD      (T2),a          ; Save piece type
        RET     Z               ; Return if empty
        LD      a,(P2)          ; Get piece encountered
        LD      hl,P1           ; Get moving piece address
        XOR     (hl)            ; Compare
        BIT     7,a             ; Do colors match ?
        JR      Z,PA1           ; Yes - jump
        LD      a,1             ; Set different color flag
        RET                     ; Return
PA1:    LD      a,2             ; Set same color flag
        RET                     ; Return
PA2:    LD      a,3             ; Set off board flag
        RET                     ; Return

;***********************************************************
; PIECE MOVER ROUTINE
;***********************************************************
; FUNCTION:   To generate all the possible legal moves for a
;             given piece.
;
; CALLED BY:  GENMOV
;
; CALLS:      PATH
;             ADMOVE
;             CASTLE
;             ENPSNT
;
; ARGUMENTS:  The piece to be moved.
;***********************************************************
MPIECE: XOR     (hl)            ; Piece to move
        AND     87H             ; Clear flag bit
        CP      BPAWN           ; Is it a black Pawn ?
        JR      NZ,rel001       ; No-Skip
        DEC     a               ; Decrement for black Pawns
rel001: AND     7               ; Get piece type
        LD      (T1),a          ; Save piece type
        LD      iy,(T1)         ; Load index to DCOUNT/DPOINT
        LD      b,(iy+DCOUNT)   ; Get direction count
        LD      a,(iy+DPOINT)   ; Get direction pointer
        LD      (INDX2),a       ; Save as index to direct
        LD      iy,(INDX2)      ; Load index
MP5:    LD      c,(iy+DIRECT)   ; Get move direction
        LD      a,(M1)          ; From position
        LD      (M2),a          ; Initialize to position
MP10:   CALL    PATH            ; Calculate next position
        CP      2               ; Ready for new direction ?
        JR      NC,MP15         ; Yes - Jump
        AND     a               ; Test for empty square
        EX      af,af'          ; Save result
        LD      a,(T1)          ; Get piece moved
        CP      PAWN+1          ; Is it a Pawn ?
        JR      C,MP20          ; Yes - Jump
        CALL    ADMOVE          ; Add move to list
        EX      af,af'          ; Empty square ?
        JR      NZ,MP15         ; No - Jump
        LD      a,(T1)          ; Piece type
        CP      KING            ; King ?
        JR      Z,MP15          ; Yes - Jump
        CP      BISHOP          ; Bishop, Rook, or Queen ?
        JR      NC,MP10         ; Yes - Jump
MP15:   INC     iy              ; Increment direction index
        DJNZ    MP5             ; Decr. count-jump if non-zerc
        LD      a,(T1)          ; Piece type
        CP      KING            ; King ?
        CALL    Z,CASTLE        ; Yes - Try Castling
        RET                     ; Return
; ***** PAWN LOGIC *****
MP20:   LD      a,b             ; Counter for direction
        CP      3             ; On diagonal moves ?
        JR      C,MP35          ; Yes - Jump
        JR      Z,MP30          ; -or-jump if on 2 square move
        EX      af,af'          ; Is forward square empty?
        JR      NZ,MP15         ; No - jump
        LD      a,(M2)          ; Get "to" position
        CP      91            ; Promote white Pawn ?
        JR      NC,MP25         ; Yes - Jump
        CP      29            ; Promote black Pawn ?
        JR      NC,MP26         ; No - Jump
MP25:   LD      hl,P2           ; Flag address
        SET     5,(hl)          ; Set promote flag
MP26:   CALL    ADMOVE          ; Add to move list
        INC     iy              ; Adjust to two square move
        DEC     b
        LD      hl,P1           ; Check Pawn moved flag
        BIT     3,(hl)          ; Has it moved before ?
        JR      Z,MP10          ; No - Jump
        JP      MP15            ; Jump
MP30:   EX      af,af'          ; Is forward square empty ?
        JR      NZ,MP15         ; No - Jump
MP31:   CALL    ADMOVE          ; Add to move list
        JP      MP15            ; Jump
MP35:   EX      af,af'          ; Is diagonal square empty ?
        JR      Z,MP36          ; Yes - Jump
        LD      a,(M2)          ; Get "to" position
        CP      91            ; Promote white Pawn ?
        JR      NC,MP37         ; Yes - Jump
        CP      29            ; Black Pawn promotion ?
        JR      NC,MP31         ; No- Jump
MP37:   LD      hl,P2           ; Get flag address
        SET     5,(hl)          ; Set promote flag
        JR      MP31            ; Jump
MP36:   CALL    ENPSNT          ; Try en passant capture
        JP      MP15            ; Jump

;***********************************************************
; EN PASSANT ROUTINE
;***********************************************************
; FUNCTION:   --  To test for en passant Pawn capture and
;                 to add it to the move list if it is
;                 legal.
;
; CALLED BY:  --  MPIECE
;
; CALLS:      --  ADMOVE
;                 ADJPTR
;
; ARGUMENTS:  --  None
;***********************************************************
ENPSNT: LD      a,(M1)          ; Set position of Pawn
        LD      hl,P1           ; Check color
        BIT     7,(hl)          ; Is it white ?
        JR      Z,rel002        ; Yes - skip
        ADD     a,10            ; Add 10 for black
rel002: CP      61            ; On en passant capture rank ?
        RET     C               ; No - return
        CP      69            ; On en passant capture rank ?
        RET     NC              ; No - return
        LD      ix,(MLPTRJ)     ; Get pointer to previous move
        BIT     4,(ix+MLFLG)    ; First move for that piece ?
        RET     Z               ; No - return
        LD      a,(ix+MLTOP)    ; Get "to" position
        LD      (M4),a          ; Store as index to board
        LD      ix,(M4)         ; Load board index
        LD      a,(ix+BOARD)    ; Get piece moved
        LD      (P3),a          ; Save it
        AND     7             ; Get piece type
        CP      PAWN          ; Is it a Pawn ?
        RET     NZ              ; No - return
        LD      a,(M4)          ; Get "to" position
        LD      hl,M2           ; Get present "to" position
        SUB     (hl)          ; Find difference
        JP      P,rel003        ; Positive ? Yes - Jump
        NEG                     ; Else take absolute value
rel003: CP      10            ; Is difference 10 ?
        RET     NZ              ; No - return
        LD      hl,P2           ; Address of flags
        SET     6,(hl)          ; Set double move flag
        CALL    ADMOVE          ; Add Pawn move to move list
        LD      a,(M1)          ; Save initial Pawn position
        LD      (M3),a
        LD      a,(M4)          ; Set "from" and "to" positions
                                ; for dummy move
        LD      (M1),a
        LD      (M2),a
        LD      a,(P3)          ; Save captured Pawn
        LD      (P2),a
        CALL    ADMOVE          ; Add Pawn capture to move list
        LD      a,(M3)          ; Restore "from" position
        LD      (M1),a

;***********************************************************
; ADJUST MOVE LIST POINTER FOR DOUBLE MOVE
;***********************************************************
; FUNCTION:   --  To adjust move list pointer to link around
;                 second move in double move.
;
; CALLED BY:  --  ENPSNT
;                 CASTLE
;                 (This mini-routine is not really called,
;                 but is jumped to to save time.)
;
; CALLS:      --  None
;
; ARGUMENTS:  --  None
;***********************************************************
ADJPTR: LD      hl,(MLLST)      ; Get list pointer
        LD      de,-6           ; Size of a move entry
        ADD     hl,de           ; Back up list pointer
        LD      (MLLST),hl      ; Save list pointer
        LD      (hl),0          ; Zero out link, first byte
        INC     hl              ; Next byte
        LD      (hl),0          ; Zero out link, second byte
        RET                     ; Return

;***********************************************************
; CASTLE ROUTINE
;***********************************************************
; FUNCTION:   --  To determine whether castling is legal
;                 (Queen side, King side, or both) and add it
;                 to the move list if it is.
;
; CALLED BY:  --  MPIECE
;
; CALLS:      --  ATTACK
;                 ADMOVE
;                 ADJPTR
;
; ARGUMENTS:  --  None
;***********************************************************
CASTLE: LD      a,(P1)          ; Get King
        BIT     3,a             ; Has it moved ?
        RET     NZ              ; Yes - return
        LD      a,(CKFLG)       ; Fetch Check Flag
        AND     a             ; Is the King in check ?
        RET     NZ              ; Yes - Return
        LD      bc,0FF03H       ; Initialize King-side values
CA5:    LD      a,(M1)          ; King position
        ADD     a,c             ; Rook position
        LD      c,a             ; Save
        LD      (M3),a          ; Store as board index
        LD      ix,(M3)         ; Load board index
        LD      a,(ix+BOARD)    ; Get contents of board
        AND     7FH           ; Clear color bit
        CP      ROOK          ; Has Rook ever moved ?
        JR      NZ,CA20         ; Yes - Jump
        LD      a,c             ; Restore Rook position
        JR      CA15            ; Jump
CA10:   LD      ix,(M3)         ; Load board index
        LD      a,(ix+BOARD)    ; Get contents of board
        AND     a             ; Empty ?
        JR      NZ,CA20         ; No - Jump
        LD      a,(M3)          ; Current position
        CP      22            ; White Queen Knight square ?
        JR      Z,CA15          ; Yes - Jump
        CP      92            ; Black Queen Knight square ?
        JR      Z,CA15          ; Yes - Jump
        CALL    ATTACK          ; Look for attack on square
        AND     a             ; Any attackers ?
        JR      NZ,CA20         ; Yes - Jump
        LD      a,(M3)          ; Current position
CA15:   ADD     a,b             ; Next position
        LD      (M3),a          ; Save as board index
        LD      hl,M1           ; King position
        CP      (hl)          ; Reached King ?
        JR      NZ,CA10         ; No - jump
        SUB     b             ; Determine King's position
        SUB     b
        LD      (M2),a          ; Save it
        LD      hl,P2           ; Address of flags
        LD      (hl),40H        ; Set double move flag
        CALL    ADMOVE          ; Put king move in list
        LD      hl,M1           ; Addr of King "from" position
        LD      a,(hl)          ; Get King's "from" position
        LD      (hl),c          ; Store Rook "from" position
        SUB     b             ; Get Rook "to" position
        LD      (M2),a          ; Store Rook "to" position
        XOR     a             ; Zero
        LD      (P2),a          ; Zero move flags
        CALL    ADMOVE          ; Put Rook move in list
        CALL    ADJPTR          ; Re-adjust move list pointer
        LD      a,(M3)          ; Restore King position
        LD      (M1),a          ; Store
CA20:   LD      a,b             ; Scan Index
        CP      1             ; Done ?
        RET     Z               ; Yes - return
        LD      bc,01FCH        ; Set Queen-side initial values
        JP      CA5             ; Jump

;***********************************************************
; ADMOVE ROUTINE
;***********************************************************
; FUNCTION:   --  To add a move to the move list
;
; CALLED BY:  --  MPIECE
;                 ENPSNT
;                 CASTLE
;
; CALLS:      --  None
;
; ARGUMENT:  --  None
;***********************************************************
ADMOVE: LD      de,(MLNXT)      ; Addr of next loc in move list
        LD      hl,MLEND        ; Address of list end
        AND     a             ; Clear carry flag
        SBC     hl,de           ; Calculate difference
        JR      C,AM10          ; Jump if out of space
        LD      hl,(MLLST)      ; Addr of prev. list area
        LD      (MLLST),de      ; Save next as previous
        LD      (hl),e          ; Store link address
        INC     hl
        LD      (hl),d
        LD      hl,P1           ; Address of moved piece
        BIT     3,(hl)          ; Has it moved before ?
        JR      NZ,rel004       ; Yes - jump
        LD      hl,P2           ; Address of move flags
        SET     4,(hl)          ; Set first move flag
rel004: EX      de,hl           ; Address of move area
        LD      (hl),0          ; Store zero in link address
        INC     hl
        LD      (hl),0
        INC     hl
        LD      a,(M1)          ; Store "from" move position
        LD      (hl),a
        INC     hl
        LD      a,(M2)          ; Store "to" move position
        LD      (hl),a
        INC     hl
        LD      a,(P2)          ; Store move flags/capt. piece
        LD      (hl),a
        INC     hl
        LD      (hl),0          ; Store initial move value
        INC     hl
        LD      (MLNXT),hl      ; Save address for next move
        RET                     ; Return
AM10:   LD      (hl),0          ; Abort entry on table ovflow
        INC     hl
        LD      (hl),0          ; TODO does this out of memory
        DEC     hl              ;      check actually work?
        RET

;***********************************************************
; GENERATE MOVE ROUTINE
;***********************************************************
; FUNCTION:  --  To generate the move set for all of the
;                pieces of a given color.
;
; CALLED BY: --  FNDMOV
;
; CALLS:     --  MPIECE
;                INCHK
;
; ARGUMENTS: --  None
;***********************************************************
GENMOV: CALL    INCHK           ; Test for King in check
        LD      (CKFLG),a       ; Save attack count as flag
        LD      de,(MLNXT)      ; Addr of next avail list space
        LD      hl,(MLPTRI)     ; Ply list pointer index
        INC     hl              ; Increment to next ply
        INC     hl
        LD      (hl),e          ; Save move list pointer
        INC     hl
        LD      (hl),d
        INC     hl
        LD      (MLPTRI),hl     ; Save new index
        LD      (MLLST),hl      ; Last pointer for chain init.
        LD      a,21            ; First position on board
GM5:    LD      (M1),a          ; Save as index
        LD      ix,(M1)         ; Load board index
        LD      a,(ix+BOARD)    ; Fetch board contents
        AND     a             ; Is it empty ?
        JR      Z,GM10          ; Yes - Jump
        CP      -1            ; Is it a border square ?
        JR      Z,GM10          ; Yes - Jump
        LD      (P1),a          ; Save piece
        LD      hl,COLOR        ; Address of color of piece
        XOR     (hl)          ; Test color of piece
        BIT     7,a             ; Match ?
        CALL    Z,MPIECE        ; Yes - call Move Piece
GM10:   LD      a,(M1)          ; Fetch current board position
        INC     a               ; Incr to next board position
        CP      99            ; End of board array ?
        JP      NZ,GM5          ; No - Jump
        RET                     ; Return

;***********************************************************
; CHECK ROUTINE
;***********************************************************
; FUNCTION:   --  To determine whether or not the
;                 King is in check.
;
; CALLED BY:  --  GENMOV
;                 FNDMOV
;                 EVAL
;
; CALLS:      --  ATTACK
;
; ARGUMENTS:  --  Color of King
;***********************************************************
INCHK:  LD      a,(COLOR)       ; Get color
INCHK1: LD      hl,POSK         ; Addr of white King position
        AND     a             ; White ?
        JR      Z,rel005        ; Yes - Skip
        INC     hl              ; Addr of black King position
rel005: LD      a,(hl)          ; Fetch King position
        LD      (M3),a          ; Save
        LD      ix,(M3)         ; Load board index
        LD      a,(ix+BOARD)    ; Fetch board contents
        LD      (P1),a          ; Save
        AND     7             ; Get piece type
        LD      (T1),a          ; Save
        CALL    ATTACK          ; Look for attackers on King
        RET                     ; Return

;***********************************************************
; ATTACK ROUTINE
;***********************************************************
; FUNCTION:   --  To find all attackers on a given square
;                 by scanning outward from the square
;                 until a piece is found that attacks
;                 that square, or a piece is found that
;                 doesn't attack that square, or the edge
;                 of the board is reached.
;
;                 In determining which pieces attack
;                 a square, this routine also takes into
;                 account the ability of certain pieces to
;                 attack through another attacking piece. (For
;                 example a queen lined up behind a bishop
;                 of her same color along a diagonal.) The
;                 bishop is then said to be transparent to the
;                 queen, since both participate in the
;                 attack.
;
;                 In the case where this routine is called
;                 by CASTLE or INCHK, the routine is
;                 terminated as soon as an attacker of the
;                 opposite color is encountered.
;
; CALLED BY:  --  POINTS
;                 PINFND
;                 CASTLE
;                 INCHK
;
; CALLS:      --  PATH
;                 ATKSAV
;
; ARGUMENTS:  --  None
;***********************************************************
ATTACK: PUSH    bc              ; Save Register B
        XOR     a             ; Clear
        LD      b,16            ; Initial direction count
        LD      (INDX2),a       ; Initial direction index
        LD      iy,(INDX2)      ; Load index
AT5:    LD      c,(iy+DIRECT)   ; Get direction
        LD      d,0             ; Init. scan count/flags
        LD      a,(M3)          ; Init. board start position
        LD      (M2),a          ; Save
AT10:   INC     d               ; Increment scan count
        CALL    PATH            ; Next position
        CP      1             ; Piece of a opposite color ?
        JR      Z,AT14A         ; Yes - jump
        CP      2             ; Piece of same color ?
        JR      Z,AT14B         ; Yes - jump
        AND     a             ; Empty position ?
        JR      NZ,AT12         ; No - jump
        LD      a,b             ; Fetch direction count
        CP      9             ; On knight scan ?
        JR      NC,AT10         ; No - jump
AT12:   INC     iy              ; Increment direction index
        DJNZ    AT5             ; Done ? No - jump
        XOR     a             ; No attackers
AT13:   POP     bc              ; Restore register B
        RET                     ; Return
AT14A:  BIT     6,d             ; Same color found already ?
        JR      NZ,AT12         ; Yes - jump
        SET     5,d             ; Set opposite color found flag
        JP      AT14            ; Jump
AT14B:  BIT     5,d             ; Opposite color found already?
        JR      NZ,AT12         ; Yes - jump
        SET     6,d             ; Set same color found flag

;
; ***** DETERMINE IF PIECE ENCOUNTERED ATTACKS SQUARE *****
AT14:   LD      a,(T2)          ; Fetch piece type encountered
        LD      e,a             ; Save
        LD      a,b             ; Get direction-counter
        CP      9             ; Look for Knights ?
        JR      C,AT25          ; Yes - jump
        LD      a,e             ; Get piece type
        CP      QUEEN         ; Is is a Queen ?
        JR      NZ,AT15         ; No - Jump
        SET     7,d             ; Set Queen found flag
        JR      AT30            ; Jump
AT15:   LD      a,d             ; Get flag/scan count
        AND     0FH           ; Isolate count
        CP      1             ; On first position ?
        JR      NZ,AT16         ; No - jump
        LD      a,e             ; Get encountered piece type
        CP      KING          ; Is it a King ?
        JR      Z,AT30          ; Yes - jump
AT16:   LD      a,b             ; Get direction counter
        CP      13            ; Scanning files or ranks ?
        JR      C,AT21          ; Yes - jump
        LD      a,e             ; Get piece type
        CP      BISHOP        ; Is it a Bishop ?
        JR      Z,AT30          ; Yes - jump
        LD      a,d             ; Get flags/scan count
        AND     0FH           ; Isolate count
        CP      1             ; On first position ?
        JR      NZ,AT12         ; No - jump
        CP      e             ; Is it a Pawn ?
        JR      NZ,AT12         ; No - jump
        LD      a,(P2)          ; Fetch piece including color
        BIT     7,a             ; Is it white ?
        JR      Z,AT20          ; Yes - jump
        LD      a,b             ; Get direction counter
        CP      15            ; On a non-attacking diagonal ?
        JR      C,AT12          ; Yes - jump
        JR      AT30            ; Jump
AT20:   LD      a,b             ; Get direction counter
        CP      15            ; On a non-attacking diagonal ?
        JR      NC,AT12         ; Yes - jump
        JR      AT30            ; Jump
AT21:   LD      a,e             ; Get piece type
        CP      ROOK          ; Is is a Rook ?
        JR      NZ,AT12         ; No - jump
        JR      AT30            ; Jump
AT25:   LD      a,e             ; Get piece type
        CP      KNIGHT        ; Is it a Knight ?
        JR      NZ,AT12         ; No - jump
AT30:   LD      a,(T1)          ; Attacked piece type/flag
        CP      7             ; Call from POINTS ?
        JR      Z,AT31          ; Yes - jump
        BIT     5,d             ; Is attacker opposite color ?
        JR      Z,AT32          ; No - jump
        LD      a,1             ; Set attacker found flag
        JP      AT13            ; Jump
AT31:   CALL    ATKSAV          ; Save attacker in attack list
AT32:   LD      a,(T2)          ; Attacking piece type
        CP      KING          ; Is it a King,?
        JP      Z,AT12          ; Yes - jump
        CP      KNIGHT        ; Is it a Knight ?
        JP      Z,AT12          ; Yes - jump
        JP      AT10            ; Jump

;***********************************************************
; ATTACK SAVE ROUTINE
;***********************************************************
; FUNCTION:   --  To save an attacking piece value in the
;                 attack list, and to increment the attack
;                 count for that color piece.
;
;                 The pin piece list is checked for the
;                 attacking piece, and if found there, the
;                 piece is not included in the attack list.
;
; CALLED BY:  --  ATTACK
;
; CALLS:      --  PNCK
;
; ARGUMENTS:  --  None
;***********************************************************
ATKSAV: PUSH    bc              ; Save Regs BC
        PUSH    de              ; Save Regs DE
        LD      a,(NPINS)       ; Number of pinned pieces
        AND     a             ; Any ?
        CALL    NZ,PNCK         ; yes - check pin list
        LD      ix,(T2)         ; Init index to value table
        LD      hl,ATKLST       ; Init address of attack list
        LD      bc,0            ; Init increment for white
        LD      a,(P2)          ; Attacking piece
        BIT     7,a             ; Is it white ?
        JR      Z,rel006        ; Yes - jump
        LD      c,7             ; Init increment for black
rel006: AND     7             ; Attacking piece type
        LD      e,a             ; Init increment for type
        BIT     7,d             ; Queen found this scan ?
        JR      Z,rel007        ; No - jump
        LD      e,QUEEN         ; Use Queen slot in attack list
rel007: ADD     hl,bc           ; Attack list address
        INC     (hl)            ; Increment list count
        LD      d,0
        ADD     hl,de           ; Attack list slot address
        LD      a,(hl)          ; Get data already there
        AND     0FH           ; Is first slot empty ?
        JR      Z,AS20          ; Yes - jump
        LD      a,(hl)          ; Get data again
        AND     0F0H          ; Is second slot empty ?
        JR      Z,AS19          ; Yes - jump
        INC     hl              ; Increment to King slot
        JR      AS20            ; Jump
AS19:   RLD                     ; Temp save lower in upper
        LD      a,(ix+PVALUE)   ; Get new value for attack list
        RRD                     ; Put in 2nd attack list slot
        JR      AS25            ; Jump
AS20:   LD      a,(ix+PVALUE)   ; Get new value for attack list
        RLD                     ; Put in 1st attack list slot
AS25:   POP     de              ; Restore DE regs
        POP     bc              ; Restore BC regs
        RET                     ; Return

;***********************************************************
; PIN CHECK ROUTINE
;***********************************************************
; FUNCTION:   --  Checks to see if the attacker is in the
;                 pinned piece list. If so he is not a valid
;                 attacker unless the direction in which he
;                 attacks is the same as the direction along
;                 which he is pinned. If the piece is
;                 found to be invalid as an attacker, the
;                 return to the calling routine is aborted
;                 and this routine returns directly to ATTACK.
;
; CALLED BY:  --  ATKSAV
;
; CALLS:      --  None
;
; ARGUMENTS:  --  The direction of the attack. The
;                 pinned piece counnt.
;***********************************************************
PNCK:   LD      d,c             ; Save attack direction
        LD      e,0             ; Clear flag
        LD      c,a             ; Load pin count for search
        LD      b,0
        LD      a,(M2)          ; Position of piece
        LD      hl,PLISTA       ; Pin list address
PC1:    CPIR                    ; Search list for position
        RET     NZ              ; Return if not found
        EX      af,af'          ; Save search parameters
        BIT     0,e             ; Is this the first find ?
        JR      NZ,PC5          ; No - jump
        SET     0,e             ; Set first find flag
        PUSH    hl              ; Get corresp index to dir list
        POP     ix
        LD      a,(ix+9)        ; Get direction
        CP      d             ; Same as attacking direction ?
        JR      Z,PC3           ; Yes - jump
        NEG                     ; Opposite direction ?
        CP      d             ; Same as attacking direction ?
        JR      NZ,PC5          ; No - jump
PC3:    EX      af,af'          ; Restore search parameters
        JP      PE,PC1          ; Jump if search not complete
        RET                     ; Return
PC5:    POP     af              ; Abnormal exit
        POP     de              ; Restore regs.
        POP     bc
        RET                     ; Return to ATTACK

;***********************************************************
; PIN FIND ROUTINE
;***********************************************************
; FUNCTION:   --  To produce a list of all pieces pinned
;                 against the King or Queen, for both white
;                 and black.
;
; CALLED BY:  --  FNDMOV
;                 EVAL
;
; CALLS:      --  PATH
;                 ATTACK
;
; ARGUMENTS:  --  None
;***********************************************************
PINFND: XOR     a             ; Zero pin count
        LD      (NPINS),a
        LD      de,POSK         ; Addr of King/Queen pos list
PF1:    LD      a,(de)          ; Get position of royal piece
        AND     a             ; Is it on board ?
        JP      Z,PF26          ; No- jump
        CP      -1            ; At end of list ?
        RET     Z               ; Yes return
        LD      (M3),a          ; Save position as board index
        LD      ix,(M3)         ; Load index to board
        LD      a,(ix+BOARD)    ; Get contents of board
        LD      (P1),a          ; Save
        LD      b,8             ; Init scan direction count
        XOR     a
        LD      (INDX2),a       ; Init direction index
        LD      iy,(INDX2)
PF2:    LD      a,(M3)          ; Get King/Queen position
        LD      (M2),a          ; Save
        XOR     a
        LD      (M4),a          ; Clear pinned piece saved pos
        LD      c,(iy+DIRECT)   ; Get direction of scan
PF5:    CALL    PATH            ; Compute next position
        AND     a             ; Is it empty ?
        JR      Z,PF5           ; Yes - jump
        CP      3             ; Off board ?
        JP      Z,PF25          ; Yes - jump
        CP      2             ; Piece of same color
        LD      a,(M4)          ; Load pinned piece position
        JR      Z,PF15          ; Yes - jump
        AND     a             ; Possible pin ?
        JP      Z,PF25          ; No - jump
        LD      a,(T2)          ; Piece type encountered
        CP      QUEEN         ; Queen ?
        JP      Z,PF19          ; Yes - jump
        LD      l,a             ; Save piece type
        LD      a,b             ; Direction counter
        CP      5             ; Non-diagonal direction ?
        JR      C,PF10          ; Yes - jump
        LD      a,l             ; Piece type
        CP      BISHOP        ; Bishop ?
        JP      NZ,PF25         ; No - jump
        JP      PF20            ; Jump
PF10:   LD      a,l             ; Piece type
        CP      ROOK          ; Rook ?
        JP      NZ,PF25         ; No - jump
        JP      PF20            ; Jump
PF15:   AND     a             ; Possible pin ?
        JP      NZ,PF25         ; No - jump
        LD      a,(M2)          ; Save possible pin position
        LD      (M4),a
        JP      PF5             ; Jump
PF19:   LD      a,(P1)          ; Load King or Queen
        AND     7             ; Clear flags
        CP      QUEEN         ; Queen ?
        JR      NZ,PF20         ; No - jump
        PUSH    bc              ; Save regs.
        PUSH    de
        PUSH    iy
        XOR     a             ; Zero out attack list
        LD      b,14
        LD      hl,ATKLST
back02: LD      (hl),a
        INC     hl
        DJNZ    back02
        LD      a,7             ; Set attack flag
        LD      (T1),a
        CALL    ATTACK          ; Find attackers/defenders
        LD      hl,WACT         ; White queen attackers
        LD      de,BACT         ; Black queen attackers
        LD      a,(P1)          ; Get queen
        BIT     7,a             ; Is she white ?
        JR      Z,rel008        ; Yes - skip
        EX      de,hl           ; Reverse for black
rel008: LD      a,(hl)          ; Number of defenders
        EX      de,hl           ; Reverse for attackers
        SUB     (hl)          ; Defenders minus attackers
        DEC     a               ; Less 1
        POP     iy              ; Restore regs.
        POP     de
        POP     bc
        JP      P,PF25          ; Jump if pin not valid
PF20:   LD      hl,NPINS        ; Address of pinned piece count
        INC     (hl)            ; Increment
        LD      ix,(NPINS)      ; Load pin list index
        LD      (ix+PLISTD),c   ; Save direction of pin
        LD      a,(M4)          ; Position of pinned piece
        LD      (ix+PLIST),a    ; Save in list
PF25:   INC     iy              ; Increment direction index
        DJNZ    PF27            ; Done ? No - Jump
PF26:   INC     de              ; Incr King/Queen pos index
        JP      PF1             ; Jump
PF27:   JP      PF2             ; Jump

;***********************************************************
; EXCHANGE ROUTINE
;***********************************************************
; FUNCTION:   --  To determine the exchange value of a
;                 piece on a given square by examining all
;                 attackers and defenders of that piece.
;
; CALLED BY:  --  POINTS
;
; CALLS:      --  NEXTAD
;
; ARGUMENTS:  --  None.
;***********************************************************
XCHNG:  EXX                     ; Swap regs.
        LD      a,(P1)          ; Piece attacked
        LD      hl,WACT         ; Addr of white attkrs/dfndrs
        LD      de,BACT         ; Addr of black attkrs/dfndrs
        BIT     7,a             ; Is piece white ?
        JR      Z,rel009        ; Yes - jump
        EX      de,hl           ; Swap list pointers
rel009: LD      b,(hl)          ; Init list counts
        EX      de,hl
        LD      c,(hl)
        EX      de,hl
        EXX                     ; Restore regs.
        LD      c,0             ; Init attacker/defender flag
        LD      e,0             ; Init points lost count
        LD      ix,(T3)         ; Load piece value index
        LD      d,(ix+PVALUE)   ; Get attacked piece value
        SLA     d               ; Double it
        LD      b,d             ; Save
        CALL    NEXTAD          ; Retrieve first attacker
        RET     Z               ; Return if none
XC10:   LD      l,a             ; Save attacker value
        CALL    NEXTAD          ; Get next defender
        JR      Z,XC18          ; Jump if none
        EX      af,af'          ; Save defender value
        LD      a,b             ; Get attacked value
        CP      l             ; Attacked less than attacker ?
        JR      NC,XC19         ; No - jump
        EX      af,af'          ; -Restore defender
XC15:   CP      l             ; Defender less than attacker ?
        RET     C               ; Yes - return
        CALL    NEXTAD          ; Retrieve next attacker value
        RET     Z               ; Return if none
        LD      l,a             ; Save attacker value
        CALL    NEXTAD          ; Retrieve next defender value
        JR      NZ,XC15         ; Jump if none
XC18:   EX      af,af'          ; Save Defender
        LD      a,b             ; Get value of attacked piece
XC19:   BIT     0,c             ; Attacker or defender ?
        JR      Z,rel010        ; Jump if defender
        NEG                     ; Negate value for attacker
rel010: ADD     a,e             ; Total points lost
        LD      e,a             ; Save total
        EX      af,af'          ; Restore previous defender
        RET     Z               ; Return if none
        LD      b,l             ; Prev attckr becomes defender
        JP      XC10            ; Jump

;***********************************************************
; NEXT ATTACKER/DEFENDER ROUTINE
;***********************************************************
; FUNCTION:   --  To retrieve the next attacker or defender
;                 piece value from the attack list, and delete
;                 that piece from the list.
;
; CALLED BY:  --  XCHNG
;
; CALLS:      --  None
;
; ARGUMENTS:  --  Attack list addresses.
;                 Side flag
;                 Attack list counts
;***********************************************************
NEXTAD: INC     c               ; Increment side flag
        EXX                     ; Swap registers
        LD      a,b             ; Swap list counts
        LD      b,c
        LD      c,a
        EX      de,hl           ; Swap list pointers
        XOR     a
        CP      b             ; At end of list ?
        JR      Z,NX6           ; Yes - jump
        DEC     b               ; Decrement list count
back03: INC     hl              ; Increment list pointer
        CP      (hl)          ; Check next item in list
        JR      Z,back03        ; Jump if empty
        RRD                     ; Get value from list
        ADD     a,a             ; Double it
        DEC     hl              ; Decrement list pointer
NX6:    EXX                     ; Restore regs.
        RET                     ; Return

;***********************************************************
; POINT EVALUATION ROUTINE
;***********************************************************
;FUNCTION:   --  To perform a static board evaluation and
;                derive a score for a given board position
;
; CALLED BY:  --  FNDMOV
;                 EVAL
;
; CALLS:      --  ATTACK
;                 XCHNG
;                 LIMIT
;
; ARGUMENTS:  --  None
;***********************************************************
POINTS: XOR     a             ; Zero out variables
        LD      (MTRL),a
        LD      (BRDC),a
        LD      (PTSL),a
        LD      (PTSW1),a
        LD      (PTSW2),a
        LD      (PTSCK),a
        LD      hl,T1           ; Set attacker flag
        LD      (hl),7
        LD      a,21            ; Init to first square on board
PT5:    LD      (M3),a          ; Save as board index
        LD      ix,(M3)         ; Load board index
        LD      a,(ix+BOARD)    ; Get piece from board
        CP      -1            ; Off board edge ?
        JP      Z,PT25          ; Yes - jump
        LD      hl,P1           ; Save piece, if any
        LD      (hl),a
        AND     7             ; Save piece type, if any
        LD      (T3),a
        CP      KNIGHT        ; Less than a Knight (Pawn) ?
        JR      C,PT6X          ; Yes - Jump
        CP      ROOK          ; Rook, Queen or King ?
        JR      C,PT6B          ; No - jump
        CP      KING          ; Is it a King ?
        JR      Z,PT6AA         ; Yes - jump
        LD      a,(MOVENO)      ; Get move number
        CP      7             ; Less than 7 ?
        JR      C,PT6A          ; Yes - Jump
        JP      PT6X            ; Jump
PT6AA:  BIT     4,(hl)          ; Castled yet ?
        JR      Z,PT6A          ; No - jump
        LD      a,+6            ; Bonus for castling
        BIT     7,(hl)          ; Check piece color
        JR      Z,PT6D          ; Jump if white
        LD      a,-6            ; Bonus for black castling
        JP      PT6D            ; Jump
PT6A:   BIT     3,(hl)          ; Has piece moved yet ?
        JR      Z,PT6X          ; No - jump
        JP      PT6C            ; Jump
PT6B:   BIT     3,(hl)          ; Has piece moved yet ?
        JR      NZ,PT6X         ; Yes - jump
PT6C:   LD      a,-2            ; Two point penalty for white
        BIT     7,(hl)          ; Check piece color
        JR      Z,PT6D          ; Jump if white
        LD      a,+2            ; Two point penalty for black
PT6D:   LD      hl,BRDC         ; Get address of board control
        ADD     a,(hl)          ; Add on penalty/bonus points
        LD      (hl),a          ; Save
PT6X:   XOR     a             ; Zero out attack list
        LD      b,14
        LD      hl,ATKLST
back04: LD      (hl),a
        INC     hl
        DJNZ    back04
        CALL    ATTACK          ; Build attack list for square
        LD      hl,BACT         ; Get black attacker count addr
        LD      a,(WACT)        ; Get white attacker count
        SUB     (hl)          ; Compute count difference
        LD      hl,BRDC         ; Address of board control
        ADD     a,(hl)          ; Accum board control score
        LD      (hl),a          ; Save
        LD      a,(P1)          ; Get piece on current square
        AND     a             ; Is it empty ?
        JP      Z,PT25          ; Yes - jump
        CALL    XCHNG           ; Evaluate exchange, if any
        XOR     a             ; Check for a loss
        CP      e             ; Points lost ?
        JR      Z,PT23          ; No - Jump
        DEC     d               ; Deduct half a Pawn value
        LD      a,(P1)          ; Get piece under attack
        LD      hl,COLOR        ; Color of side just moved
        XOR     (hl)          ; Compare with piece
        BIT     7,a             ; Do colors match ?
        LD      a,e             ; Points lost
        JR      NZ,PT20         ; Jump if no match
        LD      hl,PTSL         ; Previous max points lost
        CP      (hl)          ; Compare to current value
        JR      C,PT23          ; Jump if greater than
        LD      (hl),e          ; Store new value as max lost
        LD      ix,(MLPTRJ)     ; Load pointer to this move
        LD      a,(M3)          ; Get position of lost piece
        CP      (ix+MLTOP)    ; Is it the one moving ?
        JR      NZ,PT23         ; No - jump
        LD      (PTSCK),a       ; Save position as a flag
        JP      PT23            ; Jump
PT20:   LD      hl,PTSW1        ; Previous maximum points won
        CP      (hl)          ; Compare to current value
        JR      C,rel011        ; Jump if greater than
        LD      a,(hl)          ; Load previous max value
        LD      (hl),e          ; Store new value as max won
rel011: LD      hl,PTSW2        ; Previous 2nd max points won
        CP      (hl)          ; Compare to current value
        JR      C,PT23          ; Jump if greater than
        LD      (hl),a          ; Store as new 2nd max lost
PT23:   LD      hl,P1           ; Get piece
        BIT     7,(hl)          ; Test color
        LD      a,d             ; Value of piece
        JR      Z,rel012        ; Jump if white
        NEG                     ; Negate for black
rel012: LD      hl,MTRL         ; Get addrs of material total
        ADD     a,(hl)          ; Add new value
        LD      (hl),a          ; Store
PT25:   LD      a,(M3)          ; Get current board position
        INC     a               ; Increment
        CP      99            ; At end of board ?
        JP      NZ,PT5          ; No - jump
        LD      a,(PTSCK)       ; Moving piece lost flag
        AND     a             ; Was it lost ?
        JR      Z,PT25A         ; No - jump
        LD      a,(PTSW2)       ; 2nd max points won
        LD      (PTSW1),a       ; Store as max points won
        XOR     a             ; Zero out 2nd max points won
        LD      (PTSW2),a
PT25A:  LD      a,(PTSL)        ; Get max points lost
        AND     a             ; Is it zero ?
        JR      Z,rel013        ; Yes - jump
        DEC     a               ; Decrement it
rel013: LD      b,a             ; Save it
        LD      a,(PTSW1)       ; Max,points won
        AND     a             ; Is it zero ?
        JR      Z,rel014        ; Yes - jump
        LD      a,(PTSW2)       ; 2nd max points won
        AND     a             ; Is it zero ?
        JR      Z,rel014        ; Yes - jump
        DEC     a               ; Decrement it
        SRL     a               ; Divide it by 2
rel014: SUB     b             ; Subtract points lost
        LD      hl,COLOR        ; Color of side just moved ???
        BIT     7,(hl)          ; Is it white ?
        JR      Z,rel015        ; Yes - jump
        NEG                     ; Negate for black
rel015: LD      hl,MTRL         ; Net material on board
        ADD     a,(hl)          ; Add exchange adjustments
        LD      hl,MV0          ; Material at ply 0
        SUB     (hl)          ; Subtract from current
        LD      b,a             ; Save
        LD      a,30            ; Load material limit
        CALL    LIMIT           ; Limit to plus or minus value
        LD      e,a             ; Save limited value
        LD      a,(BRDC)        ; Get board control points
        LD      hl,BC0          ; Board control at ply zero
        SUB     (hl)          ; Get difference
        LD      b,a             ; Save
        LD      a,(PTSCK)       ; Moving piece lost flag
        AND     a             ; Is it zero ?
        JR      Z,rel026        ; Yes - jump
        LD      b,0             ; Zero board control points
rel026: LD      a,6             ; Load board control limit
        CALL    LIMIT           ; Limit to plus or minus value
        LD      d,a             ; Save limited value
        LD      a,e             ; Get material points
        ADD     a,a             ; Multiply by 4
        ADD     a,a
        ADD     a,d             ; Add board control
        LD      hl,COLOR        ; Color of side just moved
        BIT     7,(hl)          ; Is it white ?
        JR      NZ,rel016       ; No - jump
        NEG                     ; Negate for white
rel016: ADD     a,80H           ; Rescale score (neutral = 80H)
        LD      (VALM),a        ; Save score
        LD      ix,(MLPTRJ)     ; Load move list pointer
        LD      (ix+MLVAL),a    ; Save score in move list
        RET                     ; Return

;***********************************************************
; LIMIT ROUTINE
;***********************************************************
; FUNCTION:   --  To limit the magnitude of a given value
;                 to another given value.
;
; CALLED BY:  --  POINTS
;
; CALLS:      --  None
;
; ARGUMENTS:  --  Input  - Value, to be limited in the B
;                          register.
;                        - Value to limit to in the A register
;                 Output - Limited value in the A register.
;***********************************************************
LIMIT:  BIT     7,b             ; Is value negative ?
        JP      Z,LIM10         ; No - jump
        NEG                     ; Make positive
        CP      b             ; Compare to limit
        RET     NC              ; Return if outside limit
        LD      a,b             ; Output value as is
        RET                     ; Return
LIM10:  CP      b             ; Compare to limit
        RET     C               ; Return if outside limit
        LD      a,b             ; Output value as is
        RET                     ; Return

;***********************************************************
; MOVE ROUTINE
;***********************************************************
; FUNCTION:   --  To execute a move from the move list on the
;                 board array.
;
; CALLED BY:  --  CPTRMV
;                 PLYRMV
;                 EVAL
;                 FNDMOV
;                 VALMOV
;
; CALLS:      --  None
;
; ARGUMENTS:  --  None
;***********************************************************
MOVE:   LD      hl,(MLPTRJ)     ; Load move list pointer
        INC     hl              ; Increment past link bytes
        INC     hl
MV1:    LD      a,(hl)          ; "From" position
        LD      (M1),a          ; Save
        INC     hl              ; Increment pointer
        LD      a,(hl)          ; "To" position
        LD      (M2),a          ; Save
        INC     hl              ; Increment pointer
        LD      d,(hl)          ; Get captured piece/flags
        LD      ix,(M1)         ; Load "from" pos board index
        LD      e,(ix+BOARD)    ; Get piece moved
        BIT     5,d             ; Test Pawn promotion flag
        JR      NZ,MV15         ; Jump if set
        LD      a,e             ; Piece moved
        AND     7             ; Clear flag bits
        CP      QUEEN         ; Is it a queen ?
        JR      Z,MV20          ; Yes - jump
        CP      KING          ; Is it a king ?
        JR      Z,MV30          ; Yes - jump
MV5:    LD      iy,(M2)         ; Load "to" pos board index
        SET     3,e             ; Set piece moved flag
        LD      (iy+BOARD),e    ; Insert piece at new position
        LD      (ix+BOARD),0    ; Empty previous position
        BIT     6,d             ; Double move ?
        JR      NZ,MV40         ; Yes - jump
        LD      a,d             ; Get captured piece, if any
        AND     7
        CP      QUEEN         ; Was it a queen ?
        RET     NZ              ; No - return
        LD      hl,POSQ         ; Addr of saved Queen position
        BIT     7,d             ; Is Queen white ?
        JR      Z,MV10          ; Yes - jump
        INC     hl              ; Increment to black Queen pos
MV10:   XOR     a             ; Set saved position to zero
        LD      (hl),a
        RET                     ; Return
MV15:   SET     2,e             ; Change Pawn to a Queen
        JP      MV5             ; Jump
MV20:   LD      hl,POSQ         ; Addr of saved Queen position
MV21:   BIT     7,e             ; Is Queen white ?
        JR      Z,MV22          ; Yes - jump
        INC     hl              ; Increment to black Queen pos
MV22:   LD      a,(M2)          ; Get new Queen position
        LD      (hl),a          ; Save
        JP      MV5             ; Jump
MV30:   LD      hl,POSK         ; Get saved King position
        BIT     6,d             ; Castling ?
        JR      Z,MV21          ; No - jump
        SET     4,e             ; Set King castled flag
        JP      MV21            ; Jump
MV40:   LD      hl,(MLPTRJ)     ; Get move list pointer
        LD      de,8            ; Increment to next move
        ADD     hl,de
        JP      MV1             ; Jump (2nd part of dbl move)

;***********************************************************
; UN-MOVE ROUTINE
;***********************************************************
; FUNCTION:   --  To reverse the process of the move routine,
;                 thereby restoring the board array to its
;                 previous position.
;
; CALLED BY:  --  VALMOV
;                 EVAL
;                 FNDMOV
;                 ASCEND
;
; CALLS:      --  None
;
; ARGUMENTS:  --  None
;***********************************************************
UNMOVE: LD      hl,(MLPTRJ)     ; Load move list pointer
        INC     hl              ; Increment past link bytes
        INC     hl
UM1:    LD      a,(hl)          ; Get "from" position
        LD      (M1),a          ; Save
        INC     hl              ; Increment pointer
        LD      a,(hl)          ; Get "to" position
        LD      (M2),a          ; Save
        INC     hl              ; Increment pointer
        LD      d,(hl)          ; Get captured piece/flags
        LD      ix,(M2)         ; Load "to" pos board index
        LD      e,(ix+BOARD)    ; Get piece moved
        BIT     5,d             ; Was it a Pawn promotion ?
        JR      NZ,UM15         ; Yes - jump
        LD      a,e             ; Get piece moved
        AND     7             ; Clear flag bits
        CP      QUEEN         ; Was it a Queen ?
        JR      Z,UM20          ; Yes - jump
        CP      KING          ; Was it a King ?
        JR      Z,UM30          ; Yes - jump
UM5:    BIT     4,d             ; Is this 1st move for piece ?
        JR      NZ,UM16         ; Yes - jump
UM6:    LD      iy,(M1)         ; Load "from" pos board index
        LD      (iy+BOARD),e    ; Return to previous board pos
        LD      a,d             ; Get captured piece, if any
        AND     8FH           ; Clear flags
        LD      (ix+BOARD),a    ; Return to board
        BIT     6,d             ; Was it a double move ?
        JR      NZ,UM40         ; Yes - jump
        LD      a,d             ; Get captured piece, if any
        AND     7             ; Clear flag bits
        CP      QUEEN         ; Was it a Queen ?
        RET     NZ              ; No - return
        LD      hl,POSQ         ; Address of saved Queen pos
        BIT     7,d             ; Is Queen white ?
        JR      Z,UM10          ; Yes - jump
        INC     hl              ; Increment to black Queen pos
UM10:   LD      a,(M2)          ; Queen's previous position
        LD      (hl),a          ; Save
        RET                     ; Return
UM15:   RES     2,e             ; Restore Queen to Pawn
        JP      UM5             ; Jump
UM16:   RES     3,e             ; Clear piece moved flag
        JP      UM6             ; Jump
UM20:   LD      hl,POSQ         ; Addr of saved Queen position
UM21:   BIT     7,e             ; Is Queen white ?
        JR      Z,UM22          ; Yes - jump
        INC     hl              ; Increment to black Queen pos
UM22:   LD      a,(M1)          ; Get previous position
        LD      (hl),a          ; Save
        JP      UM5             ; Jump
UM30:   LD      hl,POSK         ; Address of saved King pos
        BIT     6,d             ; Was it a castle ?
        JR      Z,UM21          ; No - jump
        RES     4,e             ; Clear castled flag
        JP      UM21            ; Jump
UM40:   LD      hl,(MLPTRJ)     ; Load move list pointer
        LD      de,8            ; Increment to next move
        ADD     hl,de
        JP      UM1             ; Jump (2nd part of dbl move)

;***********************************************************
; SORT ROUTINE
;***********************************************************
; FUNCTION:   --  To sort the move list in order of
;                 increasing move value scores.
;
; CALLED BY:  --  FNDMOV
;
; CALLS:      --  EVAL
;
; ARGUMENTS:  --  None
;***********************************************************
SORTM:  LD      bc,(MLPTRI)     ; Move list begin pointer
        LD      de,0            ; Initialize working pointers
SR5:    LD      h,b
        LD      l,c
        LD      c,(hl)          ; Link to next move
        INC     hl
        LD      b,(hl)
        LD      (hl),d          ; Store to link in list
        DEC     hl
        LD      (hl),e
        XOR     a             ; End of list ?
        CP      b
        RET     Z               ; Yes - return
SR10:   LD      (MLPTRJ),bc     ; Save list pointer
        CALL    EVAL            ; Evaluate move
        LD      hl,(MLPTRI)     ; Begining of move list
        LD      bc,(MLPTRJ)     ; Restore list pointer
SR15:   LD      e,(hl)          ; Next move for compare
        INC     hl
        LD      d,(hl)
        XOR     a             ; At end of list ?
        CP      d
        JR      Z,SR25          ; Yes - jump
        PUSH    de              ; Transfer move pointer
        POP     ix
        LD      a,(VALM)        ; Get new move value
        CP      (ix+MLVAL)    ; Less than list value ?
        JR      NC,SR30         ; No - jump
SR25:   LD      (hl),b          ; Link new move into list
        DEC     hl
        LD      (hl),c
        JP      SR5             ; Jump
SR30:   EX      de,hl           ; Swap pointers
        JP      SR15            ; Jump

;***********************************************************
; EVALUATION ROUTINE
;***********************************************************
; FUNCTION:   --  To evaluate a given move in the move list.
;                 It first makes the move on the board, then if
;                 the move is legal, it evaluates it, and then
;                 restores the board position.
;
; CALLED BY:  --  SORT
;
; CALLS:      --  MOVE
;                 INCHK
;                 PINFND
;                 POINTS
;                 UNMOVE
;
; ARGUMENTS:  --  None
;***********************************************************
EVAL:   CALL    MOVE            ; Make move on the board array
        CALL    INCHK           ; Determine if move is legal
        AND     a             ; Legal move ?
        JR      Z,EV5           ; Yes - jump
        XOR     a             ; Score of zero
        LD      (VALM),a        ; For illegal move
        JP      EV10            ; Jump
EV5:    CALL    PINFND          ; Compile pinned list
        CALL    POINTS          ; Assign points to move
EV10:   CALL    UNMOVE          ; Restore board array
        RET                     ; Return

;***********************************************************
; FIND MOVE ROUTINE
;***********************************************************
; FUNCTION:   --  To determine the computer's best move by
;                 performing a depth first tree search using
;                 the techniques of alpha-beta pruning.
;
; CALLED BY:  --  CPTRMV
;
; CALLS:      --  PINFND
;                 POINTS
;                 GENMOV
;                 SORTM
;                 ASCEND
;                 UNMOVE
;
; ARGUMENTS:  --  None
;***********************************************************
FNDMOV: LD      a,(MOVENO)      ; Current move number
        CP      1             ; First move ?
        CALL    Z,BOOK          ; Yes - execute book opening
        XOR     a             ; Initialize ply number to zero
        LD      (NPLY),a
        LD      hl,0            ; Initialize best move to zero
        LD      (BESTM),hl
        LD      hl,MLIST        ; Initialize ply list pointers
        LD      (MLNXT),hl
        LD      hl,PLYIX-2
        LD      (MLPTRI),hl
        LD      a,(KOLOR)       ; Initialize color
        LD      (COLOR),a
        LD      hl,SCORE        ; Initialize score index
        LD      (SCRIX),hl
        LD      a,(PLYMAX)      ; Get max ply number
        ADD     a,2             ; Add 2
        LD      b,a             ; Save as counter
        XOR     a             ; Zero out score table
back05: LD      (hl),a
        INC     hl
        DJNZ    back05
        LD      (BC0),a         ; Zero ply 0 board control
        LD      (MV0),a         ; Zero ply 0 material
        CALL    PINFND          ; Compile pin list
        CALL    POINTS          ; Evaluate board at ply 0
        LD      a,(BRDC)        ; Get board control points
        LD      (BC0),a         ; Save
        LD      a,(MTRL)        ; Get material count
        LD      (MV0),a         ; Save
FM5:    LD      hl,NPLY         ; Address of ply counter
        INC     (hl)            ; Increment ply count
        XOR     a             ; Initialize mate flag
        LD      (MATEF),a
        CALL    GENMOV          ; Generate list of moves
        LD      a,(NPLY)        ; Current ply counter
        LD      hl,PLYMAX       ; Address of maximum ply number
        CP      (hl)          ; At max ply ?
        CALL    C,SORTM         ; No - call sort
        LD      hl,(MLPTRI)     ; Load ply index pointer
        LD      (MLPTRJ),hl     ; Save as last move pointer
FM15:   LD      hl,(MLPTRJ)     ; Load last move pointer
        LD      e,(hl)          ; Get next move pointer
        INC     hl
        LD      d,(hl)
        LD      a,d
        AND     a             ; End of move list ?
        JR      Z,FM25          ; Yes - jump
        LD      (MLPTRJ),de     ; Save current move pointer
        LD      hl,(MLPTRI)     ; Save in ply pointer list
        LD      (hl),e
        INC     hl
        LD      (hl),d
        LD      a,(NPLY)        ; Current ply counter
        LD      hl,PLYMAX       ; Maximum ply number ?
        CP      (hl)          ; Compare
        JR      C,FM18          ; Jump if not max
        CALL    MOVE            ; Execute move on board array
        CALL    INCHK           ; Check for legal move
        AND     a             ; Is move legal
        JR      Z,rel017        ; Yes - jump
        CALL    UNMOVE          ; Restore board position
        JP      FM15            ; Jump
rel017: LD      a,(NPLY)        ; Get ply counter
        LD      hl,PLYMAX       ; Max ply number
        CP      (hl)          ; Beyond max ply ?
        JR      NZ,FM35         ; Yes - jump
        LD      a,(COLOR)       ; Get current color
        XOR     80H           ; Get opposite color
        CALL    INCHK1          ; Determine if King is in check
        AND     a             ; In check ?
        JR      Z,FM35          ; No - jump
        JP      FM19            ; Jump (One more ply for check)
FM18:   LD      ix,(MLPTRJ)     ; Load move pointer
        LD      a,(ix+MLVAL)    ; Get move score
        AND     a             ; Is it zero (illegal move) ?
        JR      Z,FM15          ; Yes - jump
        CALL    MOVE            ; Execute move on board array
FM19:   LD      hl,COLOR        ; Toggle color
        LD      a,80H
        XOR     (hl)
        LD      (hl),a          ; Save new color
        BIT     7,a             ; Is it white ?
        JR      NZ,rel018       ; No - jump
        LD      hl,MOVENO       ; Increment move number
        INC     (hl)
rel018: LD      hl,(SCRIX)      ; Load score table pointer
        LD      a,(hl)          ; Get score two plys above
        INC     hl              ; Increment to current ply
        INC     hl
        LD      (hl),a          ; Save score as initial value
        DEC     hl              ; Decrement pointer
        LD      (SCRIX),hl      ; Save it
        JP      FM5             ; Jump
FM25:   LD      a,(MATEF)       ; Get mate flag
        AND     a             ; Checkmate or stalemate ?
        JR      NZ,FM30         ; No - jump
        LD      a,(CKFLG)       ; Get check flag
        AND     a             ; Was King in check ?
        LD      a,80H           ; Pre-set stalemate score
        JR      Z,FM36          ; No - jump (stalemate)
        LD      a,(MOVENO)      ; Get move number
        LD      (PMATE),a       ; Save
        LD      a,0FFH          ; Pre-set checkmate score
        JP      FM36            ; Jump
FM30:   LD      a,(NPLY)        ; Get ply counter
        CP      1             ; At top of tree ?
        RET     Z               ; Yes - return
        CALL    ASCEND          ; Ascend one ply in tree
        LD      hl,(SCRIX)      ; Load score table pointer
        INC     hl              ; Increment to current ply
        INC     hl
        LD      a,(hl)          ; Get score
        DEC     hl              ; Restore pointer
        DEC     hl
        JP      FM37            ; Jump
FM35:   CALL    PINFND          ; Compile pin list
        CALL    POINTS          ; Evaluate move
        CALL    UNMOVE          ; Restore board position
        LD      a,(VALM)        ; Get value of move
FM36:   LD      hl,MATEF        ; Set mate flag
        SET     0,(hl)
        LD      hl,(SCRIX)      ; Load score table pointer
FM37:
        CP      (hl)          ; Compare to score 2 ply above
        JR      C,FM40          ; Jump if less
        JR      Z,FM40          ; Jump if equal
        NEG                     ; Negate score
        INC     hl              ; Incr score table pointer
        CP      (hl)          ; Compare to score 1 ply above
        JP      C,FM15          ; Jump if less than
        JP      Z,FM15          ; Jump if equal
        LD      (hl),a          ; Save as new score 1 ply above
        LD      a,(NPLY)        ; Get current ply counter
        CP      1             ; At top of tree ?
        JP      NZ,FM15         ; No - jump
        LD      hl,(MLPTRJ)     ; Load current move pointer
        LD      (BESTM),hl      ; Save as best move pointer
        LD      a,(SCORE+1)     ; Get best move score
        CP      0FFH          ; Was it a checkmate ?
        JP      NZ,FM15         ; No - jump
        LD      hl,PLYMAX       ; Get maximum ply number
        DEC     (hl)            ; Subtract 2
        DEC     (hl)
        LD      a,(KOLOR)       ; Get computer's color
        BIT     7,a             ; Is it white ?
        RET     Z               ; Yes - return
        LD      hl,PMATE        ; Checkmate move number
        DEC     (hl)            ; Decrement
        RET                     ; Return
FM40:   CALL    ASCEND          ; Ascend one ply in tree
        JP      FM15            ; Jump

;***********************************************************
; ASCEND TREE ROUTINE
;***********************************************************
; FUNCTION:  --  To adjust all necessary parameters to
;                ascend one ply in the tree.
;
; CALLED BY: --  FNDMOV
;
; CALLS:     --  UNMOVE
;
; ARGUMENTS: --  None
;***********************************************************
ASCEND: LD      hl,COLOR        ; Toggle color
        LD      a,80H
        XOR     (hl)
        LD      (hl),a          ; Save new color
        BIT     7,a             ; Is it white ?
        JR      Z,rel019        ; Yes - jump
        LD      hl,MOVENO       ; Decrement move number
        DEC     (hl)
rel019: LD      hl,(SCRIX)      ; Load score table index
        DEC     hl              ; Decrement
        LD      (SCRIX),hl      ; Save
        LD      hl,NPLY         ; Decrement ply counter
        DEC     (hl)
        LD      hl,(MLPTRI)     ; Load ply list pointer
        DEC     hl              ; Load pointer to move list top
        LD      d,(hl)
        DEC     hl
        LD      e,(hl)
        LD      (MLNXT),de      ; Update move list avail ptr
        DEC     hl              ; Get ptr to next move to undo
        LD      d,(hl)
        DEC     hl
        LD      e,(hl)
        LD      (MLPTRI),hl     ; Save new ply list pointer
        LD      (MLPTRJ),de     ; Save next move pointer
        CALL    UNMOVE          ; Restore board to previous ply
        RET                     ; Return

;***********************************************************
; ONE MOVE BOOK OPENING
; **********************************************************
; FUNCTION:   --  To provide an opening book of a single
;                 move.
;
; CALLED BY:  --  FNDMOV
;
; CALLS:      --  None
;
; ARGUMENTS:  --  None
;***********************************************************
BOOK:   POP     af              ; Abort return to FNDMOV
        LD      hl,SCORE+1      ; Zero out score
        LD      (hl),0          ; Zero out score table
        LD      hl,BMOVES-2     ; Init best move ptr to book
        LD      (BESTM),hl
        LD      hl,BESTM        ; Initialize address of pointer
        LD      a,(KOLOR)       ; Get computer's color
        AND     a             ; Is it white ?
        JR      NZ,BM5          ; No - jump
        LD      a,r             ; Load refresh reg (random no)
        BIT     0,a             ; Test random bit
        RET     Z               ; Return if zero (P-K4)
        INC     (hl)            ; P-Q4
        INC     (hl)
        INC     (hl)
        RET                     ; Return
BM5:    INC     (hl)            ; Increment to black moves
        INC     (hl)
        INC     (hl)
        INC     (hl)
        INC     (hl)
        INC     (hl)
        LD      ix,(MLPTRJ)     ; Pointer to opponents 1st move
        LD      a,(ix+MLFRP)    ; Get "from" position
        CP      22            ; Is it a Queen Knight move ?
        JR      Z,BM9           ; Yes - Jump
        CP      27            ; Is it a King Knight move ?
        JR      Z,BM9           ; Yes - jump
        CP      34            ; Is it a Queen Pawn ?
        JR      Z,BM9           ; Yes - jump
        RET     C               ; If Queen side Pawn opening -
                                ; return (P-K4)
        CP      35            ; Is it a King Pawn ?
        RET     Z               ; Yes - return (P-K4)
BM9:    INC     (hl)            ; (P-Q4)
        INC     (hl)
        INC     (hl)
        RET                     ; Return to CPTRMV

;*******************************************************
; GRAPHICS DATA BASE
;*******************************************************
; DESCRIPTION:  The Graphics Data Base contains the
;               necessary stored data to produce the piece
;               on the board. Only the center 4 x 4 blocks are
;               stored and only for a Black Piece on a White
;               square. A White piece on a black square is
;               produced by complementing each block, and a
;               piece on its own color square is produced
;               by moving in a kernel of 6 blocks.
;*******************************************************
        ORG     START+384
BLBASE  EQU     START+512
BLOCK   EQU     080H ; $-BLBASE     -128
        DB      80H,80H,80H,80H ; Black Pawn on White square
        DB      80H,0A0H,90H,80H
        DB      80H,0AFH,9FH,80H
        DB      80H,83H,83H,80H
        DB      80H,0B0H,0B0H,80H ; Black Knight on White square
        DB      0BEH,0BFH,0BFH,95H
        DB      0A0H,0BEH,0BFH,85H
        DB      83H,83H,83H,81H
        DB      80H,0A0H,00H,80H ; Black Bishop on White square
        DB      0A8H,0BFH,0BDH,80H
        DB      82H,0AFH,87H,80H
        DB      82H,83H,83H,80H
        DB      80H,80H,80H,80H ; Black Rook on White square
        DB      8AH,0BEH,0BDH,85H
        DB      80H,0BFH,0BFH,80H
        DB      82H,83H,83H,81H
        DB      90H,80H,80H,90H ; Black Queen on White square
        DB      0BFH,0B4H,0BEH,95H
        DB      8BH,0BFH,9FH,81H
        DB      83H,83H,83H,81H
        DB      80H,0B8H,90H,80H ; Black King on White square
        DB      0BCH,0BAH,0B8H,94H
        DB      0AFH,0BFH,0BFH,85H
        DB      83H,83H,83H,81H
        DB      90H,0B0H,0B0H,80H ; Toppled Black King
        DB      0BFH,0BFH,0B7H,80H
        DB      9FH,0BFH,0BDH,80H
        DB      80H,80H,88H,9DH
KERNEL  EQU     0F0H ; $-BLBASE
        DB      0BFH,9FH,0AFH,0BFH,9AH,0A5H ; Pawn Kernel
        DB      89H,0AFH,0BFH,9FH,0B9H,9FH ; Knight Kernel
        DB      97H,0BEH,96H,0BDH,9BH,0B9H ; Bishop Kernel
        DB      0B5H,0A1H,92H,0BFH,0AAH,95H ; Rook Kernel
        DB      0A8H,9BH,0B9H,0B6H,0AFH,0A7H ; Queen Kernel
        DB      0A3H,85H,0A7H,9AH,0BFH,9FH ; King Kernel
        DB      0A8H,0BFH,89H,0A2H,8FH,86H ; Toppled King Kernel

;*******************************************************
; STANDARD MESSAGES
;*******************************************************
        ORG     START+1800H
GRTTNG:  DB      "WELCOME TO SARGON CHESS! CARE FOR A GAME?"
ANAMSG:  DB      "WOULD YOU LIKE TO ANALYZE A POSITION?"
CLRMSG:  DB	 13,10
	 DB      "DO YOU WANT TO PLAY WHITE(w) OR BLACK(b)?"
TITLE1:  DB      "SARGON"
TITLE2:  DB      "PLAYER"
SPACE:   DB      "          "    ; For output of blank area
MVENUM:  DB      "01 "
TITLE3:  DB      "  ====== ======"
MVEMSG:  DB      "a1-a1"
O_O:     DB      "0-0  "
O_O_O:   DB      "0-0-0"
CKMSG:   DB      "CHECK"
MTMSG:   DB      "MATE IN "
MTPL:    DB      "2"
PCS:     DB      "KQRBNP"        ; Valid piece characters
UWIN:    DB      "YOU WIN"
IWIN:    DB      "I WIN"
AGAIN:   DB      "CARE FOR ANOTHER GAME?"
CRTNES:  DB      "IS THIS RIGHT?"
PLYDEP:  DB      "SELECT LOOK AHEAD (1-6)"
TITLE4:  DB      "                "
WSMOVE:  DB      "WHOSE MOVE IS IT?"
BLANKR:  DB      '[',1CH,']'     ; Control-\
P_PEP:   DB      "PxPep"
INVAL1:  DB      "INVALID MOVE"
INVAL2:  DB      "TRY AGAIN"

;*******************************************************
; VARIABLES
;*******************************************************
BRDPOS:  DS      1               ; Index into the board array
ANBDPS:  DS      1               ; Additional index required for ANALYS
INDXER:  DW      BLBASE          ; Index into graphics data base
NORMAD:  DS      2               ; The address of the upper left hand
                                ; corner of the square on the board
LINECT:  DB      0               ; Current line number

;*******************************************************
; MACRO DEFINITIONS
;*******************************************************
; All input/output to SARGON is handled in the form of
; macro calls to simplify conversion to alternate systems.
; All of the input/output macros conform to the Jove monitor
; of the Jupiter III computer.
;*******************************************************
;*** OUTPUT <CR><LF> ***
	MACRO	CARRET
        exx
        ex af, af'
        ld  c, BDOS_Console_Output
        ld  e, 13
        call BDOS
        ld  c, BDOS_Console_Output
        ld  e, 10
        call BDOS
        ex af, af'
        exx
        ENDM

space:
        exx
        ex af, af'
        ld a, ' '
        call print_a
        ex af, af'
        exx
        ret

;*** PRINT ANY LINE (NAME, LENGTH) ***
	MACRO	PRTLIN  NAME,LNGTH
        exx
        ex af, af'
        ld b, LNGTH
        ld hl, NAME
1:
        ld e, (hl)
        inc hl
        ld c, BDOS_Console_Output
        push hl
        push bc
        call BDOS
        pop bc
        pop hl
        djnz 1b
        ld a, 13
        call print_a
        ld a, 10
        call print_a
        ex af, af'
        exx
        ENDM

;*** PRINT ANY BLOCK (NAME, LENGTH) ***
	MACRO	PRTBLK  NAME,LNGTH
        exx
        ex af, af'
        ld b, LNGTH
        ld hl, NAME
2:
        ld e, (hl)
        inc hl
        ld c, BDOS_Console_Output
        push hl
        push bc
        call BDOS
        pop bc
        pop hl
        djnz 2b
        ex af, af'
        exx
        ENDM

;*** EXIT TO MONITOR ***
	MACRO	EXIT
        jp      0
        ENDM

;***********************************************************
; MAIN PROGRAM DRIVER
;***********************************************************
; FUNCTION:   --  To coordinate the game moves.
;
; CALLED BY:  --  None
;
; CALLS:      --  INTERR
;                 INITBD
;                 DSPBRD
;                 CPTRMV
;                 PLYRMV
;                 TBCPCL
;                 PGIFND
;
; MACRO CALLS:    CLRSCR
;                 CARRET
;                 PRTLIN
;                 PRTBLK
;
; ARGUMENTS:      None
;***********************************************************
        ORG     START+1A00H     ; Above the move logic
DRIVER: LD      sp,STACK        ; Set stack pointer
        call    CLRSCR          ; Blank out screen
        ld      de, copyright_message
        call    show_string_de
        PRTLIN  GRTTNG,41       ; Output greeting
DRIV01: CALL    CHARTR          ; Accept answer
        CARRET                  ; New line
        CP      'N'             ; Is it a 'N' ?
        JP      Z,ANALYS        ; If so then jump to ANALYSing a position

        SUB     a               ; Code of White is zero
        LD      (COLOR),a       ; White always moves first

	ld	de,HELP		; Output help
	call	show_string_de

	ld	de,LoadGameMsg	; Ask 'load saved game?'
	call	show_string_de

        CALL    CHARTR          ; Accept response
	cp	'Y'
	jr	nz,dontload
loadgame:				; Load a saved game...
	ld	de,GameNrMsg	; Ask 'game nr'
	call	show_string_de

        CALL    CHARTR          ; Accept response
	cp	'0'
	jr	nc,ok1
errnr:
	ld	de,WrongNumber
	call	show_string_de
	jr	loadgame
ok1:
	cp	'9'+1
	jr	nc,errnr

	call	LoadGame
	call	CLRSCR
	jr	2f
dontload:
        LD      a,1             ; Move number is 1 at at start
        LD      (MOVENO),a      ; Save
        LD      (LINECT),a      ; Line number is one at start
        LD      hl,MVENUM       ; Address of ascii move number
        LD      (hl),30H        ; Init to '01 '
        INC     hl
        LD      (hl),31H
        INC     hl
        LD      (hl),20H

        CALL    INTERR          ; Players color/search depth
        call    CLRSCR
        CALL    INITBD          ; Initialize board array
2:
        CALL    DSPBRD          ; Set up graphics board
        PRTLIN  TITLE4,15       ; Put up player headings
        PRTLIN  TITLE3,15
DRIV04: PRTBLK  MVENUM,3        ; Display move number
        LD      a,(KOLOR)       ; Bring in computer's color
        AND     a               ; Is it white ?
        JR      NZ,DR08         ; No - jump
        CALL    PGIFND          ; New page if needed
        CP      1               ; Was page turned ?
        CALL    Z,TBCPCL        ; Yes - Tab to computers column

	ld	hl,LoadFlag	;check if game was loaded
	ld	a,(hl)
	ld	(hl),0
	or	a
	jr	z,domove
				;if yes, skip computer's move
	PRTBLK	SPACE,5
	jr	skip
domove:
        CALL    CPTRMV          ; Make and write computers move
skip:
        PRTBLK  SPACE,1         ; Output a space
        CALL    PLYRMV          ; Accept and make players move
        CARRET                  ; New line
        JR      DR0C            ; Jump
DR08:   CALL    PLYRMV          ; Accept and make players move
        PRTBLK  SPACE,1         ; Output a space
        CALL    PGIFND          ; New page if needed
        CP      1             ; Was page turned ?
        CALL    Z,TBCPCL        ; Yes - Tab to computers column
        CALL    CPTRMV          ; Make and write computers move
        CARRET                  ; New line
DR0C:   LD      hl,MVENUM+2     ; Addr of 3rd char of move
        LD      a,20H           ; Ascii space
        CP      (hl)          ; Is char a space ?
        LD      a,3AH           ; Set up test value
        JR      Z,DR10          ; Yes - jump
        INC     (hl)            ; Increment value
        CP      (hl)          ; Over Ascii 9 ?
        JR      NZ,DR14         ; No - jump
        LD      (hl),30H        ; Set char to zero
DR10:   DEC     hl              ; 2nd char of Ascii move no.
        INC     (hl)            ; Increment value
        CP      (hl)          ; Over Ascii 9 ?
        JR      NZ,DR14         ; No - jump
        LD      (hl),30H        ; Set char to zero
        DEC     hl              ; 1st char of Ascii move no.
        INC     (hl)            ; Increment value
        CP      (hl)          ; Over Ascii 9 ?
        JR      NZ,DR14         ; No - jump
        LD      (hl),31H        ; Make 1st char a one
        LD      a,30H           ; Make 3rd char a zero
        LD      (MVENUM+2),a
DR14:   LD      hl,MOVENO       ; Hexadecimal move number
        INC     (hl)            ; Increment
        JP      DRIV04          ; Jump

;***********************************************************
; INTERROGATION FOR PLY & COLOR
;***********************************************************
; FUNCTION:   --  To query the player for his choice of ply
;                 depth and color.
;
; CALLED BY:  --  DRIVER
;
; CALLS:      --  CHARTR
;
; MACRO CALLS:    PRTLIN
;                 CARRET
;
; ARGUMENTS:  --  None
;***********************************************************
INTERR: PRTLIN  CLRMSG,43       ; Request color choice
        CALL    CHARTR          ; Accept response
        CARRET                  ; New line
        CP      'B'             ; Did player request black ?
        JR      NZ,IN04          ; Yes - branch
        SUB     a               ; Set computers color to white
        LD      (KOLOR),a
        LD      hl,TITLE1       ; Prepare move list titles
        LD      de,TITLE4+2
        LD      bc,6
        LDIR
        LD      hl,TITLE2
        LD      de,TITLE4+9
        LD      bc,6
        LDIR
        JR      IN08            ; Jump
IN04:   LD      a,80H           ; Set computers color to black
        LD      (KOLOR),a
        LD      hl,TITLE2       ; Prepare move list titles
        LD      de,TITLE4+2
        LD      bc,6
        LDIR
        LD      hl,TITLE1
        LD      de,TITLE4+9
        LD      bc,6
        LDIR
IN08:   PRTLIN  PLYDEP,23       ; Request depth of search
        CALL    CHARTR          ; Accept response
        CARRET                  ; New line
        LD      hl,PLYMAX       ; Address of ply depth variabl
        LD      (hl),2          ; Default depth of search
        CP      31H           ; Under minimum of 1 ?
        RET     M               ; Yes - return
        CP      37H           ; Over maximum of 6 ?
        RET     P               ; Yes - return
        SUB     30H           ; Subtract Ascii constant
        LD      (hl),a          ; Set desired depth
        RET                     ; Return

;***********************************************************
; COMPUTER MOVE ROUTINE
;***********************************************************
; FUNCTION:   --  To control the search for the computers move
;                 and the display of that move on the board
;                 and in the move list.
;
; CALLED BY:  --  DRIVER
;
; CALLS:      --  FNDMOV
;                 FCDMAT
;                 MOVE
;                 EXECMV
;                 BITASN
;                 INCHK
;
; MACRO CALLS:    PRTBLK
;                 CARRET
;
; ARGUMENTS:  --  None
;***********************************************************
CPTRMV: CALL    FNDMOV          ; Select best move
        LD      hl,(BESTM)      ; Move list pointer variable
        LD      (MLPTRJ),hl     ; Pointer to move data
        LD      a,(SCORE+1)     ; To check for mates
        CP      1               ; Mate against computer ?
        JR      NZ, CP0C        ; No
        LD      c,1             ; Computer mate flag
        CALL    FCDMAT          ; Full checkmate ?
CP0C:   CALL    MOVE            ; Produce move on board array
        CALL    EXECMV          ; Make move on graphics board
                                ; and return info about it
        LD      a,b             ; Special move flags
        AND     a               ; Special ?
        JR      NZ,CP10         ; Yes - jump
        LD      d,e             ; "To" position of the move
        CALL    BITASN          ; Convert to Ascii
        LD      (MVEMSG+3),hl   ; Put in move message
        LD      d,c             ; "From" position of the move
        CALL    BITASN          ; Convert to Ascii
        LD      (MVEMSG),hl     ; Put in move message
        PRTBLK  MVEMSG,5        ; Output text of move
        JR      CP1C            ; Jump
CP10:   BIT     1,b             ; King side castle ?
        JR      Z,rel020        ; No - jump
        PRTBLK  O_O,5           ; Output "O-O"
        JR      CP1C            ; Jump
rel020: BIT     2,b             ; Queen side castle ?
        JR      Z,rel021        ; No - jump
        PRTBLK  O_O_O,5         ; Output "O-O-O"
        JR      CP1C            ; Jump
rel021: PRTBLK  P_PEP,5         ; Output "PxPep" - En passant
CP1C:   LD      a,(COLOR)       ; Should computer call check ?
        LD      b,a
        XOR     80H           ; Toggle color
        LD      (COLOR),a
        CALL    INCHK           ; Check for check
        AND     a             ; Is enemy in check ?
        LD      a,b             ; Restore color
        LD      (COLOR),a
        JR      Z,CP24          ; No - return
        CARRET                  ; New line
        LD      a,(SCORE+1)     ; Check for player mated
        CP      0FFH          ; Forced mate ?
        CALL    NZ,TBCPMV       ; No - Tab to computer column
        PRTBLK  CKMSG,5         ; Output "check"
        LD      hl,LINECT       ; Address of screen line count
        INC     (hl)            ; Increment for message
CP24:   LD      a,(SCORE+1)     ; Check again for mates
        CP      0FFH            ; Player mated ?
        RET     NZ              ; No - return
        LD      c,0             ; Set player mate flag
        CALL    FCDMAT          ; Full checkmate ?
        RET                     ; Return

;***********************************************************
; FORCED MATE HANDLING
;***********************************************************
; FUNCTION:   --  To examine situations where there exits
;                 a forced mate and determine whether or
;                 not the current move is checkmate. If it is,
;                 a losing player is offered another game,
;                 while a loss for the computer signals the
;                 King to tip over in resignation.
;
; CALLED BY:  --  CPTRMV
;
; CALLS:      --  MATED
;                 CHARTR
;                 TBPLMV
;
; ARGUMENTS:  --  The only value passed in a register is the
;                 flag which tells FCDMAT whether the computer
;                 or the player is mated.
;***********************************************************
FCDMAT: LD      a,(MOVENO)      ; Current move number
        LD      b,a             ; Save
        LD      a,(PMATE)       ; Move number where mate occurs
        SUB     b               ; Number of moves till mate
        AND     a               ; Checkmate ?
        JP      NZ,FM0C         ; No - jump
        BIT     0,c             ; Check flag for who is mated
        JP      Z,FM04          ; Jump if player
        CARRET                  ; New line
        PRTLIN  CKMSG,9         ; Print "CHECKMATE"
        CALL    MATED           ; Tip over King
        PRTLIN  UWIN,7          ; Output "YOU WIN"
        JR      FM08            ; Jump
FM04:   PRTLIN  MTMSG,4         ; Output "MATE"
        PRTLIN  IWIN,5          ; Output "I WIN"
FM08:   POP     hl              ; Remove return addresses
        POP     hl
        CALL    CHARTR          ; Input any char to play again
FM09:   call CLRSCR                  ; Blank screen
        PRTLIN  AGAIN,22        ; "CARE FOR ANOTHER GAME?"
        JP      DRIV01          ; Jump (Rest of game init)
FM0C:   BIT     0,c             ; Who has forced mate ?
        RET     NZ              ; Return if player
        CARRET                  ; New line
        ADD     a,30H           ; Number of moves to Ascii
        LD      (MTPL),a        ; Place value in message
        PRTLIN  MTMSG,9         ; Output "MATE IN x MOVES"
        CALL    TBPLMV          ; Tab to players column
        RET                     ; Return

;***********************************************************
; TAB TO PLAYERS COLUMN
;***********************************************************
; FUNCTION:   --  To space over in the move listing to the
;                 column in which the players moves are being
;                 recorded. This routine also reprints the
;                 move number.
;
; CALLED BY:  --  PLYRMV
;
; CALLS:      --  None
;
; MACRO CALLS:    PRTBLK
;
; ARGUMENTS:  --  None
;***********************************************************
TBPLCL: PRTBLK  MVENUM,3        ; Reproduce move number
        LD      a,(KOLOR)       ; Computers color
        AND     a             ; Is computer white ?
        RET     NZ              ; No - return
        PRTBLK  SPACE,6         ; Tab to next column
        RET                     ; Return

;***********************************************************
; TAB TO COMPUTERS COLUMN
;***********************************************************
; FUNCTION:   --  To space over in the move listing to the
;                 column in which the computers moves are
;                 being recorded. This routine also reprints
;                 the move number.
;
; CALLED BY:  --  DRIVER
;                 CPTRMV
;
; CALLS:      --  None
;
; MACRO CALLS:    PRTBLK
;
; ARGUMENTS:  --  None
;***********************************************************
TBCPCL: PRTBLK  MVENUM,3        ; Reproduce move number
        LD      a,(KOLOR)       ; Computer's color
        AND     a             ; Is computer white ?
        RET     Z               ; Yes - return
        PRTBLK  SPACE,6         ; Tab to next column
        RET                     ; Return

;***********************************************************
; TAB TO PLAYERS COLUMN W/0 MOVE NO.
;***********************************************************
; FUNCTION:   --  Like TBPLCL, except that the move number
;                 is not reprinted.
;
; CALLED BY:  --  FCDMAT
;***********************************************************
TBPLMV: PRTBLK  SPACE,3
        LD      a,(KOLOR)
        AND     a
        RET     NZ
        PRTBLK  SPACE,6
        RET

;***********************************************************
; TAB TO COMPUTERS COLUMN W/O MOVE NO.
;***********************************************************
; FUNCTION:   --  Like TBCPCL, except that the move number
;                 is not reprinted.
;
; CALLED BY:  --  CPTRMV
;***********************************************************
TBCPMV: PRTBLK  SPACE,3
        LD      a,(KOLOR)
        AND     a
        RET     Z
        PRTBLK  SPACE,6
        RET

;***********************************************************
; BOARD INDEX TO ASCII SQUARE NAME
;***********************************************************
; FUNCTION:   --  To translate a hexadecimal index in the
;                 board array into an ascii description
;                 of the square in algebraic chess notation.
;
; CALLED BY:  --  CPTRMV
;
; CALLS:      --  DIVIDE
;
; ARGUMENTS:  --  Board index input in register D and the
;                 Ascii square name is output in register
;                 pair HL.
;***********************************************************
BITASN: SUB     a             ; Get ready for division
        LD      e,10
        CALL    DIVIDE          ; Divide
        DEC     d               ; Get rank on 1-8 basis
        ADD     a,60H           ; Convert file to Ascii (a-h)
        LD      l,a             ; Save
        LD      a,d             ; Rank
        ADD     a,30H           ; Convert rank to Ascii (1-8)
        LD      h,a             ; Save
        RET                     ; Return

;***********************************************************
; PLAYERS MOVE ANALYSIS
;***********************************************************
; FUNCTION:   --  To accept and validate the players move
;                 and produce it on the graphics board. Also
;                 allows player to resign the game by
;                 entering a control-R.
;
; CALLED BY:  --  DRIVER
;
; CALLS:      --  CHARTR
;                 ASNTBI
;                 VALMOV
;                 EXECMV
;                 PGIFND
;                 TBPLCL
;
; ARGUMENTS:  --  None
;***********************************************************
PLYRMV: CALL    CHARTR          ; Accept "from" file letter
	cp	13H		; Is it CTRL^S ?
	jp	z,SaveGame
        CP      12H             ; Is it instead a Control-R ?
        JP      Z,FM09          ; Yes - jump
        LD      h,a             ; Save
        CALL    CHARTR          ; Accept "from" rank number
        LD      l,a             ; Save
        CALL    ASNTBI          ; Convert to a board index
        SUB     b               ; Gives board index, if valid
        JR      Z,PL08          ; Jump if invalid
        LD      (MVEMSG),a      ; Move list "from" position
        CALL    CHARTR          ; Accept separator & ignore it (TODO: Let's not do this, they can type E2E4 surely???)
        CALL    CHARTR          ; Repeat for "to" position
        LD      h,a
        CALL    CHARTR
        LD      l,a
        CALL    ASNTBI
        SUB     b
        JR      Z,PL08
        LD      (MVEMSG+1),a    ; Move list "to" position
        CALL    VALMOV          ; Determines if a legal move
        AND     a               ; Legal ?
        JP      NZ,PL08         ; No - jump
        CALL    EXECMV          ; Make move on graphics board
        RET                     ; Return
PL08:   LD      hl,LINECT       ; Address of screen line count
        INC     (hl)            ; Increase by 2 for message
        INC     (hl)
        CARRET                  ; New line
        CALL    PGIFND          ; New page if needed
        PRTLIN  INVAL1,12       ; Output "INVALID MOVE"
        PRTLIN  INVAL2,9        ; Output "TRY AGAIN"
        CALL    TBPLCL          ; Tab to players column
        JP      PLYRMV          ; Jump

;***********************************************************
; ASCII SQUARE NAME TO BOARD INDEX
;***********************************************************
; FUNCTION:   --  To convert an algebraic square name in
;                 Ascii to a hexadecimal board index.
;                 This routine also checks the input for
;                 validity.
;
; CALLED BY:  --  PLYRMV
;
; CALLS:      --  MLTPLY
;
; ARGUMENTS:  --  Accepts the square name in register pair HL
;                 and outputs the board index in register A.
;                 Register B = 0 if ok. Register B = Register
;                 A if invalid.
;***********************************************************
ASNTBI: 
        LD      a,l             ; Ascii rank (1 - 8)
        SUB     30H           ; Rank 1 - 8
        CP      1             ; Check lower bound
        JP      M,AT04          ; Jump if invalid
        CP      9             ; Check upper bound
        JR      NC,AT04         ; Jump if invalid
        INC     a               ; Rank 2 - 9
        LD      d,a             ; Ready for multiplication
        LD      e,10
        CALL    MLTPLY          ; Multiply
        LD      a,h             ; Ascii file letter (a - h)
        SUB     40H           ; File 1 - 8
        CP      1             ; Check lower bound
        JP      M,AT04          ; Jump if invalid
        CP      9             ; Check upper bound
        JR      NC,AT04         ; Jump if invalid
        ADD     a,d             ; File+Rank(20-90)=Board index
        LD      b,0             ; Ok flag
        RET                     ; Return
AT04:   LD      b,a             ; Invalid flag
        RET                     ; Return

;***********************************************************
; VALIDATE MOVE SUBROUTINE
;***********************************************************
; FUNCTION:   --  To check a players move for validity.
;
; CALLED BY:  --  PLYRMV
;
; CALLS:      --  GENMOV
;                 MOVE
;                 INCHK
;                 UNMOVE
;
; ARGUMENTS:  --  Returns flag in register A, 0 for valid
;                 and 1 for invalid move.
;***********************************************************
VALMOV: LD      hl,(MLPTRJ)     ; Save last move pointer
        PUSH    hl              ; Save register
        LD      a,(KOLOR)       ; Computers color
        XOR     80H           ; Toggle color
        LD      (COLOR),a       ; Store
        LD      hl,PLYIX-2      ; Load move list index
        LD      (MLPTRI),hl
        LD      hl,MLIST+1024   ; Next available list pointer
        LD      (MLNXT),hl
        CALL    GENMOV          ; Generate opponents moves
        LD      ix,MLIST+1024   ; Index to start of moves
VA5:    LD      a,(MVEMSG)      ; "From" position
        CP      (ix+MLFRP)    ; Is it in list ?
        JR      NZ,VA6          ; No - jump
        LD      a,(MVEMSG+1)    ; "To" position
        CP      (ix+MLTOP)    ; Is it in list ?
        JR      Z,VA7           ; Yes - jump
VA6:    LD      e,(ix+MLPTR)    ; Pointer to next list move
        LD      d,(ix+MLPTR+1)
        XOR     a             ; At end of list ?
        CP      d
        JR      Z,VA10          ; Yes - jump
        PUSH    de              ; Move to X register
        POP     ix
        JR      VA5             ; Jump
VA7:    LD      (MLPTRJ),ix     ; Save opponents move pointer
        CALL    MOVE            ; Make move on board array
        CALL    INCHK           ; Was it a legal move ?
        AND     a
        JR      NZ,VA9          ; No - jump
VA8:    POP     hl              ; Restore saved register
        RET                     ; Return
VA9:    CALL    UNMOVE          ; Un-do move on board array
VA10:   LD      a,1             ; Set flag for invalid move
        POP     hl              ; Restore saved register
        LD      (MLPTRJ),hl     ; Save move pointer
        RET                     ; Return

;***********************************************************
; ACCEPT INPUT CHARACTER
;***********************************************************
; FUNCTION:   --  Accepts a single character input from the
;                 console keyboard and places it in the A
;                 register. The character is also echoed on
;                 the video screen, unless it is a carriage
;                 return, line feed, or backspace.
;                 Lower case alphabetic characters are folded
;                 to upper case.
;
; CALLED BY:  --  DRIVER
;                 INTERR
;                 PLYRMV
;                 ANALYS
;
; CALLS:      --  None
;
; ARGUMENTS:  --  Character input is output in register A.
;
; NOTES:      --  This routine contains a reference to a
;                 monitor function of the Jove monitor, there-
;                 for the first few lines of this routine are
;                 system dependent.
;***********************************************************
CHARTR: 
        push    hl
CHARTR1: 
        ld      c, BDOS_Console_Direct
        ld      e, 0FFH         ; Use non-echo key read
        call    BDOS            ; Read key from keyboard (if none we will get 0)
        cp      0
        jr      z, CHARTR1 
        CP      0DH             ; Carriage return ?
        jr      Z, CHARTR2      ; Yes - return
        CP      0AH             ; Line feed ?
        jr      Z, CHARTR2      ; Yes - return
        CP      08H             ; Backspace ?
        jr      Z, CHARTR2      ; Yes - return
        push af
        ld      c, BDOS_Console_Output
        ld      e, a
        call    BDOS            ; Print the characer to screen
        pop af
        AND     7FH           ; Mask off parity bit
        CP      7BH           ; Upper range check (z+l)
        jp      P, CHARTR2               ; No need to fold - return
        CP      61H           ; Lower-range check (a)
        jp     M, CHARTR2               ; No need to fold - return
        SUB     20H           ; Change to one of A-Z
CHARTR2:
        pop     hl
        RET                     ; Return

;***********************************************************
; NEW PAGE IF NEEDED
;***********************************************************
; FUNCTION:   --  To clear move list output when the column
;                 has been filled.
;
; CALLED BY:  --  DRIVER
;                 PLYRMV
;                 CPTRMV
;
; CALLS:     --   DSPBRD
;
; ARGUMENTS: --   Returns a 1 in the A register if a new
;                 page was turned.
;***********************************************************
PGIFND: LD      hl,LINECT       ; Addr of page position counter
        INC     (hl)            ; Increment
        LD      a,20            ; Page bottom ?
        CP      (hl)
        RET     NC              ; No - return
        call    CLRSCR
        CALL    DSPBRD          ; Put up new page
        PRTLIN  TITLE4,15       ; Re-print titles
        PRTLIN  TITLE3,15
        LD      a,1             ; Set line count back to 1
        LD      (LINECT),a
        RET                     ; Return

;***********************************************************
; DISPLAY MATED KING
;***********************************************************
; FUNCTION:   --  To tip over the computers King when
;                 mated.
;
; CALLED BY:  --  FCDMAT
;
; CALLS:      --  CONVRT
;                 BLNKER
;                 INSPCE  (Abnormal Call to IP04)
;
; ARGUMENTS:  --  None
;***********************************************************
MATED:  call    SAVE_CURSOR
        LD      a,(KOLOR)       ; Computers color
        AND     a               ; Is computer white ?
        JR      Z,rel23         ; Yes - skip
        LD      c,2             ; Set black piece flag
        LD      a,(POSK+1)      ; Position of black King
        JR      MA08            ; Jump
rel23:  LD      c,a             ; Clear black piece flag
        LD      a,(POSK)        ; Position of white King
MA08:   LD      (BRDPOS),a      ; Store King position
        LD      (ANBDPS),a      ; Again

        LD      (M1),a          ; Set up board index
        LD      ix,(M1)
        ld      a,7+128
        LD      (ix+BOARD), a   ; Set mated king

        CALL    CONVRT          ; Getting norm address in HL
        call    blink_square

        call    RESTORE_CURSOR
        RET                     ; Return

;***********************************************************
; SET UP POSITION FOR ANALYSIS
;***********************************************************
; FUNCTION:   --  To enable user to set up any position
;                 for analysis, or to continue to play
;                 the game. The routine blinks the board
;                 squares in turn and the user has the option
;                 of leaving the contents unchanged by a
;                 carriage return, emptying the square by a 0,
;                 or inputting a piece of his chosing. To
;                 enter a piece, type in piece-code,color-code,
;                 moved-code.
;
;                 Piece-code is a letter indicating the
;                 desired piece:
;                       K  -  King
;                       Q  -  Queen
;                       R  -  Rook
;                       B  -  Bishop
;                       N  -  Knight
;                       P  -  Pawn
;
;                 Color code is a letter, W for white, or B for
;                 black.
;
;                 Moved-code is a number. 0 indicates the piece has never
;                 moved. 1 indicates the piece has moved.
;
;                 A backspace will back up in the sequence of blinked
;                 squares. An Escape will terminate the blink cycle and
;                 verify that the position is correct, then procede
;                 with game initialization.
;
; CALLED BY:  --  DRIVER
;
; CALLS:      --  CHARTR
;                 DPSBRD
;                 BLNKER
;                 ROYALT
;                 PLYRMV
;                 CPTRMV
;
; MACRO CALLS:    PRTLIN
;                 EXIT
;                 CLRSCR
;                 PRTBLK
;                 CARRET
;
; ARGUMENTS:  --  None
;***********************************************************
ANALYS: PRTLIN  ANAMSG,37       ; "CARE TO ANALYSE A POSITION?"
        CALL    CHARTR          ; Accept answer
        CARRET                  ; New line
        CP      4EH           ; Is answer a "N" ?
        JR      NZ,AN04         ; No - jump
        EXIT                    ; Return to monitor
AN04:   CALL    DSPBRD          ; Current board position
        LD      a,21            ; First board index
AN08:   LD      (ANBDPS),a      ; Save
        LD      (BRDPOS),a
        CALL    CONVRT          ; Norm address into HL register
        LD      (M1),a          ; Set up board index
        LD      ix,(M1)
        LD      a,(ix+BOARD)    ; Get board contents
        CP      0FFH            ; Border square ?
        JR      Z,AN19          ; Yes - jump

        call    blink_square

        CALL    CHARTR          ; Accept input
        CP      1BH             ; Is it an escape ?
        JR      Z,AN1B          ; Yes - jump
        CP      08H             ; Is it a backspace ?
        JR      Z,AN1A          ; Yes - jump
        CP      0DH             ; Is it a carriage return ?
        JR      Z,AN19          ; Yes - jump
        LD      bc,7            ; Number of types of pieces + 1
        LD      hl,PCS          ; Address of piece symbol table
        CPIR                    ; Search
        JR      NZ,AN18         ; Jump if not found
        CALL    CHARTR          ; Accept and ignore separator
        CALL    CHARTR          ; Color of piece
        CP      42H             ; Is it black ?
        JR      NZ,rel022       ; No - skip
        SET     7,c             ; Black piece indicator
rel022: CALL    CHARTR          ; Accept and ignore separator
        CALL    CHARTR          ; Moved flag
        CP      31H             ; Has piece moved ?
        JR      NZ,AN18         ; No - jump
        SET     3,c             ; Set moved indicator
AN18:   LD      (ix+BOARD),c    ; Insert piece into board array
        CALL    DSPBRD          ; Update graphics board
AN19:   LD      a,(ANBDPS)      ; Current board position
        INC     a               ; Next
        CP      99            ; Done ?
        JR      NZ,AN08         ; No - jump
        JR      AN04            ; Jump
AN1A:   LD      a,(ANBDPS)      ; Prepare to go back a square
        SUB     3             ; To get around border
        CP      20            ; Off the other end ?
        JP      NC,AN08         ; No - jump
        LD      a,98            ; Wrap around to top of screen
AN0B:   JP      AN08            ; Jump
AN1B:   PRTLIN  CRTNES,14       ; Ask if correct
        CALL    CHARTR          ; Accept answer
        CP      4EH           ; Is it "N" ?
        JP      Z,AN04          ; No - jump
        CALL    ROYALT          ; Update positions of royalty
        call    CLRSCR                  ; Blank screen
        CALL    INTERR          ; Accept color choice
AN1C:   PRTLIN  WSMOVE,17       ; Ask whose move it is
        CALL    CHARTR          ; Accept response
        CALL    DSPBRD          ; Display graphics board
        PRTLIN  TITLE4,15       ; Put up titles
        PRTLIN  TITLE3,15
        CP      57H           ; Is is whites move ?
        JP      Z,DRIV04        ; Yes - jump
        PRTBLK  MVENUM,3        ; Print move number
        PRTBLK  SPACE,6         ; Tab to blacks column
        LD      a,(KOLOR)       ; Computer's color
        AND     a             ; Is computer white ?
        JR      NZ,AN20         ; No - jump
        CALL    PLYRMV          ; Get players move
        CARRET                  ; New line
        JP      DR0C            ; Jump
AN20:   CALL    CPTRMV          ; Get computers move
        CARRET                  ; New line
        JP      DR0C            ; Jump

;***********************************************************
; UPDATE POSITIONS OF ROYALTY
;***********************************************************
; FUNCTION:   --  To update the positions of the Kings
;                 and Queen after a change of board position
;                 in ANALYS.
;
; CALLED BY:  --  ANALYS
;
; CALLS:      --  None
;
; ARGUMENTS:  --  None
;***********************************************************
ROYALT: LD      hl,POSK         ; Start of Royalty array
        LD      b,4             ; Clear all four positions
back06: LD      (hl),0
        INC     hl
        DJNZ    back06
        LD      a,21            ; First board position
RY04:   LD      (M1),a          ; Set up board index
        LD      hl,POSK         ; Address of King position
        LD      ix,(M1)
        LD      a,(ix+BOARD)    ; Fetch board contents
        BIT     7,a             ; Test color bit
        JR      Z,rel023        ; Jump if white
        INC     hl              ; Offset for black
rel023: AND     7             ; Delete flags, leave piece
        CP      KING          ; King ?
        JR      Z,RY08          ; Yes - jump
        CP      QUEEN         ; Queen ?
        JR      NZ,RY0C         ; No - jump
        INC     hl              ; Queen position
        INC     hl              ; Plus offset
RY08:   LD      a,(M1)          ; Index
        LD      (hl),a          ; Save
RY0C:   LD      a,(M1)          ; Current position
        INC     a               ; Next position
        CP      99            ; Done.?
        JR      NZ,RY04         ; No - jump
        RET                     ; Return

; Letters and numbers around the edge of the board
axis_table:
        db 80,2,'8'
        db 80,5,'7'
        db 80,8,'6'
        db 80,11,'5'
        db 80,14,'4'
        db 80,17,'3'
        db 80,20,'2'
        db 80,23,'1'
        db 42,25,'A'
        db 47,25,'B'
        db 52,25,'C'
        db 57,25,'D'
        db 62,25,'E'
        db 67,25,'F'
        db 72,25,'G'
        db 77,25,'H'

; This is the board drawing routine.
DSPBRD:
        PUSH    bc              ; Save registers
        PUSH    de
        PUSH    hl
        PUSH    af
        call    SAVE_CURSOR

        ld      de, axis_table
        ld      b, 16
axis_loop:
        ld      a, (de)
        ld      h, a
        inc     de
        ld      a, (de)
        ld      l, a
        call    move_cursor_to_hl
        inc     de
        ld      a, (de)
        inc     de
        call    print_a
        djnz    axis_loop

        LD      a,21            ; First board index
        ld      c, 8            ; Counter for 8 ranks
DSPBRD1: 
        ld      b, 8            ; Counter for 8 files in each rank
DSPBRD2:
        push    bc
        LD      (BRDPOS),a      ; Ready parameter
        CALL    CONVRT          ; X-Y coords of square into HL register
        CALL    move_cursor_to_hl
        CALL    display_piece
        LD      a, (BRDPOS)
        INC     a               ; Next square
        pop     bc
        djnz    DSPBRD2         ; Continue for all of this rank
        inc     a               ; Skip edges
        inc     a
        dec     c
        jr      nz, DSPBRD1

        call    RESTORE_CURSOR

        POP     af              ; Restore registers
        POP     hl
        POP     de
        POP     bc
        ret

move_cursor_to_hl:
    ; Pass in x coord in h, y coord in l
    ; This moves the cursor to the requested location on screen.
    push de
    push bc
    push af
    push hl
    ld a, ESC
    call print_a
    ld a, '['
    call print_a
    ld a, l
    push bc
    call print_a_as_decimal
    ld a, ';'
    call print_a
    pop bc
    ld a, h
    call print_a_as_decimal
    ld a, 'H'
    call print_a
    pop hl
    pop af
    pop bc
    pop de
    ret

; This draws a square of the board, but empty with a red background.
; If is used in routines that "flash" one of the squares of the board.
display_red_square:
        PUSH    hl              ; Save registers
        PUSH    bc
        PUSH    de
        PUSH    ix
        PUSH    af
        ld      de, red_background_codes
        call    show_string_de
        ld      de, five_spaces
        call    show_string_de
        ld      de, left5down
        call    show_string_de
        ld      de, five_spaces
        call    show_string_de
        ld      de, left5down
        call    show_string_de
        ld      de, five_spaces
        call    show_string_de
        jp      display_piece_done

; On entry to display_piece, the board location that
; we want to display must be in BRDPOS.
; Also, the x-y coords of the board location must be in hl.
; And the cursor must be positioned where we want to draw the piece.
; This routine draws the entire square and its content, so for an
; empty square it draws an empty square of the required colour.
; For a square with a piece in it draws the background of the square
; and the piece as well.
; If you request a red background it draws an empty square with a
; red background.
display_piece:
        PUSH    hl              ; Save registers
        PUSH    bc
        PUSH    de
        PUSH    ix
        PUSH    af
        call    store_background_colour
        ld      a, 0
        ld      (FOREGROUND_COLOUR), a
        LD      a,(BRDPOS)      ; Get board index
        LD      (M1),a          ; Save
        LD      ix,(M1)         ; Index into board array
        LD      a,(ix+BOARD)    ; Contents of board array
        AND     a               ; Is square empty ?
        JP      Z,display_board_space          ; Yes - jump
        CP      0FFH            ; Is it a border square ?
        JP      Z,display_board_space          ; Yes - jump
        LD      c,0             ; Clear flag register
        BIT     7,a             ; Is piece white ?
        JR      Z,wpiece        ; Yes - jump
        push    af
        LD      a, 1
        ld      (FOREGROUND_COLOUR), a
        pop     af
wpiece:
        AND     7               ; Delete flags, leave piece (1 = pawn, 2 = Knight etc)
disp_piece:
        ; By now we have the char in A, and the foreground and background colour is known
        push af

        ld de, white_background_codes
        ld a, (BACKGROUND_COLOUR)
        cp 1
        jr z, disp_piece1
        ld de, black_background_codes
disp_piece1:        
        call show_string_de

        ld de, white_foreground_codes
        ld a, (FOREGROUND_COLOUR)
        cp 0
        jr z, disp_piece2
        ld de, black_foreground_codes
disp_piece2:        
        call show_string_de

        pop af
        call show_this_piece
display_piece_done:
        ld de, normal_attribute
        call show_string_de
        pop af
        pop ix
        pop de
        pop bc
        pop hl
        ret
display_board_space:
        ld a, ' '
        jr disp_piece

show_this_piece:
        cp  1
        jp z, show_pawn
        cp  2
        jp z, show_knight
        cp  3
        jp z, show_bishop
        cp  4
        jp z, show_rook
        cp  5
        jp z, show_queen
        cp  6
        jp z, show_king
        cp  7
        jp z, show_mated_king
        ld a, ' '
        call print_a
        call print_a
        call print_a
        call print_a
        call print_a
        push af
        ld de, left5down
        call show_string_de
        pop af
        call print_a
        call print_a
        call print_a
        call print_a
        call print_a
        push af
        ld de, left5down
        call show_string_de
        pop af
        call print_a
        call print_a
        call print_a
        call print_a
        call print_a
        ret
show_pawn:
        ld de, pawn_codes
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, pawn_codes+6
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, pawn_codes+12
        call show_string_de
        ret

show_knight:
        ld de, knight_codes1
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, knight_codes2
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, knight_codes3
        call show_string_de
        ret
show_bishop:
        ld de, bishop_codes
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, bishop_codes+6
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, bishop_codes+12
        call show_string_de
        ret
show_rook:
        ld de, rook_codes
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, rook_codes+6
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, rook_codes+12
        call show_string_de
        ret
show_queen:
        ld de, queen_codes
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, queen_codes+6
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, queen_codes+12
        call show_string_de
        ret
show_king:
        ld de, king_codes1
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, king_codes2
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, king_codes3
        call show_string_de
        ret

show_mated_king:
        ld de, mated_king_codes1
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, mated_king_codes2
        call show_string_de
        ld de, left5down
        call show_string_de
        ld de, mated_king_codes3
        call show_string_de
        ret

; Assumes we have a code-page 437 font!
pawn_codes:
        db '     $'
        db '  O  $'
        db ' ',222,219,221,' $'
knight_codes1: db '     $'
knight_codes2:         db ' ',240,219,220,' $'
knight_codes3:         db 244,219,219,221,' $'
bishop_codes:
        db '  o  $'
        db ' (/) $'
        db ' ',220,219,220,' $'
rook_codes:
        db '     $'
        db ' ',221,221,221,' $'
        db ' ',219,219,221,' $'
queen_codes:
        db '\\|//$'
        db ' ',177,177,177,' $'
        db ' ',219,219,219,' $'
king_codes1: db '  ',197,'  $'
king_codes2: db ' ',219,219,219,' $'
king_codes3: db ' ',222,219,221,' $'

mated_king_codes1: db ' ',220,220,'  $'
mated_king_codes2: db '+',219,219,219,219,'$'
mated_king_codes3: db ' ',223,223,'  $'

store_background_colour:
        ld      a, h
        xor     l
        and     1
        ld      (BACKGROUND_COLOUR), a
        ret

BACKGROUND_COLOUR:
        db      0
FOREGROUND_COLOUR:
        db      0

white_background_codes:
        db ESC,'[43m$'
black_background_codes:
        db ESC,'[45m$'
red_background_codes:
        db ESC,'[41m$'
white_foreground_codes:
        db ESC,'[37m',ESC,'[1m$'
black_foreground_codes:
        db ESC,'[30m',ESC,'[2m$'
normal_attribute:
        db ESC,'[0m$'
bold_attribute:
        db ESC,'[1m$'
left5down:
        db 10,8,8,8,8,8,'$'
five_spaces:
        db '     $'

print_a_as_decimal:
    ; Prints a number (in a) from 0 to 255 in decimal
    ld c, 0                         ; c tells us if we have started printing digits
    ld b, a
    cp 100
    jr c, print_a_as_decimal_tens
    cp 200
    jr c, print_a_as_decimal_100
    ld a, '2'
    call print_a
    ld a, b
    sub 200
    jr print_a_as_decimal_101
print_a_as_decimal_100:
    ld a, '1'
    call print_a
    ld a, b
    sub 100
print_a_as_decimal_101:
    ld c, 1                         ; Yes, we have started printing digits
print_a_as_decimal_tens:
    ld b, 0
print_a_as_decimal_tens1:
    cp 10
    jr c, print_a_as_decimal_units
    sub 10
    inc b
    jp print_a_as_decimal_tens1
print_a_as_decimal_units:
    ld d, a
    ld a, b
    cp 0
    jr nz, print_a_as_decimal_show_tens
    ld a, c
    cp 0
    jr z, print_a_as_decimal_units1
print_a_as_decimal_show_tens:
    add a, '0'
    call print_a
print_a_as_decimal_units1:
    ld a, '0'
    add a, d
    call print_a
    ret

;***********************************************************
; BOARD INDEX TO NORM ADDRESS SUBR.
;***********************************************************
; FUNCTION:   --  Converts a hexadecimal board index into
;                 a Norm address for the square.
;
; CALLED BY:  --  DSPBRD
;                 INSPCE
;                 ANALYS
;                 MATED
;
; CALLS:      --  DIVIDE
;                 MLTPLY
;
; ARGUMENTS:   -- Returns the Norm address in register pair
;                 HL.
;***********************************************************
CONVRT: PUSH    bc              ; Save registers
        PUSH    de
        PUSH    af
        LD      a,(BRDPOS)      ; Get board index
        LD      d,a             ; Set up dividend
        SUB     a
        LD      e,10            ; Divisor
        CALL    DIVIDE          ; Index into D = rank and A = file
                                ; file (1-8) & rank (2-9)
        DEC     d               ; For rank (1-8)
        DEC     d               ; For rank (0-7)
        DEC     a               ; For file (0-7)
                                ; Now multiply rank by 3 and file by 5
        
        LD      c,d             ; Save rank for now
        LD      d,5             ; Multiplier for file
        LD      e,a             ; File number is multiplicand
        CALL    MLTPLY          ; Giving file displacement
        LD      a,d             ; Save
        ADD     a, 40           ; Move file across to suitable place on screen
        LD      h,a             ; Low order address byte

        LD      d,3             ; Rank multiplier
        ld      e, c            ; Rank
        call    MLTPLY
        ld      a, 22
        sub     d
        ld      l, a

        POP     af              ; Restore registers
        POP     de
        POP     bc
        RET                     ; Return

;***********************************************************
; POSITIVE INTEGER DIVISION
;   inputs hi=A lo=D, divide by E
;   output D, remainder in A
;***********************************************************
DIVIDE: PUSH    bc
        LD      b,8
DD04:   SLA     d
        RLA
        SUB     e
        JP      M,rel027
        INC     d
        JR      rel024
rel027: ADD     a,e
rel024: DJNZ    DD04
        POP     bc
        RET

;***********************************************************
; POSITIVE INTEGER MULTIPLICATION
;   inputs D, E
;   output hi=A lo=D
;***********************************************************
MLTPLY: PUSH    bc
        SUB     a
        LD      b,8
ML04:   BIT     0,d
        JR      Z,rel025
        ADD     a,e
rel025: SRA     a
        RR      d
        DJNZ    ML04
        POP     bc
        RET

;***********************************************************
; EXECUTE MOVE SUBROUTINE
;***********************************************************
; FUNCTION:   --  This routine is the control routine for
;                 MAKEMV. It checks for double moves and
;                 sees that they are properly handled. It
;                 sets flags in the B register for double
;                 moves:
;                       En Passant -- Bit 0
;                       O-O        -- Bit 1
;                       O-O-O      -- Bit 2
;
; CALLED BY:   -- PLYRMV
;                 CPTRMV
;
; CALLS:       -- MAKEMV
;
; ARGUMENTS:   -- Flags set in the B register as described
;                 above.
;***********************************************************
EXECMV: PUSH    ix              ; Save registers
        PUSH    af
        LD      ix,(MLPTRJ)     ; Index into move list
        LD      c,(ix+MLFRP)    ; Move list "from" position
        LD      e,(ix+MLTOP)    ; Move list "to" position

        CALL    MAKEMV          ; Produce move

        LD      d,(ix+MLFLG)    ; Move list flags
        LD      b,0
        BIT     6,d             ; Double move ?
        JR      Z,EX14          ; No - jump
        LD      de,6            ; Move list entry width
        ADD     ix,de           ; Increment MLPTRJ
        LD      c,(ix+MLFRP)    ; Second "from" position
        LD      e,(ix+MLTOP)    ; Second "to" position
        LD      a,e             ; Get "to" position
        CP      c               ; Same as "from" position ?
        JR      NZ,EX04         ; No - jump
        INC     b               ; Set en passant flag
        JR      EX10            ; Jump
EX04:   CP      1AH             ; White O-O ?
        JR      NZ,EX08         ; No - jump
        SET     1,b             ; Set O-O flag
        JR      EX10            ; Jump
EX08:   CP      60H             ; Black 0-0 ?
        JR      NZ,EX0C         ; No - jump
        SET     1,b             ; Set 0-0 flag
        JR      EX10            ; Jump
EX0C:   SET     2,b             ; Set 0-0-0 flag
EX10:   
        CALL    MAKEMV          ; Make 2nd move on board

EX14:   POP     af              ; Restore registers
        POP     ix
        RET                     ; Return

;***********************************************************
; MAKE MOVE SUBROUTINE
;***********************************************************
; FUNCTION:   --  Moves the piece on the board when a move
;                 is made. It blinks both the "from" and
;                 "to" positions to give notice of the move.
;
; CALLED BY:  --  EXECMV
;
; CALLS:      --  CONVRT
;                 BLNKER
;                 INSPCE
;
; ARGUMENTS:  --  The "from" position is passed in register
;                 C, and the "to" position in register E.
;***********************************************************
MAKEMV: PUSH    af              ; Save register
        PUSH    bc
        PUSH    de
        PUSH    hl

        push    de
        push    bc
        call    SAVE_CURSOR
        pop     bc
        pop     de
        push    de

        LD      a,c             ; "From" position
        LD      (BRDPOS),a      ; Set up parameter
        CALL    CONVRT          ; Getting Norm address in HL

        call    blink_square
        pop     de

        LD      a,e             ; Get "to" position
        LD      (BRDPOS),a      ; Set up parameter
        CALL    CONVRT          ; Getting Norm address in HL
        call    blink_square
        call    RESTORE_CURSOR
        POP     hl              ; Restore registers
        POP     de
        POP     bc
        POP     af
        RET                     ; Return

blink_square:
        ld      b, 5
blink_square1:
        push    bc
        push    hl
        CALL    move_cursor_to_hl
        CALL    display_red_square
        call    delay
        pop     hl
        push    hl
        CALL    move_cursor_to_hl
        CALL    display_piece
        call    delay
        pop     hl
        pop     bc
        djnz    blink_square1
        ret

delay:
        ld      bc, 200
more_delay:
        dec     bc
        push bc
        ld b, 255
even_more_delay:
        djnz    even_more_delay
        pop     bc
        ld      a, b
        or      c
        jr      nz, more_delay
        ret

BDOS equ 5
BDOS_Print_String equ 9         ; 09
BDOS_Console_Input equ 1
BDOS_Console_Output equ 2
BDOS_Console_Direct equ 6
ESC equ 27

show_string_de:
        ld c, BDOS_Print_String
        call BDOS
        ret

print_a:
        push    af
        push    bc
        push    de
        push    hl
        ld      c, BDOS_Console_Output
        ld      e, a
        call    BDOS
        pop     hl
        pop     de
        pop     bc
        pop     af
        ret

CLRSCR:
        ld      de, ClearScreenMessage
        jp      show_string_de

ClearScreenMessage:
        db ESC,'[H',ESC,'[J$'

SAVE_CURSOR:
        ld      de, SaveCursorMessage
        call    show_string_de
        call    hide_cursor
        ret

SaveCursorMessage:
        db ESC,'7$'

RESTORE_CURSOR:
        ld      de, RestoreCursorMessage
        call    show_string_de
        call    show_cursor
        ret

RestoreCursorMessage:
        db ESC,'8$'

hide_cursor:
    ld a, ESC
    call print_a
    ld a, '['
    call print_a
    ld a, '?'
    call print_a
    ld a, '2'
    call print_a
    ld a, '5'
    call print_a
    ld a, 'l'
    call print_a
    ret

show_cursor:
    ld a, ESC
    call print_a
    ld a, '['
    call print_a
    ld a, '?'
    call print_a
    ld a, '2'
    call print_a
    ld a, '5'
    call print_a
    ld a, 'h'
    call print_a
    ret

copyright_message:
    db 13,10
    db '      ::::::::      :::     :::::::::   ::::::::   ::::::::  ::::    ::: ',13,10
    db '     :+:    :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+: :+:+:   :+: ',13,10
    db '     +:+         +:+   +:+  +:+    +:+ +:+        +:+    +:+ :+:+:+  +:+ ',13,10
    db '     +#++:++#++ +#++:++#++: +#++:++#:  :#:        +#+    +:+ +#+ +:+ +#+ ',13,10
    db '            +#+ +#+     +#+ +#+    +#+ +#+   +#+# +#+    +#+ +#+  +#+#+# ',13,10
    db '     #+#    #+# #+#     #+# #+#    #+# #+#    #+# #+#    #+# #+#   #+#+# ',13,10
    db '      ########  ###     ### ###    ###  ########   ########  ###    #### ',13,10
    db 13,10
    db 'Sargon is a computer chess playing program designed and coded by Dan and',13,10
    db 'Kathe Spracklen. Copyright 1978. All rights reserved. No part of this',13,10 
    db 'publication may be reproduced without prior written permission.',13,10
    db 13,10
    db 'This version was ported to CP/M by John Squires in May 2021 specifically',13,10
    db 'for the Z80 Playground. It is based on the assembler listing found at',13,10
    db 'github.com/billforsternz/retro-sargon. See 8bitStack.co.uk for details.',13,10
    db 13,10
    db '$'
;
HELP:	 DB	13,10
	 DB	"CTRL^R to quit the game or CTRL^S to save the game"
	 DB	13,10,'$'
;
;----------------------------------------------
;
; Saved games file structure (one 128 bytes record)
;
;kolor:	(KOLOR)		1 byte
;depth:	(PLYMAX) 	1 byte	
;moveno:(MOVENO)	1 byte
;board:	(BOARDA) 	120 bytes
;dummy			5 bytes
;
;----------------------------------------------

Record:				;128 bytes buffer used at load/save game
kolor:	defs	1
depth:	defs	1
moveno:	defs	1
board:	defs	120
	defs	5

fcb:				; fcb
	defb	0		; disk+1
fname:	defm	"SARGON  SV "   ; file name
	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
fcbcr:	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2

CannotWrite:
	defb	0DH,0AH
	defm	'Could not write!$'
CannotOpen:
	defb	0DH,0AH
	defm	'Could not found saved game!$'
CannotRead:
	defb	0DH,0AH
	defm	'Could not read saved game!$'
LoadGameMsg:
	defb	0DH,0AH
	defm	'Load saved game? (Y=yes):$'
SaveMsg:defm	'Saving game...$'
GameNrMsg:
	defb	0DH,0AH
	defm	'Game number (0...9):$'
GameSaved:
	defb	0DH,0AH
	defm	'Game saved!$'
WrongNumber:
	defb	0DH,0AH
	defm	'It must be a single decimal digit!$'
;
LoadFlag:defb	0
;
;	A = game number (1...9 in ASCII)
;
LoadGame:
	ld	(fname+10),a	; Store game # as final ext char

	ld	de,fcb		; open file
	ld	c,15
	call	BDOS
	inc	a
	jr	nz,1f

	ld	de,CannotOpen	; file not found
	call	show_string_de
	jp	0		; quit
1:
	ld	de,Record	; set DMA addr
	ld	c,26
	call	BDOS

	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	de,fcb		; read record		
	ld	c,20
	call	BDOS
	or	a
	jr	z,1f

	ld	de,CannotRead	; read failed
	call	show_string_de
	jp	0		; quit
1:
	ld	de,fcb		; close file
	ld	c,16
	call	BDOS
				; store game info
	ld	a,(kolor)
	ld	(KOLOR),a

	ld	a,(depth)
	ld	(PLYMAX),a

	ld	a,(moveno)
	ld	(MOVENO),a
				;store to MVENUM A in ASCII decimal
	ld	hl,MVENUM

	ld	d,a
	ld	a,0
	ld	e,10
	call	DIVIDE
	ld	e,a		;remainder
	ld	a,d
	add	a,'0'
	ld	(hl),a

	inc	hl
	
	ld	a,e
	add	a,'0'
	ld	(hl),a

	inc	hl
	ld	(hl),' '

	ld	de,BOARDA	;to
	ld	hl,board	;from
	ld	bc,120		;count
	ldir
				;setup titles
	ld	a,(KOLOR)
	or	a
	jr	nz,1f
         	                ; Set computers color to white
        LD      hl,TITLE1       ; Prepare move list titles
        LD      de,TITLE4+2
        LD      bc,6
        LDIR
        LD      hl,TITLE2
        LD      de,TITLE4+9
        LD      bc,6
        LDIR
	
	jr	2f
1:			        ; Set computers color to black
        LD      hl,TITLE2       ; Prepare move list titles
        LD      de,TITLE4+2
        LD      bc,6
        LDIR
        LD      hl,TITLE1
        LD      de,TITLE4+9
        LD      bc,6
        LDIR
2:
	ld	a,1
	ld	(LoadFlag),a	;mark "game was loaded"

	ret
;
SaveGame:
	ld	de,SaveMsg
	call	show_string_de
savegame:
	ld	de,GameNrMsg	; Ask 'game nr'
	call	show_string_de

        CALL    CHARTR          ; Accept response
	cp	'0'
	jr	nc,2f
1:
	ld	de,WrongNumber
	call	show_string_de
	jr	savegame
2:
	cp	'9'+1
	jr	nc,1b

	ld	(fname+10),a	; Store game # as final ext char
				; store game info
	ld	a,(KOLOR)
	ld	(kolor),a

	ld	a,(PLYMAX)
	ld	(depth),a

	ld	a,(MOVENO)
	ld	(moveno),a

	ld	hl,BOARDA	;from
	ld	de,board	;to
	ld	bc,120		;count
	ldir

	ld	de,fcb		; delete file (if any...)
	ld	c,19
	call	BDOS

	ld	de,fcb		; make file
	ld	c,22
	call	BDOS

	inc	a
	jr	nz,1f

	ld	de,CannotWrite	; disk directory full
	call	show_string_de
	jp	0		; quit
1:
	ld	de,Record	; set DMA addr
	ld	c,26
	call	BDOS

	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	de,fcb		; write record		
	ld	c,21
	call	BDOS
	or	a
	jr	z,1f

	ld	de,CannotWrite	; write failed
	call	show_string_de
	jp	0		; quit
1:
	ld	de,fcb		; close file
	ld	c,16
	call	BDOS

	ld	de,GameSaved
	call	show_string_de

	jp	0		; quit
;

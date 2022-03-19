; file ps.as
.entry LIST
.extern W
MAIN: 		add r3, LIST
LOOP: 		prn #48
            
		macro m1
 			inc r6
			mov r3, W
		endm

        macro macro2
 		    lea STR, r6
        endm
        macro2
 		m1
        macro MACRO3
 		    sub r1, r4
 		    bne END
 		    cmp val1, #-6
 		    bne END[r15]
		    dec K
            sub LOOP[r10] ,r14
        endm
.entry MAIN

END:		 stop
STR: 		.string "abcd"
LIST: 		.data 6, -9
 		.data -100
.entry K
K:		 .data 31
.extern val1
MACRO3

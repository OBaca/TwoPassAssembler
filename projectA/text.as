; file ps.as
.entry LIST
.extern W

MAIN: 		add r3, LIST
LOOP: 		prn #48
		macro m1
 			inc r6
			 mov r3, W
		endm
 		lea STR, r6
 		m1
 		sub r1, r4
 		bne END
		macro m231423
			sub r2, r3 ---m23232
		endm
		macro         m321
			lol
			bitch
			fuck
			what the fuck
		endm
 		cmp val1, #-6
 		bne END[r15]
		 m321
		 dec K
.entry MAIN
 		sub LOOP[r10] ,r14
		        m231423
END:		 stop
STR: 		.string “abcd”
LIST: 		.data 6, -9
 		.data -100
.entry K
K:		 .data 31
.extern val1
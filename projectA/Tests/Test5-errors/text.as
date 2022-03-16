.extern tavit

; Errors with labels

.extern cmp
.extern add
.extern r4
.extern r15
.extern cmp[r1]



; Errors in line with no operands

stopa
2stop
stop 1
stop 		a
stop 	@

	rts1
	!rts
	rts  4
	rts   $

; Errors in line with 1 operands

prn r1,
prn ,r1
prn r1 			,
prn tavit,
prn r16
prn r-4
prn NotATavit
prn
prn r1, r2
2prn
prnO


; this commands cant get address method 0
clr #6
clr #-6
clr 6
clr 
clr 	a
clr r1,r2
clr r1,
clr r1 ,
clr , r1
clr1 r2
1clr r2
clr r20
clr r-12
clr tavit[r16]
clr noTavit


	not #6
	not #-6
	not 6
	not 
	not 	a
	not r1,r2
	not r1,
	not r1 ,
	not , r1
	not1 r2
	1not r2
	not r16
	not r-1


; this commands cant get address method 3

jmp r2
jmp r13
jmp r16
jmp r-1
jmp 
jmp tavit, tavit
jmp ,tavit
1jmp 
jmp@


;this commands get 2 operands

cmp r2, r2,
cmp , r2, r2
cmp r2, , r2
cmp r2
cmp
cmp NoTavit , r2
cmp tavit, r16
cmp #2, ##2
cmp r3, r2, r1
2cmp r2, r2
CMP r3, r2
cmp r3,tavit[r16]
cmp cmp, cmp



		sub r2, r2,
		sub , r2, r2
		sub r2 , ,r2
		sub r2
		sub
		sub NoTavit , r2
		sub tavit, r16
		sub #2, ##2
		sub r3, r2, r1
		sub2 r2, r2
		2sub r3, r2
		sub r3,tavit[r16]
		sub cmp, cmp
		sub r2, #2
		sub tavit , #-2

	
				lea r2, r2,
				lea , r2, r2
				lea r2 , ,r2
				lea r2
				lea
				lea NoTavit , r2
				lea tavit, r16
				lea #2, ##2
				lea r3, r2, r1
				lea r2, r2
				12lea r3, r2
				lea r3,tavit[r16]
				lea cmp, cmp
				lea r2, #2
				lea tavit , #-2
				lea #2 , r2
				lea r1, r2
				lea tavit, #2



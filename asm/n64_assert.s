.include "macros.inc"

.section .data
glabel __n64Assert_Filename
.skip 4
glabel __n64Assert_LineNum
.skip 4
glabel __n64Assert_Message
.skip 4

.section .text

glabel __n64Assert
.type __n64Assert, @function
#ifndef RECOMP
sw $a0, __n64Assert_Filename
sw $a1, __n64Assert_LineNum
sw $a2, __n64Assert_Message
syscall
 nop
#else // RECOMP
/*
This is an unusual function to throw through recomp,
 so we make asserts a no-op.
 Fix your bugs before porting.
*/
jr $ra
 nop
#endif // RECOMP
.size __n64Assert, . - __n64Assert

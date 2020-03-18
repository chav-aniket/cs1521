# COMP1521 19t2 ... Game of Life on a NxN grid
#
# Written by z5265106, June 2019

## Requires (from `boardX.s'):
# - N (word): board dimensions
# - board (byte[][]): initial board state
# - newBoard (byte[][]): next board state

    .data
msg1:   .asciiz "# Iterations: "
msg2:   .asciiz "=== After iteration "
msg3:   .asciiz " ===\n"
eol:    .word	10
period: .word	46
hash:   .word	35

## Provides:
	.globl	main
	.globl	decideCell
	.globl	neighbours
	.globl	copyBackAndShow


########################################################################
# .TEXT <main>
    .text
main:

# Frame:	$fp, $ra, $s0, $s1, $s2, $s3, $s4, $s5
# Uses:		$s0, $s1, $s2, $s3, $s4, $s5, $t0, $t1, $t2
# Clobbers:	$a0, $a1, $t0, $t1, $t2

# Locals:
#   - 'maxiters' in $s0
#   - 'n' in $s1 from 'N' in boardX
#   - 'i' in $s2
#   - 'j' in $s3
#   - 'nn' in $s4
#   - iteration_counter in $s5

# Structure:
#	main
#	-> [prologue]
#	-> loop
#	-> main_incr_j
#	-> main_incr_i
#	-> incr_iter
#	-> [epilogue]

# Code:

	# Stack Frame Setup
    	sw	    $fp, -4($sp)					# push $fp onto stack
    	la	    $fp, -4($sp)        			# set up $fp for this function
    	sw	    $ra, -4($fp)        			# save return address
    	sw	    $s0, -8($fp)        			# save $s0 to use
    	sw	    $s1, -12($fp)       			# save $s1 to use
    	sw      $s2, -16($fp)       			# save $s2 to use
    	sw      $s3, -20($fp)       			# save $s3 to use
    	sw      $s4, -24($fp)       			# save $s4 to use
    	sw      $s5, -28($fp)       			# save $s5 to use
    	addi	$sp, $sp, -32

    	lw 		$s1, N              			# n = N (from boardX)
    	li 		$s2, 0              			# i = 0
    	li  	$s3, 0              			# j = 0
    	la  	$s4, 4              			# load 4 bytes of space to nn
    	li  	$s5, 0              			# iter_counter = 0

    	la  	$a0, msg1
    	li  	$v0, 4
    	syscall                     			# printf("%s", msg1);

    	li  	$v0, 5
    	syscall									# scanf("%d", &maxiters);
    	move 	$s0, $v0            			# $s0 = maxiters

	loop:
	    move    $a0, $s2						# $a0 = i
	    move    $a1, $s3						# $a1 = j
	    jal 	neighbours						# jump to neighbours
	    nop
	    move    $s4, $v0            			# nn = neighbours(i, j)

	    mul 	$t0, $s1, $s2 					# N * i
        add 	$t0, $t0, $s3       			# (N * i) + j
        lb 		$t0, board($t0)          		# $t0 = board[i][j]

	    move    $a0, $t0						# $a0 = old
	    move    $a1, $s4						# $a1 = nn
	    jal 	decideCell
	    nop

	    mul 	$t2, $s1, $s2					# N * i
        add 	$t2, $t2, $s3       			# (N * i) + j
        sb		$v0, newBoard($t2)				# newBoard[i][j] = decideCell (old, nn)

    main_incr_j:
        add 	$s3, $s3, 1						# j++
        beq 	$s3, $s1, main_incr_i			# if (j == N) --> incr_col
        j 		loop							# jump to loop

	main_incr_i:
        add 	$s2, $s2, 1						# i++
        li  	$s3, 0							# j = 0;
        beq 	$s2, $s1, incr_iter				# if (i == N) --> incr_iter
        j 		loop							# jump to loop

    incr_iter:
        la  	$a0, msg2
        li  	$v0, 4
        syscall									# printf("=== After iteration ");

	    add 	$s5, $s5, 1
	    move    $a0, $s5
	    li  	$v0, 1
	    syscall                    				# printf("%d", counter);

	    la  	$a0, msg3
        li  	$v0, 4
        syscall                     			# printf(" ===\n");

        jal 	copyBackAndShow
        nop
	    beq 	$s5, $s0, main_post				# if (iter_counter == maxiters) --> main_post

	    li  	$s2, 0							# i = 0
	    li  	$s3, 0							# j = 0
	    j 		loop							# jump to loop

    main_post:
        lw  	$s5, -28($fp)          			# restore $s5 value
        lw  	$s4, -24($fp)          			# restore $s4 value
        lw  	$s3, -20($fp)          			# restore $s3 value
        lw  	$s2, -16($fp)          			# restore $s2 value
        lw		$s1, -12($fp)          			# restore $s1 value
	    lw		$s0, -8($fp)           			# restore $s0 value
	    lw		$ra, -4($fp)           			# restore $ra for return
	    la		$sp, 4($fp)            			# restore $sp (remove stack frame)
	    lw		$fp, ($fp)             			# restore $fp (remove stack frame)
	    jr		$ra                    			# return function

########################################################################
# .TEXT <decideCell>
    .text
decideCell:
# Frame:	$fp, $ra, $s0
# Uses:		$s0, $a0, $a1
# Clobbers:	$a0, $a1

# Locals:
#   - 'ret' in $s0
#	- 'old' in $a0
#	- 'nn'	in $a1

# Structure:
#	decideCell
#	-> [prologue]
#	-> if
#	-> inner_if
#	-> inner_else_if
#	-> else_if
#	-> [epilogue]

#   Stack Frame Setup
        sw		$fp, -4($sp)					# push $fp onto stack
    	la		$fp, -4($sp)					# set up $fp for this function
    	sw		$ra, -4($fp)					# save return address
    	sw		$s0, -8($fp)					# save $s0 to use
    	addi    $sp, $sp, -12

        beq 	$a0, 1, if						# if (old == 1) --> if
    	beq 	$a1, 3, else_if					# if (nn == 3) --> else_if
        li  	$s0, 0							# else ret = 0
		j		exit_decide						# jump to exit_decide

	if:
	    blt 	$a1, 2, inner_if				# if (nn < 2) --> inner_if
	    beq 	$a1, 2, inner_else_if			# if (nn == 2) --> inner_else_if
	    beq 	$a1, 3, inner_else_if			# if (nn == 3) --> inner_else_if
	    li  	$s0, 0							# else ret = 0
	    j   	exit_decide						# jump to exit_decide

    inner_if:
        li  	$s0, 0							# ret = 0
        j   	exit_decide						# jump to exit_decide

    inner_else_if:
        li  	$s0, 1							# ret = 1
        j   	exit_decide						# jump to exit_decide

	else_if:
	    li  	$s0, 1							# ret = 1
	    j   	exit_decide						# jump to exit_decide

	exit_decide:
	    move 	$v0, $s0						# return ret

	    lw		$s0, -8($fp)					# restore $s0 value
	    lw		$ra, -4($fp)					# restore $ra for return
	    la		$sp, 4($fp)						# restore $sp (remove stack frame)
	    lw		$fp, ($fp)						# restore $fp (remove stack frame)
	    jr		$ra								# return function

########################################################################
# .TEXT <neighbours>
    .text
neighbours:
# Frame:	$fp, $ra, $s0, $s1, $s2, $s3
# Uses:		$s0, $s1, $s2, $s3, $a0, $a1, $t0, $t1, $t2, $t3, $t4
# Clobbers:	$a0, $a1, $t0, $t1, $t2, $t3, $t4

# Locals:
#   - 'nn' in $s0
#   - 'x' in $s1
#   - 'y' in $s2

# Structure:
#	neighbours
#	-> [prologue]
#	-> neighbours_check
#	-> neighbours_operation
#	-> gate
#	-> incr_y
#	-> incr_x
#	-> incr_nn
#	-> [epilogue]

# Stack Frame Setup
		sw  	$fp, -4($sp)					# push $fp onto stack
    	la	    $fp, -4($sp)					# set up $fp for this function
    	sw	    $ra, -4($fp)					# save return address
    	sw	    $s0, -8($fp)					# save $s0 to use
        sw      $s1, -12($fp)					# save $s1 to use
        sw      $s2, -16($fp)					# save $s2 to use
        sw      $s3, -20($fp)					# save $s3 to use
    	addi    $sp, $sp, -24

        li      $s0, 0							# nn = 0
        li      $s1, -1							# x = -1
        li      $s2, -1							# y = -1
        lw      $s3, N							# n = N

    neighbours_check:
        add     $t0, $s1, $a0					# t0 = x + i
        add     $t1, $s2, $a1					# t1 = y + j
        add     $t2, $s3, -1					# t2 = N - 1

        bltz	$t0, incr_y						# if ((x + i) < 0) --> incr_y
		bgt		$t0, $t2, incr_y				# if ((x + i) > (N - 1)) --> then incr_y
		bltz	$t1, incr_y						# if ((y + j) < 0) --> incr_y
		bgt		$t1, $t2, incr_y				# if ((y + j) > (N - 1)) --> then incr_y
		beqz	$s1, gate						# if (x == 0) --> gate

    neighbours_operation:

        mul		$t4, $s3, $t0					# N * (x + i)
        add     $t4, $t4, $t1					# (N * (x + i)) + (y + j)
        lb      $t4, board($t4)					# $t4 = board[x + i][y + j]

        beq     $t4, 1, incr_nn					# if (boardVal == 1) --> incr_nn
        j       incr_y							# jump to inside_for

    gate:
        beqz    $s2, incr_y						# if (y == 0) --> inside_for
        j       neighbours_operation			# jump to neighbours_operation

    incr_y:
        add     $s2, $s2, 1						# y++
        bgt     $s2, 1, incr_x					# if (y > 1) --> outside_for
        j       neighbours_check				# jump to neighbours_check

    incr_x:
        add     $s1, $s1, 1						# x++
        bgt     $s1, 1, exit_neighbours			# if (x > 1) --> exit_neighbours
        li      $s2, -1							# y = 0
        j       neighbours_check				# jump to neighbours_check

    incr_nn:
        add     $s0, $s0, 1						# nn++
        j       incr_y							# jump to inside_for

    exit_neighbours:
        move    $v0, $s0

        lw      $s3, -20($fp)					# restore $s3 value
        lw      $s2, -16($fp)					# restore $s2 value
        lw      $s1, -12($fp)					# restore $s1 value
	    lw	    $s0, -8($fp)					# restore $s0 value
	    lw	    $ra, -4($fp)					# restore $ra for return
	    la	    $sp, 4($fp)						# restore $sp (remove stack frame)
	    lw	    $fp, ($fp)						# restore $fp (remove stack frame)
	    jr	    $ra								# return function

########################################################################
# .TEXT <copyBackAndShow>
    .text
copyBackAndShow:
# Frame:	$fp, $ra, $s0, $s1, $s2
# Uses:		$s0, $s1, $s2, $t0, $t1
# Clobbers:	$a0, $a1, $t0, $t1, $t2

# Locals:
#   - 'i' in $s0
#   - 'j' in $s1
#	- 'N' in $s2

# Structure:
#	copyBackAndShow
#	-> [prologue]
#	-> copy_operation
#	-> print_dot
#	-> incr_j
#	-> incr_i
#	-> [epilogue]

# Stack Frame Setup
	    sw 		$fp, -4($sp)					# push $fp onto stack
	    la		$fp, -4($sp)					# set up $fp for this function
	    sw		$ra, -4($fp)					# save return address
	    sw		$s0, -8($fp)					# save $s0 to use
	    sw		$s1, -12($fp)					# save $s1 to use
	    sw  	$s2, -16($fp)					# save $s2 to use
	    addi    $sp, $sp, -20

	    li  	$s0, 0							# i = 0
	    li  	$s1, 0							# j = 0
	    lw  	$s2, N							# n = N

    copy_operation:
        mul 	$t0, $s2, $s0					# N * i
        add 	$t0, $t0, $s1					# (N * i) + j
        lb 		$t0, newBoard($t0)				# $t0 = board[i][j]

        mul 	$t2, $s2, $s0					# N * i
        add 	$t2, $t2, $s1					# (N * i) + j
		sb		$t0, board($t2)					# board[i][j] = newBoard[i][j]

        beqz 	$t0, print_dot					# if (val == 0) --> print_dot

        lw  	$a0, hash
        li  	$v0, 11
        syscall									# putchar("#")

        j   	incr_j							# i++

    print_dot:
        lw  	$a0, period
        li  	$v0, 11
        syscall									# putchar(".")

        j   	incr_j							# i++

    incr_j:
        add 	$s1, $s1, 1						# j++
        beq 	$s1, $s2, incr_i				# if (j == N) --> incr_i

        j   	copy_operation					# jump to copy_operation

    incr_i:
        add 	$s0, $s0, 1						# i++

		lw		$a0, eol
		li		$v0, 11
		syscall									# putchar("\n")

        beq 	$s0, $s2, exit_copy				# if (i == N) --> exit_copy
        li  	$s1, 0							# j = 0
        j   	copy_operation					# jump to copy_operation

    exit_copy:
        lw  	$s2, -16($fp)					# restore $s2 value
        lw		$s1, -12($fp)					# restore $s1 value
	    lw		$s0, -8($fp)					# restore $s0 value
	    lw		$ra, -4($fp)					# restore $ra for return
	    la		$sp, 4($fp)						# restore $sp (remove stack frame)
	    lw		$fp, ($fp)						# restore $fp (remove stack frame)
	    jr		$ra								# return function

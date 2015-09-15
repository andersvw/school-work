# Author: Anders Van Winkle
# 3/26/2014
# CISC260: Quiz 3 - Problem 8

.data
zero: .float 0.0
one: .float 1.0
two: .float 2.0
prompt: .asciiz "Please enter a float between 1-10,000,000 with 5 decimal precision:\n"
rerun_prompt: .asciiz "Enter 1 to rerun and try a new number\n"
incorrect_value: .asciiz "You did not enter 1.\n"
new_line: .asciiz "\n"
goodbye: .asciiz "Goodbye."


.text
# main function
main:     
    #input
    li $v0, 4			# service 4 is print string
    la $a0, prompt
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall

    la $t0, zero
    l.s $f4, 0($t0)		# f4 = i = 0
    la $t1, one
    l.s $f6, 0($t1)		# f6 = 1
    la $t2, two
    l.s $f8, 0($t2)		# f8 = 2
    mov.s $f2, $f0		# f2 = x = n = f0
    div.s $f10, $f2, $f8	# f10 = n/2
    
loop:
    c.lt.s $f4, $f10		# if i >= n/2, go to end
    bc1f end
    div.s $f12, $f0, $f2	# f12 = n/x
    add.s $f12, $f12, $f2	# f12 = x + (n/x)
    div.s $f2, $f12, $f8	# x = (x + (n/x))/2
    add.s $f4, $f4, $f6		# i++
    j loop
    
end:
    mov.s $f12, $f2
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, new_line
    syscall
    
    # prompt for rerun
    li $v0, 4			# service 4 is print string
    la $a0, rerun_prompt
    syscall
    li $v0, 5			# service 5 is read integer
    syscall
    
    li $t0, 1
    beq $v0, $t0, main  	# if the entered number was 1 jump to bias again
    li $v0, 4			# service 4 is print string
    la $a0, incorrect_value
    syscall
    j exit			# else exit
    
    
exit:
    li $v0, 4			# service 4 is print string
    la $a0, goodbye
    syscall
    

# Author: Anders Van Winkle
# 3/24/2014
# CISC260: Chapter 3 - Homework 6

.data
two: .double 2
odds: .asciiz "The odds of picking a correct NCAA bracket are 1 in "
new_line: .asciiz "\n"
bias_prompt: .asciiz "Enter the bias: "
rerun_prompt: .asciiz "Enter 1 to rerun and try a new bias\n"
incorrect_value: .asciiz "You did not enter 1.\n"
incorrect_string: .asciiz "\nThe value you have entered is not valid, please try again. \n\n"
goodbye: .asciiz "Goodbye."


.text
# main function
main: 
    li $t0, 1
    li $t1, 63
    la $t2, two
    l.d $f2, 0($t2)
    l.d $f4, 0($t2)
    
loop:
    mul.d $f4, $f4, $f2
    addi $t0, $t0, 1
    bne $t0, $t1, loop
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, odds
    syscall
    mov.d $f12, $f4
    li  $v0, 3           	# service 3 is print double in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, new_line
    syscall

bias:
    # prompt for bias
    li $v0, 4			# service 4 is print string
    la $a0, bias_prompt
    syscall
    li $v0, 5			# service 5 is read integer
    syscall
    
    ble $t0, $zero, incorrect   # if the entered bias was < 1, reprompt
    li $t0, 1
    beq $v0, $t0, three 	# if the entered bias was 1 jump to three
    li $t0, 3
    ble $v0, $t0, nineteen	# if the entered bias was 2 or 3 jump to nineteen
    li $t0, 5
    ble $v0, $t0, thirtyone	# if the entered bias was 4 or 5 jump to thirtyone
    li $t0, 7
    ble $v0, $t0, thirtynine	# if the entered bias was 6 or 7 jump to thirtynine
    li $t0, 9
    ble $v0, $t0, fortyseven	# if the entered bias was 8 or 9 jump to fortyseven
    li $t0, 11
    ble $v0, $t0, fiftyone	# if the entered bias was 10 or 11 jump to fiftyone
    li $t0, 13
    ble $v0, $t0, fiftyfive	# if the entered bias was 12 or 13 jump to fiftyfive
    li $t0, 15
    ble $v0, $t0, fiftynine	# if the entered bias was 14 or 15 jump to fiftynine
    j sixtythree		# else, the bias was greater 16 or greater, therefore there is no bias
    
three:
    li $s0, 3
    j body

nineteen:
    li $s0, 19
    j body

thirtyone:
    li $s0, 31
    j body

thirtynine:
    li $s0, 39
    j body

fortyseven:
    li $s0, 47
    j body

fiftyone:
    li $s0, 51
    j body

fiftyfive:
    li $s0, 55
    j body

fiftynine:
    li $s0, 59
    j body

sixtythree:
    li $s0, 63
    j body

body:
    li $t0, 1
    la $t2, two
    l.d $f2, 0($t2)
    l.d $f4, 0($t2)
    
loop_2:
    mul.d $f4, $f4, $f2
    addi $t0, $t0, 1
    bne $t0, $s0, loop_2
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, odds
    syscall
    mov.d $f12, $f4
    li  $v0, 3           	# service 3 is print double in f12
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
    beq $v0, $t0, bias  	# if the entered number was 1 jump to bias again
    li $v0, 4			# service 4 is print string
    la $a0, incorrect_value
    syscall
    j exit			# else exit
   
# The entered number was not valid, send the user back to try again
incorrect:
    li $v0, 4			# service 4 is print string
    la $a0, incorrect_string
    syscall
    j bias			#try again
    
exit:
    li $v0, 4			# service 4 is print string
    la $a0, goodbye
    syscall
    


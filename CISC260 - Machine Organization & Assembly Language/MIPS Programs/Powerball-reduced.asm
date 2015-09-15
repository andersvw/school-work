################################################################################################################
# Author: Anders Van Winkle
# 4/10/2014
# CISC260: Midterm 2 - Question 11
# 
# The combination formula (COMBIN) calculates the distinct number of combinations when pulling
# a random item from a fixed set of items. 
#  
# For example, if I had 10 marbles, and I asked you to pick 3 marbles out of the 10, 
# there can only be 120 different combinations (feel free to manually prove that out). 
# 
# The formula for combination is:
#  
# (10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1)
# -----------------------------------------------
# (3 * 2 * 1) * (7 * 6 * 5 * 4 * 3 * 2 * 1)
# 
# = 120
# 
# Back to MIPS...
# 
# Write a program that will determine the odds for PowerBall...  
# Powerball pulls 5 number from a group of 59 and 1 number from a group of 35.  
# Use all four numbers (59, 5, 35, 1) as input variables.  
#
# My calcuations show odds of 1:175,223,510.  Do you come up with the same?  Why is yours (slightly) different?
#
# Programming hints:
#
# You're probably going to have to do some recursion (because of factorial)...
# You're probably going to have to use float (the numerator is gigantic).
################################################################################################################
#
# The odds of winning Powerball are the odds of choosing all 5 white balls, values 1-59, correctly and then
# choosing the red ball, valued 1-35, correctly.
# 
# The COMBIN function, aka choose, is a done without replacement and unordered. This means that you don't care
# if the numbers come out 1, 2, 3, 4, 5 but any order is fine (1, 19, 11, 59, 51). Because there is no replacement,
# you cannot draw a 2 on the first pull and a then get a 2 on another pull.
#
# This turns into the equation (5 choose 5)*(54 choose 0)/(59 choose 5) for the white balls.
# You have to choose 5 balls, thus the choose 5. You need to choose 5, any 5 white ball numbers in any order correctly.
# That is where the 5 choose 5 comes from. The 54 choose 0 is the remaining 54 white balls whose numbers you did not
# pick, you don't want to choose any of these. Because there are 59 total white balls and you need to choose 5 of them,
# the total possibilities of choices is represented as 59 choose 5 (the denominator). The numerator represents what you
# need to have to win.
# 
# Mathematically, the numerator just becomes 1. Any number N choose N is just 1, as well as, N choose 0. Therefore, the
# numerator must be 1.
# 
# We end up with 1/5006386 for the odds of choosing the 5 white balls correctly. This is then multiplied by 1/35 which
# is the probability of getting the red ball correct. Thus our total odds of winning the Powerball are:
# 1 in 175,223,510
# 
# This program could have been written better with a function to calculate N choose K, taking 2 params, however this
# simply just computes the odds to win powerball.

.data
one: .float 1.0
zero: .float 0.0
odds: .asciiz "The odds of winning Powerball are 1 in "
probability: .asciiz "The probability of winning Powerball is "
new_line: .asciiz "\n"
prompt1: .asciiz "Please enter the number of white ball options:\n"
prompt2: .asciiz "Please enter the number of white balls chosen:\n"
prompt3: .asciiz "Please enter the number of red ball options:\n"
prompt4: .asciiz "Please enter the number of red balls chosen:\n"
goodbye: .asciiz "Goodbye."


.text
# main function
main:
    #input
    li $v0, 4			# service 4 is print string
    la $a0, prompt1
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f1, $f0		# f1 holds white ball options (59)
    
    li $v0, 4			# service 4 is print string
    la $a0, prompt2
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f2, $f0		# f2 holds white balls chosen (5)
    
    li $v0, 4			# service 4 is print string
    la $a0, prompt3
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f3, $f0		# f3 holds red ball options (35)
    
    li $v0, 4			# service 4 is print string
    la $a0, prompt4
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f4, $f0		# f4 holds red balls choosen (1)
    				
    jal   factorial_reduced	# factorial of first f2 elements of f1 (59x58x57x56x55)
    mov.s $f8, $f5		# f8 holds 59x58x57x56x55
    
    mov.s $f1, $f2		# f4 holds red balls choosen (1)	
    jal   factorial 		# factorial of f1
    mov.s $f9, $f5		# f9 holds 5!
    
    div.s $f10, $f8, $f9	# f10 = f8 / f9
    
    mov.s $f1, $f3
    mov.s $f2, $f4
    
    jal factorial_reduced
    mov.s $f11, $f5		# f11 = 35
    
    mov.s $f1, $f4
    jal factorial
    mov.s $f13, $f5		# f13 = 1
    
    div.s $f14, $f11, $f13	# f14 = 35 / 1 = 35
    
    mul.s $f15, $f10, $f14	# f15 = f10 * f14 = 5006386 * 35 = 175223510
    
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, odds
    syscall
    mov.s $f12, $f15		# f12 = 175223510
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, new_line
    syscall
    
    div.s $f12, $f6, $f15	# f12 = 1 / 175,223,510
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, probability
    syscall
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, new_line
    syscall
    j exit    
    
# Takes 2 input paramaters n and k stored in f1 and f2, return value stored in f5
factorial_reduced:
    la   $t8, one
    l.s  $f5, 0($t8)		# initialize $f5 to 1.0 (return value)
    l.s  $f6, 0($t8)		# initialize $f6 to 1.0
    sub.s $f7, $f1, $f2 	# f7 = n - k (for white balls)

fact_reduced:
    addi $sp, $sp, -8
    sw   $ra, 0($sp)		# save $ra
    s.s  $f1, 4($sp)		# save n

    c.le.s $f1, $f7		# if n < n-k, exit factorial 
    bc1t return_reduced
    
    sub.s $f1, $f1, $f6		# n = n - 1
    jal  fact_reduced		# go back to factorial (until n <= n-k)
   
    l.s  $f1, 4($sp)		# pop n back off the stack
    mul.s  $f5, $f5, $f1
    
return_reduced:
    lw   $ra, 0($sp)		# restore original $ra
    addi $sp, $sp, 8		# reset the $sp
    jr 	 $ra
    
# Takes 1 parameter n which is stored in f1, return value in f5
factorial:
    la   $t8, one
    l.s  $f5, 0($t8)		# initialize $f5 to 1.0 (return value)
    l.s  $f6, 0($t8)		# initialize $f6 to 1.0

fact:    
    addi $sp, $sp, -8
    sw   $ra, 0($sp)		# save $ra
    s.s  $f1, 4($sp)		# save n

    c.le.s $f1, $f6		# if n <= 0, exit factorial 
    bc1t return
    
    sub.s $f1, $f1, $f6		# n = n - 1
    jal  fact			# go back to factorial (until n <= 0)
   
    l.s  $f1, 4($sp)		# pop n back off the stack
    mul.s  $f5, $f5, $f1
    
return:
    lw   $ra, 0($sp)		# restore original $ra
    addi $sp, $sp, 8		# reset the $sp
    jr 	 $ra
    
exit:
    li $v0, 4			# service 4 is print string
    la $a0, goodbye
    syscall
    

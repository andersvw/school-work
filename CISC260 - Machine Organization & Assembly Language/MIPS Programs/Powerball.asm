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
thirtyfive: .float 35.0
fiftynine: .float 59.0
fiftyfour: .float 54.0
five: .float 5.0
odds: .asciiz "The odds of winning Powerball are 1 in "
probability: .asciiz "The probability of winning Powerball is "
new_line: .asciiz "\n"
prompt: .asciiz "Please enter a float between 1-10,000,000 with 5 decimal precision:\n"
goodbye: .asciiz "Goodbye."


.text
# main function
main:
    la   $t0, fiftynine
    l.s  $f0, 0($t0)		# load 59 into $f0
    jal   factorial
    mov.s $f8, $f2		# f8 holds 5!
    
    la   $t0, five
    l.s  $f0, 0($t0)		# load 5 into $f0
    jal   factorial
    mov.s $f10, $f2		# f10 holds 5!
    
    la   $t0, fiftyfour
    l.s $f0, 0($t0)		# load 54 into $f0
    jal   factorial
    mov.s $f14, $f2		# f14 holds 54!
    
    div.s $f8, $f8, $f10	# f8 = 59! / 5!
    div.s $f8, $f8, $f14	# f8 = 59! / (5!*54!) = 5,006,386

    la    $t8, one
    l.s   $f10, 0($t8)		# initialize $f10 to 1.0
    div.s $f14, $f10, $f8	# f14 = 1 / 5,006,386
    
    la    $t7, thirtyfive
    l.s   $f16, 0($t7)
    div.s $f18, $f10, $f16	# f18 = 1 / 35
    mul.s $f20, $f8, $f16	# f20 = 5,006,386 x 35 = 175,223,510
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, odds
    syscall
    mov.s $f12, $f20
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, new_line
    syscall
    
    mul.s $f12, $f14, $f18	# f12 = 1 / 175,223,510
    
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
    
factorial:
    la   $t8, one
    l.s  $f2, 0($t8)		# initialize $f2 to 1.0 (return value)
    l.s  $f4, 0($t8)		# initialize $f4 to 1.0

fact:
    addi $sp, $sp, -8
    sw   $ra, 0($sp)		# save $ra
    s.s  $f0, 4($sp)

    c.le.s $f0, $f4		# if n <= 0, exit factorial 
    bc1t return
    
    sub.s $f0, $f0, $f4		# n = n - 1
    jal  fact			# go back to factorial (until n <= 0)
   
    l.s  $f0, 4($sp)		# pop n back off the stack
    mul.s  $f2, $f2, $f0
    
return:
    lw   $ra, 0($sp)		# restore original $ra
    addi $sp, $sp, 8		# reset the $sp
    jr 	 $ra
    
exit:
    li $v0, 4			# service 4 is print string
    la $a0, goodbye
    syscall
    


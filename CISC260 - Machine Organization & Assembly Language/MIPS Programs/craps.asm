################################################################################################################
# Author: Anders Van Winkle
# 4/10/2014
# CISC260: Final Exam Part 2
# 
# Find the binomial distribution of seven straight passes at craps.  For the purpose of this exercise, a pass is a seven
# or eleven on the come out roll.  You do not have to calculate the odds of hitting/making a point.
#  
# About that game
# 
# Craps is a great game to learn about binomial distribution and statistics.  Here are the steps for a round of craps:
# 
# A new shooter (person that throws the dice) throws out the initial roll.  This is called "coming out".  If the shooter 
# rolls a 7 or 11 (any combination 2/5, 3/4, etc), anyone betting on 'pass' line wins... anyone betting on "don't pass" loses.
# 
# If the shooter rolls 2, 3 or 12 on the coming out roll, also known as craps, those that bet on the 'pass' line lose, and 
# those that bet on "don't pass" win.  The round is over and the next shooter is called.
# 
# If any other combination is hit (4,5,6,8,9,10), the shooter has hit a 'point'.  The object then is to roll that same point 
# before rolling a 7.  
# 
# For example:
# Coming out, shooter rolls 6.
# 6 is the point.
# shooter rolls 8, point is not made, shooter rolls again.
# shooter rolls 4, point is not made, shooter rolls again.
# shooter rolls 5, point is not made, shooter rolls again.
# shooter rolls 2, point is not made, shooter rolls again.
# shooter rolls 12, point is not made, shooter rolls again.
# shooter rolls 6, point is made!  Pass bets win, don't pass bets lose. 
# 
# If the shooter would have rolled a 7 after the point, the round is over, pass bets lose, don't pass bets win.
# 
# There are 1,000 other ways to bet, the object if this exercise is not to teach you that side of the game (the evil side of
# the game).  I've illustrated the winners and losers so you'd know the object of the game. 
#
# Check out Wikipedia (great source for math/formulas).  
# http://en.wikipedia.org/wiki/Binomial_distribution
# Near the middle of the screen is a binomial distribution for a biased coin flipped 6 times.  Biased = not exactly 50/50 odds.
# In the example, heads is biased to 30/70 (meaning 30% chance of tossing heads on one throw).  Binomial distribution determines
# the trial's outcome on SUCCESSIVE trials.  
# 
# Note the cumulative probability....  For the six trials, it sums to exactly 1.
# 
# Let's read the results.  If you flipped a biased coin 6 times, the probability of hitting 2 heads would be around 32.41%.  
# The probability of getting 4 heads out of 6 is 5.95%.
# 
# What's the probability of seven straight passes?  If you use the probability in this example... adjust for the winning odds 
# (X / 36, where 36 = total combinations of two dice, six sides each, X = total number of winners coming out).
# 
# List the probability of each trial (zero passes, 1 pass, 2 passes, 3 passes, etc).  Check your work, the sum total of distribution
# should be 1.
# 
# Deliverable:
#
# An assembly program.  Program should prompt for:
# * Number of total trials
# * Number of successful trials
#  
# The program should then calculate the binomial distribution, on screen.
################################################################################################################

.data
one: .float 1.0
eight: .float 8.0
thirtysix: .float 36.0
probability: .asciiz "The binomial distribution would yield a probability of:\n "
new_line: .asciiz "\n"
prompt1: .asciiz "Please enter the number of total trials:\n"
prompt2: .asciiz "Please enter the number of successful trials:\n"
prompt3: .asciiz "Please enter the biased probabilty of passing:\n"
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
    mov.s $f1, $f0		# f1 holds the number of total trials (n)
    mov.s $f3, $f0		# f3 holds the number of total trials (n)
    
    li $v0, 4			# service 4 is print string
    la $a0, prompt2
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f2, $f0		# f2 holds the number of successful trials (k)
    mov.s $f4, $f0		# f4 holds the number of successful trials (k)
    
    li $v0, 4			# service 4 is print string
    la $a0, prompt3
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s $f14, $f0		# f14 holds the biased probability p
    	
    jal   factorial_reduced	# factorial of first f2 elements of f1 (n * n-1 * ... * n-k+1)
    mov.s $f8, $f5		# f8 holds result of reduced factorial
    
    mov.s $f1, $f4			
    jal   factorial 		# factorial of f1
    mov.s $f9, $f5		# f9 holds k!
    
    div.s $f10, $f8, $f9	# f10 = f8 / f9 = n choose k
    
    # For unbiased dice
    #la $t7, eight
    #la $t9, thirtysix
    #l.s $f11, 0($t7)
    #l.s $f13, 0($t9)
    #div.s $f14, $f11, $f13	# f14 = 8/36 = p
    
    la $t8, one
    l.s $f12, 0($t8)
    sub.s $f15, $f12, $f14	# f15 = 1-p
    
    cvt.w.s $f3, $f3
    mfc1 $t0, $f3		# t0 = n
    cvt.w.s $f4, $f4		
    mfc1 $t1, $f4		# t1 = k
    
    sub $t2, $t0, $t1		# t2 = n - k
    
    beq $t1, $zero, skip1
    
    li $t3, 0
    mov.s $f16, $f14
exp1:
    addi $t3, $t3, 1
    beq $t3, $t1, end1
    mul.s $f16, $f16, $f14
    j exp1
end1:
    j continue1

skip1:
    l.s $f16, 0($t8)
    
continue1:
    beq $t2, $zero, skip2
    
    li $t3, 0
    mov.s $f17, $f15
exp2:
    addi $t3, $t3, 1
    beq $t3, $t2, end2
    mul.s $f17, $f17, $f15
    j exp2
end2:
    j continue2

skip2:
    l.s $f17, 0($t8)

continue2:
    # f16 holds p^k
    # f17 holds (1-p)^(n-k)
    
    mul.s $f18, $f16, $f17
    mul.s $f18, $f18, $f10
    
    #f18 holds result: (n choose k)*(p^k)*((1-p)^(n-k))
    
    # print result
    li $v0, 4			# service 4 is print string
    la $a0, probability
    syscall
    mov.s $f12, $f18		
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
    sub.s $f7, $f1, $f2 	# f7 = n - k

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
    

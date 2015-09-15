# This program does all math with single floating point precision. It assumes the user enters a valid input.
# Author: Anders Van Winkle
# 2/26/2014
# CISC260: Chapter 3 - Homework 5

.data
start_prompt: .asciiz "Enter one of the following numbers to calculate the area: \n  1. Square\n  2. Rectangle\n  3. Circle\n  4. Triangle \n  5. Exit\n"
square_prompt: .asciiz "Enter the side length: "
rectangle_width_prompt: .asciiz "Enter the width: "
rectangle_length_prompt: .asciiz "Enter the length: "
triangle_base_prompt: .asciiz "Enter the base length: "
triangle_height_prompt: .asciiz "Enter the height: "
circle_prompt: .asciiz "Enter the radius: "
incorrect_string: .asciiz "\nThe value you have entered is not valid, please try again. \n\n"
answer_string: .asciiz "The area is: "
two_new_lines: .asciiz "\n\n"
goodbye: .asciiz "Goodbye."
pi: .float 3.141592653589793
two: .float 2.0


.text
# main function
main:
    li $v0, 4			# service 4 is print string
    la $a0, start_prompt
    syscall
    li $v0, 5			# service 5 is read integer
    syscall
    
    li $t0, 1
    beq $v0, $t0, square	# if the entered number was 1 jump to square
    li $t0, 2
    beq $v0, $t0, rectangle	# if the entered number was 2 jump to rectangle
    li $t0, 3
    beq $v0, $t0, circle	# if the entered number was 3 jump to circle
    li $t0, 4
    beq $v0, $t0, triangle 	# if the entered number was 4 jump to triangle
    li $t0, 5
    beq $v0, $t0, exit 		# if the entered number was 5, exit the program
    j incorrect			# else the number was not 1-5, try again

# Calculates the area of a square given a single side (A = s^2)
square:
    #input
    li $v0, 4			# service 4 is print string
    la $a0, square_prompt
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    
    #math
    mul.s $f12, $f0, $f0	# f12 = f0*f0 = f0^2
    
    #output
    li $v0, 4			# service 4 is print string
    la $a0, answer_string
    syscall
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, two_new_lines
    syscall
    j main 

# Calculates the area of a rectangle given the length and width (A = LxW) 
rectangle:
    #input
    li $v0, 4			# service 4 is print string
    la $a0, rectangle_width_prompt
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s  $f12, $f0		# store the width in f12
    li $v0, 4			# service 4 is print string
    la $a0, rectangle_length_prompt
    syscall
    li  $v0, 6           	# service 6 is read float
    syscall
    
    #math
    mul.s $f12, $f12, $f0	# put the result of f1 x f0 in f12 to be read and printed
    
    #output
    li $v0, 4			# service 4 is print string
    la $a0, answer_string
    syscall
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, two_new_lines
    syscall
    j main

# Calculates the area of a circle given the radius (A = pi*r^2) 
circle:
    #input
    li $v0, 4			# service 4 is print string
    la $a0, circle_prompt
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    
    #math
    la $a0, pi
    l.s $f12, 0($a0)		#f12 = pi
    mul.s $f12, $f12, $f0	#f12 = f12 * f0 = pi * r
    mul.s $f12, $f12, $f0	#f12 = pi * r^2
    
    #output
    li $v0, 4			# service 4 is print string
    la $a0, answer_string
    syscall
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, two_new_lines
    syscall
    j main

# Calculates the area of a triangle given the base and height (A = (1/2)BxH)
triangle:
    #input
    li $v0, 4			# service 4 is print string
    la $a0, triangle_base_prompt
    syscall
    li  $v0, 6           	# service 6 is read float, puts the float in $f0
    syscall
    mov.s  $f12, $f0		# store the base in f12
    li $v0, 4			# service 4 is print string
    la $a0, triangle_height_prompt
    syscall
    li  $v0, 6           	# service 6 is read float
    syscall
    
    #math
    la $a0, two
    l.s $f2, 0($a0)		#f2 = two = 2.0
    mul.s $f12, $f12, $f0	# f12 = A = BxH
    div.s $f12, $f12, $f2	# A = (1/2) BxH
    
    #output
    li $v0, 4			# service 4 is print string
    la $a0, answer_string
    syscall
    li  $v0, 2           	# service 2 is print float in f12
    syscall
    li $v0, 4			# service 4 is print string
    la $a0, two_new_lines
    syscall
    j main

# The entered number was not valid, send the user back to try again
incorrect:
    li $v0, 4			# service 4 is print string
    la $a0, incorrect_string
    syscall
    j main			#try again

exit:
    li $v0, 4			# service 4 is print string
    la $a0, goodbye
    syscall

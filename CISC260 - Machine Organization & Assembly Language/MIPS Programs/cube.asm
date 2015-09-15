.text
main:
li $a0, 4     # input value to be cubed
jal procedure # call procedure
move $a0, $v0
li $v0, 1
syscall	      # prints 64
j end

procedure:
mult $a0, $a0
mflo $t0
mult $a0, $t0
mflo $v0
jr $ra # return

end:
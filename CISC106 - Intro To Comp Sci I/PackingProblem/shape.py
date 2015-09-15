"""
------------------------------------------------------------------------------
--                                                                          --
--                           P A C K I N G                                  --
--                                                                          --
--                          P A C K I N G . P Y                             --
--                                                                          --
------------------------------------------------------------------------------
-- <Anders Van Winkle and Ryan Lavorerio, \n                                --
-- and Brian Kronenbitter and Bryan Goldman>                                --
--                                                                          --
-- <CISC106043L>                                                            --
--                                                                          --
------------------------------------------------------------------------------
-- This file contains serveral functions used in packing problem project.   --
--                                                                          --
------------------------------------------------------------------------------
"""


import sys
import time

#========================================
# Shape class
#========================================

class Shape:
    """
    Returns the shape corresponding to the letter parameter: I, J, L, O, S, T, or Z.

    A "shape" is a nested tuple, where the real world shape is
    represented as Trues. For example, an "L" shape could be
    represented as

    shape = ((False, False, True),(True, True, True))
    """

    #========================================
    # constructor
    #========================================

    def __init__(self, letter, color):

        """
        Shape constructor
        """

        self.x = 0
        self.y = 0
        self.letter = letter
        self.color = color
        self.rotations = 0

        if letter == 'I':
            self.squares = [[True,True,True,True]]
        elif letter == 'J':
            self.squares = [[True, False, False], [True, True, True]]
        elif letter == 'L':
            self.squares = [[False, False, True],[True, True, True]]
        elif letter == 'O':
            self.squares = [[True, True],[True, True]]
        elif letter == 'S':
            self.squares = [[False, True, True],[True, True, False]]
        elif letter == 'T':
            self.squares = [[True,True,True],[False,True,False]]
        elif letter == 'Z':
            self.squares = [[True,True,False],[False,True,True]]
        else:
            self.squares = [[False,False],[False,False]]

    #========================================
    # rotate90
    #========================================

    def rotate90(self):
        """
        Rotates self 90 degrees clockwise (yes, direction matters).
        Returns: None
        """

        answer = []

        for element in self.squares[0]:
            answer += [[]]

        for alist in self.squares:
            index=0
            for num in alist:
                answer[index] = [num] + answer[index]
                index += 1
        
        self.squares = answer

        self.rotations = (self.rotations + 1) % 4

    #========================================
    # show_and_rotate
    #========================================

    def show_and_rotate(self, num_rotations, game):
        """
        Draws shape and shows it rotating to correct orientation
        with a brief pause at each orientation.
        Calls: Game.draw_shape, rotate90
        Returns: None
        """

        #for i in range(4-self.rotations):
        #    self.rotate90()

        #count = 0

        #game.draw_game_board()
        #game.draw_shape(self)
        #time.sleep(1)

        #while count < num_rotations:

        #    self.rotate90()
        #    game.draw_game_board()
        #    game.draw_shape(self)
        #    time.sleep(1)
        #    count += 1

        for i in range(4-self.rotations):
            self.rotate90()

        count = 0

        game.draw_shape_2(self)
        time.sleep(1)

        while count < num_rotations:

            self.rotate90()
            game.draw_shape_2(self)
            time.sleep(1)
            count += 1

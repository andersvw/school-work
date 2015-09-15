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
from Tkinter import *
import time

#========================================
# Board class
#========================================

class Board:
    """
    A board is a 2D list containing Booleans. A "shape" is a nested
    tuple, where the real world shape is represented as Trues. For example, an "L"
    shape could be represented as 

       shape = ((False, False, True),(True, True, True))

    squares is a 2D list containing Booleans.
    """

    #========================================
    # constructor
    #========================================

    def __init__(self, rows, cols, squares):
        """
        Board constructor
        """

        self.rows = rows
        self.cols = cols
        self.squares = squares
        boardSquares = []
        if self.squares == []:
            for row in range(self.rows):
                boardSquares += [[]]
                for col in range(self.cols):
                    boardSquares[row] += [False]
                    self.squares = boardSquares

    #========================================
    # update
    #========================================

    def update(self, shape, game):
        """
        Top-level def for placing a single shape on board. Finds best
        position and rotation for shape, update
        board squares.
        Calls: find_best_place rotate90 
        Returns tuple:  (fits board) boolean and board
        """

        fits = False

        aFit = self.find_best_place(shape)

        shape.x = aFit[1]
        shape.y = aFit[2]

        if aFit[0]:

            fits = True
            while shape.rotations != aFit[3]:
                shape.rotate90()

            for elementY in range(len(shape.squares)):
                for elementX in range(len(shape.squares[elementY])):
                    if shape.squares[elementY][elementX]==True:
                        self.squares[elementY+(aFit[1])][elementX+(aFit[2])] = True

            return (fits, self)

        else:

            return (fits, self)

    #========================================
    # find_best_place
    #========================================

    def find_best_place(self, shape):
        """
        Takes a single shape, finds best position on self. Tries original and 
        three possible 90 degree rotations. Mutates shape.
        Returns tuple:  (fits, max_score_row, max_score_col, num_rotations)
        fits: bool
        max_score_row, max_score_col: upper left coordinates of best position
        for shape on board num_rotations: 0-3 rotations required for best fit.
        Calls: find_one_place, Shape.rotate90
        """

        fits = False
        max_score_row = 0
        max_score_col = 0
        num_rotations = 0
    
        max_score = -1

        for num in range(4):

            switch = True

            if len(shape.squares)> self.rows:
                switch = False
            elif len(shape.squares[0])> self.cols:
                switch = False
            
            if switch == True:
                if self.find_one_place(shape)[0] == True:
                    if self.find_one_place(shape)[3]> max_score:
                        max_score = self.find_one_place(shape)[3]
                        max_score_row = self.find_one_place(shape)[1]
                        max_score_col = self.find_one_place(shape)[2]
                        num_rotations = shape.rotations
                        fits = True

            shape.rotate90()

        return (fits, max_score_row, max_score_col, num_rotations)

    #========================================
    # find_one_place
    #========================================

    def find_one_place(self, shape):
        """
        Takes a single shape in one position and finds the best fit on the
        board. If two fits are the same, then it defaults to the lowest,
        rightmost position on the board. 
        Returns a tuple:  (fits, best_row, best_col, score)
        fits: bool
        best_row, best_col: upper left coordinates of best position for shape
        on self
        score: fill, or other heuristic. Higher is better.
        Calls: fit
        """ 

        fits = False
        best_row = 0
        best_col = 0
        score = 0

        for row in range(self.rows-len(shape.squares)+1):#this brings the shape down on the board
            for col in range(self.cols - len(shape.squares[0])+1):#this is bringing the shape over on the board
                if (self.fit(row,col, shape)[0] == True) and (self.fit(row,col, shape)[1] >= score):
                    score = self.fit(row,col, shape)[1]
                    best_row = row #this will be equal to shape.y
                    best_col = col #this will be equal to shape.x
                if fits==False:
                    if self.fit(row,col,shape)[0] == False:
                        fits = False
                        score = -1
                    elif self.fit(row,col,shape)[0]:
                        fits = True

        return (fits, best_row, best_col, score)

    #========================================
    # fit
    #========================================

    def fit(self, upleft_Row, upleft_Col, shape):
        """
        upleft_Row, upleft_Col are upper left coordinates of position we wish to
        fit shape on self.
        Returns tuple: (it_fits, fill) where it_fits is true if the shape fits
        in the specified location on the board;  fill is the amount of
        "fill" the shape has with other filled space on the board (that is, 
        where a zero in the shape lines up with a one on the board). 
        """

        itFits = True
        fill = 0

        if len(shape.squares)> self.rows:
            return (False,0)
        elif len(shape.squares[0])> self.cols:
            return (False,0)
        if self.squares == []:
            itFits = True
            fill = 0
            return (itFits,fill)
        for elementY in range(len(shape.squares)):
            for elementX in range(len(shape.squares[elementY])):
                if (shape.squares[elementY][elementX]==True) and (self.squares[elementY + upleft_Row][elementX + upleft_Col])==True:
                    fill = 0
                    itFits = False
                    return (itFits,fill)
                elif (shape.squares[elementY][elementX] == False) and (self.squares[elementY + upleft_Row][elementX + upleft_Col] == True):
                    fill += 1
                    itFits = True

        return (itFits,fill)

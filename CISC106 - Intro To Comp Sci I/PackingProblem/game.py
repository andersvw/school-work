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


from Tkinter import *

#========================================
# Game class
#========================================

class Game(Frame):
    """
    A Game is a Frame with a canvas for drawing a Tetris board.  It takes a
    reference (alias) to a list of shapes that are on the board, and a size
    in rows and columns.
    """

    #========================================
    # constructor
    #========================================

    def __init__(self, rows, cols, shapes_list, master=None):
        """
        Game constructor
        """

        self.rows = rows
        self.cols = cols
        self.shapes_list = shapes_list

        self.height = self.rows * 20
        self.width = self.cols * 20

        self.frame = Frame(master)
        self.frame.grid()

        self.shapeLabel = Label(self.frame, anchor='w', text='Shape ( currently active object )')
        self.shapeLabel.grid(row=0, column=0, sticky=N+E+S+W)

        self.shapeCanvas = Canvas(self.frame, height=self.rows*8, width=self.width, bg='white')
        self.shapeCanvas.grid(row=1, column=0)

        self.boardLabel = Label(self.frame, anchor='w', text='Board')
        self.boardLabel.grid(row=2, column=0, sticky=N+E+S+W)

        self.canvas = Canvas(self.frame, height=self.height, width=self.width, bg='white')
        self.canvas.grid(row=3, column=0)

    #========================================
    # draw_square
    #========================================

    def draw_square(self, x, y, color):
        """
        Draws on board canvas a square in GRID location x, y.  Keep in mind that a grid
        location (and therefor each square) is 20 by 20 pixels.
        """

        x1 = x * 20
        y1 = y * 20
        x2 = x1 + 20
        y2 = y1 + 20

        self.canvas.create_rectangle(x1, y1, x2, y2, fill=color)
        self.canvas.update_idletasks()

    #========================================
    # draw_square_2
    #========================================

    def draw_square_2(self, x, y, color):
        """
        Draws on shape canvas a square in GRID location x, y.  Keep in mind that a grid
        location (and therefor each square) is 20 by 20 pixels.
        """

        x1 = x * 20
        y1 = y * 20
        x2 = x1 + 20
        y2 = y1 + 20

        self.shapeCanvas.create_rectangle(x1, y1, x2, y2, fill=color)
        self.shapeCanvas.update_idletasks()

    #========================================
    # draw_shape
    #========================================

    def draw_shape(self, shape):
        """
        Draws shape on board canvas
        calls: draw_square
        """
        
        for y in range(len(shape.squares)):
            for x in range(len(shape.squares[0])):
                if shape.squares[y][x] == True:
                    self.draw_square(shape.y+x, shape.x+y, shape.color)

    #========================================
    # draw_shape_2
    #========================================

    def draw_shape_2(self, shape):
        """
        Draws shape on shape canvas
        calls: draw_square_2
        """
        
        self.shapeCanvas.delete(ALL)

        for y in range(len(shape.squares)):
            for x in range(len(shape.squares[0])):
                if shape.squares[y][x] == True:
                    self.draw_square_2(shape.y+x, shape.x+y, shape.color)

    #========================================
    # draw_game_board
    #========================================

    def draw_game_board(self):
        """
        Draws the entire game board.  This can be done by simply using self's
        alias to the list of shapes on the board.
        calls: draw_shape.
        """

        self.canvas.delete(ALL)
        for shape in self.shapes_list:
            self.draw_shape(shape)



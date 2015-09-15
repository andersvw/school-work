"""
------------------------------------------------------------------------------
--                                                                          --
--                           P A C K I N G                                  --
--                                                                          --
--                          P A C K I N G . P Y                             --
--                                                                          --
------------------------------------------------------------------------------
-- <Anders Van Winkle and Ryan Lavorerio,                                   --
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
import config
from custom_test_environment import *
from shape import *
from board import *
from game import *
import webbrowser

#========================================
# constants
#========================================

root_title = 'Project 2'
cte_title = 'Custom Test Environment'

#========================================
# start method
#========================================

def start(parent, rows, cols):
    """
    Makes a blank Board of size rows by cols, makes a corresponding Game on
    which to draw the game happening, seeds the random number generator,
    schedules run, and starts the event loop.
    Calls: run (via Game.after)
    Returns: None
    """
    try:

        # squares is a 2D list containing booleans

        config.cte.write("Creating a board ...")

        squares = []
        config.board = Board(rows, cols, squares)

        config.cte.write("Creating a game ...")

        shapes_list = []
        config.game = Game(rows, cols, shapes_list, parent)

        random.seed()

        #config.game.after(2000, run, config.board, [], config.game)

    except:

        print "Exception: "
        print sys.exc_info()[0]
        print sys.exc_info()[1]
        print sys.exc_info()[2]

#========================================
# run method
#========================================

def run(board, shapes, game):
    """
    Repeatedly generates a new shape and then fits that shape on the current
    board.  On each pass, the new shape should be shown in the upper left
    corner of the board, then rotated to the appropriate orientation (the
    rotation should be animated to show each step,) and finally placed in
    the appropriate location on the board.
    calls: get_shape, Game.draw_game_board, Board.update.
    """

    more = True
    while(more):
        shape_letters = ['I','J','L','O','S','T','Z']
        shape_colors = ['blue','yellow','green','orange','pink','red','purple']
        shape_index = random.randint(0,6)
        self.write(str(shape_index))
        shape = Shape(shape_letters[shape_index],shape_colors[shape_index])
        shape2 = Shape(shape_letters[shape_index],shape_colors[shape_index])
        fits = config.board.update(shape, config.game)[0]
        shape2.show_and_rotate(shape.rotations, config.game)
        if fits:
            config.game.shapes_list.append(shape)
            config.game.draw_game_board()
        else:
            more = False

#========================================
# MUSIC!!
#========================================

#webbrowser.open("01 Dream On.mp3")
#webbrowser.open("What Is Love.mp3")


#========================================
# 
#========================================

if __name__ == '__main__':

    root = Tk()
    root.title(root_title)

    top = Toplevel()
    top.title(cte_title)
    
    config.cte = CustomTestEnvironment(top)

    start(root, 22, 10)

    root.mainloop()
    



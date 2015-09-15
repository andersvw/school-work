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
import random
from datetime import datetime
from Tkinter import *
import Tkconstants
import tkFileDialog
import tkFont
import config
from shape import *

#========================================
# Constants
#========================================

header_text = 'Custom Test Environment'

padding_x = 4
padding_y = 4

#========================================
# Custom Test Environment class
#========================================

class CustomTestEnvironment:

    #========================================
    # Constructor
    #========================================

    def __init__(self, parent):

        try:

            # Set reference to parent

            self.parent = parent

            # Set formatting variables

            self.indentSize = 4
            self.indentLevel = 0

            # Create a top-level menu

            self.mainMenu = Menu(self.parent)
            self.parent["menu"] = self.mainMenu

            fileMenu = Menu(self.mainMenu, tearoff=0)
            self.mainMenu.add_cascade(label="File", menu=fileMenu)

            fileMenu.add_command(label="Exit", command=self.file_exit_menu_handler)

            helpMenu = Menu(self.mainMenu, tearoff=0)
            self.mainMenu.add_cascade(label="Help", menu=helpMenu)

            helpMenu.add_command(label="Version", command=self.help_version_menu_handler)
            helpMenu.add_separator()
            helpMenu.add_command(label="About", command=self.help_about_menu_handler)

            # Create the frame

            self.frame = Frame(parent)
            self.frame.grid()

            self.frame.columnconfigure(0, minsize=60)
            self.frame.columnconfigure(1, minsize=60)
            self.frame.columnconfigure(2, minsize=150)
            self.frame.columnconfigure(3, minsize=150)
            self.frame.columnconfigure(4, minsize=150)
            self.frame.columnconfigure(5, minsize=150)

            for i in range(8):
                self.frame.rowconfigure(i, minsize=20)

            self.add_padding(self.frame, padding_x, padding_y)

            # Create the widgets

            self.fntHeader = tkFont.Font(family="Tahoma", size=16, weight="bold")

            self.lblHeader = Label(self.frame, font=self.fntHeader, text=header_text)
            self.lblHeader.grid(row=0, column=0, columnspan=6)

            self.btnTest1 = Button(self.frame, text="Test 1", command=self.test_1_button_handler)
            self.btnTest1.grid(row=2, column=0, padx=2, sticky=E+W)

            self.btnTest2 = Button(self.frame, text="Test 2", command=self.test_2_button_handler)
            self.btnTest2.grid(row=3, column=0, padx=2, sticky=E+W)

            self.btnTest3 = Button(self.frame, text="Test 3", command=self.test_3_button_handler)
            self.btnTest3.grid(row=4, column=0, padx=2, sticky=E+W)

            self.btnTest4 = Button(self.frame, text="Test 4", command=self.test_4_button_handler)
            self.btnTest4.grid(row=5, column=0, padx=2, sticky=E+W)

            self.btnTest5 = Button(self.frame, text="Test 5", command=self.test_5_button_handler)
            self.btnTest5.grid(row=2, column=1, padx=2, sticky=E+W)

            self.btnTest6 = Button(self.frame, text="Test 6", command=self.test_6_button_handler)
            self.btnTest6.grid(row=3, column=1, padx=2, sticky=E+W)

            self.btnTest7 = Button(self.frame, text="Test 7", command=self.test_7_button_handler)
            self.btnTest7.grid(row=4, column=1, padx=2, sticky=E+W)

            self.btnTest8 = Button(self.frame, text="Test 8", command=self.test_8_button_handler)
            self.btnTest8.grid(row=5, column=1, padx=2, sticky=E+W)

            # Create the message display widgets

            self.btnClear = Button(self.frame, text="Clear", command=self.clear)
            self.btnClear.grid(row=7, column=0, padx=2, sticky=E+W)

            self.btnSave = Button(self.frame, text="Save", command=self.save)
            self.btnSave.grid(row=7, column=1, padx=2, sticky=E+W)

            self.dspFrame = Frame(self.frame, bd=2, relief='sunken')
            self.dspFrame.grid(row=8, column=0, columnspan=6, sticky=N+E+S+W)

            self.dspFrame.grid_rowconfigure(0, weight=1)
            self.dspFrame.grid_columnconfigure(0, weight=1)

            self.dspHScrollbar = Scrollbar(self.dspFrame, orient=HORIZONTAL)
            self.dspHScrollbar.grid(row=1, column=0, sticky=E+W)

            self.dspVScrollbar = Scrollbar(self.dspFrame, orient=VERTICAL)
            self.dspVScrollbar.grid(row=0, column=1, sticky=N+S)

            self.dspText = Text(self.dspFrame, wrap=NONE, bd=0, xscrollcommand=self.dspHScrollbar.set, yscrollcommand=self.dspVScrollbar.set)
            self.dspText.grid(row=0, column=0, sticky=N+S+E+W)

            self.dspHScrollbar.config(command=self.dspText.xview)
            self.dspVScrollbar.config(command=self.dspText.yview)

            # Create status bar widget

            self.stsFrame = Frame(self.frame)
            self.stsFrame.grid(row=9, column=0, columnspan=6, sticky=N+E+S+W)

            self.stsBar = Label(self.stsFrame, text="", bd=1, relief=SUNKEN, anchor=W)
            self.stsBar.pack(side=BOTTOM, fill=X)
 
            # Set focus

            self.frame.focus_set()

        except:

            print "Exception: "
            print sys.exc_info()[0]
            print sys.exc_info()[1]
            print sys.exc_info()[2]

    def add_padding(self, container, padx=1, pady=1):

       nc, nr = container.grid_size() 
       for i in range(nc): 
         container.grid_columnconfigure(i, pad=padx) 
       for i in range(nr): 
         container.grid_rowconfigure(i, pad=pady) 

    #========================================
    # StatusBar methods
    #========================================

    def clear_status(self):

        self.stsBar.config(text="")
        self.stsBar.update_idletasks()

    def write_status(self, msg):

        self.stsBar.config(text=msg)
        self.stsBar.update_idletasks()

    #========================================
    # Display methods
    #========================================

    def clear(self):

        self.dspText.delete("1.0", END)

    def indent(self):

        self.indentLevel += 1

    def save(self):

        fil_options = {}

        fil_options['defaultextension'] = 'log'
        fil_options['filetypes'] = [('All Files', '.*'), ('Log Files', '.log'), ('Text Files', '.txt')]
        fil_options['initialdir'] = ''
        fil_options['initialfile'] = 'display.log'
        fil_options['parent'] = self.parent
        fil_options['title'] = 'Save Display Log'

        fout = tkFileDialog.asksaveasfile(**fil_options)

        fout.write(self.dspText.get('0.0', END))
        fout.close()

    def unindent(self):

        if self.indentLevel > 0:
            self.indentLevel -= 1

    def write(self, msg):

        self.dspText.insert(END, datetime.now().strftime('%Y-%m-%d %H:%M:%S  '))
        if self.indentLevel > 0:
            self.dspText.insert(END, ' '*self.indentSize*self.indentLevel)
        self.dspText.insert(END, msg)
        self.dspText.insert(END, '\n')

    #========================================
    # Menu handlers
    #========================================

    def file_exit_menu_handler(self):

        sys.exit(0)

    def help_version_menu_handler(self):

        self.write("1.0.0.0")

    def help_about_menu_handler(self):

        self.write("Written by Anders L. Van Winkle")

    #========================================
    # Button handlers
    #========================================

    #----------------------------------------
    # Test 1 button handler
    #----------------------------------------

    def test_1_button_handler(self):

        self.write("Test number 1 initiated")
        self.indent()

        self.write("Drawing an I Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('I','blue'))

        time.sleep(1)

        self.write("Drawing a J Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('J','yellow'))
        
        time.sleep(1)

        self.write("Drawing an L Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('L','green'))
        
        time.sleep(1)

        self.write("Drawing an O Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('O','orange'))
        
        time.sleep(1)

        self.write("Drawing an S Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('S','pink'))
        
        time.sleep(1)

        self.write("Drawing a T Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('T','red'))
        
        time.sleep(1)

        self.write("Drawing a Z Shape")
        config.game.shapeCanvas.delete(ALL)
        config.game.draw_shape_2(Shape('Z','purple'))
        
        self.unindent()
        self.write("Test number 1 completed")

    #----------------------------------------
    # Test 2 button handler
    #----------------------------------------

    def test_2_button_handler(self):

        self.write("Test number 2 initiated")
        self.indent()

        shape_letters = ['I','J','L','O','S','T','Z']
        shape_colors = ['blue','yellow','green','orange','pink','red','purple']
        for i in range(6):
            shape = Shape(shape_letters[i],shape_colors[i])
            shape.show_and_rotate(3, config.game)

        self.unindent()
        self.write("Test number 2 completed")

    #----------------------------------------
    # Test 3 button handler
    #----------------------------------------

    def test_3_button_handler(self):

        self.write("Test number 3 initiated")
        self.indent()

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

        self.unindent()
        self.write("Test number 3 completed")

    #----------------------------------------
    # Test 4 button handler
    #----------------------------------------

    def test_4_button_handler(self):

        self.write("Test number 4")

    #----------------------------------------
    # Test 5 button handler
    #----------------------------------------

    def test_5_button_handler(self):

        self.write("Test number 5")
        self.write_status('Success!')

    #----------------------------------------
    # Test 6 button handler
    #----------------------------------------

    def test_6_button_handler(self):

        self.write("Test number 6")
        self.clear_status()

    #----------------------------------------
    # Test 7 button handler
    #----------------------------------------

    def test_7_button_handler(self):

        self.write("Test number 7")

    #----------------------------------------
    # Test 8 button handler
    #----------------------------------------

    def test_8_button_handler(self):

        self.indent()
        self.write("Test number 8")
        self.unindent()


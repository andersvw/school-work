/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/


package edu.udel.nicolini.pt;

import java.util.Random;

public class PTState {
	
	public PTGrid grid;
	//direction compass (0,90,180,270) (N,E,S,W)
	public int direction;
	public int positionX;
	public int positionY;
	public int number;
    
    public PTState(PTGrid grid) //default constructor
	{
        this.grid = grid;
        
        Random random = new Random();

        this.positionX = random.nextInt(grid.width);
        this.positionY = random.nextInt(grid.height);
        
        if (grid.width - positionX < grid.width/2)
        	direction = 270;
        else
        	direction = 90;
        
    }

    
    @Override
    public String toString()
    {
    	return "Player " + number + " is at (" + positionX + ", " + positionY + ") and moving at compass heading " + direction + " degrees.";
    }
    
    public PTState copy()
    {
    	PTState newState = new PTState(new PTGrid(this.grid.width, this.grid.height));
    	newState.direction = this.direction;
    	newState.number = this.number;
    	newState.positionX = this.positionX;
    	newState.positionY = this.positionY;
    	
    	return newState;
    }
    
}

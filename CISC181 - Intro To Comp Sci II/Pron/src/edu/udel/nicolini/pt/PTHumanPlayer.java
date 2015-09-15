/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/


package edu.udel.nicolini.pt;

import java.util.ArrayList;
import java.util.List;


public class PTHumanPlayer extends PTPlayer{

	
	PTHumanPlayer(PTGrid grid, PTGame game, int number)
	{
		super(grid, game, number);
	}

	
	public List<PTMove> getAllValidMoves()
	{
		List<PTMove> listOfMoves = new ArrayList<PTMove>();
		
		PTState straightState = state.copy();
		
		if (state.direction == 0)
			straightState.positionY--;
		else if (state.direction == 90)
			straightState.positionX++;
		else if (state.direction == 180)
			straightState.positionY++;
		else if (state.direction == 270)
			straightState.positionX--;
		
		PTMove straightMove = new PTMove(straightState, game);
		
		if (straightMove.isValid())
			listOfMoves.add(straightMove);
		
		return listOfMoves;
	}
	
	public void turnLeft()
    {
        switch (state.direction)
        {
            case 0:
            {
                state.direction = 270;
                break;
            }
            case 90:
            {
                state.direction = 0;
                break;
            }
            case 180:
            {
                state.direction = 90;
                break;
            }
            case 270:
            {
                state.direction = 180;
                break;
            }
        }
    }
	
	public void turnRight()
    {
        switch (state.direction)
        {
            case 0:
            {
                state.direction = 90;
                break;
            }
            case 90:
            {
                state.direction = 180;
                break;
            }
            case 180:
            {
                state.direction = 270;
                break;
            }
            case 270:
            {
                state.direction = 0;
                break;
            }
        }
    }
	
	@Override
	public PTMove getNextMove() 
	{
		
		if (getAllValidMoves().size() == 0)
			return null;
		
		return getAllValidMoves().get(0);
	}
	
	
}

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


public class PTAI extends PTPlayer{
    
	
	PTAI(PTGrid grid, PTGame game, int number)
	{
		super(grid, game, number);
	}
	
	public List<PTMove> getAllValidMoves()
	{
		List<PTMove> listOfMoves = new ArrayList<PTMove>();
		PTState leftState = state.copy();
		
		switch (leftState.direction)
		{
			case 0:
			{
				leftState.positionX--;
				leftState.direction = 270;
				break;
			}
			case 90:
			{
				leftState.positionY--;
				leftState.direction = 0;
				break;
			}
			case 180:
			{
				leftState.positionX++;
				leftState.direction = 90;
				break;
			}
			case 270:
			{
				leftState.positionY++;
				leftState.direction = 180;
				break;
			}
		}
		
		PTState rightState = state.copy();
		
		switch (rightState.direction)
		{
			case 0:
			{
				rightState.positionX++;
				rightState.direction = 90;
				break;
			}
			case 90:
			{
				rightState.positionY++;
				rightState.direction = 180;
				break;
			}
			case 180:
			{
				rightState.positionX--;
				rightState.direction = 270;
				break;
			}
			case 270:
			{
				rightState.positionY--;
				rightState.direction = 0;
				break;
			}
		}
		
		
		PTState straightState = state.copy();
		
		if (state.direction == 0)
			straightState.positionY--;
		else if (state.direction == 90)
			straightState.positionX++;
		else if (state.direction == 180)
			straightState.positionY++;
		else if (state.direction == 270)
			straightState.positionX--;
		
		PTMove leftMove = new PTMove(leftState, game);
		PTMove rightMove = new PTMove(rightState, game);
		PTMove straightMove = new PTMove(straightState, game);
		
		if (leftMove.isValid())
		{
			listOfMoves.add(leftMove);
		}
		if (rightMove.isValid())
		{
			listOfMoves.add(rightMove);
		}
		if (straightMove.isValid())
		{
			listOfMoves.add(straightMove);
		}
	
		return listOfMoves;
	}
	
	
	public PTMove getNextMove()
	{
		List<PTMove> validMoves = getAllValidMoves();
		int currScore = 0;
		int highScore = 0;
		
		for (PTMove move : validMoves)
		{
			currScore = move.getHeuristicScore();
			if (currScore > highScore)
				highScore = currScore;
		}
		
		for (PTMove move : validMoves)
		{
			if (move.getHeuristicScore() == highScore)
			{
				return move;
			}
		}
		return null;
	}
}

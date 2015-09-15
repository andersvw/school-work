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
import java.util.Random;

/**
 * PTRandomPlayer is for testing purposes only
 * 
 * @author nicknicolini
 *
 */

public class PTRandomPlayer extends PTPlayer{

	
	public PTRandomPlayer(PTGrid grid, PTGame game, int number) {
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
				leftState.positionX-= 1;
				leftState.direction = 270;
				break;
			}
			case 90:
			{
				leftState.positionY-= 1;
				leftState.direction = 0;
				break;
			}
			case 180:
			{
				leftState.positionX+= 1;
				leftState.direction = 90;
				break;
			}
			case 270:
			{
				leftState.positionY+= 1;
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
		
		if (straightState.direction == 0)
			straightState.positionY--;
		else if (straightState.direction == 90)
			straightState.positionX++;
		else if (straightState.direction == 180)
		{
			straightState.positionY++;
			System.out.println("direction 180");
		}
		else if (straightState.direction == 270)
			straightState.positionX--;
		
		
		PTMove leftMove = new PTMove(leftState, game);
		PTMove rightMove = new PTMove(rightState, game);
		
		PTMove straightMove = new PTMove(straightState, game);
		
		
		if (leftMove.isValid())
			listOfMoves.add(leftMove);
		if (rightMove.isValid())
			listOfMoves.add(rightMove);
		
		if (straightMove.isValid())
			listOfMoves.add(straightMove);
		
		return listOfMoves;
	}
	
	@Override
	public PTMove getNextMove() 
	{
		int index;
		
		Random random = new Random();
		
		if (getAllValidMoves().size() == 0)
			return null;
		

		index = random.nextInt(getAllValidMoves().size());
		return getAllValidMoves().get(index);
	}
}

/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/


package edu.udel.nicolini.pt;

public class PTMove {
	
	public PTGame game;
	public PTState state;
	public int movementRating;
	
	
	public PTMove(PTState state, PTGame game)
	{
		this.state = state;
		this.game = game;
		movementRating = 0;
	}
	
	public boolean isValid()
	{
		boolean flag = true;
		
		
		if (state.positionX >= state.grid.width)
			flag = false;
		if (state.positionY >= state.grid.height)
			flag = false;
		if (state.positionX < 0)
			flag = false;
		if (state.positionY < 0)
			flag = false;
		
		if (flag)
		{
			if (game.grid.grid[state.positionY][state.positionX] != 0)
			{
				flag = false;
			}
		}
		return flag;
	}
	
	@Override
	public String toString()
	{
		return "Current player has moved to X position " + state.positionX + " and Y position " + state.positionY + ".";
	}
	
	/**
     * Gets the heuristic score from a player for a given state. This score
     * should roughly estimate how close the player is to winning.
     * 
     * @return int score
     */
	public int getHeuristicScore()
	{
		int score = 0;
				
		if (state.direction == 0)
		{
			score = state.positionY;
		}
		else if (state.direction == 90)
		{
			score = state.grid.width - state.positionX;
		}
		else if (state.direction == 180)
		{
			score = state.grid.height - state.positionY;
		}
		else if (state.direction == 270)
		{
			score = state.positionX;
		}
		
		movementRating = score;
		
		return movementRating;
	}

}

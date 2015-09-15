/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

public class PTObstacle {

	PTGrid grid;
	PTGame game;
	
	public PTObstacle(PTGrid grid, PTGame game)
	{
		this.grid = grid;
		this.game = game;
	}
	
	public void placeObstacle(int x, int y)
	{
		grid.grid[y][x] = 9;
	}
}

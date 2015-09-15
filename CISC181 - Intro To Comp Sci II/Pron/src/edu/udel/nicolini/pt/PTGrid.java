/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/


package edu.udel.nicolini.pt;

public class PTGrid
{
	public int width;
	public int height;
	
	public int[][] grid;
	
	public PTGrid(int width, int height)
	{
		this.width = width;
		this.height = height;
		grid = new int[width][height];
		
		for (int x = 0; x<width; x++)
		{
			for (int y = 0; y<height; y++)
			{
				grid[x][y] = 0;
			}
		}
	}
	
	
	
	public boolean showGrid()
	{
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				System.out.print(String.format("%d", grid[i][j]));
			}
			System.out.println("");
		}
		System.out.println("");
		return true;
	}
	
	
	
}

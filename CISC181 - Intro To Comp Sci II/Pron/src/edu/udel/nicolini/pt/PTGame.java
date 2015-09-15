/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import java.util.*;

public class PTGame 
{
	public PTGrid grid;
	private PTPlayer player;
	private PTPlayer winner;
	private PTObstacle obstacle;
	
	private ArrayList<PTPlayer> players = new ArrayList<PTPlayer>();
	public boolean flag;
	
	PTGame()
	{
		grid = new PTGrid(20, 20);
		
		Random random = new Random();
		obstacle = new PTObstacle(grid, this);
		for (int i = 0; i < random.nextInt(grid.width/2); i++)
			obstacle.placeObstacle(random.nextInt(grid.width), random.nextInt(grid.height));
		
		player = new PTHumanPlayer(grid, this, players.size() + 1);
		players.add(player);
		placePlayer(player);
		
		player = new PTAI(grid, this, players.size() + 1);
		players.add(player);
		placePlayer(player);
		
	}
	
	PTGame(PTGrid grid, ArrayList<PTPlayer> players)
	{
		this.grid = grid;
		this.players = players;
	}
	
	public ArrayList<PTPlayer> getPlayers()
	{
		return players;
	}
	
	public void placePlayer(PTPlayer player)
	{
		grid.grid[player.state.positionY][player.state.positionX] = player.state.number;
	}
	
	public PTPlayer getWinner()
	{
		if (players.get(1).getNextMove() == null)
			winner = players.get(0);
		else
			winner = players.get(1);
		return winner;
	}
	
	public void play(int cycles)
	{
		PTMove nextMove = null;
		boolean running = true;
		for (int i = 0; i<cycles; i++)
		{
			for (PTPlayer player : players)
			{
				nextMove = player.getNextMove();
				if (nextMove == null)
				{
					running = false;
					i = cycles;
					break;
				}
				else
				{
					player.state = nextMove.state.copy();
					grid.grid[nextMove.state.positionY][nextMove.state.positionX] = nextMove.state.number;
				}
			}
		}

		if (!running)
		{
			flag = true;
		    System.out.println("Player " + player.state.number + " lost.");
		}
	}
	
	public boolean isEnded()
	{
		return flag;
	}
}

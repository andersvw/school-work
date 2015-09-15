/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

public class PTGameRecord {

	public static final Integer WON = 2;
	public static final Integer LOST = 0;
	
	private String player;
	private int result;
	private long date;
	
	public PTGameRecord(String player, int result, long date)
	{
		this.player = player;
		this.result = result;
		this.date = date;
	}
	
	public String getPlayer()
	{
		return this.player;
	}
	
	public void setPlayer(String player)
	{
		this.player = player;
	}
	
	public int getResult()
	{
		return this.result;
	}
	
	public void setResult(int result)
	{
		this.result = result;
	}
	
	public long getDate()
	{
		return this.date;
	}
	
	public void setDate(long date)
	{
		this.date = date;
	}
	
	
}

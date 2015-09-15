/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/


package edu.udel.nicolini.pt;


public abstract class PTPlayer
{
	
	public PTState state = null;
	public PTGame game = null;
	public PTGrid grid = null;

	public PTPlayer(PTGrid grid, PTGame game, int number)
	{
    	this.grid = grid;
    	this.game = game;
        this.state = new PTState(this.grid);
        this.state.number = number;
	}


	public String toString(){
    	return "Player " + this.state.number;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + state.number;
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        PTPlayer other = (PTPlayer) obj;
        if (state.number != other.state.number)
            return false;
        return true;
    }
    
    public abstract PTMove getNextMove();
    
 // ========================================
    // TurnLeft
    // ========================================

    public void TurnLeft()
    {
        switch (this.state.direction)
        {
            case 0:
            {
                this.state.direction = 270;
                break;
            }
            case 90:
            {
                this.state.direction = 0;
                break;
            }
            case 180:
            {
                this.state.direction = 90;
                break;
            }
            case 270:
            {
                this.state.direction = 180;
                break;
            }
        }
    }

    // ========================================
    // TurnRight
    // ========================================

    public void TurnRight()
    {
        switch (this.state.direction)
        {
            case 0:
            {
                this.state.direction = 90;
                break;
            }
            case 90:
            {
                this.state.direction = 180;
                break;
            }
            case 180:
            {
                this.state.direction = 270;
                break;
            }
            case 270:
            {
                this.state.direction = 0;
                break;
            }
        }
    }

}

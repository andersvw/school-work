/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import android.util.Log;

public class PTGameThread extends Thread
{
	
	boolean isActive = false;
    
    PTGame game = null;
    PTView view = null;
    
    public PTGameThread(PTGame game, PTView view)
    {
        this.game = game;
        this.view = view;
    }

    public void setActive(boolean flag)
    {
        isActive = flag;
    }

    @Override
    public void run()
    {
        try
        {
            while (isActive)
            {
                // Play the game and make moves

                game.play(1);
                
                if(game.isEnded())
                {
                    setActive(false);
                    Log.i("TronGUI","Game over!");
                }

                view.postInvalidate();
                
                Log.i("TronGUI","1 cycle");
                Thread.sleep(10000/game.grid.width);
            }
        }
        catch (Exception ex)
        {
            Log.d("TronGUI", " exception", ex);
        }
        finally
        {
            isActive = false;
        }

    }

}

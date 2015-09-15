/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import android.app.Activity;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

/**
 * 
 * P		R		 O		  N
 * 01010000 01010010 01001111 01001110
 * 
 *    _     _      _     _      _     _      _     _   
  (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)  
   / ._. \      / ._. \      / ._. \      / ._. \   
 __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__ 
(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)
   || P ||      || R ||      || O ||      || N ||   
 _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._ 
(.-./`-'\.-.)(.-./`-`\.-.)(.-./`-'\.-.)(.-./`-'\.-.)
 `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-' 
 * 
 * @author Anders Van Winkle
 *
 */


public class PTActivity extends Activity implements OnClickListener
{
    private PTGame game;
    private PTGameThread gameThread;
    private PTDatabase database;
    private MediaPlayer mP;

    private PTView view;
    private TextView messageBox;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {

        Button btn;

        super.onCreate(savedInstanceState);

        setContentView(R.layout.game);

        btn = (Button) findViewById(R.id.btnGameDown);
        btn.setOnClickListener(this);

        btn = (Button) findViewById(R.id.btnGameLeft);
        btn.setOnClickListener(this);

        btn = (Button) findViewById(R.id.btnGameRight);
        btn.setOnClickListener(this);

        btn = (Button) findViewById(R.id.btnGameUp);
        btn.setOnClickListener(this);

        // Set the message box
        
        messageBox = (TextView) findViewById(R.id.txtMessageBox);
        messageBox.setText("Touch screen to play");
        
        // Set the game view

        database = new PTDatabase(this);
        
        view = (PTView) findViewById(R.id.vPTView);
        view.setDialog(new PTEndGameDialog(view, database));
        view.setDatabase(database);
        view.setMessageBox(messageBox);
        view.setOnClickListener(this);

        // Create the thread used to implement the game

    }

    @Override
    protected void onResume()
    {
        // Ideally a game should implement onResume() and onPause()
        // to take appropriate action when the activity looses focus
        super.onResume();
        mP = MediaPlayer.create(this ,R.raw.derezzed);
        mP.setLooping(true);
        mP.start();
    }

    @Override
    protected void onPause()
    {
        // Ideally a game should implement onResume() and onPause()
        // to take appropriate action when the activity looses focus
        super.onPause();
        mP.stop();
        mP.release();

    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        database.close();
    }
    
    public boolean onCreateOptionsMenu(Menu menu){
        menu.add("Reset Records");
        menu.add("New Game");
        menu.add("Quit");
        return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item) {
        CharSequence title = item.getTitle();
        //PTGame newGame = null;
        if (title.equals("Reset Records")) {
        	database.deleteGameRecords();
        	view.invalidate();
        }
        else if (title.equals("New Game")) {
        	gameThread.setActive(false);
        	game = new PTGame();
            
            view.setGame(game);
            view.setMessageBox(messageBox);
            
            gameThread = new PTGameThread(game, view);
            gameThread.setActive(true);
            gameThread.start();
        }
        else if (title.equals("Quit")) {
            finish();
        }
        
        // If they chose one of the valid menu items, update the current
        // game being played by the view
        if (game != null) {
            view.setGame(game);
        }
        return true;
    }

    @Override
    public void onClick(View v)
    {
        PTPlayer p = null;
        
        if (game != null)
        {
            p = game.getPlayers().get(0);
        }

        if (v.getId() == R.id.vPTView)
        {
            Log.i("TronGUI", "View");

            if (game == null)
            {
                game = new PTGame();
                
                view.setGame(game);
                view.setMessageBox(messageBox);
                
                gameThread = new PTGameThread(game, view);
                gameThread.setActive(true);
                gameThread.start();
            }
            else if (game.isEnded())
            {
            	view.getDialog().insertGameRecord(game);
            	game = null;
            }
            else
            {
                boolean flag = true;
                try
                {
                    gameThread.setActive(false);
                    while (flag)
                    {
                        gameThread.join();
                        flag = false;
                    }
                }
                catch (InterruptedException e)
                {
                }
            }
        }
        
        else if (v.getId() == R.id.btnGameUp)
        {
            Log.i("TronGUI", "Up");
            switch(p.state.direction)
            {
                case 270:
                {
                    p.TurnRight();
                    break;
                }
                case 90:
                {
                    p.TurnLeft();
                    break;
                }
            }
        }
        else if (v.getId() == R.id.btnGameDown)
        {
            Log.i("TronGUI", "Down");
            switch(p.state.direction)
            {
                case 90:
                {
                    p.TurnRight();
                    break;
                }
                case 270:
                {
                    p.TurnLeft();
                    break;
                }
            }
        }
        else if (v.getId() == R.id.btnGameLeft)
        {
            Log.i("TronGUI", "Left");
            switch(p.state.direction)
            {
                case 180:
                {
                    p.TurnRight();
                    break;
                }
                case 0:
                {
                    p.TurnLeft();
                    break;
                }
            }
        }
        else if (v.getId() == R.id.btnGameRight)
        {
            Log.i("TronGUI", "Right");
            switch(p.state.direction)
            {
                case 0:
                {
                    p.TurnRight();
                    break;
                }
                case 180:
                {
                    p.TurnLeft();
                    break;
                }
            }
        }

    }

}

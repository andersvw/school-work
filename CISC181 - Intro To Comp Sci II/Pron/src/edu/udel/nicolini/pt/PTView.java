/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import java.util.Map;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

public class PTView extends View
{
    private PTGame game;

    // a message box that we can display status messages from the game in
    private TextView messageBox;

    // the width and height of the current game view
    private int width;
    private int height;
    private PTDatabase database;
    private PTEndGameDialog dialog;

    public PTView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    }

    public PTEndGameDialog getDialog()
    {
    	return this.dialog;
    }
    
    public void setDialog(PTEndGameDialog dialog)
    {
    	this.dialog = dialog;
    }
    
    public void setDatabase(PTDatabase database)
    {
    	this.database = database;
    }
    
    public void setGame(PTGame game)
    {
        this.game = game;
    }

    public void setMessageBox(TextView messageBox)
    {
        this.messageBox = messageBox;
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);

        if (game != null)
        {
            drawGrid(canvas);
            drawPieces2d(canvas);

            updateMessageBox();
        }

    }

    /**
     * The status text will display the current player whose move it is during
     * the game. When the game has completed it will show the winner or draw,
     * and a short instruction on how to play again.
     */
    private void updateMessageBox() {
        // draw text for message
        String message = "Touch to play";
        if (game.isEnded()) {
            PTPlayer winner = game.getWinner();
            Map<Integer, Integer> summary = database.getScoreSummary(dialog.getLastInitials());
            message = "Player " + winner.state.number + " wins! Touch to play again. " +
	                dialog.getLastInitials() + " W:" + summary.get(2) + " L:" + summary.get(0);
        }
        messageBox.setText(message);
    }

    /**
     * Draws a grid.
     * 
     * @param canvas
     */
    private void drawGrid(Canvas canvas)
    {
        Paint gridPaint = new Paint();
        gridPaint.setColor(Color.WHITE);
        gridPaint.setStrokeWidth(0.5f); // the "weight" of the lines
        gridPaint.setStyle(Style.FILL_AND_STROKE);

        int[][] board = game.grid.grid;
        float scaleX = width / (float) board.length;
        float scaleY = height / (float) board.length;
        // draw horizontal lines for each row
        for (int i = 0; i <= board.length; i++)
        {
            canvas.drawLine(0, i * scaleY, width, i * scaleY, gridPaint);
        }
        // draw vertical lines for each row
        for (int i = 0; i <= board.length; i++)
        {
            canvas.drawLine(i * scaleX, 0, i * scaleX, height, gridPaint);
        }
    }

    private void drawPieces2d(Canvas canvas)
    {
        // creating one Paint object for all x's
        Paint player1Paint = new Paint();
        player1Paint.setColor(Color.RED);
        player1Paint.setStrokeWidth(8);
        player1Paint.setStyle(Style.FILL_AND_STROKE);

        Paint player2Paint = new Paint();
        player2Paint.setColor(Color.BLUE);
        player2Paint.setStrokeWidth(8);
        player2Paint.setStyle(Style.FILL_AND_STROKE);
        
        Paint obstaclePaint = new Paint();
        obstaclePaint.setColor(Color.GREEN);
        obstaclePaint.setStrokeWidth(8);
        obstaclePaint.setStyle(Style.FILL_AND_STROKE);

        int[][] board = game.grid.grid;

        // loop through each position in the game state
        for (int row = 0; row < board.length; row++)
        {
            for (int col = 0; col < board[row].length; col++)
            {
                if (!game.isEnded())
                {
                    if (board[row][col] == 1)
                    {
                        drawSquare(col * (width / (float) board.length), row * (height / (float) board.length), canvas,
                                player1Paint);
                    }
                    if (board[row][col] == 2)
                    {
                        drawSquare(col * (width / (float) board.length), row * (height / (float) board.length), canvas,
                                player2Paint);
                    }
                    if (board[row][col] == 9)
                    {
                        drawSquare(col * (width / (float) board.length), row * (height / (float) board.length), canvas,
                                obstaclePaint);
                    }
                }
            }
        }
    }

    private void drawSquare(float posx, float posy, Canvas canvas, Paint paint)
    {
        int[][] board = game.grid.grid;
        float xheight = (height / (float) board.length) - 10;
        float xwidth = (width / (float) board.length) - 10;

        canvas.drawRect(posx + 5, posy + 5, posx + 5 + xwidth, posy + 5 + xheight, paint);

    }

    /**
     * This method is called by the Android platform when the app window size
     * changes. We store the initial setting of these so that we can compute the
     * exact locations to draw the components of our View.
     */
    protected void onSizeChanged(int w, int h, int oldw, int oldh)
    {
        super.onSizeChanged(w, h, oldw, oldh);

        width = w;
        height = h;
    }
}
    


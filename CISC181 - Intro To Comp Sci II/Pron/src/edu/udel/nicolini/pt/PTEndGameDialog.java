/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.widget.EditText;

public class PTEndGameDialog implements DialogInterface.OnClickListener{
    PTView parentView;
    PTDatabase database;
    EditText input;
    int result;
    String lastInitials;
    
    public PTEndGameDialog(PTView parentView, PTDatabase database) {
        super();
        this.parentView = parentView;
        this.database = database;
        this.lastInitials = "";
    }
    
    
    /**
     * Gets the last initials entered.  Will return "" if none entered yet.
     * 
     * @return
     */
    public String getLastInitials() {
        return lastInitials;
    }

    @Override
    public void onClick(DialogInterface dialog, int whichButton) {
        String value = this.input.getText().toString();
        // format value to only valid letters
        value = value.replaceAll("[^a-zA-Z]", "");
        // only take the first 3, right padded with spaces
        lastInitials = (value + "   ").substring(0,3).toUpperCase();
        
        // Do something with value!
        database.insertGameRecord(new PTGameRecord(lastInitials, result, System.currentTimeMillis()));
        
        parentView.invalidate();
    }
    
    /**
     * Obtains the new winner/loser by presenting the user with a dialog input
     * that contains a message and an editable text box.
     * 
     * @param highScore
     */
    public void insertGameRecord(PTGame game) {
        if (game.getWinner() instanceof PTHumanPlayer) {
            this.result = PTGameRecord.WON;
        }
        else {
            this.result = PTGameRecord.LOST;
        }
        
        AlertDialog.Builder alert = new AlertDialog.Builder(parentView.getContext());

        String resultString;
        if (result == PTGameRecord.WON) {
            resultString = "won";
        }
        else {
            resultString = "lost";
        }
        
        alert.setTitle("You " + resultString);
        alert.setMessage("Please enter your initials:");
        
        // get these before we replace the old edit text
        String lastInitials = getLastInitials();
        
        this.input = new EditText(parentView.getContext());
        this.input.setText(lastInitials);
        
        // Set the view to the EditText 
        alert.setView(this.input);

        alert.setPositiveButton("Ok", this);
        alert.show();
    }
}

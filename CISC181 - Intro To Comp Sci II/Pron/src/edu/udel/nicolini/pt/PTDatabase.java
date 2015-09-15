/**
 * Anders Van Winkle
 * Nick Nicolini
 * CISC181010
 * CISC181020L
 * Nick Lacock
 **/

package edu.udel.nicolini.pt;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

public class PTDatabase {
	public static final String DATABASE_NAME = "Tron";
    public static final String TABLE_NAME = "Game_Records";
    Context context;
    SQLiteDatabase database;
    
    /**
     * Creates a database for the PT application. Creates/opens a connection
     * to the underlying Android database.
     * 
     * @param context
     */
    public PTDatabase(Context context) {
        super();
        this.context = context;
        this.database = context.openOrCreateDatabase(DATABASE_NAME, Context.MODE_PRIVATE, null);
        
        createGameRecordsTable();
    }
    
    public void close() {
        this.database.close();
    }
    
    /**
     * Creates a table in the database if it does not exist already.
     */
    private void createGameRecordsTable() {
        database.execSQL("CREATE TABLE IF NOT EXISTS "
                        + TABLE_NAME
                        + " (Player TEXT,"
                        + " Result INTEGER, Date INTEGER);");
    }
    
    /**
     * Resets the database table to empty by deleting all rows.
     */
    public void deleteGameRecords() {
        database.execSQL("DELETE FROM " + TABLE_NAME);
    }
    
    /**
     * Inserts a single record (row) into the database table.
     * 
     * @param record
     */
    public void insertGameRecord(PTGameRecord record) {
        database.execSQL("INSERT INTO "
                        + TABLE_NAME
                        + " (Player, Result, Date)"
                        + " VALUES ('" + record.getPlayer()
                        + "', " + record.getResult()
                        + ", " + record.getDate() + ")");       
    }

    /**
     * Returns a query that selects the current game records for a specific player
     * by querying the table for game records with Player1 equal to name.
     * 
     * @return
     */
    public String getSelectGameRecordsQuery(String name) {
        return "SELECT Player, Result, Date" +
                " FROM " + TABLE_NAME + " WHERE Player = " + "'" + name + "'" 
                + " ORDER BY Date DESC";
    }
    
    /**
     * Queries the GameRecords table using the given query and adds the result rows
     * to a list of records that is returned.
     * 
     * @return
     */
    @SuppressWarnings("unused")
	public List<PTGameRecord> selectGameRecords(String query) {
        Cursor c = database.rawQuery(query, null);
        try {
            /* Get the indices of the Columns we will need */
            int playerColumn = c.getColumnIndex("Player");
            int resultColumn = c.getColumnIndex("Result");
            int dateColumn = c.getColumnIndex("Date");
            
            List<PTGameRecord> records = new ArrayList<PTGameRecord>();
            if (c.moveToFirst()) {
                int i = 1;
                do {
                    PTGameRecord record = new PTGameRecord(
                        c.getString(playerColumn),
                        c.getInt(resultColumn),
                        c.getLong(dateColumn));
                    records.add(record);
                    i++;
                }
                while (c.moveToNext());
            }
            
            return records;
        }
        finally {
            c.close();
        }
    }
    
    public Map<Integer, Integer> getScoreSummary(String player)
    {
    	String query = this.getSelectGameRecordsQuery(player);
    	List<PTGameRecord> recordList = this.selectGameRecords(query);
    	Map<Integer, Integer> scoreMap = new HashMap<Integer, Integer>();
    	scoreMap.put(0, 0);
    	scoreMap.put(1, 0);
    	scoreMap.put(2, 0);
    	for (PTGameRecord record : recordList)
    	{
    		int value = scoreMap.remove(record.getResult());
    		scoreMap.put(record.getResult(), value+1);
    	}
    	return scoreMap;
    }
}

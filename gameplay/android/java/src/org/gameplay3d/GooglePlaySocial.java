    
package org.gameplay3d;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.widget.Toast;
import com.google.android.gms.games.Player;
import com.google.example.games.basegameutils.BaseGameActivity;

public class GooglePlaySocial extends BaseGameActivity 
{	
	private static List<Player> mFriends = new ArrayList<Player>();
    private static final int FRIENDS_PER_PAGE = 10;
	
	public static void gameServicesSignIn() 
	{
        ((appname)mContext).runOnUiThread(new Runnable() 
		{
            public void run() {
                ((appname)mContext).beginUserInitiatedSignIn();
            }
        });
    }

    public static void updateTopScoreLeaderboard(int score) 
	{
		if (mContext.isSignedIn())
			mContext.getGamesClient().submitScore("leaderboardid", score);
    }

	public static void submitAchievement(String achievementId)
	{
        if (mContext.isSignedIn())
		{
            mContext.getGamesClient().unlockAchievement(achievementId);
        }
    }
	
	public static void incrementAchievement(String achievementId, int percentage)
	{
        if (mContext.isSignedIn())
		{
            mContext.getGamesClient().incrementAchievement(achievementId, percentage);
        }
    }

    public static void displayLeaderboards() 
	{
		if (mContext.isSignedIn())
		{
			((appname)mContext).runOnUiThread(new Runnable() 
			{
				public void run() {
					((appname)mContext).startActivityForResult(((appname)mContext).getGamesClient().getLeaderboardIntent("leaderboardidfromgoogleplay"), 5001);
				}
			});
		}
		else
		{
			gameServicesSignIn();
		}
    }

    public static void displayAchievements() 
	{
		if (mContext.isSignedIn())
		{
			((appname)mContext).runOnUiThread(new Runnable() 
			{
				public void run() {
					((appname)mContext).startActivityForResult(((appname)mContext).getGamesClient().getAchievementsIntent(), 5001);
				}
			});
		}
		else
		{
			gameServicesSignIn();
		}
    }

    public static void loadFriends() 
	{
        if (mFriends.size() > 0) {
            mFriends.clear();
        }

        ((appname)mContext).runOnUiThread(new Runnable() 
		{
                public void run() 
				{        
            ((appname)mContext).getGamesClient().loadInvitablePlayers(new OnPlayersLoadedListener() {

                @Override
                public void onPlayersLoaded(int statusCode, PlayerBuffer playerBuffer) {

                    if (statusCode == GamesClient.STATUS_OK) {
                        for (Player player : playerBuffer) {
                            mFriends.add(player);
                        }

                        if (playerBuffer.getCount() == FRIENDS_PER_PAGE) {
                            ((appname)mContext).getGamesClient().loadMoreInvitablePlayers(this, FRIENDS_PER_PAGE);
                        } else {
                            // call out and return all the friends <more code to call into C++>

                            for (Player friend : mFriends) {
                                Log.i(TAG, String.format("Found player with id [%s] and display name [%s]", friend.getPlayerId(), friend.getDisplayName()));
                            }
                        }
                    }
                }
            }, FRIENDS_PER_PAGE, false);
        });
    }
}
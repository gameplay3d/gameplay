/*
 * Copyright (C) 2013 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gameplay3d;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import com.google.android.gms.appstate.AppStateClient;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.games.GamesClient;
import com.google.android.gms.plus.PlusClient;

/**
 * Example base class for games. This implementation takes care of setting up
 * the GamesClient object and managing its lifecycle. Subclasses only need to
 * override the @link{#onSignInSucceeded} and @link{#onSignInFailed} abstract
 * methods. To initiate the sign-in flow when the user clicks the sign-in
 * button, subclasses should call @link{#beginUserInitiatedSignIn}. By default,
 * this class only instantiates the GamesClient object. If the PlusClient or
 * AppStateClient objects are also wanted, call the BaseGooglePlayActivity(int)
 * constructor and specify the requested clients. For example, to request
 * PlusClient and GamesClient, use BaseGooglePlayActivity(CLIENT_GAMES | CLIENT_PLUS).
 * To request all available clients, use BaseGooglePlayActivity(CLIENT_ALL).
 * Alternatively, you can also specify the requested clients via
 * @link{#setRequestedClients}, but you must do so before @link{#onCreate}
 * gets called, otherwise the call will have no effect.
 *
 * @author Bruno Oliveira (Google)
 */
public abstract class GooglePlayActivity extends FragmentActivity implements GooglePlayClientHelper.GamePlayClientHelperListener {

    // The game helper object. This class is mainly a wrapper around this object.
    protected GamePlayClientHelper mClient;

    // We expose these constants here because we don't want users of this class
    // to have to know about GamePlayClientHelper at all.
    public static final int CLIENT_GAMES = GamePlayClientHelper.CLIENT_GAMES;
    public static final int CLIENT_APPSTATE = GamePlayClientHelper.CLIENT_APPSTATE;
    public static final int CLIENT_PLUS = GamePlayClientHelper.CLIENT_PLUS;
    public static final int CLIENT_ALL = GamePlayClientHelper.CLIENT_ALL;

    // Requested clients. By default, that's just the games client.
    protected int mRequestedClients = CLIENT_GAMES;

    // stores any additional scopes.
    private String[] mAdditionalScopes;

    protected String mDebugTag = "GooglePlayActivity";
    protected boolean mDebugLog = false;

    /** Constructs a BaseGooglePlayActivity with default client (GamesClient). */
    protected GooglePlayActivity() {
        super();
        mClient = new GamePlayClientHelper(this);
    }

    /**
     * Constructs a GooglePlayActivity with the requested clients.
     * @param requestedClients The requested clients (a combination of CLIENT_GAMES,
     *         CLIENT_PLUS and CLIENT_APPSTATE).
     */
    protected GooglePlayActivity(int requestedClients) {
        super();
        setRequestedClients(requestedClients);
    }

    /**
     * Sets the requested clients. The preferred way to set the requested clients is
     * via the constructor, but this method is available if for some reason your code
     * cannot do this in the constructor. This must be called before onCreate in order to
     * have any effect. If called after onCreate, this method is a no-op.
     *
     * @param requestedClients A combination of the flags CLIENT_GAMES, CLIENT_PLUS
     *         and CLIENT_APPSTATE, or CLIENT_ALL to request all available clients.
     * @param additionalScopes.  Scopes that should also be requested when the auth
     *         request is made.
     */
    protected void setRequestedClients(int requestedClients, String ... additionalScopes) {
        mRequestedClients = requestedClients;
        mAdditionalScopes = additionalScopes;
    }

    @Override
    protected void onCreate(Bundle b) {
        super.onCreate(b);
        mClient = new GamePlayClientHelper(this);
        if (mDebugLog) {
            mClient.enableDebugLog(mDebugLog, mDebugTag);
        }
        mClient.setup(this, mRequestedClients, mAdditionalScopes);
    }

    @Override
    protected void onStart() {
        super.onStart();
        mClient.onStart(this);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mClient.onStop();
    }

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
        super.onActivityResult(request, response, data);
        mClient.onActivityResult(request, response, data);
    }

    protected GamesClient getGamesClient() {
        return mClient.getGamesClient();
    }

    protected AppStateClient getAppStateClient() {
        return mClient.getAppStateClient();
    }

    protected PlusClient getPlusClient() {
        return mClient.getPlusClient();
    }

    protected boolean isSignedIn() {
        return mClient.isSignedIn();
    }

    protected void beginUserInitiatedSignIn() {
        mClient.beginUserInitiatedSignIn();
    }

    protected void signOut() {
        mClient.signOut();
    }

    protected void showAlert(String title, String message) {
        mClient.showAlert(title, message);
    }

    protected void showAlert(String message) {
        mClient.showAlert(message);
    }

    protected void enableDebugLog(boolean enabled, String tag) {
        mDebugLog = true;
        mDebugTag = tag;
        if (mClient != null) {
            mClient.enableDebugLog(enabled, tag);
        }
    }

    protected String getInvitationId() {
        return mClient.getInvitationId();
    }

    protected void reconnectClients(int whichClients) {
        mClient.reconnectClients(whichClients);
    }

    protected String getScopes() {
        return mClient.getScopes();
    }

    protected String[] getScopesArray() {
        return mClient.getScopesArray();
    }

    protected boolean hasSignInError() {
        return mClient.hasSignInError();
    }

    protected GamePlayClientHelper.SignInFailureReason getSignInError() {
        return mClient.getSignInError();
    }
}

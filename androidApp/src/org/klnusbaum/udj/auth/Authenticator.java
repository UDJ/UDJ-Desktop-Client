/**
 * Copyright 2011 Kurtis L. Nusbaum
 * 
 * This file is part of UDJ.
 * 
 * UDJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * UDJ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with UDJ.  If not, see <http://www.gnu.org/licenses/>.
 */

package org.klnusbaum.udj.auth;

import android.accounts.AbstractAccountAuthenticator;
import android.accounts.Account;
import android.accounts.AccountAuthenticatorResponse;
import android.accounts.AccountManager;
import android.accounts.NetworkErrorException;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import org.klnusbaum.udj.R;

class Authenticator extends AbstractAccountAuthenticator{

  Context context;
  public Authenticator(Context context){
    super(context);
    this.context= context; 
  }

  public Bundle addAccount(AccountAuthenticatorResponse response,
    String accountType, String authTokenType, String[] requiredFeatures,
    Bundle options) throws NetworkErrorException
  {
    final Intent intent = new Intent(context, AuthActivity.class);
    intent.putExtra(AccountManager.KEY_ACCOUNT_AUTHENTICATOR_RESPONSE,
      response);
    final Bundle bundle = new Bundle();
    bundle.putParcelable(AccountManager.KEY_INTENT, intent);
    return bundle;
  }

  public Bundle confirmCredentials(AccountAuthenticatorResponse response,
    Account account, Bundle options) 
  {
    final Bundle result = new Bundle();
    result.putBoolean(AccountManager.KEY_BOOLEAN_RESULT, true);
    return result;
    /*if(options != null && options.containsKey(AccountManager.KEY_PASSWORD)){
      final String password = options.getString(AccountManager.KEY_PASSWORD);
      final verified = serverConfirmPassword(account.name, password);
      final Bundle result = new Bundle();
      result.putBoolean(AccountManager.KEY_BOOLEAN_RESULT, verified);
      return result;
    }
    return null;*/
  }

  public Bundle editProperties(AccountAuthenticatorResponse response,
    String accountType)
  {
    throw new UnsupportedOperationException();
  }

  public Bundle getAuthToken(AccountAuthenticatorResponse response,
    Account account, String authTokenType, Bundle loginOptions)
  {
    final AccountManager am = AccountManager.get(context);
    final String password = am.getPassword(account);
    final Bundle result = new Bundle();
    result.putString(AccountManager.KEY_ACCOUNT_NAME, account.name);
    result.putString(AccountManager.KEY_ACCOUNT_TYPE,context.getString(R.string.account_type));
    result.putString(AccountManager.KEY_AUTHTOKEN, password);
    return result;
  }

  public String getAuthTokenLabel(String authTokenType){
    return "password";
  }

  public Bundle hasFeatures(AccountAuthenticatorResponse response,
    Account account, String[] freatures)
  {
    final Bundle result = new Bundle();
    result.putBoolean(AccountManager.KEY_BOOLEAN_RESULT, false);
    return result;
  }

  public Bundle updateCredentials(AccountAuthenticatorResponse response,
    Account account, String authTokenType, Bundle loginOptions)
  {
    final Bundle bundle = new Bundle();
    return bundle;
  }

}


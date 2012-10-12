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
#ifndef UDJ_SERVER_CONNECTION_HPP
#define UDJ_SERVER_CONNECTION_HPP

#include <QSqlDatabase>
#include <QDateTime>
#include <QObject>
#include <vector>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ConfigDefs.hpp"

class QNetworkAccessManager;
class QNetworkCookieJar;

namespace UDJ{


/**
 * \brief Represents a connection to the UDJ server.
 */
class UDJServerConnection : public QObject{
Q_OBJECT
public:

  /** @name Constructor */
  //@{

  /**
   * \brief Constructs a UDJServerConnection.
   *
   * \param parent The parent object.
   */
  UDJServerConnection(QObject *parent=NULL);


  //@}

  /** @name Connection Controls */
  //@{

  /**
   * \brief Perform authentication with the server.
   *
   * @param username The username.
   * @param password The password.
   */
  void authenticate(const QString& username, const QString& password);

  /**
   * \brief Sets the ticket to be used when communicating with the server.
   *
   * \param ticket The ticket to be used when communicating with the server.
   */
  inline void setTicket(const QByteArray& ticket){
    ticket_hash = ticket;
  }

  /**
   * \brief Sets the user id to be used when communicating with the server.
   *
   * \param ticket The user id to be used when communicating with the server.
   */
  inline void setUserId(const user_id_t& userId){
    user_id = userId;
  }

  /**
   * \brief Sets the player id to be used when communicating with the server.
   *
   * \param ticket The player id to be used when communicating with the server.
   */
  inline void setPlayerId(const player_id_t& newPlayerId){
    playerId = newPlayerId;
  }

  //@}


public slots:

  /** @name Slots */
  //@{

  /**
   * \brief Tells the server to set the players location to the given location.
   *
   * \param streetAddress Street address of the given location.
   * \param city City of the given location.
   * \param state State of the given location.
   * \param zipcode Zipcode of the given location.
   */
  void setPlayerLocation(
    const QString& streetAddress,
    const QString& city,
    const QString& state,
    const QString& zipcode
  );

  /**
   * \brief Tesll the server to remove the player's password.
   */
  void removePlayerPassword();

  /**
   * \brief Tells the server to change the player's password.
   *
   * \param newPassword New password for the player.
   */
  void setPlayerPassword(const QString& newPassword);

  /**
   * \brief Sets the player state.
   *
   * \param newState State to which the player should be set.
   */
  void setPlayerState(const QString& newState);

  /**
   * \brief Modifies the conents of the library on the server.
   *
   * @param songsToAdd A list of song that should be added to the server.
   * @param songsToDelete A list of song ids that should be removed from the server.
   */
  void modLibContents(const QVariantList& songsToAdd, const QVariantList& songsToDelete);

  /**
   * \brief Creates a player on the server.
   *
   * @param playerName The name of the player.
   * @param password The password of the player (may be empty thus indicating now password).
   */
  void createPlayer(
    const QString& playerName,
    const QString& password);

  /**
   * \brief Creates a player on the server.
   *
   * @param playerName The name of the player.
   * @param password The password of the player (may be empty thus indicating now password).
   */
  void createPlayer(
    const QString& playerName,
    const QString& password,
    const QString& streetAddress,
    const QString& city,
    const QString& state,
    const QString& zipcode);

  /**
   * \brief Creates a player on the server using the given JSON payload.
   *
   * @param payload JSON representing the player to be created.
   */
  void createPlayer(const QByteArray& payload);

  /**
   * \brief Retrieves the latest version of the active playlist from the server.
   */
  void getActivePlaylist();

  /**
   * \brief Modifies the active playlist on the server.
   *
   * \param toAdd A list of library songs that should be added to the playlist.
   * \param toRemove A list of library songs that should be removed from the playlist.
   */
  void modActivePlaylist(
    const QSet<library_song_id_t>& toAdd,
    const QSet<library_song_id_t>& toRemove);

  /**
   * \brief Set's the current song that the player is playing on the server.
   *
   * @param currentSong Id The current song that the client is playing.
   */
  void setCurrentSong(library_song_id_t currentSong);

  /**
   * \brief Sets the volume of the player on the server.
   *
   * @param newVolume The volume to which the player should be set.
   */
  void setVolume(int newVolume);

  /**
   * \brief Tells the server that current song has finished playing.
   */
  void clearCurrentSong();

  /**
   * \brief Retrieves a list of active participants from the server.
   */
  void getParticipantList();

  void getSortingAlgorithms();

  //@}

signals:

  /** @name Signals */
  //@{

  /**
   * \brief Emitted when a connection with the server has been established.
   *
   * \param ticketHash The ticket hash that was given by the server as a result of the authentication.
   * \param userId The userId assignd to ther user that was used to authenticate with the server.
   */
  void authenticated(const QByteArray& ticketHash, const user_id_t& userId);

  /**
   * \brief Emitted when there was a failure to establish a connection with the
   * server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void authFailed(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  void gotSortingAlgorithms(const QVariantList& sortingAlgorithms);

  void getSortingAlgorithmsError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when the player's password is succesfully removed.
   */
  void playerPasswordRemoved();

  /**
   * \brief Emitted when there was an error removing the player's password.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerPasswordRemoveError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);




  /**
   * \brief Emitted when the player's password is succesfully set.
   *
   * \param password The password that was set for the player.
   */
  void playerPasswordSet(const QString& password);

  /**
   * \brief Emitted when there was an error setting the player's password.
   *
   * @param attemptedPassword The password that was attempted to be set on the player.
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerPasswordSetError(
    const QString& attemptedPassword,
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when the player's location is succesfully set.
   *
   * \param streetAddress Street address of the set location.
   * \param city City of the set location.
   * \param state State of the set location.
   * \param zipcode Zipcode of the set location.
   */
  void playerLocationSet(
    const QString& streetAddress,
    const QString& city,
    const QString& state,
    const QString&  zipcode);

  /**
   * \brief Emitted when there was an error setting the player's location.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerLocationSetError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when chaning the players name is succesful on the server.
   *
   * @param newName The new name of the player.
   */
  void playerNameChanged(const QString& newName);

  /**
   * \brief Emitted when there was an error changing the players name.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerNameChangeError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when the current song was succesfully cleared on the server.
   */
  void currentSongCleared();

  /**
   * \brief Emitted when there is an error clearing the current song on the server.
   */
  void currentSongClearError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);



  /**
   * \brief Emitted when the state on the player was changed.
   *
   * @param newState The new state.
   */
  void playerStateSet(const QString& newState);

  /**
   * \brief Emitted when there was an error changing the players state.
   *
   * @param desiredState The state which failed to be set on the player.
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerStateSetError(
    const QString& desiredState,
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);


  /**
   * \brief Emitted when a set of songs was succesfully synced on the server.
   *
   * \param syncedIds The set of ids that were succesfully synced to the server.
   */
  void libSongsSyncedToServer(const QSet<library_song_id_t>& syncedIds);

  /**
   * \brief Emitted when there was an error syncing certains library songs with the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void libModError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when an player is succesfully created.
   */
  void playerCreated(const player_id_t& issuedId);

  /**
   * \brief Emitted when there was an error creating a player on the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void playerCreationFailed(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when a new version of the active playlist is retrieved from
   * the server.
   *
   * @param newPlaylist The new playlist that was retreived from the server.
   */
  void newActivePlaylist(const QVariantMap& newPlaylist);

  /**
   * \brief Emitted when there was an error getting the active playlist from the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void getActivePlaylistFail(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when a new version of the participants list is retrieved from the server.
   *
   * @param newParticipants The new list of participants that was retrieved from the server.
   */
  void newParticipantList(const QVariantList& newParticipants);

  /**
   * \brief Emitted when there was an error getting the list of participants from the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void getParticipantsError(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when the current song that the player is playing is
   * succesfully set on the server.
   */
  void currentSongSet();

  /**
   * \brief Emitted when there in a error setting the current song to be played on the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void setCurrentSongFailed(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  /**
   * \brief Emitted when the active playlist was successfully modified on the server.
   *
   * @param added The set of songs that were succesfully added to the playlist on the server.
   * @param removed The set of songs that were succesfully removed from the playlist on the server.
   */
  void activePlaylistModified(
    const QSet<library_song_id_t>& added,
    const QSet<library_song_id_t>& removed);

  /**
   * \brief Emitted when there in a error modifying the playlist on the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void activePlaylistModFailed(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  void volumeSetOnServer();

  /**
   * \brief Emitted when there in an error setting the song on the server.
   *
   * @param errMessage A message describing the error.
   * @param errorCode The http status code that describes the error.
   * @param headers The headers from the http response that indicated a failure.
   */
  void setVolumeFailed(
    const QString& errMessage,
    int errorCode,
    const QList<QNetworkReply::RawHeaderPair>& headers);

  //@}


private slots:

  /** @name Private Slots */
  //@{

  /**
   * \brief Handles a reply from the server.
   *
   * @param reply The reply from the server.
   */
  void recievedReply(QNetworkReply *reply);

  //@}


private:
  /** @name Private Members */
  //@{

  /** \brief Ticket hash that should be used for all requests. */
  QByteArray ticket_hash;

  /** \brief Id of the user that is currently logged in. */
  user_id_t  user_id;

  /** \brief Id of the player associated with this conneciton */
  player_id_t playerId;

  /** \brief Manager for access to the network. */
  QNetworkAccessManager *netAccessManager;


  //@}

  /** @name Private Function */
  //@{

  /**
   * \brief Handle a response from the server regarding authentication.
   *
   * @param reply Response from the server.
   */
  void handleAuthReply(QNetworkReply* reply);

  /**
   * \brief Handle a response from the server regarding getting the sorting algorithm.
   *
   * @param reply Response from the server.
   */
  void handleAlgoReply(QNetworkReply* reply);


  /**
   * \brief Handles a state set reply.
   *
   * \param the response from the server.
   */
  void handleSetStateReply(QNetworkReply* reply);

  /**
   * \brief Handles a lib modificaiton reply.
   *
   * \param the response from the server.
   */
  void handleReceivedLibMod(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding player creation.
   *
   * @param reply Response from the server.
   */
  void handleCreatePlayerReply(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding a new active playlist.
   *
   * @param reply Response from the server.
   */
  void handleReceivedActivePlaylist(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the addition of a song
   * to the active playlist.
   *
   * @param reply Response from the server.
   */
  void handleReceivedActivePlaylistAdd(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the removal of a song 
   * from the active playlist.
   *
   * @param reply Response from the server.
   */
  void handleReceivedActivePlaylistRemove(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the setting of the 
   * current song that is being played.
   *
   * @param reply Response from the server.
   */
  void handleReceivedCurrentSongSet(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding a modification of the playlist.
   *
   * @param reply Response from the server.
   */
  void handleReceivedPlaylistMod(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the setting of the player volume.
   *
   * @param reply Response from the server.
   */
  void handleReceivedVolumeSet(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the removing of the player's password.
   *
   * @param reply Response from the server.
   */
  void handlePlayerPasswordRemoveReply(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the setting of the player's password.
   *
   * @param reply Response from the server.
   */
  void handlePlayerPasswordSetReply(QNetworkReply *reply);

  /**
   * \brief Handle a response from the server regarding the setting of the player location.
   *
   * @param reply response from the server.
   */
  void handleLocationSetReply(QNetworkReply *reply);

  /**
   * \brief Handles a response from the server regarding the clearing of the current song.
   *
   * @param reply The response from the server.
   */
  void handleRecievedClearCurrentSong(QNetworkReply *reply);

  /**
   * \brief Handles a response from the server regarding getting a list of active participants.
   *
   * @param reply The response from the server.
   */
  void handleParticipantsResponse(QNetworkReply *reply);

  /**
   * \brief Prepares a network request that is going to include JSON.
   *
   * @param request Request to prepare.
   */
  void prepareJSONRequest(QNetworkRequest &request);

  /**
   * \brief Gets the url used for accessing the player's password.
   *
   * \return The url used for accessing the player's password.
   */
  QUrl getPlayerPasswordUrl() const;

  QUrl getSortingAlgosUrl() const;

  /**
   * \brief Gets the url used for accessing the player's location.
   *
   * \return The url used for accessing the player's location.
   */
  QUrl getPlayerLocationUrl() const;

  /**
   * \brief Gets the url that should be used for modifying the library.
   *
   * \return The url that should be used for modifying the library.
   */
  QUrl getLibModUrl() const;

  /**
   * \brief Get the url for interacting with the active playlist from the server.
   *
   * @return The for interacting with the active playlist from the server.
   */
  QUrl getActivePlaylistUrl() const;

  /**
   * \brief Get the url to be used for setting the current song on the server.
   *
   * @return The url to be used for setting the current song on the server.
   */
  QUrl getCurrentSongUrl() const;

  /**
   * \brief Get the url to be used for retrieving the list of current participants.
   *
   * @return The url to be used for getting the current participants.
   */
  QUrl getParticipantsUrl() const;



  /**
   * \brief Gets the url that should be used for creating a player on the server.
   *
   * \return The url that sholud be used for creating a player on the server.
   */
  QUrl getCreatePlayerUrl() const;

  /**
   * \brief Gets the url that should be used for modifying or getting 
   * the player state from the server.
   *
   * \return The url that sholud be used for modifying or getting 
   * the player state from the server.
   */
  QUrl getPlayerStateUrl() const;

  /**
   * \brief Gets the url that should be used for setting the volume on the server.
   *
   * \return The url that sholud be used for setting the volume on the server.
   */
  QUrl getVolumeUrl() const;

  /**
   * \brief Determines if the given network reply is in response to a request to 
   * obtain the active playlist.
   *
   * \param reply The network reply in question.
   * \return True if the reply is a response to getting the active playlist. False otherwise.
   */
  bool isGetActivePlaylistReply(const QNetworkReply *reply) const;

  /**
   * \brief Determines if the given network reply is in response to a request to 
   * modify the active playlist.
   *
   * \param reply The network reply in question.
   * \return True if the reply is a response to modifying the active playlist. False otherwise.
   */
  bool isModActivePlaylistReply(const QNetworkReply *reply) const;

  /**
   * \brief Determines if the reply is a response to a request that the player's password be set.
   *
   * \param reply The network reply in question.
   * \return True if the reply is a response to setting the player's password. False otherwise.
   */
  bool isPasswordSetReply(const QNetworkReply* reply) const;

  /**
   * \brief Determines if the reply is a response to a request that the player's password be removed.
   *
   * \param reply The network reply in question.
   * \return True if the reply is a response to removing the player's password. False otherwise.
   */
  bool isPasswordRemoveReply(const QNetworkReply* reply) const;

  /**
   * \brief Determines if a given reply has the same Http Status code as the one specified.
   *
   * \param reply QNetworkReply in question.
   * \param code The status code to be checked.
   * \return True if the reply if has the status code provided, false otherwise.
   */
  static bool isResponseType(QNetworkReply *reply, int code);


  //@}


  /** @name Private Constants */
  //@{

  /**
   * \brief Get the port number to be used when communicating with the server.
   *
   * This port number is a memorial to Keith Nusbaum, my father. I loved him
   * deeply and he was taken from this world far too soon. Never-the-less
   * we all continue to benefit from his good deeds. Without him, I wouldn't
   * be here, and there would be no UDJ. Please, don't change this port
   * number. Keep the memory of my father alive.
   * K = 10 % 10 = 0
   * e = 4  % 10 = 4
   * i = 8  % 10 = 8
   * t = 19 % 10 = 9
   * h = 7  % 10 = 7
   * Port 4897, the Keith Nusbaum Memorial Port
   *
   * @return The port number to be used for communicating with the server.
   */
  static const QString & getServerPortNumber(){
    static const QString serverPortNumber = "4897";
    return serverPortNumber;
  }

  /**
   * \brief Gets the url path to the server in string form.
   *
   * @return The url path to the server in string form.
   */
  static const QString& getServerUrlPath(){
    static const QString SERVER_URL_PATH= 
      "https://udjplayer.com:" + getServerPortNumber() + "/udj/0_6/";
    return SERVER_URL_PATH;
  }

  /**
   * \brief Gets the url path to the server in URL form.
   *
   * @return The url path to the server in URL form.
   */
  static const QUrl& getServerUrl(){
    static const QUrl SERVER_URL(getServerUrlPath());
    return SERVER_URL;
  }

  /**
   * \brief Gets the url for authenticating with the server.
   *
   * @return The url for authenticating with the server.
   */
  static const QUrl& getAuthUrl(){
    static const QUrl AUTH_URL(getServerUrlPath() + "auth");
    return AUTH_URL;
  }

  /**
   * \brief Get the header used for identifying the ticket hash header.
   *
   * @return The header used for identifying the ticket hash header.
   */
  static const QByteArray& getTicketHeaderName(){
    static const QByteArray ticketHeaderName = "X-Udj-Ticket-Hash";
    return ticketHeaderName;
  }

  /**
   * \brief Get the header used for identifying the Missing Resource header.
   *
   * @return The header used for identifying the Missing Resource header.
   */
  static const QByteArray& getMissingResourceHeader(){
    static const QByteArray missingResourceHeader = "X-Udj-Missing-Resource";
    return missingResourceHeader;
  }

  /**
   * \brief Gets the property name for a player name property.
   *
   * \return The property name for a player name property.
   */
  static const char* getPlayerNamePropertyName(){
    static const char* playerNamePropertyName = "player_name";
    return playerNamePropertyName;
  }

  /**
   * \brief Gets the property name for a player password property.
   *
   * \return The property name for a player password property.
   */
  static const char* getPlayerPasswordPropertyName(){
    static const char* playerPasswordPropertyName = "playerPassword";
    return playerPasswordPropertyName;
  }

  /**
   * \brief Gets the property name for a location address property.
   *
   * \return The property name for a location address property.
   */
  static const char* getLocationAddressPropertyName(){
    static const char* locationAddressPropertyName = "address";
    return locationAddressPropertyName;
  }

  /**
   * \brief Gets the property name for a location city property.
   *
   * \return The property name for a location city property.
   */
  static const char* getLocationCityPropertyName(){
    static const char* locationCityPropertyName = "city";
    return locationCityPropertyName;
  }

  /**
   * \brief Gets the property name for a location state property.
   *
   * \return The property name for a location state property.
   */
  static const char* getLocationStatePropertyName(){
    static const char* locationStatePropertyName = "state";
    return locationStatePropertyName;
  }

  /**
   * \brief Gets the property name for a location zipcode property.
   *
   * \return The property name for a location zipcode property.
   */
  static const char* getLocationZipcodePropertyName(){
    static const char* locationZipcodePropertyName = "zipcode";
    return locationZipcodePropertyName;
  }

  /**
   * \brief Gets the property name for a state property.
   *
   * \return The property name for a state property.
   */
  static const char* getStatePropertyName(){
    static const char* statePropertyName = "state";
    return statePropertyName;
  }

  /**
   * \brief Gets the property name for a songs_added property.
   *
   * \return The property name for a songs_added property.
   */
  static const char* getSongsAddedPropertyName(){
    static const char* songsAddedPropertyName = "songs_added";
    return songsAddedPropertyName;
  }

  /**
   * \brief Gets the property name for a songs_deleted property.
   *
   * \return The property name for a songs_deleted property.
   */
  static const char* getSongsDeletedPropertyName(){
    static const char* songsDeletedPropertyName = "songs_deleted";
    return songsDeletedPropertyName;
  }

  /**
   * \brief Gets the property name for a songs_removed property.
   *
   * \return The property name for a songs_removed property.
   */
  static const char* getSongsRemovedPropertyName(){
    static const char* songsRemovedPropertyName = "songs_removed";
    return songsRemovedPropertyName;
  }

  //@}

};


} //end namespace
#endif //UDJ_SERVER_CONNECTION_HPP

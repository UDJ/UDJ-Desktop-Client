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
#ifndef ACTIVE_PLAYLIST_MODEL_HPP
#define ACTIVE_PLAYLIST_MODEL_HPP
#include "MusicModel.hpp"

namespace UDJ{

class DataStore;

/**
 * \brief A class serving as a model for the Active Playlist.
 */
class ActivePlaylistModel : public MusicModel{
Q_OBJECT
public:

  /** @name Constructor(s) and Destructor */
  //@{

  /**
   * \brief Constructs an ActivePlaylistModel
   *
   * @param query The query used to back the model.
   * @param dataStore The DataStore backing this instance of UDJ.
   * @param parent The parent QObject.
   */
  ActivePlaylistModel(const QString& query, DataStore *dataStore, QObject *parent);

  //@}


  /** @name Overridden from MusicModel */
  //@{

  /**
   * \brief .
   */
  virtual QVariant data(const QModelIndex& item, int role) const;

  //@}


};


}
#endif //ACTIVEPLAYLIST_MODEL_HPP

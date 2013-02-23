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
#ifndef LIBRARY_WIDGET_HPP
#define LIBRARY_WIDGET_HPP
#include "ConfigDefs.hpp"
#include <QWidget>

class QLineEdit;

namespace UDJ{

class DataStore;
class LibraryView;



/**
 * \brief Displays the Library and other widgets associated with the library.
 */
class LibraryWidget : public QWidget{
Q_OBJECT
public:
  /** @name Constructors */
  //@{

  /**
   * \brief Constructs a LibraryWidget
   *
   * @param dataStore The DataStore backing this instance of UDJ.
   * @param parent The parent widget.
   */
  LibraryWidget(DataStore *dataStore, QWidget *parent=0);

  //@}

/** @name Signals */
//@{

signals:

  /** \brief Emitted when the LibraryWidget believes the song library needs to be synced. */
  void libNeedsSync();

//@}

private:
  /** @name Private Members */
  //@{

  /** \brief The datastore backing the client. */
  DataStore *dataStore;

  /** \brief The view used to display the library. */
  LibraryView *libraryView;

  /** \brief A line edit used to search the library. */
  QLineEdit *searchEdit;

  //@}

protected:
  bool eventFilter(QObject *obj, QEvent *event);
};


}//end namespace UDJ
#endif //LIBRARY_WIDGET_HPP


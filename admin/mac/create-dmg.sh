#!/bin/sh

# === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
#
# Copyright 2011, John Maguire <john.maguire@gmail.com>
#
# Tomahawk is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tomahawk is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.

# author: max@last.fm, muesli@tomahawk-player.org
# brief:  Produces a compressed DMG from a bundle directory
# usage:  Pass the bundle directory as the only parameter
# note:   This script depends on the UDJ build system, and must be run from
#         the build directory
#Shamelessly stolen from the tomahawk project. Super thanks to them.
################################################################################



#if [ -z $VERSION ]
#then
#    echo VERSION must be set
#    exit 2
#fi

if [ -z "$1" ]
then
    echo "Please pass the bundle.app directory as the first parameter."
    exit 3
fi
################################################################################


NAME=$(basename "$1" | perl -pe 's/(.*).app/\1/')
IN="$1"
TMP="dmg/$NAME"
OUT="$NAME.dmg"
mkdir -p "$TMP"
################################################################################


# clean up
rm -rf "$TMP"
rm -f "$OUT"

# create DMG contents and copy files
mkdir  "$TMP"
#mkdir -p "$TMP/.background"
#cp ../admin/mac/dmg_background.png "$TMP/.background/background.png"
#cp ../admin/mac/DS_Store.in "$TMP/.DS_Store"
#chmod go-rwx "$TMP/.DS_Store"
ln -s /Applications "$TMP/Applications"
# copies the prepared bundle into the dir that will become the DMG 
cp -R "$IN" "$TMP"

# create
hdiutil makehybrid -hfs -hfs-volume-name UDJ -hfs-openfolder "$TMP" "$TMP" -o tmp.dmg
hdiutil convert -format UDZO -imagekey zlib-level=9 tmp.dmg -o "$OUT"

# cleanup
rm tmp.dmg

#hdiutil create -srcfolder "$TMP" \
#               -format UDZO -imagekey zlib-level=9 \
#               -scrub \
#               "$OUT" \
#               || die "Error creating DMG :("

# done !
echo 'DMG size:' `du -hs "$OUT" | awk '{print $1}'`

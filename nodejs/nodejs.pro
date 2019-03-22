# ******************************************************************************
# nodejs.pro                                                       Tao3D project
# ******************************************************************************
#
# File description:
#
#       Qt project file to download and install NodeJS
#
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2014,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2013, Jérôme Forissier <jerome@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
# ******************************************************************************

include(../main_defs.pri)

TEMPLATE = subdirs

macx|linux* {
  macx {
    URL = http://nodejs.org/dist/v0.10.18/node-v0.10.18-darwin-x64.tar.gz
    MD5 = 0aa4270c3c6b2907041ac0d4e1d06731
    MD5CMD = md5
  }

  linux* {
    # Note: we should really test QMAKE_TARGET here, not QMAKE_HOST.
    # Unfortunately Qt 4.8.1 does not define this variable (Ubuntu 12.04),
    # but since we never cross-compile, HOST == TARGET.
    contains(QMAKE_HOST.arch, i686) {
      URL = http://nodejs.org/dist/v0.10.18/node-v0.10.18-linux-x86.tar.gz
      MD5 = c4cfd75c9692b4c2716f3dcdc68f1c78
    }
    contains(QMAKE_HOST.arch, x86_64) {
      URL = http://nodejs.org/dist/v0.10.18/node-v0.10.18-linux-x64.tar.gz
      MD5 = 7e01855f266474bb4063209e391d4c61
    }
    MD5CMD = md5sum
  }

  ARCHIVE = $$basename(URL)
  TMP = $${ARCHIVE}_
  fetch.target = $$ARCHIVE
  fetch.commands = curl -o $$TMP $$URL && $$MD5CMD $$TMP | grep $$MD5 >/dev/null && mv $$TMP $$ARCHIVE
  QMAKE_EXTRA_TARGETS += fetch

  DIR = $$replace(ARCHIVE, .tar.gz, )
  install.path = $$APPINST/nodejs
  install.depends = $$ARCHIVE
  install.commands = tar zxf $$ARCHIVE --strip-components 1 -C \"$$APPINST/nodejs\"
  INSTALLS += install

  QMAKE_CLEAN += $$TMP
}

win32 {
  URL = http://nodejs.org/dist/v0.10.18/node.exe
  MD5 = c7e6c1de34c31c08e6e1d0e4f237db54

  fetch.target = node.exe
  fetch.commands = curl -o node.exe_ $$URL && md5sum node.exe_ | grep -q $$MD5 && mv node.exe_ node.exe
  QMAKE_EXTRA_TARGETS += fetch

  QMAKE_STRIP = :  # Prevent make install from stripping node.exe
  install.path = $$APPINST/nodejs/bin
  install.commands = cp node.exe $$install.path
  install.depends = node.exe
  INSTALLS += install

  QMAKE_CLEAN += node.exe_
}

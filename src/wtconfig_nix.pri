# Copyright (c) 2016 Juan Gonzalez Burgos
# 
# This file is part of WtDesigner.
# 
# WtDesigner is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# WtDesigner is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with WtDesigner.  If not, see <http://www.gnu.org/licenses/>.

DEFINES      += QT_NO_KEYWORDS WT_NO_SLOT_MACROS

INCLUDEPATH  += /usr/local/include/

LIBS         += -L/usr/local/lib/

				
CONFIG(debug, debug|release) {

        LIBS += -lboost_system \
                -lboost_thread \
                -lwttest       \
                -lwt           \
                -lwtdbo        \
                -lwtdbopostgres\
                -lwtdbosqlite3 \
                -lwthttp
} else {

OBJECTS_DIR  += release
        LIBS += -lboost_system \
                -lboost_thread \
                -lwttest       \
                -lwt           \
                -lwtdbo        \
                -lwtdbopostgres\
                -lwtdbosqlite3 \
                -lwthttp
}

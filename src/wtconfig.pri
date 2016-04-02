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

DEFINES      += WIN32 HPDF_DLL QT_NO_KEYWORDS WT_NO_SLOT_MACROS

INCLUDEPATH  += .                                                 \
                 C:/Wt-3.3.5-msvs2013-Windows-x86-SDK/include     \
				 C:/Wt-3.3.5-msvs2013-Windows-x64-SDK/include

DEPENDPATH   += .

LIBS         += -LC:/Wt-3.3.5-msvs2013-Windows-x86-SDK/lib        \
                -LC:/Wt-3.3.5-msvs2013-Windows-x86-SDK/bin        \
				-LC:/Wt-3.3.5-msvs2013-Windows-x64-SDK/lib        \
                -LC:/Wt-3.3.5-msvs2013-Windows-x64-SDK/bin        

				
CONFIG(debug, debug|release) {

        LIBS += -llibboost_atomic-vc120-mt-gd-1_59               \
                -llibboost_chrono-vc120-mt-gd-1_59               \
                -llibboost_container-vc120-mt-gd-1_59            \
                -llibboost_context-vc120-mt-gd-1_59              \
                -llibboost_coroutine-vc120-mt-gd-1_59            \
                -llibboost_date_time-vc120-mt-gd-1_59            \
                -llibboost_exception-vc120-mt-gd-1_59            \
                -llibboost_filesystem-vc120-mt-gd-1_59           \
                -llibboost_graph-vc120-mt-gd-1_59                \
                -llibboost_iostreams-vc120-mt-gd-1_59            \
                -llibboost_locale-vc120-mt-gd-1_59               \
                -llibboost_log-vc120-mt-gd-1_59                  \
                -llibboost_log_setup-vc120-mt-gd-1_59            \
                -llibboost_math_c99-vc120-mt-gd-1_59             \
                -llibboost_math_c99f-vc120-mt-gd-1_59            \
                -llibboost_math_c99l-vc120-mt-gd-1_59            \
                -llibboost_math_tr1-vc120-mt-gd-1_59             \
                -llibboost_math_tr1f-vc120-mt-gd-1_59            \
                -llibboost_math_tr1l-vc120-mt-gd-1_59            \
                -llibboost_prg_exec_monitor-vc120-mt-gd-1_59     \
                -llibboost_program_options-vc120-mt-gd-1_59      \
                -llibboost_random-vc120-mt-gd-1_59               \
                -llibboost_regex-vc120-mt-gd-1_59                \
                -llibboost_serialization-vc120-mt-gd-1_59        \
                -llibboost_signals-vc120-mt-gd-1_59              \
                -llibboost_system-vc120-mt-gd-1_59               \
                -llibboost_test_exec_monitor-vc120-mt-gd-1_59    \
                -llibboost_thread-vc120-mt-gd-1_59               \
                -llibboost_timer-vc120-mt-gd-1_59                \
                -llibboost_unit_test_framework-vc120-mt-gd-1_59  \
                -llibboost_wave-vc120-mt-gd-1_59                 \
                -llibboost_wserialization-vc120-mt-gd-1_59       \
                -llibhpdfd                                       \
                -lwttestd                                        \
                -lwtd                                            \
                -lwtdbod                                         \
                -lwtdbofirebirdd                                 \
                -lwtdbomysqld                                    \
                -lwtdbopostgresd                                 \
                -lwtdbosqlite3d                                  \
                -lwthttpd										 \
				-lwtisapid									
} else {

OBJECTS_DIR  += release
        LIBS += -llibboost_chrono-vc120-mt-1_59                  \
                -llibboost_container-vc120-mt-1_59               \
                -llibboost_context-vc120-mt-1_59                 \
                -llibboost_coroutine-vc120-mt-1_59               \
                -llibboost_date_time-vc120-mt-1_59               \
                -llibboost_exception-vc120-mt-1_59               \
                -llibboost_filesystem-vc120-mt-1_59              \
                -llibboost_graph-vc120-mt-1_59                   \
                -llibboost_iostreams-vc120-mt-1_59               \
                -llibboost_locale-vc120-mt-1_59                  \
                -llibboost_log-vc120-mt-1_59                     \
                -llibboost_log_setup-vc120-mt-1_59               \
                -llibboost_math_c99-vc120-mt-1_59                \
                -llibboost_math_c99f-vc120-mt-1_59               \
                -llibboost_math_c99l-vc120-mt-1_59               \
                -llibboost_atomic-vc120-mt-1_59                  \
                -llibboost_math_tr1f-vc120-mt-1_59               \
                -llibboost_math_tr1l-vc120-mt-1_59               \
                -llibboost_prg_exec_monitor-vc120-mt-1_59        \
                -llibboost_program_options-vc120-mt-1_59         \
                -llibboost_random-vc120-mt-1_59                  \
                -llibboost_regex-vc120-mt-1_59                   \
                -llibboost_serialization-vc120-mt-1_59           \
                -llibboost_signals-vc120-mt-1_59                 \
                -llibboost_system-vc120-mt-1_59                  \
                -llibboost_test_exec_monitor-vc120-mt-1_59       \
                -llibboost_thread-vc120-mt-1_59                  \
                -llibboost_timer-vc120-mt-1_59                   \
                -llibboost_unit_test_framework-vc120-mt-1_59     \
                -llibboost_wave-vc120-mt-1_59                    \
                -llibboost_wserialization-vc120-mt-1_59          \
				-llibboost_math_tr1-vc120-mt-1_59                \
                -llibhpdf                                        \
                -lwttest                                         \
                -lwt                                             \
                -lwtdbo                                          \
                -lwtdbofirebird                                  \
                -lwtdbomysql                                     \
                -lwtdbopostgres                                  \
                -lwtdbosqlite3                                   \
                -lwthttp                                         \
                -lwtisapi                                        \
}
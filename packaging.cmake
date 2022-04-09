###########################################################################
#
# Copyright (c) 2022, Arthur N. Klassen
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
###########################################################################
#
#    May you do good and not evil.
#    May you find forgiveness for yourself and forgive others.
#    May you share freely, never taking more than you give.
#
###########################################################################

install( TARGETS ansak ansakSqlite LIBRARY DESTINATION lib/ )
install( FILES interface/config.hxx
               interface/config_io.hxx
               interface/config_types.hxx
               interface/file_handle.hxx
               interface/file_handle_exception.hxx
               interface/file_of_lines.hxx
               interface/file_of_lines_exception.hxx
               interface/file_path.hxx
               interface/file_system_path.hxx
               interface/nullptr_exception.hxx
               interface/parse_args.hxx
               interface/runtime_exception.hxx
               interface/sqlite_db.hxx
               interface/sqlite_db_pragmas.hxx
               interface/sqlite_exception.hxx
               interface/sqlite_statement.hxx
               interface/temp_directory.hxx
               interface/time_stamp.hxx DESTINATION include/ansak )

if( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    find_program( LSB_RELEASE_EXEC lsb_release )
    execute_process( COMMAND /usr/bin/sh /usr/bin/lsb_release -is
                     OUTPUT_VARIABLE DISTRO
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
    if( (${DISTRO} STREQUAL "CentOS") OR
        (${DISTRO} STREQUAL "Fedora" ) OR
        (${DISTRO} STREQUAL "RedHat")
      )
        set( CPACK_GENERATOR TGZ RPM )

        set( CPACK_RPM_COMPONENT_INSTALL No )
        set( CPACK_RPM_PACKAGE_NAME "ansakLib" )
        set( CPACK_RPM_FILE_NAME "ansakLib.rpm" )
    else()
        set( CPACK_GENERATOR TGZ DEB )

        set( CPACK_DEB_COMPONENT_INSTALL No )
        set( CPACK_DEBIAN_PACKAGE_NAME "ansakLib" )
        set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Arthur N. Klassen <anklassen@onepost.net>" )
        set( CPACK_DEBIAN_PACKAGE_DESCRIPTION "A simple C++ application support library (config files, of linse, sqlite etc.)" )
        set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
        set( CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/ANSAKsoftware/ansak-lib" )
        set( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++" )
    endif()

elseif( WIN32 )
    set( CPACK_GENERATOR TGZ NSIS WIX )
elseif( APPLE )
    set( CPACK_GENERATOR TGZ )
endif()

set( CPACK_PACKAGE_NAME "ansakLib" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple C++ application support library (config files, of linse, sqlite etc.)" )
set( CPACK_PACKAGE_VENDOR "ANSAK Software Uninc." )
set( CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH} )
set( CPACK_VERBATIM_VARIABLES True )
set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/NSIS/ansak-library-bsd-2clause.txt" )
set( CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/NSIS/ansak.ico" )
set( CPACK_DEBIAN_PACKAGE_NAME "ansakLib" )
set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Arthur N. Klassen <anklassen@onepost.net>" )
set( CPACK_DEBIAN_PACKAGE_DESCRIPTION "A simple C++ application support library (config files, of linse, sqlite etc.)" )
set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
set( CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/ANSAKsoftware/ansak-lib" )
set( CPACK_CYGWIN_PATCH_NUMBER ${CMAKE_PROJECT_VERSION_PATCH} )

include(CPack)


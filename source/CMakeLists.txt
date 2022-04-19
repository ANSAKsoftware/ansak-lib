###########################################################################
#
# Copyright (c) 2021, Arthur N. Klassen
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

if( UNIX )
    add_compile_options( -Wall -Wextra -Werror -Weffc++ )
endif()

set( _allSources primitives/file_path.cxx
                 primitives/nullptr_exception.cxx
                 primitives/parse_args.cxx
                 primitives/runtime_exception.cxx
                 primitives/time_stamp.cxx
                 file/file_system_path.cxx
                 file/file_handle_exception.cxx
                 file/file_handle.cxx
                 file-tools/file_of_lines.cxx
                 file-tools/file_of_lines_core.cxx
                 file-tools/file_of_lines_exception.cxx
                 file-tools/temp_directory.cxx
                 file-tools/text_file_utilities.cxx
                 config/config_atom.cxx
                 config/config_io_file.cxx
                 config/config.cxx
                 )
if( WIN32 )
    list(APPEND _allSources os/win32/os_layer_windows.cxx
                            os/win32/windows_directory_scanner.cxx
                            os/win32/windows_registry_key.cxx )
else()
    list(APPEND _allSources os/linux/os_layer_linux.cxx
                            os/linux/linux_directory_scanner.cxx )
endif()

add_subdirectory( primitives )
add_subdirectory( os )
add_subdirectory( file )
add_subdirectory( file-tools )
add_subdirectory( config )

set( _target ansak )

if( APPLE OR BUILD_PACKAGE_LIB )
    # a non-scaling hack requried because the else() clause (the correct
    # way) causes xcodebuild to rm libansak.a after many schemes complete.
    add_library( ${_target} STATIC ${_allSources} )
else()
    add_library( ${_target} STATIC $<TARGET_OBJECTS:primitives>
                                   $<TARGET_OBJECTS:osLayer>
                                   $<TARGET_OBJECTS:file>
                                   $<TARGET_OBJECTS:fileTools>
                                   $<TARGET_OBJECTS:config> )
endif()

if( SQLite3_FOUND )
    add_subdirectory( sqlite )
endif()

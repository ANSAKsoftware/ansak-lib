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

MACRO( SETUP_TO_EXECUTE_GTEST TestTarget )
    if( WIN32 )
      add_custom_command( TARGET ${TestTarget} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gtest>      ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gtest_main> ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gmock>      ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gmock_main> ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR} )
    endif()
ENDMACRO()

if( WIN32 )
  set_directory_properties( PROPERTIES COMPILE_DEFINITIONS GTEST_LINKED_AS_SHARED_LIBRARY
                                       COMPILE_OPTIONS "/wd\"4275\"" )
endif()

set( _target test-init )
set( _targetTest testInit )

set( _privLibs ${ANSAK_STRING_LIB} gtest_main )

set( _targetSrc init_test.cxx )
add_executable( ${_target} ${_targetSrc} )
target_link_libraries( ${_target} PRIVATE ${_privLibs} )
SETUP_TO_EXECUTE_GTEST( ${_target} )

add_test( NAME ${_targetTest} COMMAND ${_target} )

add_subdirectory( unit )
add_subdirectory( integration )

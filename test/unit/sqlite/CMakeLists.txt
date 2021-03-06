###########################################################################
#
# Copyright (c) 2017, Arthur N. Klassen
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

set( _target test-sqlite )
set( _targetTest sqliteTest )

set( _privLibraries ansakSqlite fileTools mock-file osLayer primitives "${ANSAK_STRING_LIB}" )
set( _mockLibraries mock-sqlite mock-os gmock_main )

set( _targetSrc test_sqlite_exception.cxx
                test_sqlite_db.cxx
                test_sqlite_statement.cxx
                test_sqlite_retrieval_exception.cxx
                test_sqlite_pragmas.cxx
                )

add_executable( ${_target} ${_targetSrc} )
target_include_directories( ${_target} PRIVATE ${SQLITE3_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/mock" )
target_link_libraries( ${_target} PRIVATE ${_privLibraries} ${_mockLibraries} )
SETUP_TO_EXECUTE_GTEST( ${_target} )

add_test( NAME ${_targetTest} COMMAND ${_target} )

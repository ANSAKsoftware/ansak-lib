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

# This project requires CMake 3.1 or later with a C++11-compatible compiler.

# Be sure to update the gtest sub-module so the unit tests will work!

# If you have gcov and lcov installed on a Linux system, code coverage is provided via
#   cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=1 for this target to work properly

# On MS-Windows, this project requires VS2012 or later to use it, requires VS2015
# or later to run the Unit Tests (If you use CLANG your mileage will CERTAINLY vary.)
# Unit tests will be suppressed for VS2012 (some C++11 features used for those unit tests
# are incompatible with the VS2012 compiler).

cmake_minimum_required( VERSION 3.1 )
project( ansakLib )

set( _ansakRoot 0 )
if( "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}" )
    set( _ansakRoot 1 )
endif()
if( _ansakRoot )
    list( INSERT CMAKE_MODULE_PATH
          0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )
endif()

find_package( SQLite3 )
find_package( ANSAK REQUIRED )

if ( ANSAK_INCLUDE )
    message( "ANSAK_INCLUDE has value, ${ANSAK_INCLUDE}; adding to include path" )
    include_directories( AFTER SYSTEM "${ANSAK_INCLUDE}" )
endif()
include_directories( AFTER SYSTEM "${PROJECT_SOURCE_DIR}/interface"
                                  "${PROJECT_SOURCE_DIR}/source/include" )

##############################################################################################################
set( _useUnitTests 0 )
if( TEST_ANSAKSTRING OR _ansakRoot )
  set( _useUnitTests 1 )
endif() 

set( _ansakBuildType "release" )
if( CMAKE_BUILD_TYPE )
    string( TOLOWER "${CMAKE_BUILD_TYPE}" _ansakBuildType )
endif()

if( CMAKE_GENERATOR MATCHES "Visual Studio" )
    if( COVERAGE )
        message( FATAL_ERROR "Coverage is not available in Visual Studio Builds." )
    endif()
    if( _useUnitTests AND ${CMAKE_GENERATOR} STRLESS "Visual Studio 14 2015" )
        message( "Unit tests require Visual Studio 2015 or later. Suppressing..." )
        set( _useUnitTests 0 )
    endif()
endif()

#*************************************************************************************************************
if( COVERAGE AND NOT _useUnitTests )
    message( "Coverage builds require unit tests to be active." )
    set( _useUnitTests 1 )
endif()
##############################################################################################################

##############################################################################################################
if( _ansakRoot )
    if ( ${_ansakBuildType} STREQUAL asan )
        include( FindASan )
    endif()
    if( _useUnitTests )
        include( CTest )
        if( COVERAGE AND NOT UNIX )
            message( "Trying to build code coverage on a non-Linux system isn't going to end well." )
        endif()
        add_subdirectory( submods )
    endif()
endif()
##############################################################################################################

if( UNIX )
    set( _isGnu 1 )
    if( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU )
        set( _isGnu 0 )
    endif()
    set( CMAKE_CXX_STANDARD 11 )
    set( CMAKE_CXX_STANDARD_REQUIRED ON )
    add_compile_options( -Wall -Wextra )
    if( COVERAGE )
        add_compile_options( -g -O0 -fprofile-arcs -ftest-coverage )
        include( CodeCoverage )
        SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov" )
        APPEND_COVERAGE_COMPILER_FLAGS()
        SETUP_TARGET_FOR_COVERAGE_LCOV( NAME coverage
                                        EXECUTABLE ctest -C ${ROOT_DIR}/CTestTestFile.cmake )
    endif()
    if( _isGnu )
        add_compile_options( -Wno-missing-field-initializers )
    endif()
endif()

add_subdirectory( source )
add_subdirectory( mock )
add_subdirectory( test )

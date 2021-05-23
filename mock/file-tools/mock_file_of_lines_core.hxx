///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2021, Arthur N. Klassen
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////
//
// 2021.05.29 - First Version
//
//    May you do good and not evil.
//    May you find forgiveness for yourself and forgive others.
//    May you share freely, never taking more than you give.
//
///////////////////////////////////////////////////////////////////////////
//
// mock_file_of_lines_core.hxx -- declaration of a bare bones FileOfLinesCore for mocking
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <file_of_lines.hxx>
#include "mock_file_of_lines.hxx"
#include <gmock/gmock.h>
#include <vector>
#include <string>

namespace ansak
{

class FileOfLinesMock;

namespace internal
{

class FileOfLinesCore
{
public:

    //=======================================================================
    // Constructor

    FileOfLinesCore
    (
        const ansak::FileSystemPath& path       // I - file path to read for lines
    );

    //=======================================================================
    // Destructor -- lock_guard(m_oneUser)

    ~FileOfLinesCore();

    FileOfLinesCore(const FileOfLinesCore& src) = delete;
    FileOfLinesCore& operator=(const FileOfLinesCore& src) = delete;

    //=======================================================================
    // open the file, set it up for reading -- lock_guard(m_oneUser)
    //
    // Throws on errors, classifies the file (UTF-8, UTF-16, UCS-4) and sets
    // up to read its contents

    void open();

    //=======================================================================
    // get a string from the file and return it -- may not lock!
    //
    // Locks occur in findNextLineStart and getLine
    //
    // Throws on errors
    //
    // Returns the contents of the nth line. If string is empty and endOfFile
    // is set true, the file has been completely read. endOfFile is set false
    // otherwise.

    std::string get
    (
        unsigned int    nLine,              // I - which line to return
        bool*           endOfFile = nullptr // O - has the end-of-file been found?
    );

private:

    FileOfLinesMock* m_currentMock;
};

}

}

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
// mock_file_of_lines_exception.cxx -- implementation of FileOfLinesException for mocks
//
///////////////////////////////////////////////////////////////////////////

#include <file_of_lines_exception.hxx>

#include <string>

using namespace std;

namespace ansak {

//===========================================================================
// public, constructor

FileOfLinesException::FileOfLinesException
(
    const char*          message,   // I - a message about a FileOfLines failure
    const FilePath&,                // I - the file name being parsed, def invalidPath
    unsigned long long              // I - an offset into the file, if available, def ~0
) noexcept :
    m_what("Mocked FileOfLines(const char*, const FilePath&, unsigned long long) object threw an exception")
{
    m_what += ":  ";
    m_what += message;
}

//===========================================================================
// public, constructor

FileOfLinesException::FileOfLinesException
(
    const char*                 message,        // I - a message about a FileOfLines failure
    const FilePath&,                            // I - the file name being parsed
    const FileHandleException&,                 // I - a FileHandle problem being wrapped
    unsigned long long                          // I - an offset into the file, if available, def ~0
) noexcept :
    m_what("Mocked FileOfLines(const char*, const FilePath&, const FileHandleException&, unsigned long long) object threw an exception")
{
    m_what += ":  ";
    m_what += message;
}

//===========================================================================
// public, destructor

FileOfLinesException::~FileOfLinesException() noexcept
{
}

//===========================================================================
// public, virtual

const char* FileOfLinesException::what() const noexcept
{
    return m_what.c_str();
}

}

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
// mock_file_of_lines.cxx -- implementation of a mock to FileOfLines
//
///////////////////////////////////////////////////////////////////////////

#include "mock_file_of_lines.hxx"

#include <file_of_lines.hxx>
#include "mock_file_of_lines_core.hxx"

using namespace std;
using namespace testing;

namespace ansak
{

FileOfLinesMock* FileOfLinesMock::m_currentMock = nullptr;

FileOfLines::FileOfLines
(
    const FileSystemPath& path
) : m_core(new ansak::internal::FileOfLinesCore(path)),
    m_isOpen(false)
{
}

FileOfLines::const_iterator FileOfLines::begin() const
{
    if (FileOfLinesMock::getMock()->m_mockValues.empty()) {
        return end();
    } 
    return const_iterator(m_core);
}

bool FileOfLines::empty() const
{
    return FileOfLinesMock::getMock()->m_mockValues.empty();
}

size_t FileOfLines::size() const
{
    return FileOfLinesMock::getMock()->m_mockValues.size();
}

string FileOfLinesMock::getLine(unsigned int n, bool* eof)
{
    if (n >= m_mockValues.size())
    {
        if (eof)
        {
            *eof = true;
        }
        return string();
    }
    return m_mockValues[n];
}

bool FileOfLinesMock::shouldThrowOnNextGet()
{
    if (m_throwOnNextGet)
    {
        m_throwOnNextGet = false;
        return true;
    }
    return false;
}

}

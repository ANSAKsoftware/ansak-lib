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
// mock_file_of_lines_iterator.cxx -- implementation of a bare bones FileOfLines::const_iterator for mocking
//
///////////////////////////////////////////////////////////////////////////

#include "mock_file_of_lines.hxx"
#include <file_of_lines_exception.hxx>

using namespace std;

namespace ansak {

//===========================================================================
// private

FileOfLines::const_iterator::const_iterator(SharedCorePtr core)
  : m_iterCore(core),
    m_currentLineX(0),
    m_currentLine()
{
}

//===========================================================================
// public

FileOfLines::const_iterator::const_iterator()
  : m_iterCore(),
    m_currentLineX(0),
    m_currentLine()
{
}

const string& FileOfLines::const_iterator::operator*() const
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    return theMock->m_mockValues[m_currentLineX];
}

const string* FileOfLines::const_iterator::operator->() const
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    return &theMock->m_mockValues[m_currentLineX];
}

FileOfLines::const_iterator& FileOfLines::const_iterator::operator++()
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    ++m_currentLineX;
    return *this;
}

FileOfLines::const_iterator& FileOfLines::const_iterator::operator--()
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    --m_currentLineX;
    return *this;
}

FileOfLines::const_iterator FileOfLines::const_iterator::operator++(int)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    auto other = *this;
    ++m_currentLineX;
    return other;
}

FileOfLines::const_iterator FileOfLines::const_iterator::operator--(int)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    auto other = *this;
    --m_currentLineX;
    return other;
}

FileOfLines::const_iterator FileOfLines::const_iterator::operator+(int increment)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    auto other = *this;
    other.m_currentLineX += increment;
    return other;
}

FileOfLines::const_iterator& FileOfLines::const_iterator::operator+=(int increment)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    m_currentLineX += increment;
    return *this;
}

FileOfLines::const_iterator FileOfLines::const_iterator::operator-(int increment)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    auto other = *this;
    other.m_currentLineX -= increment;
    return other;
}

FileOfLines::const_iterator& FileOfLines::const_iterator::operator-=(int increment)
{
    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    m_currentLineX -= increment;
    return *this;
}

size_t FileOfLines::const_iterator::operator-(const_iterator src)
{

    FileOfLinesMock* theMock = FileOfLinesMock::getMock();
    if (theMock->shouldThrowOnNextGet())
    {
        throw FileOfLinesException("Throwing get as requested", FilePath("test"));
    }
    return m_currentLineX - src.m_currentLineX;
}

bool FileOfLines::const_iterator::operator==(const const_iterator& rhs) const
{
    if (&rhs == this)
    {
        return true;
    }
    return m_currentLine == rhs.m_currentLine;
}

}

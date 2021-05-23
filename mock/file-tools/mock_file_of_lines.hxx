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
// mock_file_of_lines.hxx -- declaration of a mock to FileOfLines
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <file_of_lines.hxx>
#include "mock_file_of_lines_core.hxx"
#include <gmock/gmock.h>
#include <vector>
#include <string>

namespace ansak
{

class FileOfLinesMock
{
    friend class FileOfLines;
    friend class FileOfLines::const_iterator;
    friend class ansak::internal::FileOfLinesCore;

public:

    static FileOfLinesMock* getMock() { return m_currentMock; }

    FileOfLinesMock()   { m_currentMock = this; }
    ~FileOfLinesMock()  { m_currentMock = nullptr; }

    MOCK_METHOD1(mockOpen, void(ansak::internal::FileOfLinesCore*));

    void clearStringVector() { m_mockValues.clear(); }
    void useThisStringVector(const std::vector<std::string>& mockValues) { m_mockValues = mockValues; }

    void throwOnNextGet();

private:

    std::vector<std::string> m_mockValues;
    bool m_throwOnNextGet;

    std::string getLine(unsigned int n, bool* endOfFile);
    bool shouldThrowOnNextGet();

    static FileOfLinesMock* m_currentMock;
};

}

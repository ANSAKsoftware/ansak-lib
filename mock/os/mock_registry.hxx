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
// 2021.06.02 - First Version
//
//    May you do good and not evil.
//    May you find forgiveness for yourself and forgive others.
//    May you share freely, never taking more than you give.
//
///////////////////////////////////////////////////////////////////////////
//
// mock_registry.cxx -- declaration of a mock to the registry access
//
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows_registry_key.hxx>
#include <gmock/gmock.h>

namespace ansak
{

namespace config
{

class WindowsRegKeyMock
{
public:
    static WindowsRegKeyMock* getMock() { return m_currentMock; }

    WindowsRegKeyMock()     { m_currentMock = this; }
    ~WindowsRegKeyMock()    { m_currentMock = nullptr; }

    MOCK_METHOD1(mockExists, bool(const utf8String&));
    MOCK_METHOD2(mockOpen, WindowsRegKey(const utf8String&, bool));
    MOCK_METHOD1(mockGetValueNames, WindowsRegKey::ValueNamesType(const WindowsRegKey*));
    MOCK_METHOD2(mockGetValueType, DWORD(const WindowsRegKey*, const utf8String&));
    MOCK_METHOD3(mockGetValue, bool(const WindowsRegKey*, const utf8String&, DWORD&));
    MOCK_METHOD3(mockGetValue, bool(const WindowsRegKey*, const utf8String&, utf8String&));
    MOCK_METHOD3(mockSetValue, bool(WindowsRegKey*, const utf8String&, DWORD));
    MOCK_METHOD3(mockSetValue, bool(WindowsRegKey*, const utf8String&, const utf8String&));
    MOCK_METHOD2(mockDeleteValue, void(WindowsRegKey*, const utf8String&));
    MOCK_METHOD1(mockCreateSubKey, WindowsRegKey(const utf8String&));
    MOCK_METHOD1(mockDeleteKey, void(WindowsRegKey*));
    MOCK_METHOD1(mockGetPath, utf8String(const WindowsRegKey*));

private:

    static WindowsRegKeyMock* m_currentMock;
};

}

}

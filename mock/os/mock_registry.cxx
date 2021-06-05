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

#include "mock_registry.hxx"

using namespace testing;

namespace ansak {

namespace config {

WindowsRegKeyMock* WindowsRegKeyMock::m_currentMock = nullptr;

WindowsRegKey WindowsRegKey::open(const utf8String& key, bool createIfAbsent)
{
    return WindowsRegKeyMock::getMock()->mockOpen(key, createIfAbsent);
}

WindowsRegKey::~WindowsRegKey()
{
}

std::vector<std::string> WindowsRegKey::getValueNames() const
{
    return WindowsRegKeyMock::getMock()->mockGetValueNames(this);
}

DWORD WindowsRegKey::getValueType(const utf8String& valueName) const
{
    return WindowsRegKeyMock::getMock()->mockGetValueType(this, valueName);
}

bool WindowsRegKey::getValue(const utf8String& valueName, DWORD& val) const
{
    return WindowsRegKeyMock::getMock()->mockGetValue(this, valueName, val);
}

bool WindowsRegKey::getValue(const utf8String& valueName, utf8String& val) const
{
    return WindowsRegKeyMock::getMock()->mockGetValue(this, valueName, val);
}

bool WindowsRegKey::setValue(const utf8String& valueName, DWORD val)
{
    return WindowsRegKeyMock::getMock()->mockSetValue(this, valueName, val);
}

bool WindowsRegKey::setValue(const utf8String& valueName, const utf8String& val)
{
    return WindowsRegKeyMock::getMock()->mockSetValue(this, valueName, val);
}

void WindowsRegKey::deleteValue(const utf8String& valueName)
{
    WindowsRegKeyMock::getMock()->mockDeleteValue(this, valueName);
}

}

}

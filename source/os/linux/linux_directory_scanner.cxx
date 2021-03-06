///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017, Arthur N. Klassen
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
// 2017.03.26 - First version
//
//    May you do good and not evil.
//    May you find forgiveness for yourself and forgive others.
//    May you share freely, never taking more than you give.
//
///////////////////////////////////////////////////////////////////////////
//
// linux_directory_scanner.cxx -- Linux directory scanner implementation
//
///////////////////////////////////////////////////////////////////////////

#include "linux_directory_scanner.hxx"
#include "os_layer_linux.hxx"

#include <errno.h>
#include <runtime_exception.hxx>

namespace ansak
{

/////////////////////////////////////////////////////////////////////////////
// static, public, factory

LinuxDirectoryScanner* LinuxDirectoryScanner::newIterator(const FilePath& directory)
{
    LinuxPrimitives p;
    enforce(p.pathExists(directory) && p.pathIsDir(directory),
            "newIterator must receive valid directory");
    auto dir = opendir(directory.asUtf8String().c_str());
    enforce(dir != nullptr, "opendir must return a valid pointer");
    return new LinuxDirectoryScanner(directory, dir);
}

/////////////////////////////////////////////////////////////////////////////
// public, virtual

FilePath LinuxDirectoryScanner::operator()()
{
    if (!m_dir)
    {
        return FilePath::invalidPath();
    }

    auto storage = getNextPath();
    if (storage == nullptr)
    {
        return FilePath::invalidPath();
    }

    while (storage->d_name[0] == '.' &&
            ((storage->d_name[1] == '\0') ||
             (storage->d_name[1] == '.' && storage->d_name[2] == '\0')))
    {
        storage = getNextPath();
        if (storage == nullptr)
        {
            return FilePath::invalidPath();
        }
    }

    m_lastError = 0;
    return m_path.child(storage->d_name);
}

struct dirent* LinuxDirectoryScanner::getNextPath()
{
    auto rc = readdir(m_dir.get());
    if (rc == nullptr)
    {
        m_dir.reset(nullptr);
        m_lastError = errno;
    }
    return rc;
}

}

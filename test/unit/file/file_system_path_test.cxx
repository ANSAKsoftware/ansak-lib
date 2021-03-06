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
// 2017.03.21 - First Version
//
//    May you do good and not evil.
//    May you find forgiveness for yourself and forgive others.
//    May you share freely, never taking more than you give.
//
///////////////////////////////////////////////////////////////////////////
//
// file_system_path_test.cxx -- Tests for FileSystemPath in file_system_path.cxx
//
///////////////////////////////////////////////////////////////////////////

#include <file_system_path.hxx>
#include <os/mock_operating_system.hxx>
#include <runtime_exception.hxx>
#include <memory>

#include <gmock/gmock.h>

using namespace ansak;
using namespace std;
using namespace testing;

namespace ansak
{

// a never-used mock of the file system to satisfy static-initialization and validation
unique_ptr<OperatingSystemMock> primitive;
OperatingSystemMock* theMock = nullptr;

const OperatingSystemPrimitives* getOperatingSystemPrimitives()
{
    if (theMock == nullptr && !primitive)
    {
        // this will happen during static-initialization of the FileSystemPath class
        primitive.reset(new OperatingSystemMock());
    }
    return theMock == nullptr ? primitive.get() : theMock;
}

extern bool primitivesExist;

}

namespace
{

#if defined(WIN32)

const char* homeDirP = "C:\\Users\\theUser";
const char* copySrcP = "C:\\Users\\theUser\\source";
const char* copyDestP = "C:\\Users\\theUser\\dest";
const char* copyDest2P = "C:\\Users\\theUser\\dest\\two";
const char* foreignPathP1 = "/usr/sbin/nonesuch1";
const char* foreignPathP2 = "/usr/sbin/nonesuch2";

#else

const char* homeDirP = "/home/theUser";
const char* copySrcP = "/home/theUser/source";
const char* copyDestP = "/home/theUser/dest";
const char* copyDest2P = "/home/theUser/dest/two";
const char* foreignPathP1 = "C:\\Windows\\System32\\nonesuch1";
const char* foreignPathP2 = "C:\\Windows\\System32\\nonesuch2";

#endif

}

class FileSystemPathFixture : public Test {
public:
    const FilePath m_home = FilePath(homeDirP);

    FileSystemPathFixture()
    {
        theMock = &m_osMock;
        EXPECT_CALL(m_osMock, mockGetCwd()).WillRepeatedly(Return(m_home));
    }
    ~FileSystemPathFixture()
    {
        theMock = nullptr;
    }
    OperatingSystemMock& OSMock() { return m_osMock; }

private:
    NiceMock<OperatingSystemMock>    m_osMock;
};

TEST(FileSystemPathTest, fileSystemPathChecksPrimitives)
{
    ASSERT_TRUE(ansak::primitivesExist);
    primitive.reset(nullptr);
}

TEST_F(FileSystemPathFixture, defaultConstruct)
{
    FileSystemPath uut;

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathIsDir(_)).WillOnce(Return(true));

    EXPECT_TRUE(uut.isValid());
    EXPECT_TRUE(!uut.isRelative());
    EXPECT_TRUE(!uut.isRoot());
    EXPECT_EQ(m_home.asUtf8String(), uut.asUtf8String());
    EXPECT_TRUE(uut.exists());
    EXPECT_TRUE(!uut.isFile());
    EXPECT_TRUE(uut.isDir());
}

TEST_F(FileSystemPathFixture, nonRelativeConstruct)
{
    FilePath fileThere = m_home.child("someFile");
    FileSystemPath uut(fileThere);

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(_)).WillOnce(Return(false));
    TimeStamp t { 1909, 6, 22, 10, 0, 35 };
    EXPECT_CALL(OSMock(), mockLastModTime(_)).WillOnce(Return(t));
    EXPECT_CALL(OSMock(), mockFileSize(fileThere)).WillOnce(Return(static_cast<uint64_t>(9909ul)));

    EXPECT_TRUE(uut.isValid());
    EXPECT_TRUE(!uut.isRelative());
    EXPECT_TRUE(!uut.isRoot());
    EXPECT_EQ(fileThere.asUtf8String(), uut.asUtf8String());
    EXPECT_TRUE(uut.exists());
    EXPECT_TRUE(uut.isFile());
    EXPECT_TRUE(!uut.isDir());
    EXPECT_EQ(t, uut.lastModTime());
    EXPECT_EQ(static_cast<uint64_t>(9909ul), uut.size());
}

TEST_F(FileSystemPathFixture, stringConstruct)
{
    FilePath fileThere = m_home.child("someFile");
    FileSystemPath uut(fileThere.asUtf8String());

    EXPECT_TRUE(uut.isValid());
    EXPECT_TRUE(!uut.isRelative());
    EXPECT_TRUE(!uut.isRoot());
    EXPECT_EQ(fileThere.asUtf8String(), uut.asUtf8String());
}

TEST_F(FileSystemPathFixture, getParent)
{
    FilePath fileThere = m_home.child("someFile");
    FileSystemPath someFile(fileThere);
    FileSystemPath uut(someFile.parent());

    EXPECT_TRUE(uut.isValid());
    EXPECT_TRUE(!uut.isRelative());
    EXPECT_TRUE(!uut.isRoot());
    EXPECT_EQ(m_home.asUtf8String(), uut.asUtf8String());
}

TEST_F(FileSystemPathFixture, getChildren)
{
    FileSystemPath homeDirSP(m_home);
    EXPECT_CALL(OSMock(), mockPathExists(Eq(homeDirSP))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(homeDirSP))).WillOnce(Return(true));
    FilePath child = FilePath(homeDirP).child("alpha");

    FileSystemPath::ChildrenRetriever r = FileSystemPath(m_home).children();
    auto listMock = dynamic_cast<DirectoryListMock*>(OSMock().m_lister);
    enforce(listMock != nullptr);
    EXPECT_CALL(*listMock, mockInvocation()).WillOnce(Return(FilePath(child))).WillOnce(Return(FilePath::invalidPath()));
    EXPECT_EQ(child, r());
    EXPECT_EQ(FilePath::invalidPath(), r());
}

TEST_F(FileSystemPathFixture, getFromEmptyRetriever)
{
    FileSystemPath homeDirSP(m_home);
    EXPECT_CALL(OSMock(), mockPathExists(Eq(homeDirSP))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(homeDirSP))).WillOnce(Return(true));
    FilePath child = FilePath(m_home).child("alpha");

    FileSystemPath::ChildrenRetriever r = FileSystemPath(m_home).children();
    auto listMock = dynamic_cast<DirectoryListMock*>(OSMock().m_lister);
    enforce(listMock != nullptr);
    EXPECT_CALL(*listMock, mockInvocation()).WillOnce(Return(FilePath(child))).WillOnce(Return(FilePath::invalidPath()));
    auto s = move(r);
    EXPECT_EQ(child, s());
    EXPECT_EQ(FilePath::invalidPath(), s());
    EXPECT_EQ(FilePath::invalidPath(), r());
}

TEST_F(FileSystemPathFixture, createDirectoryAlreadyThere)
{
    FileSystemPath homePath(m_home);
    FileSystemPath somePath(copySrcP);
    EXPECT_CALL(OSMock(), mockPathExists(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(m_home))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(somePath))).WillOnce(Return(false));

    EXPECT_TRUE(homePath.createDirectory());
    EXPECT_FALSE(somePath.createDirectory());
}

TEST_F(FileSystemPathFixture, createDirParentAsDirNotThere)
{
    FileSystemPath homePath(m_home);
    FileSystemPath createPath(copyDestP);
    EXPECT_CALL(OSMock(), mockPathExists(Eq(homePath.asFilePath()))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(homePath.asFilePath()))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(createPath.asFilePath()))).WillOnce(Return(false));

    ASSERT_FALSE(createPath.createDirectory());
}

TEST_F(FileSystemPathFixture, createDirParentNotThereNoRecurse)
{
    FileSystemPath homePath(m_home);
    FileSystemPath createPath(copyDestP);
    EXPECT_CALL(OSMock(), mockPathExists(Eq(homePath.asFilePath()))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(createPath.asFilePath()))).WillOnce(Return(false));

    ASSERT_FALSE(createPath.createDirectory());
}

TEST_F(FileSystemPathFixture, createDirRecursively)
{
    FileSystemPath homePath(m_home);
    FileSystemPath step1(homePath.child("row"));
    FileSystemPath step2(step1.child("your"));
    FileSystemPath step3(step2.child("boat"));

    EXPECT_CALL(OSMock(), mockPathExists(Eq(homePath.asFilePath()))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(homePath.asFilePath()))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step1.asFilePath()))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step2.asFilePath()))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step3.asFilePath()))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockCreateDirectory(_)).WillRepeatedly(Return(true));

    ASSERT_TRUE(step3.createDirectory(FileSystemPath::recursively));
}

TEST_F(FileSystemPathFixture, createDirRecursivelyFail)
{
    FileSystemPath homePath(m_home);
    FileSystemPath step1(homePath.child("row"));
    FileSystemPath step2(step1.child("your"));
    FileSystemPath step3(step2.child("boat"));

    EXPECT_CALL(OSMock(), mockPathExists(Eq(homePath.asFilePath()))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(homePath.asFilePath()))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step1.asFilePath()))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step2.asFilePath()))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(step3.asFilePath()))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockCreateDirectory(_)).WillOnce(Return(false));

    ASSERT_FALSE(step3.createDirectory(FileSystemPath::recursively));
}

TEST_F(FileSystemPathFixture, createFileExists)
{
    FileSystemPath exists(copySrcP);
    EXPECT_CALL(OSMock(), mockPathExists(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillRepeatedly(Return(true));

    ASSERT_TRUE(exists.createFile());
    ASSERT_FALSE(exists.createFile(FileSystemPath::failIfThere));
}

TEST_F(FileSystemPathFixture, createFile)
{
    FileSystemPath doesntExist(copySrcP);
    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockCreateFile(_)).WillOnce(Return(true));

    ASSERT_TRUE(doesntExist.createFile());
}

TEST_F(FileSystemPathFixture, copyWont)
{
    FileSystemPath fsp1, fsp2;
    EXPECT_FALSE(fsp1.copyFromFile(fsp2));
}

TEST_F(FileSystemPathFixture, copyWontBecause)
{
    FileSystemPath fspGood;
    FileSystemPath foreigner1(foreignPathP1);
    FileSystemPath foreigner2(foreignPathP2);

    EXPECT_FALSE(fspGood.copyFromFile(foreigner1));
    EXPECT_FALSE(foreigner2.copyFromFile(fspGood));
    EXPECT_FALSE(foreigner2.copyFromFile(foreigner1));
    EXPECT_CALL(OSMock(), mockPathExists(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillRepeatedly(Return(true));
    EXPECT_TRUE(fspGood.copyFromFile(fspGood));
}

TEST_F(FileSystemPathFixture, copyWontDestProblem)
{
    FileSystemPath fspSrc(copySrcP);
    FileSystemPath fspDest(copyDestP);

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockRemove(_)).WillOnce(Return(false));
    EXPECT_FALSE(fspDest.copyFromFile(fspSrc));
}

TEST_F(FileSystemPathFixture, copyWontDestNotInDirectory)
{
    FileSystemPath fspSrc(copySrcP);
    FileSystemPath fspDest(copyDest2P);

    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copySrcP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(Eq(FilePath(copySrcP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copyDest2P)))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copyDestP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(FilePath(copyDestP)))).WillRepeatedly(Return(false));
    EXPECT_FALSE(fspDest.copyFromFile(fspSrc));
}

TEST_F(FileSystemPathFixture, copyCallThrough)
{
    FileSystemPath fspSrc(copySrcP);
    FileSystemPath fspDest(copyDest2P);

    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copySrcP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(Eq(FilePath(copySrcP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copyDest2P)))).WillRepeatedly(Return(false));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(copyDestP)))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(FilePath(copyDestP)))).WillRepeatedly(Return(true));

    EXPECT_CALL(OSMock(), mockCreate(_, _, _)).WillOnce(Return(10221u));
    EXPECT_CALL(OSMock(), mockOpen(_, _, _, _)).WillOnce(Return(10222u));

    EXPECT_CALL(OSMock(), mockRead(_, _, _, _)).WillOnce(Return(static_cast<size_t>(4080u)));
    EXPECT_CALL(OSMock(), mockWrite(_, _, _, _)).WillOnce(Return(static_cast<size_t>(4080u)));

    EXPECT_TRUE(fspDest.copyFromFile(fspSrc));
}

TEST_F(FileSystemPathFixture, moveToWont)
{
    FileSystemPath moveNot1(copySrcP);
    FileSystemPath destination(copyDest2P);
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(moveNot1)))).WillOnce(Return(false));
    ASSERT_FALSE(moveNot1.moveTo(destination));

    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(moveNot1)))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(destination.parent())))).WillOnce(Return(false));
    ASSERT_FALSE(moveNot1.moveTo(destination));

    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(moveNot1)))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(destination.parent())))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(FilePath(destination.parent())))).WillOnce(Return(false));
    ASSERT_FALSE(moveNot1.moveTo(destination));
}

TEST_F(FileSystemPathFixture, moveTo)
{
    FileSystemPath moveNot1(copySrcP);
    FileSystemPath destination(copyDest2P);

    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(moveNot1)))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathExists(Eq(FilePath(destination.parent())))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(Eq(FilePath(destination.parent())))).WillOnce(Return(true));

    EXPECT_CALL(OSMock(), mockMove(_, _)).WillOnce(Return(true));
    ASSERT_TRUE(moveNot1.moveTo(destination));
}

TEST_F(FileSystemPathFixture, removeFile)
{
    FileSystemPath removeThisFile(copySrcP);

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(false));
    EXPECT_TRUE(removeThisFile.remove());

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockRemove(_)).WillOnce(Return(false));
    EXPECT_FALSE(removeThisFile.remove());

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockRemove(_)).WillOnce(Return(true));
    EXPECT_TRUE(removeThisFile.remove());

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsDir(_)).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathIsFile(_)).WillOnce(Return(false));
    EXPECT_FALSE(removeThisFile.remove());
}

void mockRemoveEmptyDir(DirectoryListPrimitive& mockIt)
{
    DirectoryListMock& mockMe = dynamic_cast<DirectoryListMock&>(mockIt);
    EXPECT_CALL(mockMe, mockInvocation()).WillOnce(Return(FilePath::invalidPath()));
}

TEST_F(FileSystemPathFixture, removeEmptyDirectory)
{
    FileSystemPath removeThisDir(copySrcP);

    EXPECT_CALL(OSMock(), mockPathExists(FilePath(removeThisDir))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(removeThisDir))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathIsDir(FilePath(removeThisDir))).WillRepeatedly(Return(true));

    OSMock().registerPathIteratorMocker(FilePath(removeThisDir), mockRemoveEmptyDir);

    EXPECT_CALL(OSMock(), mockRemoveDirectory(_)).WillOnce(Return(true));
    EXPECT_TRUE(removeThisDir.remove());
}

void mockWontRemoveNonemptyDir(DirectoryListPrimitive& mockIt)
{
    DirectoryListMock& mockMe = dynamic_cast<DirectoryListMock&>(mockIt);
    EXPECT_CALL(mockMe, mockInvocation()).WillOnce(Return(FilePath(copySrcP).child("alpha")));
}

void mockRemoveNonemptyDir(DirectoryListPrimitive& mockIt)
{
    DirectoryListMock& mockMe = dynamic_cast<DirectoryListMock&>(mockIt);
    EXPECT_CALL(mockMe, mockInvocation()).WillOnce(Return(FilePath(copySrcP).child("alpha"))).
                                          WillOnce(Return(FilePath(copySrcP).child("bravo"))).
                                          WillOnce(Return(FilePath(copySrcP).child("charlie"))).
                                          WillOnce(Return(FilePath::invalidPath()));
}

TEST_F(FileSystemPathFixture, dontRemoveNonEmptyDirectory)
{
    FileSystemPath removeThisDir(copySrcP);

    EXPECT_CALL(OSMock(), mockPathExists(FilePath(removeThisDir))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(removeThisDir))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathIsDir(FilePath(removeThisDir))).WillRepeatedly(Return(true));

    OSMock().registerPathIteratorMocker(FilePath(removeThisDir), mockWontRemoveNonemptyDir);

    EXPECT_FALSE(removeThisDir.remove());
}

TEST_F(FileSystemPathFixture, removeNonEmptyDirectory)
{
    FileSystemPath removeThisDir(copySrcP);

    EXPECT_CALL(OSMock(), mockPathExists(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(removeThisDir))).WillOnce(Return(false));
    EXPECT_CALL(OSMock(), mockPathIsDir(FilePath(removeThisDir))).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(copySrcP).child("alpha"))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(copySrcP).child("bravo"))).WillOnce(Return(true));
    EXPECT_CALL(OSMock(), mockPathIsFile(FilePath(copySrcP).child("charlie"))).WillOnce(Return(true));

    OSMock().registerPathIteratorMocker(FilePath(removeThisDir), mockRemoveNonemptyDir);

    EXPECT_CALL(OSMock(), mockRemove(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(OSMock(), mockRemoveDirectory(_)).WillOnce(Return(true));
    EXPECT_TRUE(removeThisDir.remove(FileSystemPath::recursively));
}

# BUILDING ansak-lib

These instructions are adapted from those written for **ansak-string**. It is a
TODO for both libraries to produce scripts to automate what can be automated
over time.

## PREPARATION

These are the steps that need to be executed to use `ansak-lib` in your
project, which contains source for `libansak.a` / `ansak.lib` and for
`libansakSqlite.a` / `ansakSqlite.a`. The first of these will be built without
regard to what else is installed in the machine where `ansak-lib` has been
cloned.

If you want the SQLite interface classes built, make sure that SQLite is
is installed in your system beforehand, in a place where the `ansak-lib` build
can find it.

On Linux or in macOS, this is easily enough accomplished through a
distro-supplied package.

On Windows, you'll need to download the `DLL`, header and `.def` files, generate
the implib `.lib` file and place them appropriately. The scheme I used was to
put `sqlite3.h` in `C:\\ProgramData\\include`, to store the generated
`sqlite3.lib` file into `C:\\ProgramData\\lib` and to set the `INCLUDE` and
`LIB` environment variables beforehand. In order to run the integration tests,
and for running the result, I stored the DLL (32-bit or 64-bit, as I was
building each version of the library) into a directory referred to by the
`PATH` environment.

1. `git clone` of any commit of `ansak-lib` downstream of `821272e`
   (Don't forget to run `git submodule update --init` afterward so as to bring
   in googletest and googlemock)
2. create a directory, preferably outside the local image of the repository for
   building the software. _The next steps are specific to each platform._
3. run `cmake` in that other directory, using the generator of your choice on
   that platform
4. execute the build tool for that platform against the result of cmake
5. push the resulting library to a place where other build tools can find them.
   _This completes the platform specific steps in the process._
6. prepare to use the resulting files in other projects

In the remainder of the document, I will give the platform-specific steps for
each of Linux, Windows and under macOS, and then finish with the post-build
preparation step after that.

The following sections assume a directory structure like this:

```
source
  +---- ansak-lib
  +---- build
```

but the structure could be anything else at all, including the configuration
that I do **NOT** recommend of putting the build directory inside where the
`ansak-lib` repo files live.


## On Linux

This is also how to do it under `Cygwin` and `MinGW`.

3. On Linux you probably won't need the `-G Makefile` part of the command line
   as the Makefile generator is the default on many platforms:

```
cd source\build
cmake ..\ansak-lib
```

4. Running `make` will produce `libansak.a` (and maybe `libansakSqlite.a`) and
   the unit and integration tests. You can see a full catalog of make-targets
   by executing `make help`

   Go ahead and run `ctest .` in the same directory to make sure all is well.
   You can run the tests directly in case of failure by executing programs you
   find by running `find test -perm 0755` from the build directory, and
   googletest allows individual suites and tests to be selected by command-line
   parameter.

5. Copy library product(s) to your favourite library file location, probably
   to `/usr/local/lib'.
   Copy the header files from `ansak-lib/interface` to a a directory inside a
   neighbouring include directory, probably to `/usr/local/include/ansak`.


## On Windows

3. On Windows, run `cmake --help` and choose the Visual Studio generator
   closest to the version of MSDev installed on your system. Choose the
   architecture you need to support more, Win32 or x64 (probalby x64) and then
   run:

```
cd source\build
cmake ..\ansak-lib -G "Visual Studio 16 2019" -A x64
```

4. Start up Developer Studio and open the project cmake created.

   (TODO: write commands that can be used to get msdev to build this from
   the command line)

   Using the Build menu's Batch facility, build all of Debug, MinSizeRel,
   Release and RelWithDebInfo targets.
   Using the Build menu's Batch facility, build all of Debug, MinSizeRel,
   Release and RelWithDebInfo for the `ansak` target (and `ansakSqlite`, if
   available). You can do the same wtih the `test-` and `integration-` targets
   in order to run the tests in each configuration.

5. There is no default structure for includes and libraries that's standard
   on Windows operating systems. You may have a system of your own. These steps
   will prep your system for default operation with `FindANSAK.cmake` to build
   `ansak-lib` and enabling your components to be built that way.

   Create this structure under `C:\\ProgramData`:

   * `include`
   * `include\\ansak`
   * `lib`
   * `lib\\x64`
   * `lib\\x64\\Debug`
   * `lib\\x64\\MinSizeRel`
   * `lib\\x64\\Release`
   * `lib\\x64\\RelWithDebInfo`

   Copy recursively) the header files from `ansak-lib\\interface` to
   `C:\\ProgramData\\include\\ansak`.

   Copy the different `ansak.lib` (and maybe also `ansakSqlite.lib`) files to
   the different sub-directories of `C:\\ProgramData\\lib`.

   If you also build 32-bit components, consider repeating steps 4 and 5 in
   a build-32 directory with the commands:

```
cd source\build-32
cmake ..\ansak-lib -G "Visual Studio 16 2019" -A Win32
```

   The destination for Win32 build projects would be under a `lib\\Win32\\...`
   sub-directory next to the `lib\\x64\\...` sub-directories.

   If you don't want to use a `lib` and `include` directory under
   `C:\\ProgramData`, through use of the `PROGRAM\_DATA` parameter or
   environment value, or through the `ANSAK\_DIR` parameter or environment
   variable, `FindANSAK.cmake` will still be able to find your components.

## On macOS

3. On MacOS, you may want to use the Makefile generator, in which case the
   building steps will be very similar to those under Linux. You can opt to
   use the Xcode generator, though, and that will enable debugging the unit
   tests in Xcode which may prove helpful. Either way, you will be using clang
   in Apple's somewhat unique environment, which for many purposes ignores the
   file-system `/usr...` directories, prefixing them at the slightest
   provocation to be from within the Xcode Application Package. (which has
   implications for step 5. This true, even if you explicitly prefix your
   `cmake` command with `CC=gcc CXX=g++`. You may ask for gcc but without
   contortions, Apple will give you clang.

```
cd source\build
cmake ..\ansak-lib -G Xcode
```

4. Running `xcodebuild -project ansakLib.xcodeproj -list` will show the list of
   schemes. In order to build the librarie in Release mode run:

```
xcodebuild -C Release -scheme ansak
xcodebuild -C Release -scheme ansakSqlite
```

   You can build debug versions using `-C Debug` (or MinSizeRel or
   RelWithDebInfo), or the test- and integration- components by choosing each
   of the appropriate schemes.

5. Apple being based on Unix, you'd think that `/usr/local/include` and
   `/usr/local/lib` (or the varieties without `local`) would work. But they
   don't. At least not unless you accept that `/usr` is rooted inside the
   Xcode package. That strikes me as a bad idea but maybe it works for you.
   Assuming you've also set up a soft-link from `$HOME/usrlocal` to
   `/usr/local` directory, copy the files about in the same way as you would
   on Linux.

## Preparing to use `ansak-lib`

6. If you aren't using `cmake` for your projects, for Linux you need do no
   more, especially if you copied the results to a "standard place".

   If you are planning to use `ansak-lib` for your own projects AND
   [ (a) you used a non-"standard" destination, OR
     (b) you are building on macOS or Windows ] AND
     (c) you want to use cmake for your projects:

   (a) Then note that destination for when you are going to invoke cmake
   (b) in order to find ANSAK components correctly from there, copy
   `./ansak-lib/FindANSAK.cmake` to somewhere your other project can find it

   (This will often be to a cmake/modules directory within your project,
   with appropriate search path changes in your root `CMakeLists.txt` file but
   you may choose to add it to the default modules of your cmake installation.)

   (c) When you execute cmake for your project, if you think your environment
   may not be able to find `ansak-lib` components unaided, add
   `-DANSAK\_DIR=\<other-location\>` to the cmake line for your project, and
   name the appropriate include path and library file in your project as
   needed.

   On Linux, I don't bother with a `ANSAK\_DIR` value because I put the files
   in `/usr/local/...`.

   Even on Windows, the Makefile generators (and possilby others) set things up
   to build only one kind of product (Debug or Release or whatever). The Visual
   Studio generator creats a solution that can build any of four types of
   executable product. `FindANSAK.cmake` creates appropriate generator
   expressions to operate correctly in that case.

   Also, on Windows, I don't bother with `ANSAK\_DIR` because I am using
   `C:\\ProgramData\\...` and `FindANSAK.cmake` looks there by default.

   On the Mac, if I were using the `usr/local/...` directory inside Xcode, I
   wouldn't use `ANSAK\_DIR` there either but since I want to use `/usr/local`
   and I'm using `$HOME/usrlocal/...` to refer to it, I need to use
   `ANSAK\_DIR` explicitly in order to refer to it.

## Coming "Real Soon Now"

Currently, there aren't a lot of users of this library but if that changes, I am
open to adding scripting to produce Debian, RedHat or Arch packages, or an NSIS
installer for the result.

I think I'll want to automate the process for myself at least, so I'll probably
dump whatever scripts I write into this repo as well.

If this code does stuff you need in ways you like, please feel free to download,
run and enjoy it.

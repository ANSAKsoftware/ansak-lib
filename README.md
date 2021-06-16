# ansak-lib
general purpose C++11 (and later) widgets for cross platform software development

## The main ANSAK library

* platform-independent timestamp, argument parsing and file-path handling,
  includes a nullptr-exception to that can be thrown when a pointer is
  required, and a runtime exception that can be used as part of an `enforce`
  inline function (an "assert" that can fire in release as well as debug code),
  as a way to enforce a contract between an object and its clients.

** This kind of exception was also something I fell in love with while XP-ing in
  Java: write some method's declaration and a dummy body containing nothing but
  a `throw` in the body to say something is unimplemented, then have something
  that compiles and you can run tests (that fail) against.

* an OS-abstraction layer that supports a platform independent file-system path
  and file-handle classes.

* a file-tools unit with RAII-implemented temporary file and directory handling
  and an abstraction around files of lines, that enables handling their
  contents as though it were a vector of strings.

* a configuration-setting system that allows using `.rc` files on platforms
  where that is usual (everywhere but Windows) or the registry where that is
  expected (Windows)

## The ANSAK Sqlite library

* a means for RAII-opening a SQLite database

* preparing and binding-in values into queries whose compiled statements can be
  re-used with new values as needed, and results can be retrieved by whatever
  types into expected C++ types.

## Testing

For my own satisfaction and as a learning experience, I wrote tests as code.

* I wrote unit tests, in which one set of objects at a time was subject to
  scrutiny, their partners provided through googlemock.

* And integration tests, in which they were tested as though being used.

* lcov targets are included to prove that all or nearly all lines of each
  file is being tested

## `FindANSAK.cmake`

Is provided as an echo of the one provided by ansak-string. It should enable
using ansak-string and ansak-lib whether on Linux, macOS or Windows.

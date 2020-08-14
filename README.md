# GEL7 Project Setup Instructions
* [MinGW64 Setup Instructions (32bit and 64bit)](docs/README-mingw.md) - Windows MSYS2
* [Linux/WSL Setup Instructions](docs/README-linux.md) - Ubuntu or WSL
* [Android Setup Instructions](docs/README-android.md) - Gradle
* [Apple Setup Instructions (macOS, iOS, tabletOS, tvOS)](docs/README-apple.md) - macOS

# `config.mk` options
Create this file in the root of your project and you can add settings used by the GNU Make toolchain.

### `DEBUG := 1`
Enable debug mode. Necessary for MinGW64 to work with GDB, and to get logging while debugging with GDB

### `RELEASE := 1`
Enable release mode. This generates optimized code, enables stripping, and other things useful for generating a sharable executable.

### `STATIC := 1`
Enables static linking mode. Not supported everywhere, but where it is you'll get a larger executable with fewer dependencies. Enabled by default on certain targets.

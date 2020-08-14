# /Build/
The `Build` folder contains GNU Makefile modules for various build targets. A target is either a unique system or platform (i.e. a Windows and a Steam build are different).

Some platforms are not practical to be built using Makefile's and the CLI, so in those cases source builds with project files are generated instead.

* [Docs/](Docs/) - Documentation on how to make specific builds, and how to setup the build environment
* [Target/](Target/) - Build targets
* [Include/](Include/) - Mini makefiles included by targets to avoid redudant code

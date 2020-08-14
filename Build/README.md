# /Build/
The `Build` folder contains GNU Makefile modules for various build targets. A target is either a unique system or platform (i.e. a Windows and a Steam build are different).

Some platforms are not practical to be built using Makefile's and the CLI, so in those cases source builds with project files are generated instead.

* `/Target/` - The various build targets
* `/Include/` - Mini makefiles included by targets to avoid redudant code
* `/Docs/` - Documentation on how to make specific builds (how to setup the build environment)

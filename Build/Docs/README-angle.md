**IMPORTANT**: I'm including this for legacy reasons. Building Angle from sources is a PAIN IN THE ASS!
It's easier to simply steal the DLL's from Chrome. The details below are likely incomplete. The reference is
"sort of" what you need to get started, but it's kinda crappy.

The key takeaway from attempting this is the following:

* Angle cannot be built on MinGW64
* Managing and downloading the Angle sources requires something like WSL and Python.
* Angle for Windows must be built with Visual Studio

I mistakenly thought my problem with Angle was the DLL's I stole, but in fact I neglected to "hint" or
"suggest" to SDL that I wanted an OpenGL ES window. Without this hint, on Windows SDL will attempt to
create a regular OpenGL window. With the hint, it searches for the EGL and the GLES driver as desired.

## Building Angle
Reference: [DevSetup](https://github.com/google/angle/blob/master/doc/DevSetup.md)

### Prerequisites: Setting up depot_tools
To build Angle, you need Google/Chromium's `depot_tools` to fetch all the required packages. This is a collection of python scripts that can simply be checked-out and used.

```
cd Work/build
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
```

Once downloaded, add it to the path.

```
export PATH=~/Work/build/depot_tools:$PATH
```

I'm not 100% sure, but I think you need to do the following to finish setting up.

```
gclient
```

## Prerequisite: Making Python 2 your main python version
Start by checking your python version.

```
python --version
```

If you're running python 2, you're good!

If the command fails, you either need to install python, or use a helper package to pick the current python.

```
# Make Python 2 the default
sudo apt install python-is-python2

# Make Python 3 the default
sudo apt install python-is-python3
```

## Downloading Angle (and its dependencies)
This will take a while.

```
cd ~/Work/build
export PATH=~/Work/build/depot_tools:$PATH
export DEPOT_TOOLS_WIN_TOOLCHAIN=0

git clone https://chromium.googlesource.com/angle/angle
cd angle
python scripts/bootstrap.py
gclient sync
git checkout master

./build/install-build-deps.sh
```

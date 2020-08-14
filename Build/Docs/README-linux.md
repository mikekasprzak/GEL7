TODO: Describe setup from a stock Ubuntu install.

```
sudo apt install libsdl2-dev libglew-dev 
sudo apt install valgrind
```

# Setting up WSL2
References: [OMG Ubuntu](https://www.omgubuntu.co.uk/how-to-install-wsl2-on-windows-10), [PureInfoTech](https://pureinfotech.com/install-windows-subsystem-linux-2-windows-10/)

Make sure you have at least Windows 10 version `2004` (May 2020).

Open PowerShell as Administrator.

```
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

Virtual Machine Platform is necessary for WSL2 support. If your CPU doesn't support virtualization, you're limited to WSL1. If that's you, you're finished. Simply reboot and you're ready to install Linux. For WSL2 there are a few more steps.

**Reboot** your PC.

Open PowerShell as Administrator.

```
wsl --set-default-version 2
```

WSL2 is now the default. Any new Linux installations will now use a WSL2 backend.

## Installing Linux on WSL
Open the **Microsoft Store** and search for your desired Linux distro (`Ubuntu 20.04` or such).

## Converting WSL1 to WSL2
Open PowerShell.

```
wsl -l -v
```

This will list your available Linux installations. Something like this:

```
  NAME            STATE           VERSION
* Ubuntu-20.04    Running         1
```

If state is running, close your WSL windows and wait until STATE is `Stopped`.

Now do the following:

```
wsl --set-version Ubuntu-20.04 2
```

This might take a while. Once it finishes, you can use `wsl -l -v` to confirm it worked. Start your WSL shell as usual.


## Using OpenGL on WSL
References: [WSL on GitHub](https://github.com/microsoft/WSL/issues/2855), [Superuser/Stackoverflow](https://superuser.com/questions/1476086/error-cant-open-display-0)

Even though it's not technically "supported", you can run OpenGL XWindows Applications on WSL.

### Installing a Windows XServer
Download and install **VcXsrv**:

[https://sourceforge.net/projects/vcxsrv/](https://sourceforge.net/projects/vcxsrv/)

Start a VcXserv with the following options:

* Multipe Windows (for native XWindows apps)
* Display = `0` (but default might be 0 anyway)
* Start with "No Client"
* Disable native OpenGL (*shrug*)
* Options: `-ac` (for security reasons this was necessary for me to get it working)

The last option is supposed to be optional, but there's probably another setting somewhere that isn't obvious.

Your XServer is ready to recieve Windows.

### Installing XServer Support on Ubuntu
I'm not 100% sure this is the best route, but the easiest way to make sure you have all the xwindows interfacing libraries is to install the ubuntu desktop.

```
sudo apt install ubuntu-desktop 
```

Then you can (attempt) to launch XWindows applications (though it will fail at this point).

```
xcalc
xclock
xeyes
```

### Connecting to the XServer
Connecting to the XServer should be as simple as specifying the display.

```
export DISPLAY=localhost:0
```

Alas, we appear to be firewalled and unable to access it. Maybe it's a WSL2 issue, since WSL2 requires virtualization support. Not sure.

The alternative is to do the following.

```
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
```

This looks up your IP address and uses that, binding to display `:0`.

With this, OpenGL and Windowed applications should now work.

```
sudo apt install mesa-utils
glxgears
```

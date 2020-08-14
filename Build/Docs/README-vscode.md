# Visual Studio Code
VSCode is **ALMOST** an ideal tool for developing many of the targets, but it has quirks.

## Windows/WSL filesystem case sensitivity
Beware if you have case sensitivity enabled on your filesystem! I ran in to this problem, and somehow managed to trace it all the way back to a design flaw. I've documented a workaround in the official issue thread.

[https://github.com/microsoft/vscode-cpptools/issues/1994](https://github.com/microsoft/vscode-cpptools/issues/1994)

# Project README

## Overview
This project is a simple NES emulator built in C. It uses various libraries for graphics, sound, and input handling. The emulator supports rendering the NES screen, playing audio, and basic user inputs.

## Features
- Rendering of NES game screens.
- Playing audio from NES games.
- Basic keyboard and controller input handling.
- Multi-platform support (Linux, Windows, Wine, WebAssembly).

## Project Structure
```
Gui_NES/
├── build/              # .exe files produced by Main.c
├── src/                # Source code
│   ├── Main.c          # Entry point
│   ├── *.h             # Standalone Header-based C-files without *.c implementations
├── Makefile.linux      # Linux Build configuration
├── Makefile.windows    # Windows Build configuration
├── Makefile.wine       # Wine Build configuration
└── README.md           # This file
```

### Prerequisites
- GCC or Clang compiler and debugger for the respective platform (Linux, Windows).
- Make utility.
- Standard development tools.
- Libraries needed:
  - Linux: X11, ALSA
  - Windows: WINAPI, Xinput
  - Wine: Same as Linux libraries
  - WebAssembly: Emscripten

## Build & Run
### Linux
To build the project on Linux:
```bash
cd Gui_NES/
make -f Makefile.linux all
```
To run the emulator:
```bash
make -f Makefile.linux exe
```

### Windows
To build the project on Windows:
```cmd
cd Gui_NES\
make -f Makefile.windows
```
To run the emulator:
```cmd
make -f Makefile.windows exe
```

### Wine
For Linux cross-compilation for Windows:
```bash
cd Gui_NES/
make -f Makefile.wine all
```
To run the emulator on Wine:
```bash
WINEPREFIX=~/wine64 WINEARCH=win64 wine build/Main.exe
```

### WebAssembly
For building and running in a web browser using Emscripten:
```bash
cd Gui_NES/
make -f Makefile.web all
```
To run the emulator in a web browser, navigate to `build/index.html` in your browser.
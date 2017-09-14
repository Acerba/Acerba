# How to (WIP)

1. Download [CMake](https://cmake.org/download/)
2. Run CMake-Gui
  - Set Source Path (Where is the source code)
  - Set Build Path (Where to build the binaries)
  - Press **Configure**
     - Set Visual Studio Version [**NOTE: Visual Studio 2015 has a bug, read how to fix it**](https://github.com/Acerba/Acerba/blob/master/docs/MSB3073_fix_illustration.pdf)
  - Press **Generate**
  - Press **Open Project** ***OR*** go to **Build path** and **open project file** there
3. Visual Studio
   - Build Solution
   - **IF** something failed -> Check [**NOTE: Visual Studio 2015 bug**](https://github.com/Acerba/Acerba/blob/master/docs/MSB3073_fix_illustration.pdf)

Note: Acerba requires OpenAL files and we are going to automate this problem ASAP.

# Guide to Installing and Learning C++

## Introduction

C++ is a powerful, high-performance programming language widely used for systems/software development, game development, and more. This guide will help you set up a C++ environment on your system and introduce you to basic programming concepts in C++.

## Table of Contents

- [Guide to Installing and Learning C++](#guide-to-installing-and-learning-c)
  - [Introduction](#introduction)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
    - [Windows](#windows)
    - [macOS](#macos)
    - [Linux](#linux)
  - [Setting Up a C++ IDE](#setting-up-a-c-ide)
    - [Visual Studio Code](#visual-studio-code)

## Installation

### Windows

1. **Install MinGW**:
   - Go to the [MinGW-w64 download page](https://www.mingw-w64.org/download/) or Download Using [MinGW-w64 download page](https://github.com/msys2/msys2-installer/releases/download/2023-05-26/msys2-x86_64-20230526.exe).
   - Download and install the appropriate version.
   - Run the terminal MSYS2 or type MINGW.
   - In that terminal run this command `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain` and enter.
     - Accept by pressing Y.
   - After updating MSYS2 we then paste this command `pacman -S mingw-w64-ucrt-x86_64-gcc`
     - This installs the gcc compilers for your projects.
     - To view after installing just type `gcc --version`.
   - Add the path of your MinGW-w64 bin folder to the Windows PATH environment variable.
     - Press Windows Key and type Edit Environment Variables.
       - Either in User or System, select PATH and Edit.
     - Make New and add the path for your MINGW, default is: `C:\msys64\ucrt64\bin`

2. **Verify Installation**:
   - Open Command Prompt and type:
     ```bash
     gcc --version
     g++ --version
     gdb --version
     ```
   - You should see the installed version of `g++`.


### macOS

1. **Install Xcode Command Line Tools**:
   - Open Terminal and type:
     ```bash
     xcode-select --install
     ```

2. **Verify Installation**:
   - In Terminal, type:
     ```bash
     g++ --version
     ```
   - The installed version of `g++` should be displayed.

### Linux

1. **Install GCC**:
   - For Debian/Ubuntu-based systems:
     ```bash
     sudo apt update
     sudo apt install build-essential
     ```

   - For Red Hat/CentOS-based systems:
     ```bash
     sudo yum groupinstall 'Development Tools'
     ```

2. **Verify Installation**:
   - In Terminal, type:
     ```bash
     g++ --version
     ```

## Setting Up a C++ IDE

### Visual Studio Code

[Follow the Guide in VSCode to Setup GCC in your IDE](https://code.visualstudio.com/docs/languages/cpp)
1. Recommend to install these Extensions:
   - Code Runner
   - C/C++
2. Then make your first program:
   1. When running the first time `Run C/C++ File` select the debug **C/C++:g++.exe**
   2. It will become default and will add to your **task.json** and **settings.json**

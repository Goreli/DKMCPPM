# PERM_GEN - Permutation Generator
<p align="center">Melbourne C++ Meetup Materials</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Content Overview](#content-overview)
3. [Build Notes](#build-notes)
4. [Links](#links)

## Introduction

This folder contains C++ source code written to address the String Permutation Generator challenge discussed at the Melbourne C++ Meetup Olympiad on 12/Nov/2019. 


## Content Overview

The source code of the solution consists of the following components located in the [perm_gen](https://github.com/Goreli/DKMCPPM/tree/master/20191112/perm_gen) project directory:

... list the content here

## Build Notes

The project directory containes build artefacts that allow to build the binary executables on two platforms. There is a Makefile file that compiles the source code on the Linux platform using the clang++ compiler. Also, the [vs_projects](https://github.com/Goreli/DKMCPPM/tree/master/20191112/perm_gen/vs_projects) subdirectory contains Visual Studio 2019 project files that can be used to compile Windows Command Line executables.

Here is a summary of the Makefile targets:

... list'em here

To build Windows executables open the dkmccpm.sln solution file located in the root folder of the repository and navigate to this project in the Solution Explorer panel. Here is the project configuration log:
1. Set the language standard to c++17;
2. Let the c++ compiler know the locations of include areas: ../..
3. Switch precompiled headers off;
4. Set the Command Line Arguments in the Debugging section to "Hello World!". Make sure to include the double quoting marks.


## Links

1. Melbourne C++ Meetup on meetup.com:
	https://www.meetup.com/MelbourneCpp/
2. Melbourne C++ Meetup on GitHub.com:
	https://github.com/melbourne-cpp
3. Melbourne C++ Meetup Olympiad materials on GitHub.com: 
	https://github.com/melbourne-cpp/olympiad
4. The Olympiad Wiki page on GitHub.com: 
	https://github.com/melbourne-cpp/olympiad/wiki
5. This repository on GitHub.com:
	https://github.com/Goreli/DKMCPPM
6. This solution on GitHub.com:
	https://github.com/Goreli/DKMCPPM/tree/master/20191112/perm_gen

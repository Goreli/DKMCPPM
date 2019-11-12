# 2D Convex Hull
<p align="center">Around the World in 80 Milliseconds</p>

___

## Table of Contents
1. [Introduction](#introduction)
2. [Content Overview](#content-overview)
3. [Build Notes](#build-notes)
4. [Links](#links)

## Overview

This folder contains source code written to address the 2D Convex Hull challenge published on the Wiki page of the Melbourne C++ Meetup Olimpiad repo: https://github.com/melbourne-cpp/olympiad/wiki. A program has been written in C++ that solves the challenge. Also, a chart has been created using the output of the program that demonstrates a geometric relationship between the input and output datasets. The image below demonstrates how the output of the program forms a 2D convex hull that encompasses an area occupied by the input dataset.

![alt text](https://media.githubusercontent.com/media/Goreli/DKMCPPM/master/20191112/2d_convex_hull/data/100-points.jpg "Input and output datasets combined.")


## Content Overview

The source code of the solution consists of the following components located in the [2d_convex_hull](https://github.com/Goreli/DKMCPPM/tree/master/20191112/2d_convex_hull) directory:

* [ch2d-main.cpp](https://github.com/Goreli/DKMCPPM/blob/master/20191112/2d_convex_hull/ch2d-main.cpp) - the entry module of the C++ program. The program does not parse any command line options and expects an input file with a hardcoded inp-file.csv name. Example input files can be found in the [data](https://github.com/Goreli/DKMCPPM/tree/master/20191112/2d_convex_hull/data) subdirectory;
* [ch2d-alg.hpp](https://github.com/Goreli/DKMCPPM/blob/master/20191112/2d_convex_hull/ch2d-alg.hpp) and [ch2d-alg.cpp](https://github.com/Goreli/DKMCPPM/blob/master/20191112/2d_convex_hull/ch2d-alg.cpp) - these are the main modules that define the solution logic at a high level;
* [ch2d-data-point.hpp](https://github.com/Goreli/DKMCPPM/blob/master/20191112/2d_convex_hull/ch2d-data-point.hpp) and [ch2d-data-point.cpp](https://github.com/Goreli/DKMCPPM/blob/master/20191112/2d_convex_hull/ch2d-data-point.cpp) - an implementation of a 2D geometric point class that provides certain useful functionality utilised in the solution logic module.

The [2d_convex_hull](https://github.com/Goreli/DKMCPPM/tree/master/20191112/2d_convex_hull) directory also contains a Bash script called 


## Build Notes

Navigate to the 

## Links

1. Melbourne C++ Meetup on meetup.com:
	https://www.meetup.com/MelbourneCpp/
2. Melbourne C++ Meetup on GitHub.com:
	https://github.com/melbourne-cpp
3. Melbourne C++ Meetup Olimpiad materials on GitHub.com: 
	https://github.com/melbourne-cpp/olympiad
4. The Olimpiad Wiki page on GitHub.com: 
	https://github.com/melbourne-cpp/olympiad/wiki
5. This repository on GitHub.com:
	https://github.com/Goreli/DKMCPPM
6. This solution on GitHub.com:
	https://github.com/Goreli/DKMCPPM/tree/master/20191112/2d_convex_hull

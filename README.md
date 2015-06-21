Conway's Game of Life OpenCL/OpenGL Interoperability
==================================

Based on a fork from [Mobiletainment](https://github.com/Mobiletainment/Conways-Game-of-Life-Parallelized) this implementation uses openCL 1.1 to compute the GameofLife  algorithm.
Furthermore rendering is done in OpenGL with context sharing.

Benchmark:
4k Resolution, 45 FramesPerSecond @nvidia Geforce GTX970, Intel 2500k

What is special about this?
------------
Creates 2 textures in OpenGL and applys GoL in a OpenCL Kernel, by sharing the memory. 

Warning
------------
Ugly Code, with globals and nasty stuff. Even some errors in the gol computation.
Dont use the Code as it is or something like that, its just a demo of OpenGL/openCL Interoperability. 

Requirements
------------
What you need for compiling:
* [Visual C++ or rather Visual Studio 2010 or above](http://www.microsoft.com/visualstudio/eng#products/visual-studio-express-products)
One OpenCL Implementation
* [NVIDIA CUDA OpenCL SDK](https://developer.nvidia.com/cuda-downloads)
* [Intel SDK for OpenCL Applications 2012](http://software.intel.com/en-us/vcsource/tools/opencl-sdk)
* [AMD APP SDK](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)
* GLUT

How to compile?
------------
1. Install the NVIDIA und Intel OpenCL SDKs
2. Get the source code and open the project in Visual Studio
3. Add Include and Libary destinations (APPSDK, CUDA, INTEL) if paths are not default
4. Change Output and Working Directory
5. Make sure you have glut dlls etc.
6. Build



License
------------
(The MIT License)

Copyright (c) 2015 [ixeption](http://www.ixeption.de)

Original Copyright Notes:

Copyright (c) 2012 [David Pertiller](http://www.pertiller.net)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Fast Runtime-Flexible Multi-dimensional Arrays and Views in C++
============

Copyright (c) 2013 by Bjoern Andres, http://www.andres.sc/


Synopsis
------------

Marray is a single header file for fast multi-dimensional arrays and views
in C++. Unlike in other implementations such as boost MultiArray and
Blitz++, the dimension of Marray views and arrays can be set and changed at
runtime. Dimension is not a template parameter in Marray. Arrays and views
that have the same type of entries but different dimension are therefore of
the same C++ type. In conjunction with the comprehensive and
convenient Marray interface, this brings some of the flexibility known from
high-level languages such as Python, R and MATLAB to C++.


Features
------------

- Multi-dimensional arrays and views whose dimension, shape, size and
  indexing order (first or last coordinate major order) can be set and
  changed at runtime
- Access to entries via coordinates, initializer lists, scalar indices and
  STL-compliant random access iterators
- Arithmetic operators with type promotion and expression templates
- Support for STL-compliant allocators
- MIT License for worry-free use and distribution


Tutorial
------------

- An introductory tutorial can be found at src/tutorial/tutorial.cxx


Installation
------------

Marray is a single header file, marray.hxx, that can be used without compiling
or installing libraries. In addition, the Marray repository contains a
tutorial, a unit test and a configuration file for CMake 2.6 (or higher) to
simplify the process of building the tutorial, unit test and reference
documentation.


License
------------

Copyright (c) 2013 by Bjoern Andres, http://www.andres.sc/

This software was developed by Bjoern Andres.
Enquiries shall be directed to bjoern@andres.sc.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The name of the author must not be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

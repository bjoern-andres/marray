// Copyright (c) 2011 by Bjoern Andres (bjoern@andres.sc).
//
// This software was developed by Bjoern Andres.
// Enquiries shall be directed to bjoern@andres.sc.
//
// All advertising materials mentioning features or use of this software must
// display the following acknowledgement: ``This product includes the Marray 
// package developed by Bjoern Andres. Please direct enquiries concerning the 
// Marray package to bjoern@andres.sc''.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// - All advertising materials mentioning features or use of this software must 
//   display the following acknowledgement: ``This product includes the Marray 
//   package developed by Bjoern Andres. Please direct enquiries concerning the 
//   Marray package to bjoern@andres.sc''.
// - The name of the author must not be used to endorse or promote products 
//   derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
#include <iostream>

#include "marray/marray.hxx"

int main() {
    // I. How to construct vectors, matrices and multi-dimensional arrays
    {
        marray::Vector<int> a(5); 
        // a vector with 5 entries initialized as int() (which is 0)

        marray::Vector<int> b(5, 1);
        // a vector with 5 entries initialized as 1

        marray::Vector<int> c(marray::SkipInitialization, 1000);
        // a vector with 1000 entries that remain un-initialized
        
        marray::Matrix<int> d(3, 4);
        // a 3-by-4 matrix with entries initialized as int()        

        marray::Matrix<int> e(3, 4, 1);
        // a 3-by-4 matrix with entries initialized as 1        

        marray::Matrix<int> f(marray::SkipInitialization, 100, 100);
        // a 100-by-100 matrix whose entries remain un-initialized

        marray::Matrix<int> g(3, 4, 1, marray::FirstMajorOrder);
        // a 3-by-4 matrix with entries initialized as 1 
        // and stored in first coordinate major order

        marray::Matrix<int> h(marray::SkipInitialization, 100, 100, 
            marray::FirstMajorOrder);
        // you get the point

        size_t shape[] = {3, 4, 2};
        marray::Marray<int> i(shape, shape + 3);
        // a 3-dimensional array with 2x4x3 entries initialized as int()

        marray::Marray<int> j(shape, shape + 3, 1);
        // a 3-dimensional array with 2x4x3 entries initialized as 1

        marray::Marray<int> k(marray::SkipInitialization, shape, shape + 3);
        // you guessed this would work

        marray::Marray<int> l(shape, shape + 3, marray::FirstMajorOrder);
        // and this

        marray::Marray<int> m(marray::SkipInitialization, shape, shape + 3, 
            marray::FirstMajorOrder);
        // and this
    }

    // II. How to access the entries of arrays, matrices and vectors
    {
        // As matrices and vectors are in fact derived from arrays, the
        // following operations for accessing and manipulating entries can
        // be used in the same way with all three types of objects.
        // Let's take this 3-dimensional array as one specific example:
        size_t shape[] = {3, 4, 2};
        marray::Marray<int> a(shape, shape + 3);

        // Now, there are three ways to access its entries:
        // 1. by coordinates
        a(0, 0, 0) = 1; 
        a(2, 3, 1) = 2;        
        std::cout << a.asString();

        // 2. by scalar indices
        for(size_t j=0; j<a.size(); ++j) {
            std::cout << a(j) << ' ';
        }
        std::cout << std::endl;

        // 3. by STL-compliant random access iterators        
        for(marray::Marray<int>::const_iterator it = a.begin(); 
        it != a.end(); ++it) {
            std::cout << *it << ' ';
        }
        std::cout << std::endl;
    }

    // III. How to do arithmetics with Marray
    {
        // Simple arithmetics works just as you would expect it:
        {
            size_t shape[] = {3, 4, 2};
            marray::Marray<int> a(shape, shape + 3, 2);
            marray::Marray<int> b(shape, shape + 3, 2);
            marray::Marray<int> c;

            ++a;
            --a;

            a += 2;
            a -= 2;
            a /= 2;
            a *= 2;

            c = a + b;
            c = a - b;
            c = a * b;
            c = a / b;

            c = a*a + 2*a*b + b*b;
        }

        // Different types (e.g. int and float) can be combined in one 
        // arithmetic expression, and Marray will promote these types 
        // according to the standard rules:
        {
            marray::Vector<int> a(4, 1);
            marray::Vector<float> b(4, 1.1f);
            marray::Vector<float> c = a + b;
            std::cout << c.asString();
        }

        // Marray has expression templates. The following expression is
        // thus evaluated only for one entry at coordinate (2, 0, 0):
        {
            size_t shape[] = {3, 4, 2};
            marray::Marray<int> a(shape, shape + 3, 2);
            marray::Marray<float> b(shape, shape + 3, 3.1f);

            float c = (a*a*a + 2*a*a*b + 2*a*b*b + b*b*b)(2, 0, 0);
            std::cout << c << std::endl;
        }
    }

    // IV. Constructing and transforming views
    {
        // A view makes a subset of memory look as if it was stored in an 
        // array. With the help of a view, data in a subset of memory can 
        // be accessed and manipulated conveniently. In contrast to arrays
        // which allocate and de-allocate their own memory, views only 
        // reference memory that has been allocated by other means.
        // perhaps the simplest and most important use of views is to
        // read and manipulate sub-arrays:
        {
            // Let's start with a simple matrix...
            marray::Matrix<int> a(15, 5);
            for(size_t j=0; j<a.size(); ++j) {
                a(j) = 10 + static_cast<int>(j);
            }

            // ... and define a view to a 7x3 sub-matrix that starts at
            // coordinates (5, 1):
            size_t base[] = {5, 1};
            size_t shape[] = {7, 3};
            marray::View<int> b = a.view(base, shape); // construct a view
            std::cout << a.asString();
            std::cout << b.asString();        

            // Let's now manipulate the sub-matrix (i.e. the data under the 
            // view) and print the original matrix
            b = 10; 
            std::cout << a.asString();

            // Views can also be used to make data look differently. Here are 
            // two examples:
            
            // 1. Transposition:
            marray::View<int, true> c = a; 
                // Construct a view. 'true' makes the data under the view constant
                // so that this particular view cannot be used to change the data
            std::cout << c.asString();
            c.transpose();
            std::cout << c.asString();

            // 2. Reshaping:
            marray::View<int, true> d = a; 
            size_t newShape[] = {5, 3, 5};
            d.reshape(newShape, newShape + 3);
            std::cout << d.asString();
            
            // The doxygen reference documentation tells you more about shifting, 
            // reshaping and squeezing views.
        }

        // A more advanced use of views is to provide a convenient interface
        // for the multi-dimensional arrays that are native to C:
        {
            int e[3][4][2];
            for(size_t j=0; j<24; ++j) {
                (**e)[j] = static_cast<int>(j);
            }

            size_t shape[] = {3, 4, 2};
            marray::View<int> f(shape, shape + 3, **e, marray::FirstMajorOrder, 
                marray::FirstMajorOrder);
                // The first marray::FirstMajorOrder determines how coordinates
                // are mapped to memory. The second marray::FirstMajorOrder 
                // determines how scalar indices into the view are mapped to
                // coordinates in the view.

            // Let's compare the acces by scalar indices...
            for(size_t j=0; j<24; ++j) {
                std::cout << (**e)[j] << ", " << f(j) << std::endl;
            }

            // ...and by coordinates:
            for(size_t x=0; x<shape[0]; ++x) {
                for(size_t y=0; y<shape[1]; ++y) {
                    for(size_t z=0; z<shape[2]; ++z) {
                        std::cout << "e[" << x << "][" << y << "][" << z << "] = "
                            << e[x][y][z] << ", "
                            << "f(" << x << ", " << y << ", " << z << ") = "
                            << f(x, y, z) << std::endl;
                    }
                }
            }

            // All constructors of View are described in the doxygen reference
            // documentation.
        }
    }

    // Enjoy Marray! And please do not hesitate to contact me at bjoern@andres.sc
    return 0;
}


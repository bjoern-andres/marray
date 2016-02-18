// http://www.andres.sc/marray.html
//
#include <iostream>

#include "andres/marray.hxx"

int main() {
    // how to construct arrays:
    {
        size_t shape[] = {3, 4, 2};
        andres::Marray<int> i(shape, shape + 3);
        // a 3-dimensional array with 2x4x3 entries initialized as int()

        andres::Marray<int> j(shape, shape + 3, 1);
        // a 3-dimensional array with 2x4x3 entries initialized as 1

        andres::Marray<int> k(andres::SkipInitialization, shape, shape + 3);
        // a 3-dimensional array with 2x4x3 entries which remain un-initialized

        andres::Marray<int> l(shape, shape + 3, andres::FirstMajorOrder);
        // a 3-dimensional array with 2x4x3 entries which indexed in
        // first coordinate major order

        andres::Marray<int> m(andres::SkipInitialization, shape, shape + 3, 
            andres::FirstMajorOrder);
        // you get the point
    }

    // how to access the entries of arrays:
    {
        size_t shape[] = {3, 4, 2};
        andres::Marray<int> a(shape, shape + 3);

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
        for(andres::Marray<int>::const_iterator it = a.begin(); it != a.end(); ++it) {
            std::cout << *it << ' ';
        }
        std::cout << std::endl;
    }

    // how to do arithmetics with arrays:
    {
        // simple arithmetics work just as you would expect.
        {
            size_t shape[] = {3, 4, 2};
            andres::Marray<int> a(shape, shape + 3, 2);
            andres::Marray<int> b(shape, shape + 3, 2);
            andres::Marray<int> c;

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

        // different types (e.g. int and float) can be combined in one arithmetic expression.
        // marray will promote the types according to the standard rules.
        {
            size_t shape[] = {4};
            andres::Marray<int> a(shape, shape + 1, 1);
            andres::Marray<float> b(shape, shape + 1, 1.1f);
            andres::Marray<float> c = a + b;
            std::cout << c.asString();
        }

        // marray implements expression templates. the following expression is
        // thus evaluated only for one entry at coordinate (2, 0, 0):
        {
            size_t shape[] = {3, 4, 2};
            andres::Marray<int> a(shape, shape + 3, 2);
            andres::Marray<float> b(shape, shape + 3, 3.1f);

            float c = (a*a*a + 2*a*a*b + 2*a*b*b + b*b*b)(2, 0, 0);
            std::cout << c << std::endl;
        }
    }

    // how to construct and transform views
    {
        // a View makes a contiguous interval in memory look as if it was an 
        // Marray. In contrast to Marray which allocates and de-allocates 
        // memory via an STL-compliant allocator, a View provides an interface 
        // to a contiguous interval in memory that needs to be allocated by 
        // other means. One important use of View is to access sub-arrays.
        {
            // let's start with a simple matrix...
            size_t shape[] = {15, 5};
            andres::Marray<int> a(shape, shape + 2);
            for(size_t j=0; j<a.size(); ++j) {
                a(j) = 10 + static_cast<int>(j);
            }

            // ... and define a view to a 7x3 sub-matrix that starts at
            // coordinates (5, 1):
            size_t base[] = {5, 1};
            shape[0] = 7;
            shape[1] = 3;
            andres::View<int> b = a.view(base, shape); // construct a view
            std::cout << a.asString();
            std::cout << b.asString();        

            // let's now manipulate the sub-matrix (i.e. the data under the 
            // view) and print the original matrix
            b = 10; 
            std::cout << a.asString();

            // A View can also be used to make data look differently. 
            // here are two examples:
            
            // 1. transposition
            andres::View<int, true> c = a; 
                // Construct a View. 'true' makes the data under the view constant,
                // i.e., this particular view cannot be used to change the data.
            std::cout << c.asString();
            c.transpose();
            std::cout << c.asString();

            // 2. reshaping
            andres::View<int, true> d = a; 
            size_t newShape[] = {5, 3, 5};
            d.reshape(newShape, newShape + 3);
            std::cout << d.asString();
            
            // the doxygen reference documentation tells more about shifting, 
            // reshaping and squeezing views.
        }

        // a more advanced use of views is to provide a convenient interface
        // for the multi-dimensional arrays that are native to C:
        {
            int e[3][4][2];
            for(size_t j=0; j<24; ++j) {
                (**e)[j] = static_cast<int>(j);
            }

            size_t shape[] = {3, 4, 2};
            andres::View<int> f(shape, shape + 3, **e, andres::FirstMajorOrder, 
                andres::FirstMajorOrder);
                // the first andres::FirstMajorOrder determines how coordinates
                // are mapped to memory. The second andres::FirstMajorOrder 
                // determines how scalar indices into the view are mapped to
                // coordinates in the view.

            // let's compare the element access by scalar indices...
            for(size_t j=0; j<24; ++j) {
                std::cout << (**e)[j] << ", " << f(j) << std::endl;
            }

            // ...to the element access by coordinates
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

            // all constructors of View are described in the doxygen reference
            // documentation.
        }
    }

    // enjoy Marray!
    return 0;
}

// Copyright (c) 2011 by Bjoern Andres.
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
#include <vector>
#include <string>
#include <iostream>

#include "marray/marray_hdf5.hxx"

inline void test(const bool& x)
{
    if(!x) {
        throw std::logic_error("test failed.");
    }
}

class MarrayHDF5Test {
public:
	MarrayHDF5Test() : filename_("test.h5") {};

	void fileHandlingTest();
	template<class T, marray::CoordinateOrder ORDER>
		void saveLoadTest();
        // tests load, save, and loadShape
	template<class T, marray::CoordinateOrder ORDER>
		void loadHyperslabTest();
	template<class T, marray::CoordinateOrder ORDER>
		void saveHyperslabTest();
    template<class T>
        void saveStdVectorTest();
	template<class T, marray::CoordinateOrder ORDER>
		void createTest();
	template<marray::CoordinateOrder ORDER>
		void additionalTest();
    template<class Type1, class Type2>
        void typeTest();
private:
	std::string filename_;
};

int main() 
{
	{ MarrayHDF5Test t; t.fileHandlingTest(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned char, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned char, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned short, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned short, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned int, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned int, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned long, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<unsigned long, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<char, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<char, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<short, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<short, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<int, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<int, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<long, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<long, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<float, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<float, marray::FirstMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<double, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveLoadTest<double, marray::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.saveStdVectorTest<int>(); }
    { MarrayHDF5Test t; t.saveStdVectorTest<float>(); }

	{ MarrayHDF5Test t; t.loadHyperslabTest<int, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.loadHyperslabTest<int, marray::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.saveHyperslabTest<int, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.saveHyperslabTest<int, marray::FirstMajorOrder>(); }

	{ MarrayHDF5Test t; t.createTest<int, marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.createTest<int, marray::FirstMajorOrder>(); }

	{ MarrayHDF5Test t; t.additionalTest<marray::LastMajorOrder>(); }
	{ MarrayHDF5Test t; t.additionalTest<marray::FirstMajorOrder>(); }
    
	{ MarrayHDF5Test t; t.typeTest<int, float>(); }
	{ MarrayHDF5Test t; t.typeTest<float, int>(); }
	{ MarrayHDF5Test t; t.typeTest<int, double>(); }
	{ MarrayHDF5Test t; t.typeTest<double, int>(); }
	{ MarrayHDF5Test t; t.typeTest<short int, int>(); }
	{ MarrayHDF5Test t; t.typeTest<int, short int>(); }
	{ MarrayHDF5Test t; t.typeTest<unsigned int, int>(); }

    std::cout << "All tests have been passed." << std::endl;
	return 0;
}

void MarrayHDF5Test::fileHandlingTest()
{
    {
        hid_t handle = marray::hdf5::createFile(filename_);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }
    {
        hid_t handle = marray::hdf5::openFile(
            filename_, marray::hdf5::READ_ONLY);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }
    {
        hid_t handle = marray::hdf5::openFile(
            filename_, marray::hdf5::READ_WRITE);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }

    {
        hid_t handle = marray::hdf5::createFile(
            filename_, marray::hdf5::LATEST_HDF5_VERSION);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }
    {
        hid_t handle = marray::hdf5::openFile(
            filename_, marray::hdf5::READ_ONLY, 
            marray::hdf5::LATEST_HDF5_VERSION);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }
    {
        hid_t handle = marray::hdf5::openFile(
            filename_, marray::hdf5::READ_WRITE, 
            marray::hdf5::LATEST_HDF5_VERSION);
        test(handle > 0);
        marray::hdf5::closeFile(handle);
    }
}

template<class T, marray::CoordinateOrder ORDER>
void MarrayHDF5Test::saveLoadTest()
{
	size_t shape[3] = {3, 4, 5};
	marray::Marray<T> m1(&shape[0], &shape[3], 0, ORDER);
	for(size_t j=0; j<m1.size(); ++j) {
		m1(j) = T(j);
	}
	marray::View<T, true> v1 = m1;

    hid_t file = marray::hdf5::createFile(filename_);
	test(file >= 0);
	marray::hdf5::save(file, "array", m1);
	marray::hdf5::save(file, "view", v1);
    marray::hdf5::closeFile(file);

	marray::Marray<T> m2;
	marray::Marray<T> m3;
	marray::Vector<size_t> s2;
	marray::Vector<size_t> s3;
    file = marray::hdf5::openFile(filename_);
	test(file >= 0);
	marray::hdf5::load(file, "array", m2);
	marray::hdf5::load(file, "view", m3);
	marray::hdf5::loadShape(file, "array", s2);
	marray::hdf5::loadShape(file, "view", s3);
    marray::hdf5::closeFile(file);

	test(m1.dimension() == m2.dimension());
	test(m1.size() == m2.size());
	test(m1.coordinateOrder() == m2.coordinateOrder());
	for(size_t j=0; j<m1.dimension(); ++j) {
		test(m1.shape(j) == m2.shape(j));
	}
	for(size_t j=0; j<m1.size(); ++j) {
		test(m1(j) == m2(j));
	}

	test(v1.dimension() == m3.dimension());
	test(v1.size() == m3.size());
	test(v1.coordinateOrder() == m3.coordinateOrder());
	for(size_t j=0; j<v1.dimension(); ++j) {
		test(v1.shape(j) == m3.shape(j));
	}
	for(size_t j=0; j<v1.size(); ++j) {
		test(v1(j) == m3(j));
	}

	test(s2.size() == m2.dimension());
	for(size_t j=0; j<s2.size(); ++j) {
		test(s2(j) == m2.shape(j));
	}

	test(s3.size() == m3.dimension());
	for(size_t j=0; j<s3.size(); ++j) {
		test(s3(j) == m3.shape(j));
	}
}

template<class T>
void MarrayHDF5Test::saveStdVectorTest()
{
    std::vector<T> v(10);
    for(size_t j=0; j<v.size(); ++j) {
        v[j] = static_cast<T>(j);
    }
    hid_t handle = marray::hdf5::createFile(filename_);
    marray::hdf5::save(handle, "stdvector", v);
    marray::hdf5::closeFile(handle);

    handle = marray::hdf5::openFile(filename_);
    marray::Vector<T> w;
    marray::hdf5::load(handle, "stdvector", w);
    marray::hdf5::closeFile(handle);

    test(v.size() == w.size());
    for(size_t j=0; j<v.size(); ++j) {
        test(v[j] == w[j]);
    }
}

template<class T, marray::CoordinateOrder ORDER>
void MarrayHDF5Test::loadHyperslabTest()
{
	size_t shape[4] = {3, 4, 5, 6};
	marray::Marray<T> m1(&shape[0], &shape[4], 0, ORDER);
	for(size_t j=0; j<m1.size(); ++j) {
		m1(j) = T(j);
	}

    hid_t file = marray::hdf5::createFile(filename_);
	test(file >= 0);
	marray::hdf5::save(file, "array", m1);
    marray::hdf5::closeFile(file);

	size_t viewBase[4] = {0, 2, 1, 3};
	size_t viewShape[4] = {3, 2, 1, 3};
	marray::View<T, true> v1;
	m1.constView(viewBase, viewShape, v1);

	marray::Marray<T> m2;
    file = marray::hdf5::openFile(filename_);
	test(file >= 0);
	marray::hdf5::loadHyperslab(file, "array", &viewBase[0],
		&viewBase[4], viewShape, m2);
    marray::hdf5::closeFile(file);

	test(v1.dimension() == m2.dimension());
	test(v1.size() == m2.size());
	test(v1.coordinateOrder() == m2.coordinateOrder());
	for(size_t j=0; j<v1.dimension(); ++j) {
		test(v1.shape(j) == m2.shape(j));
	}
	for(size_t j=0; j<v1.size(); ++j) {
		test(v1(j) == m2(j));
	}
}

template<class T, marray::CoordinateOrder ORDER>
void MarrayHDF5Test::saveHyperslabTest()
{
	size_t shape1[2] = {5, 6};
	marray::Marray<T> m1(&shape1[0], &shape1[2], 0, ORDER);

	size_t base2[2] = {1, 2};
	size_t shape2[2] = {2, 3};
	marray::Marray<T> m2(&shape2[0], &shape2[2], 1, ORDER);

	// simulate writing to hyperslab in memory
	marray::Marray<T> m3 = m1;
	marray::View<T> v;
	m3.view(base2, shape2, v);
	v = T(1);

	// write hyperslab
    hid_t file = marray::hdf5::createFile(filename_);
	test(file >= 0);
	marray::hdf5::save(file, "array", m1);
	marray::hdf5::saveHyperslab(file, "array", &base2[0], &base2[2], shape2, m2);
    marray::hdf5::closeFile(file);

	// load entire array
	marray::Marray<T> m4;
    file = marray::hdf5::openFile(filename_);
	test(file >= 0);
	marray::hdf5::load(file, "array", m4);
    marray::hdf5::closeFile(file);

	// compare results
	test(m3.dimension() == m4.dimension());
	test(m3.size() == m4.size());
	test(m3.coordinateOrder() == m4.coordinateOrder());
	for(size_t j=0; j<m3.dimension(); ++j) {
		test(m3.shape(j) == m4.shape(j));
	}
	for(size_t j=0; j<m3.size(); ++j) {
		test(m3(j) == m4(j));
	}
}

template<class T, marray::CoordinateOrder ORDER>
void MarrayHDF5Test::createTest()
{
	size_t shape[2] = {5, 6};
	marray::Marray<T> m1(&shape[0], &shape[2], 0, ORDER);

    hid_t file = marray::hdf5::createFile(filename_);
	test(file >= 0);
	marray::hdf5::create<T>(file, "creation", &shape[0], &shape[2], ORDER);
	marray::Marray<T> m2;
	marray::hdf5::load(file, "creation", m2);	
    marray::hdf5::closeFile(file);

	// compare results
	test(m1.dimension() == m2.dimension());
	test(m1.size() == m2.size());
	test(m1.coordinateOrder() == m2.coordinateOrder());
	for(size_t j=0; j<m1.dimension(); ++j) {
		test(m1.shape(j) == m2.shape(j));
	}
	for(size_t j=0; j<m1.size(); ++j) {
		test(m1(j) == m2(j));
	}
}

template<class Type1, class Type2>
void MarrayHDF5Test::typeTest() {
    size_t shape[3] = {3, 4, 5};
    {
        marray::Marray<Type1> m(shape, shape+3, 0, marray::FirstMajorOrder);
        for(size_t j=0; j<m.size(); ++j) {
            m(j) = Type1(j);
        }

        hid_t file = marray::hdf5::createFile(filename_);
        test(file >= 0);
        marray::hdf5::save(file, "array", m);
        marray::hdf5::closeFile(file);
    }
}

template<marray::CoordinateOrder ORDER>
void MarrayHDF5Test::additionalTest() {
	{
        size_t size[4] = {1,1,2,2};
        size_t start[4] = {0,0,0,0};
        
        hid_t file = marray::hdf5::createFile("testfile.h5");

        marray::hdf5::create<int>(file, "test", size, size+4, ORDER);
        
        marray::Marray<int> m(size, size+4, -1, ORDER);
        m(0,0,0,0) = 100;
        m(0,0,0,1) = 101;
        m(0,0,1,0) = 102;
        m(0,0,1,1) = 103;
        
        marray::hdf5::saveHyperslab(file, "test", start, start+4, size, m);
        marray::hdf5::closeFile(file);
    }
    {
        marray::Marray<int> m;
        hid_t file = marray::hdf5::openFile("testfile.h5");
        marray::hdf5::load(file, "test", m);
        
        test(m(0,0,0,0) == 100);
        test(m(0,0,0,1) == 101);
        test(m(0,0,1,0) == 102);
        test(m(0,0,1,1) == 103);
        
        marray::hdf5::closeFile(file);
    }
}

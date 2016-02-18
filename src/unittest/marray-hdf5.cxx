// http://www.andres.sc/marray.html
//
#include <vector>
#include <string>
#include <iostream>

#include "andres/marray-hdf5.hxx"

inline void test(const bool& x) {
    if(!x) throw std::logic_error("test failed.");
}

class MarrayHDF5Test {
public:
	MarrayHDF5Test() : filename_("test.h5") {};

    template<class T, andres::CoordinateOrder ORDER>
        void saveLoadTest(); // tests load, save, and loadShape
    template<class T, andres::CoordinateOrder ORDER>
		void loadHyperslabTest();
    template<class T, andres::CoordinateOrder ORDER>
		void saveHyperslabTest();
    template<class T, andres::CoordinateOrder ORDER>
		void createTest();
    template<andres::CoordinateOrder ORDER>
		void additionalTest();
    template<class Type1, class Type2>
        void typeTest();
private:
	std::string filename_;
};

int main() {
    { MarrayHDF5Test t; t.saveLoadTest<unsigned char, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<unsigned short, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<unsigned int, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<unsigned long, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<char, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<short, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<int, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<long, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<float, andres::FirstMajorOrder>(); }
    { MarrayHDF5Test t; t.saveLoadTest<double, andres::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.loadHyperslabTest<int, andres::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.saveHyperslabTest<int, andres::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.createTest<int, andres::FirstMajorOrder>(); }

    { MarrayHDF5Test t; t.additionalTest<andres::FirstMajorOrder>(); }
    
	{ MarrayHDF5Test t; t.typeTest<int, float>(); }
	{ MarrayHDF5Test t; t.typeTest<float, int>(); }
	{ MarrayHDF5Test t; t.typeTest<int, double>(); }
	{ MarrayHDF5Test t; t.typeTest<double, int>(); }
	{ MarrayHDF5Test t; t.typeTest<short int, int>(); }
	{ MarrayHDF5Test t; t.typeTest<int, short int>(); }
	{ MarrayHDF5Test t; t.typeTest<unsigned int, int>(); }

	return 0;
}

template<class T, andres::CoordinateOrder ORDER>
void MarrayHDF5Test::saveLoadTest()
{
	size_t shape[3] = {3, 4, 5};
    andres::Marray<T> m1(&shape[0], &shape[3], 0, ORDER);
	for(size_t j=0; j<m1.size(); ++j) {
		m1(j) = T(j);
	}
    andres::View<T, true> v1 = m1;

    hid_t file = andres::hdf5::createFile(filename_);
	test(file >= 0);
    andres::hdf5::save(file, "array", m1);
    andres::hdf5::save(file, "view", v1);
    andres::hdf5::closeFile(file);

    andres::Marray<T> m2;
    andres::Marray<T> m3;
    std::vector<size_t> s2;
    std::vector<size_t> s3;
    file = andres::hdf5::openFile(filename_);
	test(file >= 0);
    andres::hdf5::load(file, "array", m2);
    andres::hdf5::load(file, "view", m3);
    andres::hdf5::loadShape(file, "array", s2);
    andres::hdf5::loadShape(file, "view", s3);
    andres::hdf5::closeFile(file);

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
        test(s2[j] == m2.shape(j));
	}

	test(s3.size() == m3.dimension());
	for(size_t j=0; j<s3.size(); ++j) {
        test(s3[j] == m3.shape(j));
	}
}

template<class T, andres::CoordinateOrder ORDER>
void MarrayHDF5Test::loadHyperslabTest()
{
	size_t shape[4] = {3, 4, 5, 6};
    andres::Marray<T> m1(&shape[0], &shape[4], 0, ORDER);
	for(size_t j=0; j<m1.size(); ++j) {
		m1(j) = T(j);
	}

    hid_t file = andres::hdf5::createFile(filename_);
	test(file >= 0);
    andres::hdf5::save(file, "array", m1);
    andres::hdf5::closeFile(file);

	size_t viewBase[4] = {0, 2, 1, 3};
	size_t viewShape[4] = {3, 2, 1, 3};
    andres::View<T, true> v1;
	m1.constView(viewBase, viewShape, v1);

    andres::Marray<T> m2;
    file = andres::hdf5::openFile(filename_);
	test(file >= 0);
    andres::hdf5::loadHyperslab(file, "array", &viewBase[0],
		&viewBase[4], viewShape, m2);
    andres::hdf5::closeFile(file);

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

template<class T, andres::CoordinateOrder ORDER>
void MarrayHDF5Test::saveHyperslabTest()
{
	size_t shape1[2] = {5, 6};
    andres::Marray<T> m1(&shape1[0], &shape1[2], 0, ORDER);

	size_t base2[2] = {1, 2};
	size_t shape2[2] = {2, 3};
    andres::Marray<T> m2(&shape2[0], &shape2[2], 1, ORDER);

	// simulate writing to hyperslab in memory
    andres::Marray<T> m3 = m1;
    andres::View<T> v;
	m3.view(base2, shape2, v);
	v = T(1);

	// write hyperslab
    hid_t file = andres::hdf5::createFile(filename_);
	test(file >= 0);
    andres::hdf5::save(file, "array", m1);
    andres::hdf5::saveHyperslab(file, "array", &base2[0], &base2[2], shape2, m2);
    andres::hdf5::closeFile(file);

	// load entire array
    andres::Marray<T> m4;
    file = andres::hdf5::openFile(filename_);
	test(file >= 0);
    andres::hdf5::load(file, "array", m4);
    andres::hdf5::closeFile(file);

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

template<class T, andres::CoordinateOrder ORDER>
void MarrayHDF5Test::createTest()
{
	size_t shape[2] = {5, 6};
    andres::Marray<T> m1(&shape[0], &shape[2], 0, ORDER);

    hid_t file = andres::hdf5::createFile(filename_);
	test(file >= 0);
    andres::hdf5::create<T>(file, "creation", &shape[0], &shape[2]);
    andres::Marray<T> m2;
    andres::hdf5::load(file, "creation", m2);
    andres::hdf5::closeFile(file);

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
        andres::Marray<Type1> m(shape, shape+3, 0, andres::FirstMajorOrder);
        for(size_t j=0; j<m.size(); ++j) {
            m(j) = Type1(j);
        }

        hid_t file = andres::hdf5::createFile(filename_);
        test(file >= 0);
        andres::hdf5::save(file, "array", m);
        andres::hdf5::closeFile(file);
    }
}

template<andres::CoordinateOrder ORDER>
void MarrayHDF5Test::additionalTest() {
	{
        size_t size[4] = {1,1,2,2};
        size_t start[4] = {0,0,0,0};
        
        hid_t file = andres::hdf5::createFile("test.h5");

        andres::hdf5::create<int>(file, "test", size, size+4);
        
        andres::Marray<int> m(size, size+4, -1, ORDER);
        m(0,0,0,0) = 100;
        m(0,0,0,1) = 101;
        m(0,0,1,0) = 102;
        m(0,0,1,1) = 103;
        
        andres::hdf5::saveHyperslab(file, "test", start, start+4, size, m);
        andres::hdf5::closeFile(file);
    }
    {
        andres::Marray<int> m;
        hid_t file = andres::hdf5::openFile("test.h5");
        andres::hdf5::load(file, "test", m);
        
        test(m(0,0,0,0) == 100);
        test(m(0,0,0,1) == 101);
        test(m(0,0,1,0) == 102);
        test(m(0,0,1,1) == 103);
        
        andres::hdf5::closeFile(file);
    }
}


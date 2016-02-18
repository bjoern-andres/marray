// http://www.andres.sc/marray.html
//
#include "andres/hdf5.hxx"

inline void test(const bool& x) {
    if(!x) throw std::logic_error("test failed.");
}

void fileHandlingTest() {
    const std::string filename = "test.h5";
    {
        hid_t handle = andres::hdf5::createFile(filename);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }
    {
        hid_t handle = andres::hdf5::openFile(
            filename, andres::hdf5::READ_ONLY);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }
    {
        hid_t handle = andres::hdf5::openFile(
            filename, andres::hdf5::READ_WRITE);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }

    {
        hid_t handle = andres::hdf5::createFile(
            filename, andres::hdf5::HDF5_VERSION_LATEST);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }
    {
        hid_t handle = andres::hdf5::openFile(
            filename, andres::hdf5::READ_ONLY,
            andres::hdf5::HDF5_VERSION_LATEST);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }
    {
        hid_t handle = andres::hdf5::openFile(
            filename, andres::hdf5::READ_WRITE,
            andres::hdf5::HDF5_VERSION_LATEST);
        test(handle > 0);
        andres::hdf5::closeFile(handle);
    }
}

template<class T>
void saveStdVectorTest() {
    const std::string filename = "test.h5";
    std::vector<T> v(10);
    for(size_t j=0; j<v.size(); ++j) {
        v[j] = static_cast<T>(j);
    }
    hid_t handle = andres::hdf5::createFile(filename);
    andres::hdf5::save(handle, "stdvector", v);
    andres::hdf5::closeFile(handle);

    handle = andres::hdf5::openFile(filename);
    std::vector<T> w;
    andres::hdf5::load(handle, "stdvector", w);
    andres::hdf5::closeFile(handle);

    test(v.size() == w.size());
    for(size_t j=0; j<v.size(); ++j) {
        test(v[j] == w[j]);
    }
}

int main() {
    fileHandlingTest();

    saveStdVectorTest<int>();
    saveStdVectorTest<float>();

    return 0;
}

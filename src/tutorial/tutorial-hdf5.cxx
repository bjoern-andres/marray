#include <iostream>

#include "andres/marray-hdf5.hxx"

int main() {
    // how to save a View or Marray, and how to load a Marray
    {
        size_t shape[] = {3, 4, 2};
        andres::Marray<int> i(shape, shape + 3, 0, andres::FirstMajorOrder);

        // to an HDF5 dataset in the file
        {
            hid_t file = andres::hdf5::createFile("test.h5");

            andres::hdf5::save(file, "dataset", i);

            andres::Marray<int> j;
            andres::hdf5::load(file, "dataset", j);

            andres::hdf5::closeFile(file);
        }

        // ...and now to an HDF5 dataset within an HDF5 group of the file
        {
            hid_t file = andres::hdf5::createFile("test.h5");
            hid_t group = andres::hdf5::createGroup(file, "group");

            andres::hdf5::save(group, "dataset", i);

            andres::Marray<int> j;
            andres::hdf5::load(group, "dataset", j);

            andres::hdf5::closeGroup(group);
            andres::hdf5::closeFile(file);
        }
    }

    // how to create an HDF5 dataset (without saving a View or Marray)
    {
        size_t shape[2] = {5, 6};

        hid_t file = andres::hdf5::createFile("test.h5");
        andres::hdf5::create<int>(file, "creation", &shape[0], &shape[2]);
        andres::hdf5::closeFile(file);
    }

    // how to save a View or Marray to an HDF5 hyperslab (the analogon of View)
    // and how to load a Marray from an HDF5 hyperslab
    {
        size_t shape1[2] = {5, 6};
        andres::Marray<int> m1(&shape1[0], &shape1[2], 0, andres::FirstMajorOrder);

        size_t base2[2] = {1, 2};
        size_t shape2[2] = {2, 3};
        andres::Marray<int> m2(&shape2[0], &shape2[2], 1, andres::FirstMajorOrder);

        // simulate writing to hyperslab in memory
        andres::Marray<int> m3 = m1;
        andres::View<int> v;
        m3.view(base2, shape2, v);
        v = 1;

        // write hyperslab
        hid_t file = andres::hdf5::createFile("test.h5");
        andres::hdf5::save(file, "array", m1);
        andres::hdf5::saveHyperslab(file, "array", &base2[0], &base2[2], shape2, m2);

        // load entire array
        andres::Marray<int> m4;
        andres::hdf5::load(file, "array", m4);

        std::cout << m4.asString();

        // load hyperslab
        andres::Marray<int> m5;
        andres::hdf5::loadHyperslab(file, "array", &base2[0], &base2[2], shape2, m5);
        andres::hdf5::closeFile(file);

        std::cout << m5.asString();
    }

    return 0;
}

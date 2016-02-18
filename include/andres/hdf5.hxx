// http://www.andres.sc
//
#pragma once
#ifndef ANDRES_HDF5_HXX
#define ANDRES_HDF5_HXX

#include <cassert>
#include <string>
#include <vector>
#include <stdexcept>

#include "hdf5.h"

namespace andres {

/// HDF5 import/export support.
namespace hdf5 {

enum FileAccessMode {READ_ONLY, READ_WRITE};
enum HDF5Version {HDF5_VERSION_DEFAULT, HDF5_VERSION_LATEST};

template<bool B> class HandleCheck;
template<> class HandleCheck<false> {
public:
    HandleCheck()
        { counter_ = H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL); }
    void check()
        { assert(counter_ == H5Fget_obj_count(H5F_OBJ_ALL, H5F_OBJ_ALL)); }
private:
    ssize_t counter_;
};
template<> class HandleCheck<true> {
public:
    void check() {}
};

template<class T>
inline hid_t uintTypeHelper() {
   switch(sizeof(T)) {
       case 1:
           return H5T_STD_U8LE;
       case 2:
           return H5T_STD_U16LE;
       case 4:
           return H5T_STD_U32LE;
       case 8:
           return H5T_STD_U64LE;
       default:
           throw std::runtime_error("No matching HDF5 type.");
   }
}

template<class T>
inline hid_t intTypeHelper() {
   switch(sizeof(T)) {
       case 1:
           return H5T_STD_I8LE;
       case 2:
           return H5T_STD_I16LE;
       case 4:
           return H5T_STD_I32LE;
       case 8:
           return H5T_STD_I64LE;
       default:
           throw std::runtime_error("No matching HDF5 type.");
   }
}

template<class T>
inline hid_t floatingTypeHelper() {
   switch(sizeof(T)) {
       case 4:
           return H5T_IEEE_F32LE;
       case 8:
           return H5T_IEEE_F64LE;
       default:
           throw std::runtime_error("No matching HDF5 type.");
   }
}

template<class T>
    inline hid_t hdf5Type();
template<> inline hid_t hdf5Type<unsigned char>()
    { return uintTypeHelper<unsigned char>(); }
template<> inline hid_t hdf5Type<unsigned short>()
    { return uintTypeHelper<unsigned short>(); }
template<> inline hid_t hdf5Type<unsigned int>()
    { return uintTypeHelper<unsigned int>(); }
template<> inline hid_t hdf5Type<unsigned long>()
    { return uintTypeHelper<unsigned long>(); }
template<> inline hid_t hdf5Type<unsigned long long>()
    { return uintTypeHelper<unsigned long long>(); }
template<> inline hid_t hdf5Type<signed char>()
    { return intTypeHelper<signed char>(); }
template<> inline hid_t hdf5Type<char>()
    { return uintTypeHelper<char>(); }
template<> inline hid_t hdf5Type<short>()
    { return intTypeHelper<short>(); }
template<> inline hid_t hdf5Type<int>()
    { return intTypeHelper<int>(); }
template<> inline hid_t hdf5Type<long>()
    { return intTypeHelper<long>(); }
template<> inline hid_t hdf5Type<long long>()
    { return intTypeHelper<long long>(); }
template<> inline hid_t hdf5Type<float>()
    { return floatingTypeHelper<float>(); }
template<> inline hid_t hdf5Type<double>()
    { return floatingTypeHelper<double>(); }

/// Create an HDF5 file.
///
/// \param filename Name of the file.
/// \param hdf5version HDF5 version tag.
///
/// \returns HDF5 handle
///
/// \sa openFile(), closeFile()
///
inline hid_t
createFile(
    const std::string& filename,
    HDF5Version hdf5version = HDF5_VERSION_DEFAULT
) {
    hid_t version = H5P_DEFAULT;
    if(hdf5version == HDF5_VERSION_LATEST) {
        version = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_libver_bounds(version, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);
    }

    hid_t fileHandle = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, version);
    if(fileHandle < 0) {
        throw std::runtime_error("Could not create HDF5 file: " + filename);
    }

    return fileHandle;
}

/// Open an HDF5 file.
///
/// \param filename Name of the file.
/// \param fileAccessMode File access mode.
/// \param hdf5version HDF5 version tag.
///
/// \returns HDF5 handle
///
/// \sa closeFile(), createFile()
///
inline hid_t
openFile(
    const std::string& filename,
    FileAccessMode fileAccessMode = READ_ONLY,
    HDF5Version hdf5version = HDF5_VERSION_DEFAULT
) {
    hid_t access = H5F_ACC_RDONLY;
    if(fileAccessMode == READ_WRITE) {
        access = H5F_ACC_RDWR;
    }

    hid_t version = H5P_DEFAULT;
    if(hdf5version == HDF5_VERSION_LATEST) {
        version = H5Pcreate(H5P_FILE_ACCESS);
        H5Pset_libver_bounds(version, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);
    }

    hid_t fileHandle = H5Fopen(filename.c_str(), access, version);
    if(fileHandle < 0) {
        throw std::runtime_error("Could not open HDF5 file: " + filename);
    }

    return fileHandle;
}

/// Close an HDF5 file
///
/// \param handle Handle to the HDF5 file.
///
/// \sa openFile(), createFile()
///
inline void closeFile(
    const hid_t& handle
) {
    H5Fclose(handle);
}

/// Create an HDF5 group.
///
/// \param parentHandle HDF5 handle on the parent group or file.
/// \param groupName Name of the group.
/// \returns HDF5 handle on the created group
///
/// \sa openGroup(), closeGroup()
///
inline hid_t
createGroup(
    const hid_t& parentHandle,
    const std::string& groupName
) {
    hid_t groupHandle = H5Gcreate(parentHandle, groupName.c_str(),
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(groupHandle < 0) {
        throw std::runtime_error("Could not create HDF5 group.");
    }
    return groupHandle;
}

/// Open an HDF5 group.
///
/// \param parentHandle HDF5 handle on the parent group or file.
/// \param groupName Name of the group.
/// \returns HDF5 handle on the opened group.
///
/// \sa createGroup(), closeGroup()
///
inline hid_t
openGroup(
    const hid_t& parentHandle,
    const std::string& groupName
) {
    hid_t groupHandle = H5Gopen(parentHandle, groupName.c_str(), H5P_DEFAULT);
    if(groupHandle < 0) {
        throw std::runtime_error("Could not open HDF5 group.");
    }
    return groupHandle;
}

/// Close an HDF5 group.
///
/// \param handle HDF5 handle on group to close.
///
/// \sa openGroup(), createGroup()
///
inline void
closeGroup(
    const hid_t& handle
) {
    H5Gclose(handle);
}

/// Save an std::vector to an HDF5 dataset.
///
template<class T>
inline void
save(
    const hid_t parentHandle,
    const std::string datasetName,
    const std::vector<T>& data
) {
    hsize_t shape[] = {data.size()};
    hid_t dataspace = H5Screate_simple(1, shape, NULL);
    if(dataspace < 0) {
        throw std::runtime_error("could not create HDF5 dataspace.");
    }
    hid_t dataset = H5Dcreate(parentHandle, datasetName.c_str(), hdf5Type<T>(), dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(dataset < 0) {
        H5Sclose(dataspace);
        throw std::runtime_error("could not create HDF5 dataset.");
    }
    hid_t status = H5Dwrite(dataset, hdf5Type<T>(), H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());
    H5Dclose(dataset);
    H5Sclose(dataspace);
    if(status < 0) {
        throw std::runtime_error("could not write to HDF5 dataset.");
    }
}

/// Load an std::vector from an HDF5 dataset.
///
template<class T>
inline void
load(
    const hid_t parentHandle,
    const std::string datasetName,
    std::vector<T>& data,
    HDF5Version hdf5Version = HDF5_VERSION_DEFAULT
) {
    // open dataset and get types
    hid_t dataset = H5Dopen(parentHandle, datasetName.c_str(), H5P_DEFAULT);
    if(dataset < 0) {
        throw std::runtime_error("could not open HDF5 dataset.");
    }
    hid_t typeFile = H5Dget_type(dataset);

    // get dimension and shape
    hid_t filespace = H5Dget_space(dataset);
    int dimension = H5Sget_simple_extent_ndims(filespace);
    if(dimension != 1) {
        throw std::runtime_error("HDF5 dataset is not one-dimensional.");
    }
    hsize_t size = 0;
    herr_t status = H5Sget_simple_extent_dims(filespace, &size, NULL);
    if(status < 0) {
        H5Dclose(dataset);
        H5Tclose(typeFile);
        H5Sclose(filespace);
        throw std::runtime_error("could not get shape of HDF5 dataset.");
    }

    // read
    data.resize(size);
    status = H5Dread(dataset, hdf5Type<T>(), H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());

    // close dataset and types
    H5Dclose(dataset);
    H5Tclose(typeFile);
    H5Sclose(filespace);
    if(status < 0) {
        throw std::runtime_error("could not read from HDF5 dataset 'points'.");
    }
}

/// Open an HDF5 file (read only), load an std::vector from an HDF5 dataset, and close the file.
///
/// \param filename Name of the file.
/// \param datasetName Name of the HDF5 dataset.
/// \param out std::vector.
/// \param hdf5version HDF5 version tag.
///
/// \sa load()
///
template<class T>
inline void
load(
    const std::string& filename,
    const std::string& datasetName,
    std::vector<T>& out,
    HDF5Version hdf5version
) {
    hid_t file = openFile(filename, READ_ONLY, hdf5version);
    load(file, datasetName, out);
    closeFile(file);
}

} // namespace hdf5
} // namespace andres

#endif // #ifndef ANDRES_HDF5_HXX

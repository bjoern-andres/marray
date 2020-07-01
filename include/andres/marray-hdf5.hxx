// http://www.andres.sc/marray.html
//
#pragma once
#ifndef ANDRES_MARRAY_HDF5_HXX
#define ANDRES_MARRAY_HDF5_HXX

#include "marray.hxx"
#include "hdf5.hxx"

namespace andres {
namespace hdf5 {

std::string const errorMessageLastMajorOrder = R"(
    HDF5 up to at least version 1.8.16 does not support LastMajorOrder.
    (Quote from the HDF5 1.8.16 manual: \"HDF5 uses C storage conventions,
    assuming that the last listed dimension is the fastest-changing dimension
    and the first-listed dimension is the slowest changing.
    The indexing order of the Marray or View at hand is LastMajorOrder.
    In order to avoid confusion, such Marrays and Views are not saved directly to HDF5.
    Consider copying to an Marray in FirstMajorOrder or a one-dimensional Marray)";

template<class T>
    void save(const hid_t&, const std::string&, const Marray<T>&);
template<class T, bool isConst>
    void save(const hid_t&, const std::string&, const View<T, isConst>&);
template<class T, class BaseIterator, class ShapeIterator>
    void saveHyperslab(const hid_t&, const std::string&,
        BaseIterator, BaseIterator, ShapeIterator, const Marray<T>&);
template<class T, class ShapeIterator>
    void create(const hid_t&, const std::string&, ShapeIterator, ShapeIterator);

template<class T>
   void load(const hid_t&, const std::string&, Marray<T>&);
template<class T>
    void load(const std::string&, const std::string&, Marray<T>&, HDF5Version = HDF5_VERSION_DEFAULT);

template<class T>
    void loadShape(const hid_t&, const std::string&, std::vector<T>&);
template<class T, class BaseIterator, class ShapeIterator>
    void loadHyperslab(const hid_t&, const std::string&,
        BaseIterator, BaseIterator, ShapeIterator, Marray<T>&);

/// Create and close an HDF5 dataset to store Marray data.
///
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param begin Iterator to the beginning of a sequence that determines the shape of the dataset.
/// \param end Iterator to the end of a sequence that determines the shape of the dataset.
///
/// \sa save(), saveHyperslab()
///
template<class T, class ShapeIterator>
void create(
    const hid_t& groupHandle,
    const std::string& datasetName,
    ShapeIterator begin,
    ShapeIterator end
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);
    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    // build dataspace
    hid_t datatype = H5Tcopy(hdf5Type<T>());
    std::size_t dimension = std::distance(begin, end);
    std::vector<hsize_t> shape(dimension);
    for(std::size_t j=0; j<dimension; ++j) {
        shape[j] = hsize_t(*begin);
        ++begin;
    }
    hid_t dataspace = H5Screate_simple(dimension, &shape[0], NULL);
    if(dataspace < 0) {
        H5Tclose(datatype);
        throw std::runtime_error("Marray cannot create dataspace.");
    }

    // create new dataset
    hid_t dataset = H5Dcreate(groupHandle, datasetName.c_str(), datatype,
        dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(dataset < 0) {
        H5Sclose(dataspace);
        H5Tclose(datatype);
        throw std::runtime_error("Marray cannot create dataset.");
    }

    // clean up
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Tclose(datatype);
    handleCheck.check();
}

/// Save an Marray as an HDF5 dataset.
///
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param in Marray.
///
/// \sa saveHyperslab()
///
template<class T>
void save(
    const hid_t& groupHandle,
    const std::string& datasetName,
    const Marray<T>& in
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);

    if(in.coordinateOrder() == LastMajorOrder) {
        throw std::runtime_error(errorMessageLastMajorOrder);
    }

    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    // build dataspace
    hid_t datatype = H5Tcopy(hdf5Type<T>());
    std::vector<hsize_t> shape(in.dimension());
    for(std::size_t j=0; j<in.dimension(); ++j) {
        shape[j] = hsize_t(in.shape(j));
    }
    hid_t dataspace = H5Screate_simple(in.dimension(), &shape[0], NULL);
    if(dataspace < 0) {
        H5Tclose(datatype);
        throw std::runtime_error("Marray cannot create dataspace.");
    }

    // create new dataset
    hid_t dataset = H5Dcreate(groupHandle, datasetName.c_str(), datatype,
        dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(dataset < 0) {
        H5Sclose(dataspace);
        H5Tclose(datatype);
        throw std::runtime_error("Marray cannot create dataset.");
    }

    // write
    herr_t status = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &in(0));
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Tclose(datatype);
    if(status < 0) {
        throw std::runtime_error("Marray cannot write to dataset.");
    }

    handleCheck.check();
}

/// Save a View as an HDF5 dataset.
/// 
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param in View.
///
/// \sa saveHyperslab()
///
template<class T, bool isConst>
inline void save(
    const hid_t& groupHandle,
    const std::string& datasetName,
    const View<T, isConst>& in
) {
    Marray<T> m = in;
    save(groupHandle, datasetName, m);
}

/// Open an HDF5 file (read only), load an Marray from an HDF5 dataset, and close the file.
///
/// \param filename Name of the file.
/// \param datasetName Name of the HDF5 dataset.
/// \param out Marray.
/// \param hdf5version HDF5 version tag.
///
/// This function sets the out.coordinateOrder() to FirstMajorOrder!
///
/// \sa load(), loadHyperslab()
///
/// TODO: write a unit test for this function
template<class T>
inline void 
load(
    const std::string& filename,
    const std::string& datasetName,
    Marray<T>& out,
    HDF5Version hdf5version
) {
    hid_t file = openFile(filename, READ_ONLY, hdf5version);
    load(file, datasetName, out);
    closeFile(file);
}

/// Load an Marray from an HDF5 dataset.
///
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param out Marray.
///
/// This function sets the out.coordinateOrder() to FirstMajorOrder!
///
/// \sa loadHyperslab()
///
template<class T>
void load(
    const hid_t& groupHandle,
    const std::string& datasetName,
    Marray<T>& out
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);

    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    hid_t dataset = H5Dopen(groupHandle, datasetName.c_str(), H5P_DEFAULT);
    if(dataset < 0) {
        throw std::runtime_error("Marray cannot open dataset.");
    }
    hid_t filespace = H5Dget_space(dataset);
    hid_t type = H5Dget_type(dataset);
    hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DESCEND);
    if(!H5Tequal(nativeType, hdf5Type<T>())) {
        H5Dclose(dataset);
        H5Tclose(nativeType);
        H5Tclose(type);
        H5Sclose(filespace);
        throw std::runtime_error("Data types not equal error.");
    }
    int dimension = H5Sget_simple_extent_ndims(filespace);
    std::vector<hsize_t> shape(dimension);
    herr_t status = H5Sget_simple_extent_dims(filespace, &shape[0], NULL);
    if(status < 0) {
        H5Dclose(dataset);
        H5Tclose(nativeType);
        H5Tclose(type);
        H5Sclose(filespace);
        throw std::runtime_error("H5Sget_simple_extent_dims error.");
    }
    hid_t memspace = H5Screate_simple(dimension, &shape[0], NULL);

    // resize marray
    std::vector<std::size_t> newShape((std::size_t)(dimension));
    for(std::size_t j=0; j<newShape.size(); ++j) {
        newShape[j] = (std::size_t)(shape[j]);
    }
    out = Marray<T>(SkipInitialization, newShape.begin(), newShape.end(), FirstMajorOrder);

    // read
    status = H5Dread(dataset, nativeType, memspace, filespace, H5P_DEFAULT, &out(0));
    H5Dclose(dataset);
    H5Tclose(nativeType);
    H5Tclose(type);
    H5Sclose(memspace);
    H5Sclose(filespace);
    if(status < 0) {
        throw std::runtime_error("Marray cannot read from dataset.");
    }

    handleCheck.check();
}

/// Load the shape of an HDF5 dataset.
///
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param out Shape.
///
/// \sa load()
///
template<class T>
void loadShape(
   const hid_t& groupHandle,
   const std::string& datasetName,
   std::vector<T>& out
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);
    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    // load shape from HDF5 file
    hid_t dataset = H5Dopen(groupHandle, datasetName.c_str(), H5P_DEFAULT);
    if(dataset < 0) {
        throw std::runtime_error("Marray cannot open dataset.");
    }
    hid_t filespace = H5Dget_space(dataset);
    hsize_t dimension = H5Sget_simple_extent_ndims(filespace);
    hsize_t* shape = new hsize_t[(std::size_t)(dimension)];
    herr_t status = H5Sget_simple_extent_dims(filespace, shape, NULL);
    if(status < 0) {
        H5Dclose(dataset);
        H5Sclose(filespace);
        delete[] shape;
        throw std::runtime_error("Marray cannot get extension of dataset.");
    }

    // write shape to out
    out = std::vector<T>(dimension);
    for(std::size_t j=0; j<out.size(); ++j) {
        out[j] = T(shape[j]);
    }

    // clean up
    delete[] shape;
    H5Dclose(dataset);
    H5Sclose(filespace);
    handleCheck.check();
}

/// Load a hyperslab from an HDF5 dataset into an Marray.
/// 
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param baseBegin Iterator to the beginning of the sequence that determines the first coordinate of the hyperslab.
/// \param baseEnd Iterator to the end of the sequence that determines the first coordinate of the hyperslab.
/// \param shapeBegin Iterator to the beginning of the sequence that determines the shape of the hyperslab.
/// \param out Marray.
///
/// This function sets the out.coordinateOrder() to FirstMajorOrder!
///
/// \sa saveHyperslab(), create()
///
template<class T, class BaseIterator, class ShapeIterator>
void loadHyperslab(
    const hid_t& groupHandle,
    const std::string& datasetName,
    BaseIterator baseBegin,
    BaseIterator baseEnd,
    ShapeIterator shapeBegin,
    Marray<T>& out
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);
    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    // open dataset
    hid_t dataset = H5Dopen(groupHandle, datasetName.c_str(), H5P_DEFAULT);
    if(dataset < 0) {
        throw std::runtime_error("Marray cannot open dataset.");
    }

    // determine shape of hyperslab and array
    std::size_t size = std::distance(baseBegin, baseEnd);
    std::vector<hsize_t> offset(size);
    std::vector<hsize_t> slabShape(size);
    std::vector<hsize_t> marrayShape(size);
    for(std::size_t j=0; j<size; ++j) {
        offset[j] = hsize_t(*baseBegin);
        slabShape[j] = hsize_t(*shapeBegin);
        marrayShape[j] = slabShape[j];
        ++baseBegin;
        ++shapeBegin;
    }
    
    // select dataspace hyperslab
    hid_t datatype = H5Dget_type(dataset);
    
    if(!H5Tequal(datatype, hdf5Type<T>())) {
        throw std::runtime_error("data type of stored hdf5 dataset and passed array do not match in loadHyperslab");
    }
    
    hid_t dataspace = H5Dget_space(dataset);
    herr_t status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, &offset[0], NULL, &slabShape[0], NULL);
    if(status < 0) {
        H5Tclose(datatype);
        H5Sclose(dataspace);
        H5Dclose(dataset);
        throw std::runtime_error("Marray cannot select hyperslab. Check offset and shape!");
    }

    // select memspace hyperslab
    hid_t memspace = H5Screate_simple(int(size), &marrayShape[0], NULL);
    std::vector<hsize_t> offsetOut(size, 0); // no offset
    status = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, &offsetOut[0], NULL, &marrayShape[0], NULL);
    if(status < 0) {
        H5Sclose(memspace); 
        H5Tclose(datatype);
        H5Sclose(dataspace);
        H5Dclose(dataset);
        throw std::runtime_error("Marray cannot select hyperslab. Check offset and shape!");
    }

    // read from dataspace into memspace
    out = Marray<T>(SkipInitialization, &marrayShape[0], (&marrayShape[0])+size, FirstMajorOrder);
    status = H5Dread(dataset, datatype, memspace, dataspace, H5P_DEFAULT, &(out(0)));

    // clean up
    H5Sclose(memspace); 
    H5Tclose(datatype);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    if(status < 0) {
        throw std::runtime_error("Marray cannot read from dataset.");
    }
    handleCheck.check();
}

/// Save an Marray as a hyperslab into an HDF5 dataset.
/// 
/// \param groupHandle Handle of the parent HDF5 file or group.
/// \param datasetName Name of the HDF5 dataset.
/// \param baseBegin Iterator to the beginning of the sequence that determines the first coordinate of the hyperslab.
/// \param baseEnd Iterator to the end of the sequence that determines the first coordinate of the hyperslab.
/// \param shapeBegin Iterator to the beginning of the sequence that determines the shape of the hyperslab.
/// \param in Marray.
///
/// \sa loadHyperslab(), create()
///
template<class T, class BaseIterator, class ShapeIterator>
void 
saveHyperslab(
    const hid_t& groupHandle,
    const std::string& datasetName,
    BaseIterator baseBegin,
    BaseIterator baseEnd,
    ShapeIterator shapeBegin,
    const Marray<T>& in
) {
    marray_detail::Assert(MARRAY_NO_ARG_TEST || groupHandle >= 0);

    if(in.coordinateOrder() == LastMajorOrder) {
        throw std::runtime_error(errorMessageLastMajorOrder);
    }

    HandleCheck<MARRAY_NO_DEBUG> handleCheck;

    // open dataset
    hid_t dataset = H5Dopen(groupHandle, datasetName.c_str(), H5P_DEFAULT);
    if(dataset < 0) {
        throw std::runtime_error("Marray cannot open dataset.");
    }

    // determine hyperslab shape
    std::vector<hsize_t> memoryShape(in.dimension());
    for(std::size_t j=0; j<in.dimension(); ++j) {
        memoryShape[j] = in.shape(j);
    }
    std::size_t size = std::distance(baseBegin, baseEnd);
    std::vector<hsize_t> offset(size);
    std::vector<hsize_t> slabShape(size);
    for(std::size_t j=0; j<size; ++j) {
        offset[j] = hsize_t(*baseBegin);
        slabShape[j] = hsize_t(*shapeBegin);
        ++baseBegin;
        ++shapeBegin;
    }

    // select dataspace hyperslab
    hid_t datatype = H5Dget_type(dataset);
    hid_t dataspace = H5Dget_space(dataset);
    herr_t status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, &offset[0], NULL, &slabShape[0], NULL);
    if(status < 0) {
        H5Tclose(datatype);
        H5Sclose(dataspace);
        H5Dclose(dataset);
        throw std::runtime_error("Marray cannot select hyperslab. Check offset and shape!");
    }

    // select memspace hyperslab
    hid_t memspace = H5Screate_simple(int(in.dimension()), &memoryShape[0], NULL);
    std::vector<hsize_t> memoryOffset(int(in.dimension()), 0); // no offset
    status = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, &memoryOffset[0], NULL, &memoryShape[0], NULL);
    if(status < 0) {
        H5Sclose(memspace); 
        H5Tclose(datatype);
        H5Sclose(dataspace);
        H5Dclose(dataset);
        throw std::runtime_error("Marray cannot select hyperslab. Check offset and shape!");
    }

    // write from memspace to dataspace
    status = H5Dwrite(dataset, datatype, memspace, dataspace, H5P_DEFAULT, &(in(0)));

    // clean up
    H5Sclose(memspace); 
    H5Tclose(datatype);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    if(status < 0) {
        throw std::runtime_error("Marray cannot write to dataset.");
    }
    handleCheck.check();
}

} // namespace hdf5
} // namespace andres

#endif

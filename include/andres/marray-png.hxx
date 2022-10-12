#pragma once
#ifndef MARRAY_PNG_HXX
#define MARRAY_PNG_HXX

#include <stdexcept>
#include <string>
#include <fstream>

#include <png.h>

#include "marray.hxx"

namespace andres {
namespace png {

template<class T>
   void load(std::string const &, Marray<T>&);
template<class T>
    void save(std::string const &, Marray<T> const &);

/// Load a PNG file as a Marray
///
/// \param fileName Name of the file.
/// \param out Marray.
///
/// out.coordinateOrder() defines the ordering of the data in memory. Iff
/// out.coordinateOrder() == FirstMajorOrder, the data is first read and then
/// re-ordered in memory, as data in PNG files is stored in last major order.
///
template<>
void
load(
    std::string const & fileName,
    Marray<unsigned char>& out
) {
    if(out.coordinateOrder() == FirstMajorOrder) {
        Marray<unsigned char> tmp(0, LastMajorOrder);
        load(fileName, tmp); // recurse!
        // note: out = tmp; would change out.coordinateOrder() which we do not want. thus:
        out.resize(tmp.shapeBegin(), tmp.shapeEnd());
        static_cast<andres::View<unsigned char, false> >(out) = tmp; // re-order in memory
        assert(out.coordinateOrder() == FirstMajorOrder);
        return;
    }

    assert(out.coordinateOrder() == LastMajorOrder);

    FILE *file = std::fopen(fileName.c_str(), "rb");

    png_structp png = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr
    );
    if(!png) {
        std::fclose(file);
        throw std::runtime_error("error during png_create_read_struct");
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
        std::fclose(file);
        throw std::runtime_error("error during png_create_info_struct");
    }

    if(setjmp(png_jmpbuf(png))) {
        std::fclose(file);
        throw std::runtime_error("error during png_create_info_struct");
    }

    png_init_io(png, file);
    png_read_info(png, info);

    png_uint_32 const width = png_get_image_width(png, info);
    png_uint_32 const height = png_get_image_height(png, info);
    png_byte const color_type = png_get_color_type(png, info);
    png_byte const bit_depth = png_get_bit_depth(png, info);

    if(color_type != PNG_COLOR_TYPE_GRAY) {
        throw std::runtime_error("not implemented for PNG color type other than PNG_COLOR_TYPE_GRAY.");
    }

    if(bit_depth > 8) {
        throw std::runtime_error("not implemented for PNG bit depth >8.");
    }
    else if(bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    png_read_update_info(png, info);

    out.resize({width, height});

    std::vector<unsigned char*> rowPointers(height);
    for(size_t y = 0; y < height; ++y) {
        rowPointers[y] = &out(0, y);
    }

    png_read_image(png, rowPointers.data());

    std::fclose(file);
    png_destroy_read_struct(&png, &info, nullptr);
}

/// Save an Marray as a PNG file
///
/// \param fileName Name of the file to be created.
/// \param in Marray to be saved.
///
/// in.shape(0) becomes the width of the image.
/// in.shape(1) becomes the width of the image.
/// If in.coordinateOrder() == FirstMajorOrder, the data is re-ordered
/// temporarily in memory before writing it to a PNG file, as PNG files are in
/// last major order.
///
template<>
void
save(
    std::string const & fileName,
    Marray<unsigned char> const & in
) {
    if(in.dimension() != 2) {
        throw std::runtime_error("not implemented for Marray of dimension other than 2.");
    }
    if(in.shape(0) > std::numeric_limits<png_uint_32>::max()
    || in.shape(1) > std::numeric_limits<png_uint_32>::max()) {
        throw std::runtime_error("marray is too large for export to PNG");
    }
    if(in.coordinateOrder() == FirstMajorOrder) {
        Marray<unsigned char> tmp(in.shapeBegin(), in.shapeEnd(), LastMajorOrder);
        // note: tmp = in; would change in.coordinateOrder() which we do not want. thus:
        static_cast<andres::View<unsigned char, false> >(tmp) = in; // re-order in memory
        assert(tmp.coordinateOrder() == LastMajorOrder);
        save(fileName, tmp); // recurse!
        return;
    }

    assert(in.coordinateOrder() == LastMajorOrder);

    FILE *file = std::fopen(fileName.c_str(), "wb");
    if(!file) {
        throw std::runtime_error("could not open PNG file for writing");
    }
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(file);
        throw std::runtime_error("png_create_write_struct failed");
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(file);
        throw std::runtime_error("png_create_info_struct failed");
    }
    if(setjmp(png_jmpbuf(png_ptr))) {
        fclose(file);
        throw std::runtime_error("error during init_io");
    }
    png_init_io(png_ptr, file);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(file);
        throw std::runtime_error("error during writing PNG header");
    }

    png_uint_32 const width = static_cast<png_uint_32>(in.shape(0)); // bound has been checked before
    png_uint_32 const height = static_cast<png_uint_32>(in.shape(1)); // bound has been checked before
    std::vector<png_bytep> rowPointers(height);
    for(size_t y = 0; y < height; ++y) {
        rowPointers[y] = &in(0, y);
    }
    png_byte const color_type = PNG_COLOR_TYPE_GRAY;
    png_byte const bit_depth = 8;
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        bit_depth,
        color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE,
        PNG_FILTER_TYPE_BASE
    );
    png_write_info(png_ptr, info_ptr);
    // write bytes
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(file);
        throw std::runtime_error("Error during writing bytes");
    }
    png_write_image(png_ptr, rowPointers.data());
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(file);
        throw std::runtime_error("Error during end of write");
    }
    png_write_end(png_ptr, nullptr);
    fclose(file);
}

} // namespace png
} // namespace andres

#endif

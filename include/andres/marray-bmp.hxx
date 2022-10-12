// TODO: implement loading

#pragma once
#ifndef MARRAY_BMP_HXX
#define MARRAY_BMP_HXX

#include <fstream>

#include "marray.hxx"

namespace andres {
namespace bmp {

std::uint32_t const FILE_HEADER_SIZE = 14;
std::uint32_t const INFO_HEADER_SIZE = 40;
std::uint32_t const COLOR_TABLE_SIZE = 256 * 4; // RGBA32

template<class T>
   void load(std::string const &, Marray<T>&);
template<class T>
    void save(std::string const &, Marray<T> const &);
template<class T>
    void save(std::ostream &, Marray<T> const &);

std::uint8_t*
fileHeader(
    std::uint32_t const height,
    std::uint32_t const stride
) {
    std::uint32_t const fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_TABLE_SIZE + (stride * height);
    std::uint32_t const offset = FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_TABLE_SIZE;

    static std::uint8_t fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved, should be 0
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = static_cast<std::uint8_t>('B');
    fileHeader[ 1] = static_cast<std::uint8_t>('M');
    fileHeader[ 2] = static_cast<std::uint8_t>(fileSize      );
    fileHeader[ 3] = static_cast<std::uint8_t>(fileSize >>  8);
    fileHeader[ 4] = static_cast<std::uint8_t>(fileSize >> 16);
    fileHeader[ 5] = static_cast<std::uint8_t>(fileSize >> 24);
    fileHeader[10] = static_cast<std::uint8_t>(offset);
    fileHeader[11] = static_cast<std::uint8_t>(offset >> 8);
    fileHeader[12] = static_cast<std::uint8_t>(offset >> 16);
    fileHeader[13] = static_cast<std::uint8_t>(offset >> 24);

    return fileHeader;
}

std::uint8_t*
infoHeader(
    std::uint32_t const width,
    std::uint32_t const height,
    std::uint16_t const bitsPerPixel
) {
    static std::uint8_t infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table, 0 for 0^bits per pixel
        0,0,0,0, /// important color count, unsused and should be 0
    };

    infoHeader[ 0] = static_cast<std::uint8_t>(INFO_HEADER_SIZE);
    infoHeader[ 4] = static_cast<std::uint8_t>(width      );
    infoHeader[ 5] = static_cast<std::uint8_t>(width >>  8);
    infoHeader[ 6] = static_cast<std::uint8_t>(width >> 16);
    infoHeader[ 7] = static_cast<std::uint8_t>(width >> 24);
    infoHeader[ 8] = static_cast<std::uint8_t>(height      );
    infoHeader[ 9] = static_cast<std::uint8_t>(height >>  8);
    infoHeader[10] = static_cast<std::uint8_t>(height >> 16);
    infoHeader[11] = static_cast<std::uint8_t>(height >> 24);
    infoHeader[12] = static_cast<std::uint8_t>(1);
    infoHeader[14] = static_cast<std::uint8_t>(bitsPerPixel);
    infoHeader[15] = static_cast<std::uint8_t>(bitsPerPixel >> 8);

    return infoHeader;
}

template<>
void
save(
    std::ostream & stream,
    Marray<std::uint8_t> const & in
) {
    if(in.dimension() != 2) {
        throw std::runtime_error("not implemented for Marray of dimension other than 2.");
    }
    if(in.shape(0) > std::numeric_limits<std::uint32_t>::max()
    || in.shape(1) > std::numeric_limits<std::uint32_t>::max()) {
        throw std::runtime_error("marray is too large for export to BMP");
    }
    if(in.coordinateOrder() == FirstMajorOrder) {
        Marray<unsigned char> tmp(in.shapeBegin(), in.shapeEnd(), LastMajorOrder);
        // note: tmp = in; would change in.coordinateOrder() which we do not want. thus:
        static_cast<andres::View<unsigned char, false> >(tmp) = in; // re-order in memory
        assert(tmp.coordinateOrder() == LastMajorOrder);
        save(stream, tmp); // recurse!
        return;
    }

    assert(in.coordinateOrder() == LastMajorOrder);

    std::uint32_t const BYTES_PER_PIXEL = 1;
    std::uint32_t const width = static_cast<std::uint32_t>(in.shape(0));
    std::uint32_t const height = static_cast<std::uint32_t>(in.shape(1));
    std::uint32_t const widthInBytes = width * BYTES_PER_PIXEL;
    std::uint8_t const padding[3] = {0, 0, 0};
    std::uint32_t const paddingSize = (4 - (widthInBytes) % 4) % 4;
    std::uint32_t const stride = widthInBytes + paddingSize;

    stream.write(reinterpret_cast<char const *>(fileHeader(height, stride)), FILE_HEADER_SIZE);
    stream.write(reinterpret_cast<char const *>(infoHeader(width, height, BYTES_PER_PIXEL * 8)), INFO_HEADER_SIZE);

    // write color palette (required by BMP format for <= 8 bits per pixel)
    for(std::uint8_t color = 0; true; ++color) {
        stream << color << color << color << 0; // RGBA32
        if(color == 255) {
            break;
        }
    }

    // write image
    for(size_t j = 0; j < height; ++j) {
        stream.write(reinterpret_cast<char const *>(&in(0, height - j - 1)), width);
        stream.write(reinterpret_cast<char const *>(padding), paddingSize);
    }
}

template<>
inline void
save(
    std::string const & fileName,
    Marray<std::uint8_t> const & in
) {
    std::ofstream file;
    file.open(fileName.c_str());
    save(file, in);
    file.close();
}

} // namespace bmp
} // namespace andres

#endif

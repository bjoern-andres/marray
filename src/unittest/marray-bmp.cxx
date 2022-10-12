// TODO: implement and test loading

#include "andres/marray-bmp.hxx"

inline void test(const bool& x) {
    if(!x) throw std::logic_error("test failed.");
}

template<andres::CoordinateOrder ORDER_SAVE, andres::CoordinateOrder ORDER_LOAD>
void testSaveLoad() {
    size_t const shape[] = {2, 3};
    std::string const fileName = "test.bmp";
    {
        andres::Marray<unsigned char> m(shape, shape + 2, 0, ORDER_SAVE);
        m(0, 0) = 127;
        m(0, 2) = 255;
        m(1, 0) = 200;
        andres::bmp::save(fileName, m);
    }
    /*
    {
        andres::Marray<unsigned char> m(0, ORDER_LOAD);
        andres::png::load(fileName, m);
        test(m.coordinateOrder() == ORDER_LOAD);
        test(m.dimension() == 2);
        test(m.shape(0) == shape[0]);
        test(m.shape(1) == shape[1]);
        test(m(0, 0) == 127);
        test(m(0, 2) == 255);
        test(m(1, 0) == 200);
    }
    */
}

int main() {
    testSaveLoad<andres::LastMajorOrder, andres::LastMajorOrder>();
    testSaveLoad<andres::LastMajorOrder, andres::FirstMajorOrder>();
    testSaveLoad<andres::FirstMajorOrder, andres::LastMajorOrder>();
    testSaveLoad<andres::FirstMajorOrder, andres::FirstMajorOrder>();

    return 0;
}

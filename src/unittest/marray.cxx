// http://www.andres.sc/marray.html
//
#include <vector>
#include <iostream>

#include "andres/marray.hxx"

inline void test(const bool& x)
    { if(!x) throw std::logic_error("test failed."); }

struct TestType {
    std::size_t data_;
};

class GlobalFunctionTest {
public:
    void shapeStrideTest();
};

class ViewTest {
private:
    int scalar_;
    int data_[24];
    int data2x_[24];
    int data100_[100];

public:
    ViewTest(); 
    template<bool constTarget>
        void coordinatesToOffsetTest();
    template<bool constTarget> 
        void coordinatesToIndexTest();
    template<bool constTarget> 
        void indexToCoordinatesTest();
    template<bool constTarget> 
        void indexToOffsetTest();
    template<bool constTarget>
        void emptyConstructorTest();
    template<bool constTarget>
        void scalarConstructorTest();
    void copyConstructorTest();
    template<bool constTarget>
        void shapeConstructorTest();
    template<bool constTarget>
        void shapeAssignTest();
    template<bool constTarget>
        void shapeStrideConstructorTest();
    template<bool constTarget>
        void shapeStrideAssignTest();
    template<andres::CoordinateOrder internalFirstMajorOrder>   
        void assignmentOperatorTest();
    template<bool constTarget>
        void queryTest();
    template<bool constTarget>
        void elementAccessTest();
    template<bool constTarget>
        void subViewsTest();
    void subConstViewsTest();
    template<bool constTarget>
        void iteratorAccessTest();
    template<bool constTarget>
        void coordinateBindingTest();
    template<bool constTarget>
        void transposeTest();
    template<bool constTarget>
        void permuteTest();
    template<bool constTarget, andres::CoordinateOrder internalFirstMajorOrder>
        void shiftOperatorTest();
    void arithmeticOperatorsTest();
    template<bool constTarget>
        void asStringTest();
    void reshapeTest();
    void overlapTreatmentTest();
    void compatibilityFunctionsTest();
};

class IteratorTest {
private:
    int data_[24];

public:
    IteratorTest(); 
    template<bool constTarget>
        void constructorTest();
    template<bool constTarget>
        void accessIteratorOperations();
    template<bool constTarget>
        void arithmeticOperatorsTest(); 
    template<bool constTarget>
        void comparisonOperators();
    template<bool constTarget>
        void hasMoreTest();
    template<bool constTarget>
        void indexTest();
    template<bool constTarget>
        void coordinateTest();
};

class MarrayTest {
private:
    int scalar_;
    int data_[24];
    int data2x_[24];
    int data100_[100];

public:
    MarrayTest(); 
    void constructorTest();
  void assignTest();
    template<bool constTarget>
        void copyConstructorTest();
    template<bool constTarget>
        void assignmentOperatorTest();
    void reshapeTest();
    template<andres::CoordinateOrder coordinateOrder>
        void resizeTest();
};

class ExpressionTemplateTest
{
private:
    int dataInt_[24];

public:
    ExpressionTemplateTest();
    void constructionAndAssignmentTest();
    void arithmeticOperatorsTest();
};

class DifferingTypesTest {
private:
    int dataInt_[24];
    short dataShort_[24];

public:
    DifferingTypesTest();
    void constructionTest();
    void assignmentTest();
    void arithmeticOperatorsTest();
    void nonBasicTypesTest();
    void typePromotionTest();
};

// implementation

void GlobalFunctionTest::shapeStrideTest() {
    // firstMajorOrder == false
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 7;
        shape[1] = 3;
        shape[2] = 5;
        std::vector<std::size_t> strides(3);
        andres::marray_detail::stridesFromShape(shape.begin(), shape.end(),
            strides.begin(), andres::LastMajorOrder);

        test(strides[0] == 1 &&
               strides[1] == shape[0] &&
               strides[2] == shape[0]*shape[1]);
        test(shape[0] == 7 && shape[1] == 3 && shape[2] == 5);
    }
    // firstMajorOrder == true
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 7;
        shape[1] = 3;
        shape[2] = 5;
        std::vector<std::size_t> strides(3);
        andres::marray_detail::stridesFromShape(shape.begin(), shape.end(),
            strides.begin(), andres::FirstMajorOrder);

        test(strides[0] == shape[1]*shape[2] &&
               strides[1] == shape[2] &&
               strides[2] == 1);
        test(shape[0] == 7 && shape[1] == 3 && shape[2] == 5);
    }
}

ViewTest::ViewTest() : scalar_(42) {
    for(int j=0; j<24; ++j) {
        data_[j] = j;
        data2x_[j] = j*2;
    }
    for(int j=0; j<100; ++j) {
        data100_[j] = j;
    }
}

template<bool constTarget> 
void ViewTest::coordinatesToOffsetTest() {
    // externalFirstMajorOrder == false
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder);
        
        std::vector<std::size_t> c(3);
        std::size_t trueOffset = 0;
        for(c[2]=0; c[2]<shape[2]; ++c[2]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[0]=0; c[0]<shape[0]; ++c[0]) {
            std::size_t offset = 100;
            v.coordinatesToOffset(c.begin(), offset);
            test(offset == trueOffset);
            ++trueOffset;
        }
    }
    // externalFirstMajorOrder == true
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::FirstMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t trueOffset = 0;
        for(c[0]=0; c[0]<shape[0]; ++c[0])
        for(c[1]=0; c[1]<shape[1]; ++c[1])

        for(c[2]=0; c[2]<shape[2]; ++c[2]) { 
            std::size_t offset = 100;
            v.coordinatesToOffset(c.begin(), offset);
            test(offset == trueOffset);
            ++trueOffset;
        }
    }
    // test also with non-trivial strides and non-zero offset
    // externalFirstMajorOrder == false
    {
        std::size_t list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64, 
            65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        std::vector<std::size_t> strides(3);
        strides[0] = 2;
        strides[1] = 10;
        strides[2] = 35;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
            data100_+30, andres::LastMajorOrder);
        
        std::vector<std::size_t> c(3);
        std::size_t index = 0;
        for(c[2]=0; c[2]<shape[2]; ++c[2]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[0]=0; c[0]<shape[0]; ++c[0]) {
            std::size_t offset = 0;
            v.coordinatesToOffset(c.begin(), offset);
            test(offset == list[index]-30);
            ++index;
        }
    }
    // externalFirstMajorOrder == true
    {
        std::size_t list[] = {30, 65, 40, 75, 50, 85, 60, 95, 32, 67, 42, 77, 
            52, 87, 62, 97, 34, 69, 44, 79, 54, 89, 64, 99};
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        std::vector<std::size_t> strides(3);
        strides[0] = 2;
        strides[1] = 10;
        strides[2] = 35;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
            data100_+30, andres::FirstMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t index = 0;
        for(c[0]=0; c[0]<shape[0]; ++c[0])
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[2]=0; c[2]<shape[2]; ++c[2]) { 
            std::size_t offset = 100;
            v.coordinatesToOffset(c.begin(), offset);
            test(offset == list[index]-30);
            ++index;
        }
    }
}

template<bool constTarget> 
void ViewTest::coordinatesToIndexTest() {
    // internalFirstMajorOrder == false
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::LastMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t trueIndex = 0;
        for(c[2]=0; c[2]<shape[2]; ++c[2]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[0]=0; c[0]<shape[0]; ++c[0]) {
            std::size_t index = 100;
            v.coordinatesToIndex(c.begin(), index);
            test(index == trueIndex);
            ++trueIndex;
        }
    }
    // internalFirstMajorOrder == true
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::FirstMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t trueIndex = 0;
        for(c[0]=0; c[0]<shape[0]; ++c[0]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[2]=0; c[2]<shape[2]; ++c[2]) {
            std::size_t index = 100;
            v.coordinatesToIndex(c.begin(), index);
            test(index == trueIndex);
            ++trueIndex;
        }
    }
}

template<bool constTarget> 
void ViewTest::indexToCoordinatesTest() {
    // internalFirstMajorOrder == false
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::LastMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t index = 0;
        for(c[2]=0; c[2]<shape[2]; ++c[2]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[0]=0; c[0]<shape[0]; ++c[0]) {
            std::vector<std::size_t> d(3);
            v.indexToCoordinates(index, d.begin());
            test(d[0] == c[0] && d[1] == c[1] && d[2] == c[2]);
            ++index;
        }
    }
    // internalFirstMajorOrder == true
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::FirstMajorOrder);

        std::vector<std::size_t> c(3);
        std::size_t index = 0;
        for(c[0]=0; c[0]<shape[0]; ++c[0]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[2]=0; c[2]<shape[2]; ++c[2]) {
            std::vector<std::size_t> d(3);
            v.indexToCoordinates(index, d.begin());
            test(d[0] == c[0] && d[1] == c[1] && d[2] == c[2]);
            ++index;
        }
    }
}

template<bool constTarget> 
void ViewTest::indexToOffsetTest() {
    // test cases: (externalFirstMajorOrder, internalFirstMajorOrder)
    // (false, false)
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::LastMajorOrder);

        std::size_t trueOffset = 0;
        for(std::size_t index=0; index<v.size(); ++index) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == trueOffset);
            ++trueOffset;
        }
    }
    // (false, true)
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::FirstMajorOrder);
        andres::View<int> w(shape.begin(), shape.end(),
            data_, andres::LastMajorOrder, andres::LastMajorOrder);
    
        std::vector<std::size_t> c(3);
        std::size_t index = 0;
        for(c[0]=0; c[0]<shape[0]; ++c[0]) 
        for(c[1]=0; c[1]<shape[1]; ++c[1])
        for(c[2]=0; c[2]<shape[2]; ++c[2]) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == static_cast<std::size_t>(w(c[0], c[1], c[2])));
            ++index;
        }
    
    }
    // (true, false)
    {
        std::size_t list[] = {0, 8, 16, 4, 12, 20, 1, 9, 17, 5, 13, 21, 
            2, 10, 18, 6, 14, 22, 3, 11, 19, 7, 15, 23}; 
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::FirstMajorOrder, andres::LastMajorOrder);

        for(std::size_t index=0; index<v.size(); ++index) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == list[index]);
        }
    }
    
    // (true, true)
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 2;
        shape[2] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(),
            data_, andres::FirstMajorOrder, andres::FirstMajorOrder);

        std::size_t trueOffset = 0;
        for(std::size_t index=0; index<v.size(); ++index) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == trueOffset);
            ++trueOffset;
        }
    }

    // test also with non-trivial strides and non-zero offset
    // (false)
    {
        std::size_t list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64, 
            65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99}; 
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        std::vector<std::size_t> strides(3);
        strides[0] = 2;
        strides[1] = 10;
        strides[2] = 35;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
            data100_+30, andres::LastMajorOrder);
        
        for(std::size_t index=0; index<v.size(); ++index) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == list[index]-30);
        }
    }
    // (true)
    {
        std::size_t list[] = {30, 65, 40, 75, 50, 85, 60, 95, 32, 67, 42, 77, 
            52, 87, 62, 97, 34, 69, 44, 79, 54, 89, 64, 99};
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        std::vector<std::size_t> strides(3);
        strides[0] = 2;
        strides[1] = 10;
        strides[2] = 35;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
            data100_+30, andres::FirstMajorOrder);
        
        for(std::size_t index=0; index<v.size(); ++index) {
            std::size_t offset = 0;
            v.indexToOffset(index, offset);
            test(offset == list[index]-30);
        }
    }
}

template<bool constTarget>
void ViewTest::emptyConstructorTest() {
    andres::View<int, constTarget> v;   
    
    test(v.size() == 0
        );
}

template<bool constTarget>
void ViewTest::scalarConstructorTest() {
    andres::View<int, constTarget> v(&scalar_); 
    
    test(v.size() == 1 &&
           v.dimension() == 0);
}

void ViewTest::copyConstructorTest() {
    // scalar 
    {
        // false, false
        {
            andres::View<int, false> v(&scalar_);
            andres::View<int, false> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0)));
        }
        // false, true
        {
            andres::View<int, false> v(&scalar_);
            andres::View<int, true> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0)));
        }
        // true, true
        {
            andres::View<int, true> v(&scalar_);
            andres::View<int, true> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0)));
        }
    }
    // 1D 
    {
        // false, false
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, false> v(shape.begin(), shape.end(), data_);
            andres::View<int, false> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
        }
        // false, true
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, false> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
        }
        // true, true
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
        }
    }
    // 2D 
    {
        // false, false
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int> v(shape.begin(), shape.end(), data_);
            andres::View<int> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(w(j, k) == v(j, k));
                }
            }
        }
        // false, true
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);

            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(w(j, k) == v(j, k));
                }
            }
        }
        // true, true
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);
            
            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(w(j, k) == v(j, k));
                }
            }
        }
    }
    // 3D 
    {
        // false, false
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, false> v(shape.begin(), shape.end(), data_);
            andres::View<int, false> w(v);
            
            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
        // false, true
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, false> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);
            
            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
        // true, true
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            andres::View<int, true> w(v);
            
            test(w.dimension() == v.dimension() &&
                   w.size() == v.size() &&
                   &(w(0)) == &(v(0))
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(w.shape(j) == v.shape(j));
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(w(j) == v(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::shapeConstructorTest() {
    // 1D
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        
        test(v.dimension() == shape.size() &&
               v.size() == 24
        );
        for(unsigned short j=0; j<shape.size(); ++j) {
            test(v.shape(j) == shape[j]);
        }
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == data_[j]);
        }
    }
    // 2D
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6;
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        
        test(v.dimension() == shape.size() &&
               v.size() == 24
        );
        for(unsigned short j=0; j<shape.size(); ++j) {
            test(v.shape(j) == shape[j]);
        }
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == data_[j]);
        }
        for(std::size_t j=0; j<v.shape(0); ++j) {
            for(std::size_t k=0; k<v.shape(1); ++k) {
                test(v(j, k) == data_[j+k*shape[0]]);
            }
        }
    }
    // 3D
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 2;
        shape[1] = 4;
        shape[2] = 3;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        
        test(v.dimension() == shape.size() &&
               v.size() == 24
        );
        for(unsigned short j=0; j<shape.size(); ++j) {
            test(v.shape(j) == shape[j]);
        }
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == data_[j]);
        }
        for(std::size_t j=0; j<v.shape(0); ++j) {
            for(std::size_t k=0; k<v.shape(1); ++k) {
                for(std::size_t p=0; p<v.shape(2); ++p) {
                    test(v(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1] ]);
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::shapeAssignTest() {
    // scalar... 
    {
        // to scalar
        {
            andres::View<int, constTarget> v(&scalar_);
            std::vector<std::size_t> shape(0);
            v.assign(shape.begin(), shape.end(), data_);

            test(v.dimension() == shape.size() &&
                   v.size() == 1
            );
        }
        // to 1D
        {
            andres::View<int, constTarget> v(&scalar_);
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            v.assign(shape.begin(), shape.end(), data_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data_[j]);
            }
        }
        // to 2D
        {
            andres::View<int, constTarget> v(&scalar_);
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            v.assign(shape.begin(), shape.end(), data_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == data_[j+k*shape[0]]);
                }
            }
        }
        // to 3D
        {
            andres::View<int, constTarget> v(&scalar_);
            std::vector<std::size_t> shape(3);
            shape[0] = 2;
            shape[1] = 4;
            shape[2] = 3;
            v.assign(shape.begin(), shape.end(), data2x_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == data2x_[ j + k*shape[0] + p*shape[0]*shape[1] ]);
                    }
                }
            }
        }
    }
    // 1D... 
    {
        // to scalar 
        { 
            std::vector<std::size_t> shape(1), shape0(0);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.assign(shape0.begin(), shape0.end(), data_);

            test(v.dimension() == shape0.size() &&
               v.size() == 1
            );
        }
        // to 1D
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.assign(shape.begin(), shape.end(), data2x_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
        }
        // to 2D
        {
            std::vector<std::size_t> shape1D(1), shape2D(2);
            shape1D[0] = 24;
            shape2D[0] = 6;
            shape2D[1] = 4;
            andres::View<int, constTarget> v(shape1D.begin(), shape1D.end(), data_);
            v.assign(shape2D.begin(), shape2D.end(), data2x_);

            test(v.dimension() == shape2D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape2D.size(); ++j) {
                test(v.shape(j) == shape2D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == data2x_[j+k*shape2D[0]]);
                }
            }
        }
        // to 3D
        {
            std::vector<std::size_t> shape1D(1), shape3D(3);
            shape1D[0] = 24;
            shape3D[0] = 2;
            shape3D[1] = 4;
            shape3D[2] = 3;
            andres::View<int, constTarget> v(shape1D.begin(), shape1D.end(), data_);
            v.assign(shape3D.begin(), shape3D.end(), data2x_);

            test(v.dimension() == shape3D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape3D.size(); ++j) {
                test(v.shape(j) == shape3D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == data2x_[ j + k*shape3D[0] + p*shape3D[0]*shape3D[1] ]);
                    }
                }
            }
        }
    }
    // 2D... 
    {
        // to scalar
        {
            std::vector<std::size_t> shape2D(2), shape0(0);
            shape2D[0] = 6;
            shape2D[1] = 4; 
            andres::View<int, constTarget> v(shape2D.begin(), shape2D.end(), data_);
            v.assign(shape0.begin(), shape0.end(), data_);

            test(v.dimension() == shape0.size() &&
                   v.size() == 1
            );
        }
        // to 1D
        {
            std::vector<std::size_t> shape2D(2), shape1D(1);
            shape2D[0] = 6;
            shape2D[1] = 4; 
            shape1D[0] = 24;
            andres::View<int, constTarget> v(shape2D.begin(), shape2D.end(), data_);
            v.assign(shape1D.begin(), shape1D.end(), data2x_);

            test(v.dimension() == shape1D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape1D.size(); ++j) {
                test(v.shape(j) == shape1D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
        }
        // to 2D
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.assign(shape.begin(), shape.end(), data2x_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == data2x_[j+k*shape[0]]);
                }
            }
        }
        // to 2D with different shape
        {
            std::vector<std::size_t> shape(2), shapeBefore(2);
            shapeBefore[0] = 4;
            shapeBefore[1] = 6;
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, constTarget> v(shapeBefore.begin(), shapeBefore.end(), data_);
            v.assign(shape.begin(), shape.end(), data2x_);

            test(v.dimension() == shape.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == data2x_[j+k*shape[0]]);
                }
            }
        }
        // to 3D
        {
            std::vector<std::size_t> shape2D(2), shape3D(3);
            shape2D[0] = 6;
            shape2D[1] = 4;
            andres::View<int, constTarget> v(shape2D.begin(), shape2D.end(), data_);
            shape3D[0] = 2;
            shape3D[1] = 4;
            shape3D[2] = 3;
            v.assign(shape3D.begin(), shape3D.end(), data_);

            test(v.dimension() == shape3D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape3D.size(); ++j) {
                test(v.shape(j) == shape3D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == data_[ j + k*shape3D[0] + p*shape3D[0]*shape3D[1] ]);
                    }
                }
            }
        }
    }
    // 3D...  
    {
        // to scalar
        {
            std::vector<std::size_t> shape3D(3), shape0(0);
            shape3D[0] = 3;
            shape3D[1] = 4;
            shape3D[2] = 2;
            andres::View<int, constTarget> v(shape3D.begin(), shape3D.end(), data_);
            v.assign(shape0.begin(), shape0.end(), data_);

            test(v.dimension() == shape0.size() &&
                   v.size() == 1
            );
        }
        // to 1D
        {
            std::vector<std::size_t> shape3D(3), shape1D(1);
            shape3D[0] = 3;
            shape3D[1] = 4;
            shape3D[2] = 2;
            shape1D[0] = 24; 
            andres::View<int, constTarget> v(shape3D.begin(), shape3D.end(), data_);
            v.assign(shape1D.begin(), shape1D.end(), data2x_);

            test(v.dimension() == shape1D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape1D.size(); ++j) {
                test(v.shape(j) == shape1D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
        }
        // to 2D
        {
            std::vector<std::size_t> shape3D(3), shape2D(2);
            shape3D[0] = 2;
            shape3D[1] = 4;
            shape3D[2] = 3;
            andres::View<int, constTarget> v(shape3D.begin(), shape3D.end(), data_);
            shape2D[0] = 6;
            shape2D[1] = 4;
            v.assign(shape2D.begin(), shape2D.end(), data_);
            
            test(v.dimension() == shape2D.size() &&
                   v.size() == 24
            );
            for(unsigned short j=0; j<shape2D.size(); ++j) {
                test(v.shape(j) == shape2D[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == data_[j+k*shape2D[0]]);
                }
            }
        }
        // to 3D
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 2;
            shape[1] = 4;
            shape[2] = 3;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.assign(shape.begin(), shape.end(), data2x_);

            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == data2x_[ j + k*shape[0] + p*shape[0]*shape[1] ]);
                    }
                }
            }
        }
        // to 3D with different shape
        {
            std::vector<std::size_t> shape(3), shapeBefore(3);
            shapeBefore[0] = 3;
            shapeBefore[1] = 2;
            shapeBefore[2] = 4;
            shape[0] = 2;
            shape[1] = 4;
            shape[2] = 3;
            andres::View<int, constTarget> v(shapeBefore.begin(), shapeBefore.end(), data_);
            v.assign(shape.begin(), shape.end(), data2x_);

            for(unsigned short j=0; j<shape.size(); ++j) {
                test(v.shape(j) == shape[j]);
            }
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data2x_[j]);
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == data2x_[ j + k*shape[0] + p*shape[0]*shape[1] ]);
                    }
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::shapeStrideConstructorTest() {
    // 1D... 
    {
        // without offset
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(),
                strides.begin(), data_, andres::LastMajorOrder);

            int i = 0; 
            for(int x = 0; x < 6; ++x){
                test(v(x) == data_[i]);
                i+=4;
            }
            test(v.dimension() == shape.size() && 
                   v.size() == 6);
        }
        // with offset
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(),
                strides.begin(), data_+2, andres::LastMajorOrder);

            int i = 2; 
            for(int x = 0; x < 6; ++x){
                test(v(x) == data_[i]);
                i+=4;
            }
            test(v.dimension() == shape.size() &&
                   v.size() == 6);
        }
    }
    // 2D...
    {
        // without offset
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 3;
            shape[1] = 2;
            std::vector<std::size_t> strides(2);
            strides[0] = 1;
            strides[1] = 3;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(), data_, andres::LastMajorOrder);

            int i = 0; 
            for(int y = 0; y < 2; ++y){
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                }
            }
            test(v.dimension() == shape.size() &&
                   v.size() == 6);
        }
        // with offset
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 3;
            shape[1] = 2;
            std::vector<std::size_t> strides(2);
            strides[0] = 1;
            strides[1] = 3;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                data100_+50, andres::LastMajorOrder);

            int i = 50; 
            for(int y = 0; y < 2; ++y){
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data100_[i]);
                    ++i;
                }
            }
            test(v.dimension() == shape.size() &&
                   v.size() == 6);
        }
    }
    // 3D...
    {
        // without offset
        {        
            int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            std::vector<std::size_t> strides(3);
            strides[0] = 2;
            strides[1] = 10;
            strides[2] = 35;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                data100_, andres::LastMajorOrder);

            test(v.dimension() == shape.size() && v.size() == 24
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 4; ++y)
            for(int x = 0; x < 3; ++x){
                test(v(x, y, z) == list[index]);
                ++index;
            }
        }
        // with offset
        {       
            int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            std::vector<std::size_t> strides(3);
            strides[0] = 2;
            strides[1] = 10;
            strides[2] = 35;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                data100_+30, andres::LastMajorOrder);

            test(v.dimension() == shape.size() && v.size() == 24
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 4; ++y)
            for(int x = 0; x < 3; ++x){
                test(v(x, y, z) == list[index]);
                ++index;
            }
        }
    }
}

template<bool constTarget>
void ViewTest::shapeStrideAssignTest()
{
    // scalar... 
    {
        // to scalar...
        {
            // without offset
            {
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(0);
                std::vector<std::size_t> strides(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 1 &&
                       v(0) == 0
                );
            }
            // to with offset 
            {
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(0);
                std::vector<std::size_t> strides(1);
                strides[0] = 2;

                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_+10, andres::LastMajorOrder);
                test(v.dimension() == shape.size() &&
                       v.size() == 1 &&
                       v(0) == 10
                );
            }
        }
        // to 1D...
        {
            // without offset
            {
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 2;

                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);
                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            // with offset 
            {
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
        }
        // to 2D...
        {
            // without offset
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            // with offset 
            {
                int list[] = {50, 52, 54, 54, 56, 58};
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+50, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
        }
        // to 3D...
        {
            // without offset
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // with offset
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                andres::View<int, constTarget> v(&scalar_);
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);

                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35; 
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
        }
    }
    // 1D...
    {
        // to scalar
        {
            std::vector<std::size_t> shape(1), shape0(0);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 1;
            andres::View<int, constTarget> v(shape.begin(), shape.end(),
                strides.begin(), data_, andres::LastMajorOrder);
            v.assign(shape0.begin(), shape0.end(), strides.begin(),
                  data_, andres::LastMajorOrder);

            test(v.dimension() == shape0.size() &&
                     v.size() == 1
            );
        }
        // to 1D...
        {
            // without offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 1;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            // with offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 1;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data_+5, andres::LastMajorOrder);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(), data_+1,
                    andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            {
                int list[] = {1, 3, 5, 7, 9, 11};
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 1;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                for(int x = 0; x < 6; ++x)
                    test(v(x) == list[x]);
            }
            {
                int list[] = {0, 2, 4, 6, 8, 10};
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 1;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data_+5, andres::LastMajorOrder);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                for(int x = 0; x < 6; ++x)
                    test(v(x) == list[x]);
            }
        }
        // to 2D...
        {
            // without offset and resize
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            // with offset and resize
            {
                int list[] = {50, 52, 54, 54, 56, 58};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {50, 52, 54, 54, 56, 58};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
        }
        // to 3D...
        {
            // without offset and resize
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // with offset
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_+1, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                std::vector<std::size_t> strides(1);
                strides[0] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_+2, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
        }
    }
    // 2D...
    {
        // to 1D...
        {
            // without offset abd resize
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4; 
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            // with offset and resize
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4; 
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_+5, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            {
                int list[] = {1, 3, 5, 7, 9, 11};
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4; 
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6;
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                for(int x = 0; x < 6; ++x)
                    test(v(x) == list[x]);
            }
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4; 
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_+5, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
        }
        // to 2D...
        {
            // without offset
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            // without offset and different shapes
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(2);
                shape[0] = 2;
                shape[1] = 3;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 2;
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            // with offset
            {
                int list[] = {50, 52, 54, 54, 56, 58}; 
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+1, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {50, 52, 54, 54, 56, 58}; 
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+1, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            // with offset and different shapes
            {
                int list[] = {50, 52, 54, 54, 56, 58};
                std::vector<std::size_t> shape(2);
                shape[0] = 2;
                shape[1] = 3;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+1, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 2;
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {50, 52, 54, 54, 56, 58};
                std::vector<std::size_t> shape(2);
                shape[0] = 2;
                shape[1] = 3;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 2;
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+50, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 4, 6, 8};
                std::vector<std::size_t> shape(2);
                shape[0] = 2;
                shape[1] = 3;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+1, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 2;
                strides[0] = 2;
                strides[1] = 4;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == list[index]);
                    ++index;
                }
            }
        }
        // to 3D...
        {
            // without offset and resize
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // with offset and resize
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+2, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+2, andres::LastMajorOrder);
                shape.resize(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides.resize(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
        }
    }
    // 3D...
    {
        // to 1D...
        {
            // without offset abd resize
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3; 
                strides[2] = 8; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            // with offset and resize
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3; 
                strides[2] = 8;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_+5, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3; 
                strides[2] = 8;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6;
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3; 
                strides[2] = 8;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data100_+5, andres::LastMajorOrder);
                shape.resize(1);
                shape[0] = 6; 
                strides.resize(1);
                strides[0] = 2;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == data_[i]);
                    i+=2;
                }
            }
        }
        // to 2D...
        {
            // without offset and resize
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 4;
                strides[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 1; 
                strides[1] = 3; 
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                    }
                }
            }
            // with offset and resize
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 4;
                strides[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data100_+10, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 1; 
                strides[1] = 3; 
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                    }
                }
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 4;
                strides[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data100_, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 1; 
                strides[1] = 3; 
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_+1, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 1; 
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                    }
                }
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 4;
                strides[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data100_+10, andres::LastMajorOrder);
                shape.resize(2);
                shape[0] = 3;
                shape[1] = 2;
                strides.resize(2);
                strides[0] = 1; 
                strides[1] = 3; 
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 6
                );
                int i = 0; 
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                    }
                }
            }
        }
        // to 3D...
        {
            // without offset
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // without offset and different shapes
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // with offset
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+1, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_+2, andres::LastMajorOrder);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            // with offset and different shapes
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_+1, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {30, 32, 34, 40, 42, 44, 50, 52, 54, 60, 62, 64,
                    65, 67, 69, 75, 77, 79, 85, 87, 89, 95, 97, 99};
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);

                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
            {
                int list[] = {0, 2, 4, 10, 12, 14, 20, 22, 24, 30, 32, 34,
                    35, 37, 39, 45, 47, 49, 55, 57, 59, 65, 67, 69};
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3;
                shape[2] = 4;
                std::vector<std::size_t> strides(3);
                strides[0] = 1;
                strides[1] = 3;
                strides[2] = 7;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), 
                    strides.begin(), data_+2, andres::LastMajorOrder);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                v.assign(shape.begin(), shape.end(), strides.begin(), 
                    data100_, andres::LastMajorOrder);
                
                test(v.dimension() == shape.size() &&
                       v.size() == 24
                );
                int index = 0; 
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z) == list[index]);
                    ++index;
                }
            }
        }
    }
}

template<andres::CoordinateOrder internalFirstMajorOrder>
void ViewTest::assignmentOperatorTest()
{

    // 'to' is mutable (isConst == false) and initialized (data_ != 0) 
    // 'from' is mutable (isConst == false)
    {
        // 'to' and 'from' are scalar
        {
            int scalarv_ = 33;
            int scalarw_ = 54;
            andres::View<int, false> v(&scalarv_), w(&scalarw_);
            v.operator=(w);

            test(v(0)==w(0));
        }
        //'to' and 'from' are 1D
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
        }
        // to == isSimple, w != isSimple
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
            andres::View<int, false> w(shape.begin(), shape.end(), strides.begin(), dataw, andres::FirstMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
                }
        }
        // to != isSimple, w == isSimple 
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), strides.begin(), datav, andres::FirstMajorOrder);
            andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
                }
        }
        // to != isSimple, w != isSimple 
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 6;
            std::vector<std::size_t> strides(1);
            strides[0] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), strides.begin(), datav, andres::LastMajorOrder);
            andres::View<int, false> w(shape.begin(), shape.end(), strides.begin(), dataw, andres::FirstMajorOrder);
            v.operator=(w);
            
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
                }
        }
        // 'to' and 'from' are 2D
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == w(j, k));
                }
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == w(j, k));
                }
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(v(j, k) == w(j, k));
                }
            }
        }
        // 'to' and 'from' are 3D
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
        {
            int datav[24];
            int dataw[24];
            for(int j=0; j<24; ++j) {
                datav[j] = j;
                dataw[j] = j*2;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder),
                                     w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);

            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(v(j, k, p) == w(j, k, p));
                    }
                }
            }
        }
    }
    // 'to' is mutable (isConst == false) and un-initialized (data_ == 0) 
    // 'from' is mutable (isConst == false)
    {
        // 'from' is 1D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, false> v, w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
        }
        // 'from' is 2D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, false> v, w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(&v(j, k) == &w(j, k));
                }
            }
        }
        // 'from' is 3D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, false> v, w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(&v(j, k, p) == &w(j, k, p));
                    }
                }
            }
        } 
    }
    // 'to' is mutable (isConst == false)
    // 'from' is constant (isConst == true)
    {
        // 'to' 1D and...
        {
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
            // to == isSimple, w != isSimple 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), strides.begin(), dataw, andres::FirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
            // to != isSimple, w == isSimple 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), strides.begin(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
            // to != isSimple, w != isSimple 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), strides.begin(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), strides.begin(), dataw, andres::FirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
            }
        }
        // 'to' is 2D and...
        {
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(v(j, k) == w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(v(j, k) == w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(v(j, k) == w(j, k));
                    }
                }
            }
        }
        // 'to' is 3D and...
        {
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, false> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(v(j, k, p) == w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(v(j, k, p) == w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, false> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(v(j, k, p) == w(j, k, p));
                        }
                    }
                }
            } 
        }
    } 
    // 'to' is constant (isConst == true) and un-initialized (data_ == 0)
    // 'from' is mutable (isConst == false)
    {
        // 'from' is scalar
        {
            int  scalar = 33;
            andres::View<int, true> v;
            andres::View<int, false> w(&scalar);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size() &&
                   &v(0)==&w(0)
            );
        }
        // 'from' is 1D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v; 
            andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
        }
        // 'from' is 2D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, true> v; 
            andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(&v(j, k) == &w(j, k));
                }
            }
        }
        // 'from' is 3D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, true> v; 
            andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(&v(j, k, p) == &w(j, k, p));
                    }
                }
            }
        } 
    }
    // 'to' is constant (isConst == true) and un-initialized (data_ == 0)
    // 'from' is constant (isConst == true)
    {
        // 'from' is scalar
        {
            int  scalar = 33;
            andres::View<int, true> v;
            andres::View<int, true> w(&scalar);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size() &&
                   &v(0)==&w(0)
            );
        }
        // 'from' is 1D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v; 
            andres::View<int, true> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
        }
        // 'from' is 2D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::View<int, true> v; 
            andres::View<int, true> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    test(&v(j, k) == &w(j, k));
                }
            }
        }
        // 'from' is 3D
        {
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = j;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4; 
            andres::View<int, true> v; 
            andres::View<int, true> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
            v.operator=(w);

            test(v.dimension() == w.dimension() &&
                   v.size() == w.size()
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(&v(j) == &w(j));
            }
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(&v(j, k, p) == &w(j, k, p));
                    }
                }
            }
        } 
    }
    // 'to' is constant (isConst == true) and initialized (data_ != 0) 
    // 'from' is mutable (isConst == false)
    {
        // 'to' is scalar and...
        {
            // 'from' is scalar
            {
                int scalarv = 56;
                int  scalarw = 33;
                andres::View<int, true> v(&scalarv);
                andres::View<int, false> w(&scalarw);
                v.operator=(w);

                test(&v(0)==&w(0));
            }
            // 'from' is 1D
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(&scalar);
                andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            // 'from' is 2D
            {


                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(2); 
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, true> v(&scalar);
                andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3; 
                shape[2] = 4;
                andres::View<int, true> v(&scalar);
                andres::View<int, false> w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 1D and...
        {
            // 'from' is scalar
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                andres::View<int, false> w(&scalar);
                v.operator=(w);
                
                test(v.dimension() == w.dimension() &&
                       v.size() == w.size() &&
                       &v(0) == &w(0)
                );
            }
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j)
                );
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j)
                );
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == w(j)
                );
                }
            }
            // 'to' is 1D and 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 2D and...
        {
            //  'from' is scalar
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                andres::View<int, false> w(&scalar);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size() &&
                       &v(0) == &w(0));
            }
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 2D with different shapes
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4;
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4;
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4;
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 3D and...
        {
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            // 'from' is 3D with different shapes
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;
                shapew[1] = 4;
                shapew[2] = 2;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;

                shapew[1] = 4;
                shapew[2] = 2;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;
                shapew[1] = 4;
                shapew[2] = 2;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, false> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
    }
    // 'to' is constant (isConst == true) and initialized (data_ != 0) 
    // 'from' is constant (isConst == true)
    {
        // 'to' is scalar and...
        {
            //  'from' is scalar
            {
                int scalarv = 3;
                int scalarw = 76;
                andres::View<int, true> v(&scalarv), w(&scalarw);
                v.operator=(w);
                
                test(&v(0) == &w(0));
            }
            // 'from' is 1D
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(&scalar), 
                                        w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            // 'from' is 2D
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(2); 
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, true> v(&scalar), w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3; 
                shape[2] = 4;
                andres::View<int, true> v(&scalar), w(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 1D and...
        {
            // 'from' is scalar
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                andres::View<int, true> w(&scalar);
                v.operator=(w);
                
                test(v.dimension() == w.dimension() &&
                       v.size() == w.size() &&
                       &v(0) == &w(0));
            }
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);
                
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(2); 
                shapev[0] = 24;
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }

                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];

                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(1), shapew(3);
                shapev[0] = 24;
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 2D and...
        {
            // 'from' is scalar
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                andres::View<int, true> w(&scalar);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size() &&
                       &v(0) == &w(0));
            }
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(1);
                shapev[0] = 6;
                shapev[1] = 4;
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
            }
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 2D with different shapes
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4; 
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4; 
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(2);
                shapev[0] = 6; 
                shapev[1] = 4;
                shapew[0] = 4; 
                shapew[1] = 6;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'to' is 2D and 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(2), shapew(3);
                shapev[0] = 6;
                shapev[1] = 4; 
                shapew[0] = 2;
                shapew[1] = 3; 
                shapew[2] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t p=0; p<w.shape(2); ++p) {
                    for(std::size_t k=0; k<w.shape(1); ++k) {
                        for(std::size_t j=0; j<w.shape(0); ++j) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
        // 'to' is 3D and...
        {
            // 'from' is scalar
            {
                int scalar = 53;
                int data[24];
                for(int j=0; j<24; ++j) {
                    data[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 3; 
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), data, internalFirstMajorOrder);
                andres::View<int, true> w(&scalar);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size() &&
                       v(0) == w(0));
            }
            // 'from' is 1D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(1);
                shapev[0] = 2;
                shapev[1] = 3; 
                shapev[2] = 4; 
                shapew[0] = 24;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }       
            }
            // 'from' is 2D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(2);
                shapev[0] = 2;
                shapev[1] = 3;
                shapev[2] = 4; 
                shapew[0] = 6;
                shapew[1] = 4;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                test(v.dimension() == w.dimension() &&
                       v.size() == w.size()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t k=0; k<w.shape(1); ++k) {
                    for(std::size_t j=0; j<w.shape(0); ++j) {
                        test(&v(j, k) == &w(j, k));
                    }
                }
            }
            // 'from' is 3D
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            } 
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 2;
                shape[2] = 4; 
                andres::View<int, true> v(shape.begin(), shape.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            } 
            // 'from' is 3D with different shapes
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;
                shapew[1] = 4;
                shapew[2] = 3;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, internalFirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, internalFirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;
                shapew[1] = 4;
                shapew[2] = 3;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::FirstMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::LastMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
            {
                int datav[24];
                int dataw[24];
                for(int j=0; j<24; ++j) {
                    datav[j] = j;
                    dataw[j] = j*2;
                }
                std::vector<std::size_t> shapev(3), shapew(3);
                shapev[0] = 3;
                shapev[1] = 2;
                shapev[2] = 4;
                shapew[0] = 2;
                shapew[1] = 4;
                shapew[2] = 3;
                andres::View<int, true> v(shapev.begin(), shapev.end(), datav, andres::LastMajorOrder);
                andres::View<int, true> w(shapew.begin(), shapew.end(), dataw, andres::FirstMajorOrder);
                v.operator=(w);

                for(std::size_t j=0; j<v.size(); ++j) {
                    test(&v(j) == &w(j));
                }
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(&v(j, k, p) == &w(j, k, p));
                        }
                    }
                }
            }
        }
    }
    // assignment of base types
    {
        { // unstrided view 
            int data[24];
            for(int j=0; j<24; ++j) {
                data[j] = 3;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 2;
            shape[2] = 4;
            andres::View<int, false> v(shape.begin(), shape.end(), data, andres::LastMajorOrder);

            v = 4;

            for(std::size_t j=0; j<24; ++j) {
                test(data[j] == 4);
            }
        }
        { // strided view
            int data[24];
            for(std::size_t j=0; j<24; ++j) {
                data[j] = 3;
            }
            std::vector<std::size_t> shape(2);
            shape[0] = 3;
            shape[1] = 2;
            std::vector<std::size_t> strides(2);
            strides[0] = 2;
            strides[1] = 8;
            andres::View<int, false> v(shape.begin(), shape.end(), 
                strides.begin(), data, andres::LastMajorOrder);

            v = 4;

            test(data[0] == 4);
            test(data[1] == 3);
            test(data[2] == 4);
            test(data[3] == 3);
            test(data[4] == 4);
            test(data[5] == 3);
            test(data[6] == 3);
            test(data[7] == 3);
            test(data[8] == 4);
            test(data[9] == 3);
            test(data[10] == 4);
            test(data[11] == 3);
            test(data[12] == 4);
            for(std::size_t j=13; j<24; ++j) {
                test(data[j] == 3);
            }
        }
    }
}

template<bool constTarget>
void ViewTest::queryTest(){
    // scalar
    {
        andres::View<int, constTarget> v(&scalar_);
        test(v.dimension()== 0 &&
               v.size()== 1);
    }
    // 1D
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

        test(v.dimension()==1 && 
               v.size()==24 &&
               v.shape(0)==24);
    }
    // 2D 
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 4;
        shape[1] = 6; 
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

        test(v.dimension()== 2 &&
               v.size()== 24 &&
               v.shape(0)== 4 &&
               v.shape(1)== 6); 
    }
    // 3D
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

        test(v.dimension()== 3 &&
               v.size()== 24 && 
               v.shape(0)== 3 && 
               v.shape(1)== 4 &&
               v.shape(2)== 2);
    }
}

template<bool constTarget>
void ViewTest::elementAccessTest(){
    // element access by coordinate
    {
        // 2D
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> w(2);
        
            for(std::size_t x=0; x<6; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    w[0] = x;
                    w[1] = y;
                    test(v(x, y) == v(w.begin()));
                }
            }
        }
        // 3D 
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> w(3);
        
            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        w[0] = x;
                        w[1] = y;
                        w[2] = z; 
                        test(v(x, y, z) == v(w.begin()));
                    }
                }
            }
        }
    }
    // scalar
    {
        {
            andres::View<int, constTarget> v(&scalar_);
            
            test(v(0) == 42);
        }
        // const
        {
            andres::View<int, constTarget> const v(&scalar_);
            
            test(v(0) == 42);
        }
    }
    // 1D 
    {
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

            for(std::size_t j=0; j<24; ++j) {
                test(v(j) == data_[j]); 
            }
        }
        // const 
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> const v(shape.begin(), shape.end(), data_);

            for(std::size_t j=0; j<24; ++j) {
                test(v(j) == data_[j]); 
            }
        }
    }
    // 2D
    {
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 4;
            shape[1] = 6;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

            int i = 0;
            for(int y = 0; y < 6; ++y){
                for(int x = 0; x < 4; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                }
            }
        }
        // const 
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 4;
            shape[1] = 6;
            andres::View<int, constTarget> const v(shape.begin(), shape.end(), data_);

            int i = 0;
            for(int y = 0; y < 6; ++y){
                for(int x = 0; x < 4; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                }
            }
        }
    }
    // 3D 
    {
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);

            int i = 0;
            for(int z = 0; z < 2; ++z){
                for(int y = 0; y < 4; ++y){
                    for(int x = 0; x < 3; ++x){
                        test(v(x, y, z) == data_[i]);
                        ++i;
                    }
                }
            }
        }
        // const
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> const v(shape.begin(), shape.end(), data_);

            int i = 0;
            for(int z = 0; z < 2; ++z){
                for(int y = 0; y < 4; ++y){
                    for(int x = 0; x < 3; ++x){
                        test(v(x, y, z) == data_[i]);
                        ++i;
                    }
                }
            }
        }
    }
    // 4D 
    {
        {
            std::vector<std::size_t> shape(4);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            shape[3] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data100_);

            int i = 0;
            for(int z = 0; z < 2; ++z){
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 4; ++x){
                        for(int w = 0; w < 3; ++w){
                            test(v(w, x, y, z) == data100_[i]);
                            ++i;
                        }
                    }
                }
            }
        }
        // const 
        {
            std::vector<std::size_t> shape(4);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            shape[3] = 2;
            andres::View<int, constTarget> const v(shape.begin(), shape.end(), data100_);

            int i = 0;
            for(int z = 0; z < 2; ++z){
                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 4; ++x){
                        for(int w = 0; w < 3; ++w){
                            test(v(w, x, y, z) == data100_[i]);
                            ++i;
                        }
                    }
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::subViewsTest(){
    // 1D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 1;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(),  s);

            test(s.size() == 1 && s.dimension()==1
            );
            test(s(0)==4);
        }
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 1;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(),  s);

            test(s.size() == 1 && s.dimension()==1
            );
            test(s(0)==4);

        }
        // 1D sub-view
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 3;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(),  s);

            test(s.size() == 3 && s.dimension()==1
            );
            test(s(0)==4 && s(1)==5 && s(2)==6);
        }
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 3;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(),  s);

            test(s.size() == 3 && s.dimension()==1
            );
            test(s(0)==4 && s(1)==5 && s(2)==6);
        }
    }
    // 2D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] =2;
            base[1] =2;
            shape[0] = 1; 
            shape[1] = 1;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==2
            );
            test(s(0,0)==14);
        } 
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] =2;
            base[1] =2;
            shape[0] = 1; 
            shape[1] = 1;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==2
            );
            test(s(0,0)==10);
        }     
        // 1D sub-view
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] =0;
            base[1] =0;
            shape[0] = 1; 
            shape[1] = 4;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 4 && s.dimension()==2
            );
            test(s(0,0)==0 && s(0,1)==6 && s(0,2)==12 && s(0,3)==18 );
        } 
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] =0;
            base[1] =0;
            shape[0] = 1; 
            shape[1] = 4;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 4 && s.dimension()==2
            );
            test(s(0,0)==0 && s(0,1)==1 && s(0,2)==2 && s(0,3)==3);
        } 
        // 2D sub-view
        {
            int list[] = {8, 9, 10, 14, 15, 16};
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] = 2;
            base[1] = 1;
            shape[0] = 3;
            shape[1] = 2; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==2
            );
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(s(x, y) == list[index]);
                ++index;
            }
        } 
        {
            int list[] = {9, 13, 17, 10, 14, 18};
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] = 2;
            base[1] = 1;
            shape[0] = 3;
            shape[1] = 2; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==2
            );
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(s(x, y) == list[index]);
                ++index;
            }
        } 
    }
    // 3D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 1;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==3
            );
            test(s(0,0,0)==4);
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 1;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==3
            );
            test(s(0,0,0)==10);
        }
        // 1D sub-view
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 3 && s.dimension()==3
            );
            test(s(0,0,0)==4 && s(0,1,0)==7 && s(0,2,0)==10);
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 3 && s.dimension()==3
            );
            test(s(0,0,0)==10 && s(0,1,0)==12 && s(0,2,0)==14);
        }
        // 2D sub-view
        {
            int list[] = {4, 5, 7, 8, 10, 11};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 2;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 1; ++z)
            for(int y = 0; y < 3; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        }
        {
            int list[] = {10, 18, 12, 20, 14, 22};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 2;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 1; ++z)
            for(int y = 0; y < 3; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        }
        // 3D sub-view
        {
            int list[] = {4, 5, 7, 8, 16, 17, 19, 20};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0;
            shape[0] =2;
            shape[1] =2;
            shape[2] =2;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 8 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        } 
        {
            int list[] = {10, 18, 12, 20, 11, 19, 13, 21};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0;
            shape[0] =2;
            shape[1] =2;
            shape[2] =2;
            andres::View<int, constTarget> s;
            v.view(base.begin(), shape.begin(), s);

            test(s.size() == 8 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        } 
    }
}

void ViewTest::subConstViewsTest(){
    // 1D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 1;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(),  s);

            test(s.size() == 1 && s.dimension()==1
            );
            test(s(0)==4);
        }
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 1;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(),  s);

            test(s.size() == 1 && s.dimension()==1
            );
            test(s(0)==4);

        }
        // 1D sub-view
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 3;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(),  s);

            test(s.size() == 3 && s.dimension()==1
            );
            test(s(0)==4 && s(1)==5 && s(2)==6);
        }
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(1);
            base[0] = 4;
            shape[0] = 3;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(),  s);

            test(s.size() == 3 && s.dimension()==1
            );
            test(s(0)==4 && s(1)==5 && s(2)==6);
        }
    }
    // 2D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] =2;
            base[1] =2;
            shape[0] = 1; 
            shape[1] = 1;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==2
            );
            test(s(0,0)==14);
        } 
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] =2;
            base[1] =2;
            shape[0] = 1; 
            shape[1] = 1;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==2
            );
            test(s(0,0)==10);
        }     
        // 1D sub-view
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] =0;
            base[1] =0;
            shape[0] = 1; 
            shape[1] = 4;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 4 && s.dimension()==2
            );
            test(s(0,0)==0 && s(0,1)==6 && s(0,2)==12 && s(0,3)==18 );
        } 
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] =0;
            base[1] =0;
            shape[0] = 1; 
            shape[1] = 4;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 4 && s.dimension()==2
            );
            test(s(0,0)==0 && s(0,1)==1 && s(0,2)==2 && s(0,3)==3);
        } 
        // 2D sub-view
        {
            int list[] = {8, 9, 10, 14, 15, 16};
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(2);
            base[0] = 2;
            base[1] = 1;
            shape[0] = 3;
            shape[1] = 2; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==2
            );
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(s(x, y) == list[index]);
                ++index;
            }
        } 
        {
            int list[] = {9, 13, 17, 10, 14, 18};
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(2);
            base[0] = 2;
            base[1] = 1;
            shape[0] = 3;
            shape[1] = 2; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==2
            );
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(s(x, y) == list[index]);
                ++index;
            }
        } 
    }
    // 3D with...
    {
        // scalar sub-view
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 1;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==3
            );
            test(s(0,0,0)==4);
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 1;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 1 && s.dimension()==3
            );
            test(s(0,0,0)==10);
        }
        // 1D sub-view
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 3 && s.dimension()==3
            );
            test(s(0,0,0)==4 && s(0,1,0)==7 && s(0,2,0)==10);
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 1;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 3 && s.dimension()==3
            );
            test(s(0,0,0)==10 && s(0,1,0)==12 && s(0,2,0)==14);
        }
        // 2D sub-view
        {
            int list[] = {4, 5, 7, 8, 10, 11};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 2;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 1; ++z)
            for(int y = 0; y < 3; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        }
        {
            int list[] = {10, 18, 12, 20, 14, 22};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0; 
            shape[0] = 2;
            shape[1] = 3;
            shape[2] = 1; 
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 6 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 1; ++z)
            for(int y = 0; y < 3; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        }
        // 3D sub-view
        {
            int list[] = {4, 5, 7, 8, 16, 17, 19, 20};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0;
            shape[0] =2;
            shape[1] =2;
            shape[2] =2;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 8 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        } 
        {
            int list[] = {10, 18, 12, 20, 11, 19, 13, 21};
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2; 
            andres::View<int, true> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            std::vector<std::size_t> base(3);
            base[0] = 1;
            base[1] = 1;
            base[2] = 0;
            shape[0] =2;
            shape[1] =2;
            shape[2] =2;
            andres::View<int, true> s;
            v.constView(base.begin(), shape.begin(), s);

            test(s.size() == 8 && s.dimension()==3
            );
            int index = 0; 
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 2; ++x){
                test(s(x, y, z) == list[index]);
                ++index;
            }
        } 
    }
}

template<bool constTarget>
void ViewTest::iteratorAccessTest(){
    // 1D
    {
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
        // reverse
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::reverse_iterator it;
            it = v.rbegin();
            for(int i = 23; i >= 0; --i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.rend());
            test(*(--it)==0);
        }
        // const
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::const_iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
    }
    // 2D
    {
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
        // reverse
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::reverse_iterator it;
            it = v.rbegin();
            for(int i = 23; i >= 0; --i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.rend());
            test(*(--it)==0);
        }
        // const
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6;
            shape[1] = 4;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::const_iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
    }
    // 3D
    {
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
        // reverse
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::reverse_iterator it;
            it = v.rbegin();
            for(int i = 23; i >= 0; --i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.rend());
            test(*(--it)==0);
        }
        // cosnt
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            typename andres::View<int, constTarget>::const_iterator it;
            it = v.begin();
            for(int i = 0; i < 24; ++i){
                test(*it == data_[i]);
                ++it;
            }
            test(it == v.end());
            test(*(--it)==23);
        }
    }
}

template<bool constTarget>
void ViewTest::coordinateBindingTest() {
    // 1D
    {
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w = v.boundView(0,7);
            test(w.dimension()==0 && w.size()==1);
            test(w(0)==7);
        }
    }
    // 2D with... 
    {
        // bind(0,x)
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w = v.boundView(0,3);
            test(w.dimension()==1 && w.size()==4);
            test(w(0)==3 && w(1)==9 && w(2)==15 && w(3)==21);
        }
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w = v.boundView(0,3);
            
            test(w.dimension()==1 && w.size()==4);
            test(w(0)==12 && w(1)==13 && w(2)==14 && w(3)==15);
        }
        // bind(1,x)
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w = v.boundView(1,2);
            
            test(w.dimension()==1 && w.size()==6);
            int i = 12; 
            for(int x = 0; x < 6; ++x){
                test(w(x) == data_[i]);
                ++i;
            }
        }
        {
            int list[] = {2, 6, 10, 14, 18, 22};
            std::vector<std::size_t> shape(2); 
            shape[0] = 6;
            shape[1] = 4; 
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w = v.boundView(1,2);
            
            test(w.dimension()==1 && w.size()==6);
            for(int x = 0; x < 6; ++x){
                test(w(x) == list[x]);
            }
        }
    }
    // 3D with...
    {
        // bind(0,x)
        {
            int list[] = {2, 5, 8, 11, 14, 17, 20, 23};
            std::vector<std::size_t> shape(3); 
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w = v.boundView(0,2);
        
            test(w.size()==8 && w.dimension()==2);
            int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 4; ++x){
                    test(w(x, y) == list[index]);
                    ++index;
                }
        }
        {
            int list[] = {16, 18, 20, 22, 17, 19, 21, 23};
            std::vector<std::size_t> shape(3); 
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w = v.boundView(0,2);
            
            test(w.size()==8 && w.dimension()==2);
            int index = 0; 
                for(int y = 0; y < 2; ++y)
                for(int x = 0; x < 4; ++x){
                    test(w(x, y) == list[index]);
                    ++index;
                }
        }
        // bind(1,x)
        {
            int list[] = {6, 7, 8, 18, 19, 20};
            std::vector<std::size_t> shape(3); 
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w = v.boundView(1,2);
            
            test(w.size()==6 && w.dimension()==2);
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(w(x, y) == list[index]);
                ++index;
            }
        }
        {
            int list[] = {4, 12, 20, 5, 13, 21};
            std::vector<std::size_t> shape(3); 
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w = v.boundView(1,2);

            test(w.size()==6 && w.dimension()==2);
            int index = 0; 
            for(int y = 0; y < 2; ++y)
            for(int x = 0; x < 3; ++x){
                test(w(x, y) == list[index]);
                ++index;
            }
        }
    }
    // squeeze with...
    {
        // 2D to scalar 
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 1;
            shape[1] = 1;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();
            
            test(v.size()==1 && v.dimension()==0);
            test(v(0)==0);
        }
        // 2D to 1D
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 24;
            shape[1] = 1;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();

            test(v.size()== 24 && v.dimension()==1);
            for(int i = 0; i < 24; ++i)
                test(v(i)==data_[i]);
        }
        // 3D to scalar 
        {
            std::vector<std::size_t> shape(3); 
            shape[0] = 1;
            shape[1] = 1;
            shape[2] = 1;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();

            test(v.size()==1 && v.dimension()==0);
            test(v(0)==0);
        } 
        // 3D to 1D with squeeze
        {
            std::vector<std::size_t> shape(3); 
            shape[0] = 1;
            shape[1] = 24;
            shape[2] = 1;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();

            test(v.size()==24 && v.dimension()==1
            );
            for(int i = 0; i < 24; ++i)
                test(v(i)==data_[i]);
        } 
        // 3D to 2D
        {
            std::vector<std::size_t> shape(3); 
            shape[0] = 1;
            shape[1] = 3;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();

            test(v.size()==6 && v.dimension()==2
            );
            int i = 0; 
            for(int y = 0; y < 2; ++y){
                for(int x = 0; x < 3; ++x){
                    test(v(x, y) == data_[i]);
                    ++i;
                }
            }
        }
        // 6D to 3D
        {
            std::vector<std::size_t> shape(6);
            shape[0] = 1;
            shape[1] = 2;
            shape[2] = 1;
            shape[3] = 1;
            shape[4] = 3;
            shape[5] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            v.squeeze();

            test(v.size()==12 && v.dimension()==3
            );
            int i = 0; 
            for(int z = 0; z < 2; ++z){
                for(int y = 0; y < 3; ++y){
                    for(int x = 0; x < 2; ++x){
                        test(v(x, y, z) == data_[i]);
                        ++i;
                    }
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::transposeTest()
{
    // 2D
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_);
        w1.transpose(0,1);
        w2.transpose(0,0);
        w3.transpose(1,1);

        for(std::size_t x=0; x<6; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                test(v(x, y) == w1(y, x));
                test(v(x, y) == w2(x, y));
                test(v(x, y) == w3(x, y));
            }
        }
    }
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        w1.transpose(0,1);
        w2.transpose(0,0);
        w3.transpose(1,1);

        for(std::size_t x=0; x<6; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                test(v(x, y) == w1(y, x));
                test(v(x, y) == w2(x, y));
                test(v(x, y) == w3(x, y));
            }
        }
    }
    // 3D with...
    {
        // shape[0], shape[1]
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_);
            w.transpose(0,1);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        test(v(x, y, z) == w(y, x, z));
                    }
                }
            }
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            w.transpose(0,1);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        test(v(x, y, z) == w(y, x, z));
                    }
                }
            }
        }
        // shape[0], shape[2]
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_);
            w.transpose(0,2);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {

                        test(v(x, y, z) == w(z, y, x));
                    }
                }
            }
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            w.transpose(0,2);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        test(v(x, y, z) == w(z, y, x));
                    }
                }
            }
        }
        // shape[1], shape[2]
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_);
            w.transpose(1,2);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        test(v(x, y, z) == w(x, z, y));
                    }
                }
            }
        }
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4;
            shape[2] = 2;
            andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            andres::View<int, constTarget> w(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
            w.transpose(1,2);

            for(std::size_t x=0; x<3; ++x) {
                for(std::size_t y=0; y<4; ++y) {
                    for(std::size_t z=0; z<2; ++z) {
                        test(v(x, y, z) == w(x, z, y));
                    }
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::permuteTest() {
    // 2D
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_);
        std::vector<std::size_t> permutation(2);
        permutation[0] = 1;
        permutation[1] = 0;
        w1.permute(permutation.begin());
        permutation[0] = 0;
        permutation[1] = 1;
        w2.permute(permutation.begin());

        for(std::size_t x=0; x<6; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                test(v(x, y) == w1(y, x));
                test(v(x, y) == w2(x, y));
            }
        }
    }
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        std::vector<std::size_t> permutation(2);
        permutation[0] = 1;
        permutation[1] = 0;
        w1.permute(permutation.begin());
        permutation[0] = 0;
        permutation[1] = 1;
        w2.permute(permutation.begin());

        for(std::size_t x=0; x<6; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                test(v(x, y) == w1(y, x));
                test(v(x, y) == w2(x, y));
            }
        }
    }
    // 3D 
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w4(shape.begin(), shape.end(), data_);
        andres::View<int, constTarget> w5(shape.begin(), shape.end(), data_);
        std::vector<std::size_t> permutation(3);
        permutation[0] = 0;
        permutation[1] = 1; 
        permutation[2] = 2;
        w1.permute(permutation.begin());
        permutation[0] = 1;
        permutation[1] = 0; 
        permutation[2] = 2;
        w2.permute(permutation.begin());
        permutation[0] = 0;
        permutation[1] = 2; 
        permutation[2] = 1;
        w3.permute(permutation.begin());
        permutation[0] = 2;
        permutation[1] = 1; 
        permutation[2] = 0;
        w4.permute(permutation.begin());
        permutation[0] = 1;
        permutation[1] = 2; 
        permutation[2] = 0;
        w5.permute(permutation.begin());

        for(std::size_t x=0; x<3; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                for(std::size_t z=0; z<2; ++z) {
                    test(v(x, y, z) == w1(x, y, z));
                    test(v(x, y, z) == w2(y, x, z));
                    test(v(x, y, z) == w3(x, z, y));
                    test(v(x, y, z) == w4(z, y, x));
                    test(v(x, y, z) == w5(y, z, x));
                }
            }
        }
    }
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4;
        shape[2] = 2;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w4(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        andres::View<int, constTarget> w5(shape.begin(), shape.end(), data_, andres::FirstMajorOrder);
        std::vector<std::size_t> permutation(3);
        permutation[0] = 0;
        permutation[1] = 1; 
        permutation[2] = 2;
        w1.permute(permutation.begin());
        permutation[0] = 1;
        permutation[1] = 0; 
        permutation[2] = 2;
        w2.permute(permutation.begin());
        permutation[0] = 0;
        permutation[1] = 2; 
        permutation[2] = 1;
        w3.permute(permutation.begin());
        permutation[0] = 2;
        permutation[1] = 1; 
        permutation[2] = 0;
        w4.permute(permutation.begin());
        permutation[0] = 1;
        permutation[1] = 2; 
        permutation[2] = 0;
        w5.permute(permutation.begin());

        for(std::size_t x=0; x<3; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                for(std::size_t z=0; z<2; ++z) {
                    test(v(x, y, z) == w1(x, y, z));
                    test(v(x, y, z) == w2(y, x, z));
                    test(v(x, y, z) == w3(x, z, y));
                    test(v(x, y, z) == w4(z, y, x));
                    test(v(x, y, z) == w5(y, z, x));
                }
            }
        }
    }
}

template<bool constTarget, andres::CoordinateOrder internalFirstMajorOrder>
void ViewTest::shiftOperatorTest() {
    // 2D
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w4(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w5(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w6(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        w1.shift(1);
        w2.shift(2);
        w3.shift(3);
        w4.shift(-1);
        w5.shift(-2);
        w6.shift(-3);

        for(std::size_t x=0; x<6; ++x) {
            for(std::size_t y=0; y<4; ++y) {
                test(v(x, y) == w1(y, x));
                test(v(x, y) == w2(x, y));
                test(v(x, y) == w3(y, x));
                test(v(x, y) == w4(y, x));
                test(v(x, y) == w5(x, y));
                test(v(x, y) == w6(y, x));
            }
        }
    }
    // 3D
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3; 
        shape[1] = 4;
        shape[2] = 2;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w1(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w2(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w3(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w4(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w5(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w6(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w7(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        andres::View<int, constTarget> w8(shape.begin(), shape.end(), data_, internalFirstMajorOrder);
        w1.shift(1);
        w2.shift(2);
        w3.shift(3);
        w4.shift(4);
        w5.shift(-1);
        w6.shift(-2);
        w7.shift(-3);
        w8.shift(-4);

        for(std::size_t x=0; x<3; ++x) {
            for(std::size_t y=0; y<4; ++y) {

                for(std::size_t z=0; z<2; ++z) {
                    test(v(x, y, z) == w1(z, x, y));
                    test(v(x, y, z) == w2(y, z, x));
                    test(v(x, y, z) == w3(x, y, z));
                    test(v(x, y, z) == w4(z, x, y));
                    test(v(x, y, z) == w5(y, z, x));
                    test(v(x, y, z) == w6(z, x, y));
                    test(v(x, y, z) == w7(x, y, z));
                    test(v(x, y, z) == w8(y, z, x));
                }
            }
        }
    }
}

void ViewTest::arithmeticOperatorsTest(){
    // operator+=
    {
        // 1D with... 
        {
            // a variable
            {   
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& w = (v += 1);

                test(&v == &w);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] + 1);
                }
            }
            {   
                float datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                andres::View<float>& w = (v += 2.0f);

                test(&v == &w);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] + 2.0f);
                }
            }
            // v is a scalar and w is a scalar 
            {   
                float scalar1 = 815.0f;
                float scalar2 = 4711.0f;
                andres::View<float> v(&scalar1);
                andres::View<float, true> w(&scalar2);

                andres::View<float>& r = (v += w);
                test(&r == &v);
                test(scalar1 == 815.0f + 4711.0f);
            }
            // v is not a scalar and w is a scalar 
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                float scalarw = 4711.0f;
                andres::View<float, true> w(&scalarw);

                andres::View<float>& r = (v += w);
                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == float(j) + scalarw);
                }
            }
            // w != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(), datav_, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::FirstMajorOrder);
                andres::View<int>& r = (v += w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i + 1);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // v != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v += w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i + 1);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // (v & w) != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v += w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i * 2);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // a View
            {   
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::View<int>& r = (v += w);

                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 3);
                }
            }
            {   
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v += v);

                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 2);
                }
            }
            // v is not simple, += scalar
            {   
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                }
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 2;
                strides[1] = 8;
                andres::View<int> v(shape.begin(), shape.end(), 
                    strides.begin(), datav_, andres::LastMajorOrder);
                andres::View<int>& r = (v += 2);

                test(&r == &v);
                std::vector<bool> covered(24, false);
                for(std::size_t x=0; x<shape[0]; ++x)
                for(std::size_t y=0; y<shape[1]; ++y) {
                    covered[x*strides[0]+y*strides[1]] = true;
                }
                for(std::size_t j=0; j<24; ++j) {
                    if(covered[j]) {
                        test(datav_[j] == 3);
                    }
                    else {
                        test(datav_[j] == 1);
                    }
                }
            }
        }
    }

    // operator+
    {
        // 3D with...
        {
            // v && w scalar
            {
                int scalarv = 1;
                int scalarw = 2;
                andres::View<int, true> v(&scalarv);
                andres::View<int, true> w(&scalarw);
                andres::Marray<int> m;
                m = v + w;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 3);
            }
            {
                float scalarv = 1.0f;
                float scalarw = 2.0f;
                andres::View<float, true> v(&scalarv);
                andres::View<float, true> w(&scalarw);
                andres::Marray<float> m;
                m = v + w;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 3.0f);
            }
            // a View and variable
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = v + 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 3.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m;
                m = v + 1;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] + 1);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m;
                m = v + 1.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] + 1.0f);
                        }
                    }
                }
            }
            // variable and a View
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = 2.0f + v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 3.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m;
                m = 1 + v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] + 1);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m;
                m = 1.0f + v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] + 1.0f);
                        }
                    }
                }
            }
            // two Views
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::Marray<int> m;
                m = v + w;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 3);
                }
            }
            {
                int scalar = 3;
                andres::View<int, true> v(&scalar);
                andres::Marray<int> m;
                m = v + v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                test(m(0) == 6);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m;
                m = v + v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 2);
                }
            }
        }
    }
    // prefix operator++
    {
        // scalar
        {   
            int scalar = 2;
            andres::View<int> v(&scalar);
            andres::View<int>& r = (++v);

            test(&r == &v
            );
            test(r(0) == 3);
        }
        // 1D 
        {   
            int datav_[24];
            for(int j=0; j<24; ++j) {
                datav_[j] = j;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int> v(shape.begin(), shape.end(), datav_);
            andres::View<int>& r = (++v);

            test(&r == &v
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data_[j] + 1);
            }
        }
    }
    // operator-=
    {
        // 1D with...
        {
            // variable
            {   
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v -= 1);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] - 1);
                }
            }
            {   
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                andres::View<float>& r = (v -= 1.0f);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] - 1.0f);
                }
            }
            // v is a scalar and w is a scalar 
            {   
                float scalar1 = 815.0f;
                float scalar2 = 4711.0f;
                andres::View<float> v(&scalar1);
                andres::View<float, true> w(&scalar2);

                andres::View<float>& r = (v -= w);
                test(&r == &v);
                test(scalar1 == 815.0f - 4711.0f);
            }
            // v is not a scalar and w is a scalar 
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                float scalarw = 4711.0f;
                andres::View<float, true> w(&scalarw);

                andres::View<float>& r = (v -= w);
                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == float(j) - scalarw);
                }
            }
            // w != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(), datav_, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::FirstMajorOrder);
                andres::View<int>& r = (v -= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == 1 - i );
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // v != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v -= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i - 1);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // (v & w) != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v -= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == 0);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // a View
            {   
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::View<int>& r = (v -= w);

                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == -1);
                }
            }
            {   
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v -= v);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 0);
                }
            }
        }
    }
    // prefix operator--
    {
        // scalar
        {   
            int scalar = 2;
            andres::View<int> v(&scalar);
            andres::View<int>& r = (--v);

            test(&r == &v
            );
            test(r(0) == 1);
        }
        // 1D 
        {   
            int datav_[24];
            for(int j=0; j<24; ++j) {
                datav_[j] = j;
            }
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::View<int> v(shape.begin(), shape.end(), datav_);
            andres::View<int>& r = (--v);

            test(&r == &v
            );
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == data_[j] - 1);
            }
        }
    }
    // unary operator-
    {
        // scalar
        {
            int scalar = 2;
            andres::View<int, true> v(&scalar);
            andres::Marray<int> m;
            m = -v;

            test(m.size()==v.size() && m.dimension()==v.dimension());
            test(m(0) == -2);
        }
        // 3D
        {
            int datav_[24];
            for(int j=0; j<24; ++j) {
                datav_[j] = j;
            }
            std::vector<std::size_t> shape(3);
            shape[0] = 2;
            shape[1] = 4;
            shape[2] = 3;
            andres::View<int, true> v(shape.begin(), shape.end(), datav_);
            andres::Marray<int> m;
            m = -v;

            test(m.size()==v.size() && m.dimension()==v.dimension());
            for(std::size_t j=0; j<v.shape(0); ++j) {
                for(std::size_t k=0; k<v.shape(1); ++k) {
                    for(std::size_t p=0; p<v.shape(2); ++p) {
                        test(m(j, k, p) == -data_[j + k*shape[0] + p*shape[0]*shape[1]]);
                    }
                }
            }
        }
    }
    // operator-
    {
        // 3D with...
        {
            // v && w scalar 
            {
                int scalarv = 2;
                int scalarw = 1;
                andres::View<int, true> v(&scalarv);
                andres::View<int, true> w(&scalarw);
                andres::Marray<int> m;
                m = v - w;

                test(m.size()==v.size() && m.dimension()==v.dimension());
                test(m(0) == 1);
            }
            {
                float scalarv = 2.0f;
                float scalarw = 1.0f;
                andres::View<float, true> v(&scalarv);
                andres::View<float, true> w(&scalarw);
                andres::Marray<float> m;
                m = v - w;

                test(m.size()==v.size() && m.dimension()==v.dimension());
                test(m(0) == 1.0f);
            }
            // a View and variable
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = v - 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == -1.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m;
                m = v - 1;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] - 1);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m;
                m = v - 1.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] - 1.0f);
                        }
                    }
                }
            }
            // variable and a View
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = 2.0f - v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 1.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = 1 - v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == 1 - data_[ j + k*shape[0] + p*shape[0]*shape[1]]);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m = 1.0f - v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == 1.0f - data_[ j + k*shape[0] + p*shape[0]*shape[1]]);
                        }
                    }
                }
            }
            // two Views
            {
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::Marray<int> m = v - w;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == -1);
                }
            }
            {
                int scalar = 3;
                andres::View<int, true> v(&scalar);
                andres::Marray<int> m;
                m = v - v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                test(m(0) == 0);
            }
            {
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = v - v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 0);
                }
            }
        }
    }
    // operator*=
    {
        // 1D with...
        {
            // variable
            {   
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v *= 2);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] * 2);
                }
            }
            {   
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                andres::View<float>& r = (v *= 2.0f);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == data_[j] * 2.0f);
                }
            }
            // v is a scalar and w is a scalar 
            {   
                float scalar1 = 815.0f;
                float scalar2 = 4711.0f;
                andres::View<float> v(&scalar1);
                andres::View<float, true> w(&scalar2);

                andres::View<float>& r = (v *= w);
                test(&r == &v);
                test(scalar1 == 815.0f * 4711.0f);
            }
            // v is not a scalar and w is a scalar 
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                float scalarw = 4711.0f;
                andres::View<float, true> w(&scalarw);

                andres::View<float>& r = (v *= w);
                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == float(j) * scalarw);
                }
            }
            // w != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(), datav_, andres::LastMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::FirstMajorOrder);
                andres::View<int>& r = (v *= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // v != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v *= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // (v & w) != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                int datav_[24];
                int dataw_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                    dataw_[j] = j;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int> v(shape.begin(), shape.end(),
                    strides.begin(), datav_, andres::FirstMajorOrder);
                andres::View<int, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_, andres::LastMajorOrder);
                andres::View<int>& r = (v *= w);

                test(&r == &v);
                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i * i);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // two Views
            {   
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::View<int>& r = (v *= w);

                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 2);
                }
            }
            {   
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v *= v);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 4);
                }
            }
        }
    }
    // operator*
    {
        // 3D with...
        {
            // v && w scalar 
            {
                int scalarv = 2;
                int scalarw = 3;
                andres::View<int, true> v(&scalarv);
                andres::View<int, true> w(&scalarw);
                andres::Marray<int> m;
                m = v * w;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 6);
            }
            {
                float scalarv = 2.0f;
                float scalarw = 3.0f;
                andres::View<float, true> v(&scalarv);
                andres::View<float, true> w(&scalarw);
                andres::Marray<float> m;
                m = v * w;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 6.0f);
            }
            // a View and variable
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = v * 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 2.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = v * 2;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] * 2);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m = v * 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] * 2.0f);
                        }
                    }
                }
            }
            // variable and a View
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = 2.0f * v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 2.0f);
            }
            {
                int datav_[24];
                for(int j=0; j<24; ++j) {
                    datav_[j] = j;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = 2*v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] * 2);
                        }
                    }
                }
            }
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m = 2.0f * v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == data_[ j + k*shape[0] + p*shape[0]*shape[1]] * 2.0f);
                        }
                    }
                }
            }
            // two Views
            {
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::Marray<int> m = v*w;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 2);
                }
            }
            {
                int scalar = 3;
                andres::View<int, true> v(&scalar);
                andres::Marray<int> m;
                m = v * v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                test(m(0) == 9);
            }
            {
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = v * v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 4);
                }
            }
        }
    }
    // operator/=
    {
        // 1D with
        {
            // variable
            {   
                float datav_[24];
                float dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                    dataw_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                andres::View<float>& r = (v /= 2.0f);
                
                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(r(j) == dataw_[j] / 2.0f);
                }
            }
            // v is a scalar and w is a scalar 
            {   
                float scalar1 = 815.0f;
                float scalar2 = 4711.0f;
                andres::View<float> v(&scalar1);
                andres::View<float, true> w(&scalar2);

                andres::View<float>& r = (v /= w);
                test(&r == &v);

                test(scalar1 == 815.0f / 4711.0f);
            }
            // v is not a scalar and w is a scalar 
            {
                float datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<float> v(shape.begin(), shape.end(), datav_);
                float scalarw = 2.0f;
                andres::View<float, true> w(&scalarw);

                andres::View<float>& r = (v /= w);
                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == float(j) / scalarw);
                }
            }
            // w != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                float datav_[25];
                float dataw_[25];
                for(std::size_t j=0; j<25; ++j) {
                    datav_[j] = 1;
                    dataw_[j] = static_cast<float>(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<float> v(shape.begin(), shape.end(), datav_, andres::LastMajorOrder);
                andres::View<float, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_+1, andres::FirstMajorOrder);
                andres::View<float>& r = (v /= w);

                test(&r == &v);
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    //test(v(x) == 1.0f / i);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // v != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                float datav_[25];
                float dataw_[25];
                for(std::size_t j=0; j<25; ++j) {
                    datav_[j] = static_cast<float>(j);
                    dataw_[j] = 1;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<float> v(shape.begin(), shape.end(),
                    strides.begin(), datav_+1, andres::FirstMajorOrder);
                andres::View<float, true> w(shape.begin(), shape.end(), dataw_, andres::LastMajorOrder);
                andres::View<float>& r = (v /= w);

                test(&r == &v);
                int i = 1; 
                for(int x = 0; x < 6; ++x){
                    test(v(x) == i / 1.0f);
                    i+=4;
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // (v & w) != isSimple && v.coordinateOrder() != w.coordinateOrder() 
            {
                float datav_[25];
                float dataw_[25];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = static_cast<float>(j);
                    dataw_[j] = static_cast<float>(j);
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<float> v(shape.begin(), shape.end(),
                    strides.begin(), datav_+1, andres::FirstMajorOrder);
                andres::View<float, true> w(shape.begin(), shape.end(),
                    strides.begin(), dataw_+1, andres::LastMajorOrder);
                andres::View<float>& r = (v /= w);

                test(&r == &v);
                for(int x = 0; x < 6; ++x){
                    test(v(x) == 1);
                }
                test(v.dimension() == shape.size() && 
                       v.size() == 6);
            }
            // two Views
            {   
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 4;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::View<int>& r = (v /= w);

                test(&r == &v);
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 2);
                }
            }
            {   
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 4;
                }
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int> v(shape.begin(), shape.end(), datav_);
                andres::View<int>& r = (v /= v);

                test(&r == &v
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(v(j) == 1);
                }
            }
        }
    }
    // operator/
    {
        // 3D with...
        {
            // v && w scalar 
            {
                int scalarv = 4;
                int scalarw = 2;
                andres::View<int, true> v(&scalarv);
                andres::View<int, true> w(&scalarw);
                andres::Marray<int> m;
                m = v / w;

                test(m.size()==v.size() && m.dimension()==v.dimension());
                test(m(0) == 2);
            }
            {
                float scalarv = 4.0f;
                float scalarw = 2.0f;
                andres::View<float, true> v(&scalarv);
                andres::View<float, true> w(&scalarw);
                andres::Marray<float> m;
                m = v / w;

                test(m.size()==v.size() && m.dimension()==v.dimension());
                test(m(0) == 2.0f);
            }
            // a View and variable
            {
                float scalar = 1;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = v / 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 0.5f);
            }
            {
                float datav_[24];
                float dataf_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = float(j);
                    dataf_[j] = float(j);
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m = v / 2.0f;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == dataf_[ j + k*shape[0] + p*shape[0]*shape[1]] / 2);
                        }
                    }
                }
            }
            // variable and a View
            {
                float scalar = 4.0f;
                andres::View<float, true> v(&scalar);
                andres::Marray<float> m;
                m = 2.0f / v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                test(m(0) == 0.5f);
            }
            {
                float datav_[24];
                float dataf_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 4.0f;
                    dataf_[j] = 4.0f;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<float, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<float> m = 2.0f / v;

                test(m.size()==v.size() && m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.shape(0); ++j) {
                    for(std::size_t k=0; k<v.shape(1); ++k) {
                        for(std::size_t p=0; p<v.shape(2); ++p) {
                            test(m(j, k, p) == 2.0f / dataf_[ j + k*shape[0] + p*shape[0]*shape[1]]);
                        }
                    }
                }
            }
            // two Views
            {
                int datav_[24];
                int dataw_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 4;
                    dataw_[j] = 2;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::View<int, true> w(shape.begin(), shape.end(), dataw_);
                andres::Marray<int> m = v / w;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 2);
                }
            }
            {
                int scalar = 3;
                andres::View<int, true> v(&scalar);
                andres::Marray<int> m;
                m = v / v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                test(m(0) == 1);
            }
            {
                int datav_[24];
                for(std::size_t j=0; j<24; ++j) {
                    datav_[j] = 4;
                }
                std::vector<std::size_t> shape(3);
                shape[0] = 2;
                shape[1] = 4;
                shape[2] = 3;
                andres::View<int, true> v(shape.begin(), shape.end(), datav_);
                andres::Marray<int> m = v / v;

                test(m.size()==v.size() &&  m.dimension()==v.dimension()
                );
                for(std::size_t j=0; j<v.size(); ++j) {
                    test(m(j) == 1);
                }
            }
        }
    }
}

template<bool constTarget>
void ViewTest::asStringTest()
{
    {
        std::size_t shape[3] = {3, 2, 4};

        andres::View<int, constTarget> v(&shape[0], &shape[3], data_);
        std::stringstream s;
        s << v.asString(andres::MatrixStyle);
        s << v.asString(andres::TableStyle);

        const andres::View<int, constTarget> w(&shape[0], &shape[3], data_);
        s << w.asString(andres::MatrixStyle);
        s << w.asString(andres::TableStyle);
    }
}

void ViewTest::reshapeTest()
{
    float data[24];
    for(std::size_t j=0; j<24; ++j) {
        data[j] = static_cast<float>(j);
    }
    std::size_t shape[] = {2, 4, 3};
    andres::View<float, false> v(&shape[0], &shape[3], data);
    andres::View<float, false> w = v; // copy
    
    std::size_t newShape[] = {4, 6};
    v.reshape(&newShape[0], &newShape[2]);

    test(v.dimension() == 2);
    test(v.size() == 24);
    test(v.shape(0) == 4);
    test(v.shape(1) == 6);
    test(v.isSimple());
    test(&v(0) == &w(0));
    for(std::size_t j=0; j<v.size(); ++j) {
        test(v(j) == w(j));
    }

    if(!andres::MARRAY_NO_DEBUG) {
        // negative test
        std::size_t base[] = {0, 1, 1};
        std::size_t shape[] = {2, 3, 2};
        std::size_t newShape[] = {3, 4};
        andres::View<float, false> x = v.view(base, shape);
        try {
            x.reshape(&newShape[0], &newShape[2]);
            test(0 == 1); // reshape should have thrown runtime_error
        }
        catch(std::runtime_error&) {}
    }
}

void ViewTest::overlapTreatmentTest()
{
    {
        std::size_t shape[] = {3, 3};
        andres::Marray<int> m(shape, shape + 2, 0);
        m(0, 0) = 1; m(0, 1) = 2; m(1, 0) = 3; m(1, 1) = 4;
        std::size_t base[] = {0, 0};
        shape[0] = 2;
        shape[1] = 2;
        andres::View<int> v = m.view(base, shape);
        base[0] = 1; base[1] = 1;
        andres::View<int> w = m.view(base, shape);

        w = v;

        test(w.overlaps(v) && v.overlaps(w));
        test(w(0, 0) == 1 && w(0, 1) == 2 && w(1, 0) == 3 && w(1, 1) == 4);
    }
    {
        std::size_t shape[] = {3, 3};
        andres::Marray<int> m(shape, shape + 2, 0);
        m(0, 0) = 1; m(0, 1) = 2; m(1, 0) = 3; m(1, 1) = 4;
        andres::Marray<int> n = m;
        std::size_t base[] = {0, 0};
        shape[0] = 2;
        shape[1] = 2;
        andres::View<int> v = m.view(base, shape);
        base[0] = 1; base[1] = 1;
        andres::View<int> w = m.view(base, shape);

        w += v;

        test(w.overlaps(v) && v.overlaps(w));
        test(   w(0, 0) == n(1, 1)+n(0, 0)
               && w(0, 1) == n(1, 2)+n(0, 1)
               && w(1, 0) == n(2, 1)+n(1, 0)
               && w(1, 1) == n(2, 2)+n(1, 1) );
    }
    {
        std::size_t shape[] = {3, 3};
        andres::Marray<int> m(shape, shape + 2, 0);
        m(0, 0) = 1; m(0, 1) = 2; m(1, 0) = 3; m(1, 1) = 4;
        andres::Marray<int> n = m;
        std::size_t base[] = {0, 0};
        shape[0] = 2;
        shape[1] = 2;
        andres::View<int> v = m.view(base, shape);
        base[0] = 1; base[1] = 1;
        andres::View<int> w = m.view(base, shape);

        w -= v;

        test(w.overlaps(v) && v.overlaps(w));
        test(   w(0, 0) == n(1, 1)-n(0, 0)
               && w(0, 1) == n(1, 2)-n(0, 1)
               && w(1, 0) == n(2, 1)-n(1, 0)
               && w(1, 1) == n(2, 2)-n(1, 1) );
    }
    {
        std::size_t shape[] = {3, 3};
        andres::Marray<int> m(shape, shape + 2, 1);
        m(0, 0) = 2; m(0, 1) = 3; m(1, 0) = 4; m(1, 1) = 5;
        andres::Marray<int> n = m;
        std::size_t base[] = {0, 0};
        shape[0] = 2;
        shape[1] = 2;
        andres::View<int> v = m.view(base, shape);
        base[0] = 1; base[1] = 1;
        andres::View<int> w = m.view(base, shape);

        w *= v;

        test(w.overlaps(v) && v.overlaps(w));
        test(   w(0, 0) == n(1, 1)*n(0, 0)
               && w(0, 1) == n(1, 2)*n(0, 1)
               && w(1, 0) == n(2, 1)*n(1, 0)
               && w(1, 1) == n(2, 2)*n(1, 1) );
    }
    {
        std::size_t shape[] = {3, 3};
        andres::Marray<float> m(shape, shape + 2, 1);
        m(0, 0) = 2; m(0, 1) = 4; m(1, 0) = 8; m(1, 1) = 16;
        andres::Marray<float> n = m;
        std::size_t base[] = {0, 0};
        shape[0] = 2;
        shape[1] = 2;
        andres::View<float> v = m.view(base, shape);
        base[0] = 1; base[1] = 1;
        andres::View<float> w = m.view(base, shape);

        w /= v;

        test(w.overlaps(v) && v.overlaps(w));
        test(   w(0, 0) == n(1, 1)/n(0, 0)
               && w(0, 1) == n(1, 2)/n(0, 1)
               && w(1, 0) == n(2, 1)/n(1, 0)
               && w(1, 1) == n(2, 2)/n(1, 1) );
    }
}

void ViewTest::compatibilityFunctionsTest()
{
    #ifdef MARRAY_COMPATIBILITY
    // permuteDimensions
    {
        std::size_t shape[] = {2, 4, 3}; 
        andres::Marray<float> m(&shape[0], &shape[3]);
        std::size_t perm[] = {0, 2, 1};
        andres::View<float> v = m.permuteDimensions(perm);
        andres::View<float> w = m.permutedView(perm);
        
        test(v.dimension() == w.dimension());
        for(std::size_t j=0; j<v.dimension(); ++j) {
            test(v.shape(j) == w.shape(j));
        }
        test(v.size() == w.size());
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == w(j));
        }
    }
    // shiftDimensions
    {
        std::size_t shape[] = {2, 4, 3}; 
        andres::Marray<float> m(&shape[0], &shape[3]);
        andres::View<float> v = m.shiftDimensions(2);
        andres::View<float> w = m.shiftedView(2);
        test(v.dimension() == w.dimension());
        for(std::size_t j=0; j<v.dimension(); ++j) {
            test(v.shape(j) == w.shape(j));
        }
        test(v.size() == w.size());
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == w(j));
        }
    }
    // subarray
    {
        std::size_t shape[] = {2, 4, 3}; 
        andres::Marray<float> m(&shape[0], &shape[3]);
        std::size_t b[] = {0, 2, 1};
        std::size_t s[] = {2, 1, 2};
        andres::View<float> v = m.view(b, s);
        std::size_t e[] = {2, 3, 3};
        andres::View<float> w = m.subarray(b, e);
        test(v.dimension() == w.dimension());
        for(std::size_t j=0; j<v.dimension(); ++j) {
            test(v.shape(j) == w.shape(j));
        }
        test(v.size() == w.size());
        for(std::size_t j=0; j<v.size(); ++j) {
            test(v(j) == w(j));
        }
    }
    #endif // #ifdef MARRAY_COMPATIBILITY
}

IteratorTest::IteratorTest() {
    for(int j=0; j<24; ++j) {
        data_[j] = j*2;
    }
}

template<bool constTarget>
void IteratorTest::constructorTest(){
    // empty
    {
        andres::Iterator<int, constTarget> it;
        test(it.index() == 0);
    }
    // view and index
    {
        // scalar
        {
            int scalar = 42;
            andres::View<int, constTarget> v(&scalar);
            andres::Iterator<int, constTarget> it(v, 0);
            
            test(*it == v(0));
        }
        {
            int scalar = 42;
            const andres::View<int, true> v(&scalar);
            andres::Iterator<int, true> it(v, 0);
            
            test(*it == v(0));
        }
        {
            int scalar = 42;
            andres::View<int, false> v(&scalar);
            andres::Iterator<int, true> it(v, 0);
            
            test(*it == v(0));
        }
        // 1D
        {
            // zero offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            // non-zero offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 12);
                
                test(it.index() == 12
                );
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12
                );
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12
                );
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
        }
        // 2D
        {
            // zero offset
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            // non-zero offset
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6;
                shape[1] = 4;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
        }
        // 3D
        {
            // zero offset
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 0);
                
                for(std::size_t j = 0; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            // non-zero offset
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, constTarget> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                const andres::View<int, true> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                andres::View<int, false> v(shape.begin(), shape.end(), data_);
                andres::Iterator<int, true> it(v, 12);
                
                test(it.index() == 12);
                for(std::size_t j = 12; j < 24; ++j){
                    test(*it == data_[j]);
                    ++it;
                }
                test(it == v.end() &&
                      *(--it) == 46);
            }
        }
    }
    // conversion from mutable to const
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, false> it;
        it = v.begin();
        andres::Iterator<int, true> itc(it);

        for(int i = 0; i < 24; ++i){
            test(*itc == data_[i]);
            ++itc;
        }
        test(itc == v.end());
        test(*(--itc)==46);
    }
}

template<bool constTarget>
void IteratorTest::accessIteratorOperations() {
    // operator*
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    // operator->
    {
        TestType data[24];
        for(std::size_t j=0; j<24; ++j) {
            data[j].data_ = j;
        }
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<TestType, constTarget> v(shape.begin(),
            shape.end(), &data[0]);
        andres::Iterator<TestType, constTarget> it = v.begin();
        for(std::size_t j=0; j<24; ++j) {
            test(it->data_ == j);
            ++it;
        }
    }
    {
        TestType data[24];
        for(std::size_t j=0; j<24; ++j) {
            data[j].data_ = j;
        }
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<TestType, true> v(shape.begin(),
            shape.end(), &data[0]);
        andres::Iterator<TestType, true> it = v.begin();
        for(std::size_t j=0; j<24; ++j) {
            test(it->data_ == j);
            ++it;
        }
    }
    {
        TestType data[24];
        for(std::size_t j=0; j<24; ++j) {
            data[j].data_ = j;
        }
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<TestType, false> v(shape.begin(),
            shape.end(), &data[0]);
        andres::Iterator<TestType, true> it = v.begin();
        for(std::size_t j=0; j<24; ++j) {
            test(it->data_ == j);
            ++it;
        }
    }
    // operator[]
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it[i] == data_[i]);
        }
        test(it == v.begin() &&
               it[4] == data_[4] &&
               it[16] == data_[16]);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it[i] == data_[i]);
        }
        test(it == v.begin() &&
               it[4] == data_[4] &&
               it[16] == data_[16]);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it[i] == data_[i]);
        }
        test(it == v.begin() &&
               it[4] == data_[4] &&
               it[16] == data_[16]);
    }
}

template<bool constTarget>
void IteratorTest::arithmeticOperatorsTest(){
    // operator+=
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it += 1;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it += 1;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it += 1;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    // operator-=
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.end();

        
        for(int i = 23; i >= 0; --i){
            it -= 1;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            it -= 1;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            it -= 1;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    // operator++ prefix
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            ++it;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    // operator-- prefix
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            --it;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            --it;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            --it;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    // operator++ postfix
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it++;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it++;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();
        
        for(int i = 0; i < 24; ++i){
            test(*it == data_[i]);
            it++;
        }
        test(it == v.end());
        test(*(--it)==46);
    }
    // operator-- postfix
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            it--;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            it--;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.end();
        
        for(int i = 23; i >= 0; --i){
            it--;
            test(*it == data_[i]);
        }
        test(it == v.begin());
        test(*it==0);
    }
}

template<bool constTarget>
void IteratorTest::comparisonOperators(){
    // operator-
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it1;
        }
        test((it1-it2)==24);
        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==0);
        
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==-24);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it1;
        }
        test((it1-it2)==24);
        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==0);
        
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==-24);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it1;
        }
        test((it1-it2)==24);
        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==0);
        
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            ++it2;
        }
        test((it1-it2)==-24);
    }
    // operator==
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it1==it2);
            ++it1;
            ++it2;
        }
        test(it1 == v.end() && it2 == v.end());
        test(*(--it1)==46 && *(--it2)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it1==it2);
            ++it1;
            ++it2;
        }
        test(it1 == v.end() && it2 == v.end());
        test(*(--it1)==46 && *(--it2)==46);
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it1==it2);
            ++it1;
            ++it2;
        }
        test(it1 == v.end() && it2 == v.end());
        test(*(--it1)==46 && *(--it2)==46);
    }
    //operator!=
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1!=it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1!=it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1!=it2);
            ++it2;
        }
    }
    //operator<
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it2;
            test(it1<it2);
            ++it1;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it2;
            test(it1<it2);
            ++it1;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it2;
            test(it1<it2);
            ++it1;
        }
    }
    //operator>
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1>it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1>it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            ++it1;
            test(it1>it2);
            ++it2;
        }
    }
    //operator<=
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1<=it2);
            ++it2;
            test(it1<=it2);
            ++it1;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1<=it2);
            ++it2;
            test(it1<=it2);
            ++it1;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1<=it2);
            ++it2;
            test(it1<=it2);
            ++it1;
        }
    }
    //operator>=
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it1;
        andres::Iterator<int, constTarget> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1>=it2);
            ++it1;
            test(it1>=it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1>=it2);
            ++it1;
            test(it1>=it2);
            ++it2;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it1;
        andres::Iterator<int, true> it2;
        it1 = v.begin();
        it2 = v.begin();

        for(int i = 0; i < 23; ++i){
            test(it1>=it2);
            ++it1;
            test(it1>=it2);
            ++it2;
        }
    }
}

template<bool constTarget>
void IteratorTest::hasMoreTest() {
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.hasMore());
            ++it;
        }
        test(!it.hasMore());
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.hasMore());
            ++it;
        }
        test(!it.hasMore());
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.hasMore());
            ++it;
        }
        test(!it.hasMore());
    }
}

template<bool constTarget>
void IteratorTest::indexTest() {
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.index()== static_cast<std::size_t>(data_[i]/2));
            ++it;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.index()== static_cast<std::size_t>(data_[i]/2));
            ++it;
        }
    }
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it;
        it = v.begin();

        for(int i = 0; i < 24; ++i){
            test(it.index()== static_cast<std::size_t>(data_[i]/2));
            ++it;
        }
    }
}

template<bool constTarget>
void IteratorTest::coordinateTest() {
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 3;
        shape[1] = 8;
        andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, constTarget> it(v, 0);

        for(std::size_t y=0; y<8; ++y) {
            for(std::size_t x=0; x<3; ++x) {
                std::vector<std::size_t> c(2);
                it.coordinate(c.begin());
                test(c[0] == x && c[1] == y);
                ++it;
            }
        }
    }
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 3;
        shape[1] = 8;
        const andres::View<int, true> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it(v, 0);

        for(std::size_t y=0; y<8; ++y) {
            for(std::size_t x=0; x<3; ++x) {
                std::vector<std::size_t> c(2);
                it.coordinate(c.begin());
                test(c[0] == x && c[1] == y);
                ++it;
            }
        }
    }
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 3;
        shape[1] = 8;
        andres::View<int, false> v(shape.begin(), shape.end(), data_);
        andres::Iterator<int, true> it(v, 0);

        for(std::size_t y=0; y<8; ++y) {
            for(std::size_t x=0; x<3; ++x) {
                std::vector<std::size_t> c(2);
                it.coordinate(c.begin());
                test(c[0] == x && c[1] == y);
                ++it;
            }
        }
    }
}

MarrayTest::MarrayTest() : scalar_(42) {
    for(int j=0; j<24; ++j) {
        data_[j] = j;
        data2x_[j] = j*2;
    }
    for(int j=0; j<100; ++j) {
        data100_[j] = j;
    }
}

void MarrayTest::constructorTest(){
    // empty 
    {
        andres::Marray<int> m;
    }
    // scalar
    {
        andres::Marray<int> m(scalar_);
        
        test(m.dimension()==0);
        test(m.size()==1);
    }
    // 1D
    {
        std::vector<std::size_t> shape(1);
        shape[0] = 24;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_);

        test(m.size()==24 && m.dimension()==1);
        test(m.shape(0) == 24);
        for(int i = 0; i < 24; ++i)
            test(m(i)==scalar_);
    }
    // 2D
    {
        std::vector<std::size_t> shape(2);
        shape[0] = 6; 
        shape[1] = 4;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_);

        test(m.size()==24 && m.dimension()==2);
        test(m.shape(0) == 6);
        test(m.shape(1) == 4);
        for(int x = 0; x < 6; ++x){
            for(int y = 0; y < 4; ++y)
                test(m(x, y)==scalar_);
        }
    }
    // 3D 
    {
        std::vector<std::size_t> shape(3); 
        shape[0] = 3;
        shape[1] = 4; 
        shape[2] = 2; 
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_);

        test(m.size()==24 && m.dimension()==3);
        test(m.shape(0) == 3);
        test(m.shape(1) == 4);
        test(m.shape(2) == 2);
        for(int x = 0; x < 3; ++x){
            for(int y = 0; y < 4; ++y){
                for(int z = 0; z < 2; ++z)
                    test(m(x, y, z)==scalar_);
            }
        }
    }
    // 3D skip initialization
    {
        std::vector<std::size_t> shape(3); 
        shape[0] = 3;
        shape[1] = 4; 
        shape[2] = 2; 
        andres::Marray<int> m(andres::SkipInitialization,
            shape.begin(), shape.end());

        test(m.size()==24 && m.dimension()==3);
        test(m.shape(0) == 3);
        test(m.shape(1) == 4);
        test(m.shape(2) == 2);
    }
}

void MarrayTest::assignTest()
{
        std::vector<std::size_t> shape(3); 
        shape[0] = 3;
        shape[1] = 4; 
        shape[2] = 2; 
        andres::Marray<int> m(andres::SkipInitialization,
            shape.begin(), shape.end());
    m.assign();
    test(m.size() == 0);
}

template<bool constTarget>
void MarrayTest::copyConstructorTest() {
    // contruction from a Marray
    {
        // empty 
        {
            andres::Marray<int> m;
            andres::Marray<int> n(m);
        }
        // scalar
        {
            andres::Marray<int> m(scalar_);
            andres::Marray<int> n(m);

            test(m.dimension() == n.dimension());
            test(m.size() == n.size());
            test(m(0) == n(0));
        }
        // 1D
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
            andres::Marray<int> n(m);

            test(m.size()==n.size() && m.dimension()==n.dimension()
            );
            for(int x = 0; x < 24; ++x)
                test(m(x)==n(x));
        }
        // 2D
        {
            std::vector<std::size_t> shape(2);
            shape[0] = 6; 
            shape[1] = 4;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
            andres::Marray<int> n(m);

            test(m.size()==n.size() && m.dimension()==n.dimension()
            );
            for(int x = 0; x < 6; ++x){
                for(int y = 0; y < 4; ++y)
                    test(m(x, y)==n(x, y));
            }
        }
        // 3D 
        {
            std::vector<std::size_t> shape(3); 
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
            andres::Marray<int> n(m);

            test(m.size()==n.size() && m.dimension()==n.dimension()
            );
            for(int x = 0; x < 3; ++x){
                for(int y = 0; y < 4; ++y){
                    for(int z = 0; z < 2; ++z)
                        test(m(x, y, z)==n(x, y, z));
                }
            }
        }
    }
    // contruction from a view
    {
        // scalar
        {
            andres::View<int, constTarget> v(&scalar_);
            andres::Marray<int> m(v);
            test(v.size()==m.size() &&
                   v.dimension()==m.dimension() &&
                   v(0)==m(0));
        }
        // View is 1D...
        {
            // without strides
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m(v);
                
                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int i = 0; i < 24; ++i)
                    test(v(i)==m(i));
            }
            // with strides and without offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                int i = 0; 
                for(int x = 0; x < 6; ++x){
                    test(m(x) == data_[i]);
                    i+=4;
                }
                test(v.dimension() == m.dimension() &&
                       v(0) == m(0) &&
                       v.size() == m.size());
            }
            // with strides and offset
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 6;
                std::vector<std::size_t> strides(1);
                strides[0] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(),
                    strides.begin(), data_+2, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                int i = 2; 
                for(int x = 0; x < 6; ++x){
                    test(m(x) == data_[i]);
                    i+=4;
                }
                test(v.dimension() == m.dimension() &&
                       v(0) == m(0) &&
                       v.size() == m.size());
            }
        }
        // View is 2D...
        {
            // without strides
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m(v);

                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int x = 0; x < 6; ++x){
                    for(int y = 0; y < 4; ++y)
                        test(v(x, y)==m(x, y));
                }
            }
            // with strides and without offset
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(), data_, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                        test(v(x, y)==m(x, y));
                    }
                }
                test(v.dimension() == m.dimension() &&
                       v(0) == m(0) &&
                       v.size() == m.size());
            }
            // with strides and offset
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 3;
                shape[1] = 2;
                std::vector<std::size_t> strides(2);
                strides[0] = 1;
                strides[1] = 3;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_+50, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                for(int y = 0; y < 2; ++y){
                    for(int x = 0; x < 3; ++x){
                        test(v(x, y)==m(x, y));
                    }
                }
                test(v.dimension() == m.dimension() &&
                       v(0) == m(0) &&
                       v.size() == m.size());
            }
        }
        // View is 3D...
        {
            // without strides
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4; 
                shape[2] = 2; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m(v);

                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int x = 0; x < 3; ++x)
                for(int y = 0; y < 4; ++y)
                for(int z = 0; z < 2; ++z){
                    test(v(x, y, z)==m(x, y, z));
                    
                }
            }
            // with strides and without offset
            {        
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z)==m(x, y, z));
                }
            }
            // with strides and offset
            {       
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4;
                shape[2] = 2;
                std::vector<std::size_t> strides(3);
                strides[0] = 2;
                strides[1] = 10;
                strides[2] = 35;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), strides.begin(),
                    data100_+30, andres::LastMajorOrder);
                andres::Marray<int> m(v);

                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int z = 0; z < 2; ++z)
                for(int y = 0; y < 4; ++y)
                for(int x = 0; x < 3; ++x){
                    test(v(x, y, z)==m(x, y, z));
                }
            }
        }
    }
}

template<bool constTarget>
void MarrayTest::assignmentOperatorTest(){
    // empty
    {
        // from a Marray
        {
            // empty
            {
                andres::Marray<int> m;
                andres::Marray<int> n;
                n = m;
            }
            // scalar
            {
                andres::Marray<int> m(scalar_);
                andres::Marray<int> n;
                n = m;

                test(m.dimension() == n.dimension() &&
                       m.size() == n.size() &&
                       m(0) == n(0));
            }
            // 1D
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
                andres::Marray<int> n;
                n = m;

                test(m.size()==n.size() && m.dimension()==n.dimension()
                );
                for(int x = 0; x < 24; ++x)
                    test(m(x)==n(x));
            }
            // 2D
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::Marray<int> m(shape.begin(), shape.end(), scalar_);

                andres::Marray<int> n;
                n = m;

                test(m.size()==n.size() && m.dimension()==n.dimension()
                );
                for(int x = 0; x < 6; ++x){
                    for(int y = 0; y < 4; ++y)
                        test(m(x, y)==n(x, y));
                }
            }
            // 3D 
            {
                std::vector<std::size_t> shape(3); 
                shape[0] = 3;
                shape[1] = 4; 
                shape[2] = 2; 
                andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
                andres::Marray<int> n;
                n = m;

                test(m.size()==n.size() && m.dimension()==n.dimension()
                );
                for(int x = 0; x < 3; ++x){
                    for(int y = 0; y < 4; ++y){
                        for(int z = 0; z < 2; ++z)
                            test(m(x, y, z)==n(x, y, z));
                    }
                }
            }
        }
        // from a view
        {
            // empty
            {
                andres::View<int, constTarget> v;
                andres::Marray<int> m;
                m = v;
            }
            // scalar
            {
                andres::View<int, constTarget> v(&scalar_);
                andres::Marray<int> m;
                m = v;

                test(v.size()==m.size() &&
                       v.dimension()==m.dimension() &&
                       v(0)==m(0));
            }
            // 1D
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m;
                m = v;
                
                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int i = 0; i < 24; ++i)
                    test(v(i)==m(i));
            }
            // 2D
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m;
                m = v;

                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int x = 0; x < 6; ++x){
                    for(int y = 0; y < 4; ++y)
                        test(v(x, y)==m(x, y));
                }
            }
            // 3D
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4; 
                shape[2] = 2; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                andres::Marray<int> m;
                m = v;

                test(v.size()==m.size() &&
                       v.dimension()==m.dimension());
                for(int x = 0; x < 3; ++x){
                    for(int y = 0; y < 4; ++y){
                        for(int z = 0; z < 2; ++z)
                            test(v(x, y, z)==m(x, y, z));
                    }
                }
            }
        }
    }
    // scalar
    {
        // from a Marray
        {
            // empty
            {
                andres::Marray<int> m;
                andres::Marray<int> n(scalar_);
                n = m;
            }
            // scalar
            {
                int scalar = 2;
                andres::Marray<int> m(scalar);
                andres::Marray<int> n(scalar_);
                int* n_address = &n(0);
                n = m;
                
                test(&n(0) == n_address);
                test(m.dimension() == n.dimension() &&
                       m.size() == n.size() &&
                       m(0) == n(0));
            }
        }
        // from a view
        {
            // empty
            {
                andres::View<int, constTarget> v;
                andres::Marray<int> m(scalar_);
                m = v;
            }
            // scalar
            {
                andres::View<int, constTarget> v(&scalar_);
                andres::Marray<int> m(scalar_);
                int* m_address = &m(0);
                m = v;
                
                test(&m(0) == m_address);
                test(v.size()==m.size() &&
                       v.dimension()==m.dimension() &&
                       v(0)==m(0));
            }
        }
    }
    // 1D
    {
        // from a Marray
        {
            // empty
            {
                andres::Marray<int> m;
                std::vector<std::size_t> shapen(1);
                shapen[0] = 24;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                n = m;
            }
            // 1D
            {
                int scalar = 2;
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::Marray<int> m(shape.begin(), shape.end(), scalar);
                std::vector<std::size_t> shapen(1);
                shapen[0] = 24;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                int* n_address = &n(0);
                n = m;
                
                test(&n(0) == n_address
                );
                test(m.size()==n.size() && m.dimension()==n.dimension()
                );
                for(int x = 0; x < 24; ++x)
                    test(m(x)==n(x));
            }
        }
        // from a view
        {
            // empty
            {
                andres::View<int, constTarget> v;
                std::vector<std::size_t> shapem(1);
                shapem[0] = 24;
                andres::Marray<int> m(shapem.begin(), shapem.end(), scalar_);
                m = v;
            }
            // 1D
            {
                std::vector<std::size_t> shape(1);
                shape[0] = 24;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                std::vector<std::size_t> shapem(1);
                shapem[0] = 24;
                andres::Marray<int> m(shapem.begin(), shapem.end(), scalar_);
                int* m_address = &m(0);
                m = v;
                
                test(&m(0) == m_address
                );
                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int i = 0; i < 24; ++i)
                    test(v(i)==m(i));
            }
        }
    }
    // 2D
    {
        // from a Marray
        {
            // empty
            {
                andres::Marray<int> m;
                std::vector<std::size_t> shapen(2);
                shapen[0] = 4;
                shapen[1] = 6;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                n = m;
            }
            // 2D
            {
                int scalar = 2;
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::Marray<int> m(shape.begin(), shape.end(), scalar);
                std::vector<std::size_t> shapen(2);
                shapen[0] = 6;
                shapen[1] = 4;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                int* n_address = &n(0);
                n = m;
                
                test(&n(0) == n_address);
                test(m.size()==n.size() && m.dimension()==n.dimension());
                for(int x = 0; x < 6; ++x){
                    for(int y = 0; y < 4; ++y)
                        test(m(x, y)==n(x, y));
                }
            }
        }
        // from a view
        {
            // empty
            {
                andres::View<int, constTarget> v;
                std::vector<std::size_t> shapen(2);
                shapen[0] = 4;
                shapen[1] = 6;
                andres::Marray<int> m(shapen.begin(), shapen.end(), scalar_);
                m = v;
            }
            // 2D
            {
                std::vector<std::size_t> shape(2);
                shape[0] = 6; 
                shape[1] = 4;
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                std::vector<std::size_t> shapen(2);
                shapen[0] = 6;
                shapen[1] = 4;
                andres::Marray<int> m(shapen.begin(), shapen.end(), scalar_);
                int* m_address = &m(0);
                m = v;
                
                test(&m(0) == m_address);
                test(v.size()==m.size() && v.dimension()==m.dimension());
                for(int x = 0; x < 6; ++x){
                    for(int y = 0; y < 4; ++y)
                        test(v(x, y)==m(x, y));
                }
            }
        }
    }
    // 3D
    {
        // from a Marray
        {
            // empty
            {
                andres::Marray<int> m;
                std::vector<std::size_t> shapen(3);
                shapen[0] = 2;
                shapen[1] = 3;
                shapen[2] = 4;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                n = m;
            }
            // 3D 
            {
                int scalar = 2;
                std::vector<std::size_t> shape(3); 
                shape[0] = 3;
                shape[1] = 4; 
                shape[2] = 2; 
                andres::Marray<int> m(shape.begin(), shape.end(), scalar);
                std::vector<std::size_t> shapen(3);
                shapen[0] = 3;
                shapen[1] = 4;
                shapen[2] = 2;
                andres::Marray<int> n(shapen.begin(), shapen.end(), scalar_);
                int* n_address = &n(0);
                n = m;
                
                test(&n(0) == n_address);
                test(m.size()==n.size() && m.dimension()==n.dimension());
                for(int x = 0; x < 3; ++x){
                    for(int y = 0; y < 4; ++y){
                        for(int z = 0; z < 2; ++z)
                            test(m(x, y, z)==n(x, y, z));
                    }
                }
            }
        }
        // from a view
        {
            // empty
            {
                andres::View<int, constTarget> v;
                std::vector<std::size_t> shapen(3);
                shapen[0] = 2;
                shapen[1] = 3;
                shapen[2] = 4;
                andres::Marray<int> m(shapen.begin(), shapen.end(), scalar_);
                m = v;
            }
            // 3D
            {
                std::vector<std::size_t> shape(3);
                shape[0] = 3;
                shape[1] = 4; 
                shape[2] = 2; 
                andres::View<int, constTarget> v(shape.begin(), shape.end(), data_);
                std::vector<std::size_t> shapen(3);
                shapen[0] = 3;
                shapen[1] = 4;
                shapen[2] = 2;
                andres::Marray<int> m(shapen.begin(), shapen.end(), scalar_);
                int* m_address = &m(0);
                m = v;
                
                test(&m(0) == m_address
                );
                test(v.size()==m.size() && v.dimension()==m.dimension()
                );
                for(int x = 0; x < 3; ++x){
                    for(int y = 0; y < 4; ++y){
                        for(int z = 0; z < 2; ++z)
                            test(v(x, y, z)==m(x, y, z));
                    }
                }
            }
        }
    }
}

void MarrayTest::reshapeTest() {
    // 2D 
    {
        std::vector<std::size_t> shape(2); 
        shape[0] = 6; 
        shape[1] = 4;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
        shape[0] = 4; 
        shape[1] = 6; 
        m.reshape(shape.begin(), shape.end());

        test(m.shape(0)==4 &&
               m.shape(1)==6);
    }
    {
        std::vector<std::size_t> shape(2); 
        shape[0] = 6; 
        shape[1] = 4;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_, andres::FirstMajorOrder);
        shape[0] = 4; 
        shape[1] = 6; 
        m.reshape(shape.begin(), shape.end());

        test(m.shape(0)==4 &&
               m.shape(1)==6);
    }
    // 3D
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4; 
        shape[2] = 2;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_);
        shape[0] = 2;
        shape[1] = 3; 
        shape[2] = 4;
        m.reshape(shape.begin(), shape.end());
    
        test(m.shape(0)==2 &&
               m.shape(1)==3 &&
               m.shape(2)==4);
    }
    {
        std::vector<std::size_t> shape(3);
        shape[0] = 3;
        shape[1] = 4; 
        shape[2] = 2;
        andres::Marray<int> m(shape.begin(), shape.end(), scalar_, andres::FirstMajorOrder);
        shape[0] = 2;
        shape[1] = 3; 
        shape[2] = 4;
        m.reshape(shape.begin(), shape.end());
    
        test(m.shape(0)==2 &&
               m.shape(1)==3 &&
               m.shape(2)==4);
    }
}

template<andres::CoordinateOrder coordinateOrder>
void MarrayTest::resizeTest() {
    // 1D
    {
        // shrink
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 12; 
            m.resize(shape.begin(), shape.end(), 12);

            test(m.shape(0)==12 &&
                   m.size()==12 &&
                   m.dimension()==1);
        }
        // shrink to dimension == 0 
        {
            std::vector<std::size_t> shape(1), shape0(0);
            shape[0] = 24; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            m.resize(shape0.begin(), shape0.end(), 1);
        }
        // shrink to scalar 
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 1; 
            m.resize(shape.begin(), shape.end(), 3);

            test(m.shape(0)==1 &&
                   m.size()==1 && 
                   m(0) == scalar_);
        }
        // grow scalar to 1D
        {
            int scalar = 1;
            std::vector<std::size_t> shape(1);
            shape[0] = 24; 
            andres::Marray<int> m(scalar);
            m.resize(shape.begin(), shape.end(), 3);

            test(m.shape(0)==24 &&
                   m.size()==24 &&
                   m.dimension() == 1);
            test(m(0) == 1);
            for(int i = 1; i < 24; ++i){
                test(m(i) == 3);
            }
        }
        // grow 1D
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 12; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 24; 
            m.resize(shape.begin(), shape.end(), 3);

            test(m.shape(0)==24 &&
                   m.size()==24 &&
                   m.dimension()==1);
            for(int i = 0; i < 24; ++i){
                if(i < 12) {
                    test(m(i) == scalar_);
                }
                else {
                    test(m(i) == 3); 
                }
            }
        }
        // grow scalar to 3D
        {
            int scalar = 1;
            andres::Marray<int> m(scalar); 
            std::vector<std::size_t> newShape(3);
            newShape[0] = 5;
            newShape[1] = 5; 
            newShape[2] = 2;
            m.resize(newShape.begin(), newShape.end(), 3);
        
            test(m.shape(0)==5 &&
                   m.shape(1)==5 &&
                   m.shape(2)==2 &&
                   m.size()==50 &&
                   m.dimension()==3);
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 5; ++y){
                for(int x = 0; x < 5; ++x){
                    if(y == 0 && z == 0 && x == 0) {
                        test(m(x, y, z) == 1);
                    }
                    else {
                        test(m(x, y, z) == 3);
                    }
                }
            }
        }
        // grow to 3D
        {
            std::vector<std::size_t> shape(1);
            shape[0] = 12; 
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder); 
            std::vector<std::size_t> newShape(3);
            newShape[0] = 5;
            newShape[1] = 5; 
            newShape[2] = 2;
            m.resize(newShape.begin(), newShape.end(), 3);
        
            test(m.shape(0)==5 &&
                   m.shape(1)==5 &&
                   m.shape(2)==2 &&
                   m.size()==50 &&
                   m.dimension()==3);
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 5; ++y) {
                for(int x = 0; x < 5; ++x) {
                    if(y == 0 && z == 0 && x < 5) {
                        test(m(x, y, z) == scalar_);
                    }
                    else {
                        test(m(x, y, z) == 3);
                    }
                }
            }
        }
    }
    // 2D 
    {
        // shrink
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6; 
            shape[1] = 4;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 3; 
            shape[1] = 2; 
            m.resize(shape.begin(), shape.end(), 6);

            test(m.shape(0)==3 &&
                   m.shape(1)==2 &&
                   m.size()==6 &&
                   m.dimension()==2);
        }
        // grow
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 3; 
            shape[1] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 6; 
            shape[1] = 4; 
            m.resize(shape.begin(), shape.end(), 3);

            test(m.shape(0)==6 &&
                   m.shape(1)==4 &&
                   m.size()==24 &&
                   m.dimension()==2);
            for(int y = 0; y < 4; ++y){
                for(int x = 0; x < 6; ++x){
                    if(y < 2 && x < 3) {
                        test(m(x, y) == scalar_);
                    }
                    else {
                        test(m(x, y) == 3);
                    }
                }
            }
        }
        // shrink to dimension == 0
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6; 
            shape[1] = 4;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(0); 
            m.resize(newShape.begin(), newShape.end(), 3);
        }
        // shrink to scalar
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6; 
            shape[1] = 4;

            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(1); 
            newShape[0] = 12; 
            m.resize(newShape.begin(), newShape.end(), 3);

            test(m.shape(0)==12 &&
                   m.size()==12);
        }
        // shrink to 1D
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 6; 
            shape[1] = 4;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(1); 
            newShape[0] = 12; 
            m.resize(newShape.begin(), newShape.end(), 3);

            test(m.shape(0)==12 &&
                   m.size()==12 &&
                   m.dimension()==1);
            for(int i = 0; i < 6; ++i)
                test(m(i) == scalar_);
            for(int i = 6; i < 12; ++i)
                test(m(i) == 3);
        }
        // grow to 3D
        {
            std::vector<std::size_t> shape(2); 
            shape[0] = 3; 
            shape[1] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(3);
            newShape[0] = 5;
            newShape[1] = 5; 
            newShape[2] = 2;
            m.resize(newShape.begin(), newShape.end(), 3);
        
            test(m.shape(0)==5 &&
                   m.shape(1)==5 &&
                   m.shape(2)==2 &&
                   m.size()==50 &&
                   m.dimension()==3);
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 5; ++y){
                for(int x = 0; x < 5; ++x){
                    if(y < 2 && z == 0 && x < 3) {
                        test(m(x, y, z) == scalar_);
                    }
                    else {
                        test(m(x, y, z) == 3);
                    }
                }
            }
        }
    }
    // 3D
    {
        // shrink
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 2;
            shape[1] = 2; 
            shape[2] = 2;
            m.resize(shape.begin(), shape.end(), 8);
        
            test(m.shape(0)==2 &&
                   m.shape(1)==2 &&
                   m.shape(2)==2 &&
                   m.size()==8 &&
                   m.dimension()==3);
        }
        // grow
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            shape[0] = 5;
            shape[1] = 5; 
            shape[2] = 2;
            m.resize(shape.begin(), shape.end(), 3);
        
            test(m.shape(0)==5 &&
                   m.shape(1)==5 &&
                   m.shape(2)==2 &&
                   m.size()==50 &&
                   m.dimension()==3);
            for(int z = 0; z < 2; ++z)
            for(int y = 0; y < 5; ++y){
                for(int x = 0; x < 5; ++x){
                    if(y < 4 && x < 3) {
                        test(m(x, y, z) == scalar_);
                    }                   
                    else {
                        test(m(x, y, z) == 3);
                    }
                }
            }
        }
        // shrink to dimension == 0
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(0); 
            m.resize(newShape.begin(), newShape.end(), 3);
        }
        // shrink to scalar
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(1); 
            newShape[0] = 12; 
            m.resize(newShape.begin(), newShape.end(), 3);

            test(m.shape(0)==12 &&
                   m.size()==12 &&
                   m(0) == scalar_);
        }
        // shrink to 1D
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(1); 
            newShape[0] = 12; 
            m.resize(newShape.begin(), newShape.end(), 3);

            test(m.shape(0)==12 &&
                   m.size()==12 &&
                   m.dimension()==1);
            for(int i = 0; i < 3; ++i)
                test(m(i) == scalar_);
            for(int i = 3; i < 12; ++i)
                test(m(i) == 3);
        }
        // shrink to 2D
        {
            std::vector<std::size_t> shape(3);
            shape[0] = 3;
            shape[1] = 4; 
            shape[2] = 2;
            andres::Marray<int> m(shape.begin(), shape.end(), scalar_, coordinateOrder);
            std::vector<std::size_t> newShape(2); 
            newShape[0] = 6; 
            newShape[1] = 4; 
            m.resize(newShape.begin(), newShape.end(), 3);

            test(m.shape(0)==6 &&
                   m.shape(1)==4 &&
                   m.size()==24 &&
                   m.dimension()==2);
            for(int y = 0; y < 4; ++y){
                for(int x = 0; x < 6; ++x){
                    if(y < 4 && x < 3) {
                        test(m(x, y) == scalar_);
                    }
                    else {
                        test(m(x, y) == 3);
                    }
                }
            }
        }
    }
}

ExpressionTemplateTest::ExpressionTemplateTest()
{
    for(std::size_t j=0; j<24; ++j) {
        dataInt_[j] = static_cast<int>(j+1);
    }
}

void ExpressionTemplateTest::arithmeticOperatorsTest()
{
    std::size_t shape[] = {2, 3, 2};
    andres::View<int> v(shape, shape+3, dataInt_);
    andres::View<int> w(shape, shape+3, dataInt_ + 2);
    for(std::size_t z=0; z<2; ++z) {
        for(std::size_t y=0; y<3; ++y) {
            for(std::size_t x=0; x<2; ++x) {
                test((+v)(x,y,z) == + v(x,y,z));

                test((-v)(x,y,z) == - v(x,y,z));

                test((2+v)(x,y,z) == 2 + v(x,y,z));
                test((v+2)(x,y,z) == v(x,y,z) + 2);
                test((v+w)(x,y,z) == v(x,y,z) + w(x,y,z));

                test((2-v)(x,y,z) == 2 - v(x,y,z));
                test((v-2)(x,y,z) == v(x,y,z) - 2);
                test((v-w)(x,y,z) == v(x,y,z) - w(x,y,z));

                test((2*v)(x,y,z) == 2 * v(x,y,z));
                test((v*2)(x,y,z) == v(x,y,z) * 2);
                test((v*w)(x,y,z) == v(x,y,z) * w(x,y,z));

                test((2/v)(x,y,z) == 2 / v(x,y,z));
                test((v/2)(x,y,z) == v(x,y,z) / 2);
                test((v/w)(x,y,z) == v(x,y,z) / w(x,y,z));

                test((v*v - 2*v*w + w*w)(x,y,z)
                    == v(x,y,z)*v(x,y,z) - 2*v(x,y,z)*w(x,y,z)
                       + w(x,y,z)*w(x,y,z));
            }
        }
    }
}

void ExpressionTemplateTest::constructionAndAssignmentTest()
{
    // Marray
    {
        std::size_t shape[] = {2, 3, 2};
        andres::View<int> v(shape, shape+3, dataInt_);
        andres::View<int> w(shape, shape+3, dataInt_ + 2);
        andres::Marray<int> r(v + w); // construction
        for(std::size_t z=0; z<2; ++z) {
            for(std::size_t y=0; y<3; ++y) {
                for(std::size_t x=0; x<2; ++x) {
                    test( r(x,y,z) == (v + w)(x,y,z) );
                }
            }
        }
        r = v * w; // assignment
        for(std::size_t z=0; z<2; ++z) {
            for(std::size_t y=0; y<3; ++y) {
                for(std::size_t x=0; x<2; ++x) {
                    test( r(x,y,z) == (v * w)(x,y,z) );
                }
            }
        }
    }
}

DifferingTypesTest::DifferingTypesTest()
{
    for(std::size_t j=0; j<24; ++j) {
        dataShort_[j] = static_cast<short>(j+1);
        dataInt_[j] = static_cast<int>(j+1);
    }
}

void DifferingTypesTest::constructionTest()
{
    // Marray
    {
        // from View 
        {
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            andres::Marray<int> m(v);
            test(v.size() == m.size());
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == m(j));
            }
        }
        // from ViewExpression
        {
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            andres::Marray<int> m(v + v);
            test(v.size() == m.size());
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j)+v(j) == m(j));
            }
        }
        // from Marray
        {
            std::size_t shape[] = {3, 2, 4};
            andres::Marray<short> m(shape, shape+3);
            andres::Marray<int> n(m);
            test(n.size() == m.size());
            for(std::size_t j=0; j<n.size(); ++j) {
                test(n(j) == m(j));
            }
        }
    }
}

void DifferingTypesTest::assignmentTest()
{
    // View
    {
        // from constant
        {
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            v = 2;
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == 2);
            }
        }
        // from View
        { 
            // compilers may issue a warning here
            if(sizeof(float) == sizeof(int)) {
                std::size_t shape[] = {3, 2, 4};
                andres::View<int> v(shape, shape+3, dataInt_);
                andres::View<float, true> w;
                w = v;
                for(std::size_t j=0; j<v.size(); ++j) {
                    test( w(j) == *static_cast<float*>(
                        static_cast<void*>(&dataInt_[j])));
                }
            }
        }
        // from ViewExpression
        { 
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            andres::View<int> w(shape, shape+3, dataInt_);
            w = v + v;
            for(std::size_t j=0; j<w.size(); ++j) {
                test(w(j) == (v + v)(j));
            }
        }
    }
    // Marray
    {
        // from constant
        {
            std::size_t shape[] = {3, 2, 4};
            andres::Marray<short> m(shape, shape+3);
            m = 2;
            for(std::size_t j=0; j<m.size(); ++j) {
                test(m(j) == 2);
            }
        }
        // from View 
        {
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            andres::Marray<int> m;
            m = v;
            test(v.size() == m.size());
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j) == m(j));
            }
        }
        // from ViewExpression
        {
            std::size_t shape[] = {3, 2, 4};
            andres::View<short> v(shape, shape+3, dataShort_);
            andres::Marray<int> m;
            m = v + v;
            test(v.size() == m.size());
            for(std::size_t j=0; j<v.size(); ++j) {
                test(v(j)+v(j) == m(j));
            }
        }
        // from Marray
        {
            std::size_t shape[] = {3, 2, 4};
            andres::Marray<short> m(shape, shape+3);
            andres::Marray<int> n;
            n = m;
            test(n.size() == m.size());
            for(std::size_t j=0; j<n.size(); ++j) {
                test(n(j) == m(j));
            }
        }
    }
}

void DifferingTypesTest::arithmeticOperatorsTest()
{
    // ViewExpression
    {
        std::size_t shape[] = {2, 3, 2};
        andres::View<short> v(shape, shape+3, dataShort_);
        andres::View<int> w(shape, shape+3, dataInt_);
        for(std::size_t z=0; z<2; ++z) {
            for(std::size_t y=0; y<3; ++y) {
                for(std::size_t x=0; x<2; ++x) {
                    test((2+v)(x,y,z) == 2 + v(x,y,z));
                    test((v+2)(x,y,z) == v(x,y,z) + 2);
                    test((v+w)(x,y,z) == v(x,y,z) + w(x,y,z));

                    test((2-v)(x,y,z) == 2 - v(x,y,z));
                    test((v-2)(x,y,z) == v(x,y,z) - 2);
                    test((v-w)(x,y,z) == v(x,y,z) - w(x,y,z));

                    test((2*v)(x,y,z) == 2 * v(x,y,z));
                    test((v*2)(x,y,z) == v(x,y,z) * 2);
                    test((v*w)(x,y,z) == v(x,y,z) * w(x,y,z));

                    test((2/v)(x,y,z) == 2 / v(x,y,z));
                    test((v/2)(x,y,z) == v(x,y,z) / 2);
                    test((v/w)(x,y,z) == v(x,y,z) / w(x,y,z));

                    test((v*v - 2*v*w + w*w)(x,y,z)
                        == v(x,y,z)*v(x,y,z) - 2*v(x,y,z)*w(x,y,z)
                           + w(x,y,z)*w(x,y,z));
                }
            }
        }
    }
    // Marray (implicitly: View)
    {
        // += constant
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m(shape, shape+3);
            for(std::size_t j=0; j<m.size(); ++j) {
                m(j) = static_cast<short>(j+1);
            }
            andres::Marray<short> n(m); // copy for comparsion
            m += 2;
            for(std::size_t j=0; j<m.size(); ++j) {
                test(m(j) == n(j) + 2);
            }
        }
        // += Marray
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m1(shape, shape+3);
            andres::Marray<int> m2(shape, shape+3);
            for(std::size_t j=0; j<m1.size(); ++j) {
                m1(j) = static_cast<short>(j+1);
                m2(j) = static_cast<int>(j+1);
            }
            andres::Marray<short> n(m1); // copy for comparison
            m1 += m2;
            for(std::size_t j=0; j<m1.size(); ++j) {
                test(m1(j) == n(j) + m2(j));
            }
        }
        // -= constant
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m(shape, shape+3);
            for(std::size_t j=0; j<m.size(); ++j) {
                m(j) = static_cast<short>(j+1);
            }
            andres::Marray<short> n(m); // copy for comparsion
            m -= 2;
            for(std::size_t j=0; j<m.size(); ++j) {
                test(m(j) == n(j) - 2);
            }
        }
        // -= Marray
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m1(shape, shape+3);
            andres::Marray<int> m2(shape, shape+3);
            for(std::size_t j=0; j<m1.size(); ++j) {
                m1(j) = static_cast<short>(j+1);
                m2(j) = static_cast<int>(j+1);
            }
            andres::Marray<short> n(m1); // copy for comparison
            m1 -= m2;
            for(std::size_t j=0; j<m1.size(); ++j) {
                test(m1(j) == n(j) - m2(j));
            }
        }
        // *= constant
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m(shape, shape+3);
            for(std::size_t j=0; j<m.size(); ++j) {
                m(j) = static_cast<short>(j+1);
            }
            andres::Marray<short> n(m); // copy for comparsion
            m *= 2;
            for(std::size_t j=0; j<m.size(); ++j) {
                test(m(j) == n(j) * 2);
            }
        }
        // *= Marray
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m1(shape, shape+3);
            andres::Marray<int> m2(shape, shape+3);
            for(std::size_t j=0; j<m1.size(); ++j) {
                m1(j) = static_cast<short>(j+1);
                m2(j) = static_cast<int>(j+1);
            }
            andres::Marray<short> n(m1); // copy for comparison
            m1 *= m2;
            for(std::size_t j=0; j<m1.size(); ++j) {
                test(m1(j) == n(j) * m2(j));
            }
        }
        // /= constant
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m(shape, shape+3);
            for(std::size_t j=0; j<m.size(); ++j) {
                m(j) = static_cast<short>(j+1);
            }
            andres::Marray<short> n(m); // copy for comparsion
            m /= 2;
            for(std::size_t j=0; j<m.size(); ++j) {
                test(m(j) == n(j) / 2);
            }
        }
        // /= Marray
        {
            std::size_t shape[] = {2, 3, 4};
            andres::Marray<short> m1(shape, shape+3);
            andres::Marray<int> m2(shape, shape+3);
            for(std::size_t j=0; j<m1.size(); ++j) {
                m1(j) = static_cast<short>(j+1);
                m2(j) = static_cast<int>(j+1);
            }
            andres::Marray<short> n(m1); // copy for comparison
            m1 /= m2;
            for(std::size_t j=0; j<m1.size(); ++j) {
                test(m1(j) == n(j) / m2(j));
            }
        }
    }
}

struct MyTestType {};

void DifferingTypesTest::nonBasicTypesTest()
{
    std::size_t shape[] = {3, 4};
    andres::Marray<MyTestType> m(shape, shape + 2);
    std::size_t base[] = {0, 0};
    shape[0] = 2;
    shape[1] = 2;
    andres::View<MyTestType> v = m.view(base, shape);
    v(0, 1) = MyTestType();
}

void DifferingTypesTest::typePromotionTest()
{
    std::size_t shape[] = {3, 4};
    andres::Marray<unsigned char> a(shape, shape + 2, 250);
    andres::Marray<int> b(shape, shape + 2, 1000);
    for(std::size_t j=0; j<a.size(); ++j) {
        test((a + b)(j) == 1250);
        test((a - b)(j) == -750);
        test((b + a)(j) == 1250);
        test((b - a)(j) == 750);
        test((4 * a)(j) == 4 * a(j));
        test((a * 4)(j) == a(j) * 4);
    }
}

#ifdef HAVE_CPP0X_INITIALIZER_LISTS
class Cpp0xTest {
    int data_[24];
    
    public:
    Cpp0xTest() {
        for(std::size_t j=0; j<24; ++j) {
            data_[j] = j;
        }
    }
        
    void test() {
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            std::size_t a = 100;
            v.coordinatesToIndex({1,1,3}, a);
            v.coordinatesToOffset({1,1,3}, a);
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            v.assign({6,4}, data_);
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            v.assign({6,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            v.assign({24}, {3}, data_, andres::LastMajorOrder);
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            andres::View<int, true> v2;
            andres::View<int, false> v3;
            v.view({0,0,0}, {1,1,1}, v3);
            v.view({0,0,0}, {1,1,1}, andres::LastMajorOrder, v3);
            v.view({0,0,0}, {1,1,1}, andres::LastMajorOrder, v3);
            v.constView({0,0,0}, {1,1,1}, v2);
            v.constView({0,0,0}, {1,1,1}, andres::LastMajorOrder, v2);
        }
        {
            andres::View<int, true> v1({6,4}, {4}, data_, andres::LastMajorOrder);
            andres::View<int, true> v2({6,4}, {4}, data_, andres::LastMajorOrder);
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder, andres::LastMajorOrder);
            v.permute({0,2,1});
        }
        {
            andres::View<int, false> v({3,2,4}, data_, andres::LastMajorOrder);
        }
        {
            andres::Marray<int> v({3,2,4}, 0);
            v.reshape({6,4});
        }
        {
            andres::Marray<int> v({3,2,4}, 0);
            v.resize({1,2,4});
        }
        {
            andres::Vector<int> v({1,2,3,4,5,6,7,8,9});
        }
    }
};
#endif

int main() 
{
    { GlobalFunctionTest t; t.shapeStrideTest(); }

    { ViewTest t; t.coordinatesToOffsetTest<false>(); }
    { ViewTest t; t.coordinatesToOffsetTest<true>(); }
    { ViewTest t; t.coordinatesToIndexTest<false>(); }
    { ViewTest t; t.coordinatesToIndexTest<true>(); }
    { ViewTest t; t.indexToCoordinatesTest<false>(); }
    { ViewTest t; t.indexToCoordinatesTest<true>(); }
    { ViewTest t; t.indexToOffsetTest<false>(); }
    { ViewTest t; t.indexToOffsetTest<true>(); }
    { ViewTest t; t.emptyConstructorTest<false>(); }
    { ViewTest t; t.emptyConstructorTest<true>(); }
    { ViewTest t; t.scalarConstructorTest<false>(); }
    { ViewTest t; t.scalarConstructorTest<true>(); }
    { ViewTest t; t.shapeConstructorTest<false>(); }
    { ViewTest t; t.shapeConstructorTest<true>(); }
    { ViewTest t; t.shapeAssignTest<false>(); }
    { ViewTest t; t.shapeAssignTest<true>(); }
    { ViewTest t; t.shapeStrideConstructorTest<false>(); }
    { ViewTest t; t.shapeStrideConstructorTest<true>(); } 
    { ViewTest t; t.shapeStrideAssignTest<false>(); }
    { ViewTest t; t.shapeStrideAssignTest<true>(); }
    { ViewTest t; t.copyConstructorTest(); } 
    { ViewTest t; t.assignmentOperatorTest<andres::LastMajorOrder>(); } 
    { ViewTest t; t.assignmentOperatorTest<andres::FirstMajorOrder>(); }
    { ViewTest t; t.queryTest<false>(); }
    { ViewTest t; t.queryTest<true>(); }
    { ViewTest t; t.elementAccessTest<false>(); }
    { ViewTest t; t.elementAccessTest<true>(); }
    { ViewTest t; t.subViewsTest<false>(); }
    { ViewTest t; t.subViewsTest<true>(); }
    { ViewTest t; t.subConstViewsTest(); }
    { ViewTest t; t.iteratorAccessTest<false>(); }
    { ViewTest t; t.iteratorAccessTest<true>(); } 
    { ViewTest t; t.coordinateBindingTest<false>(); }
    { ViewTest t; t.coordinateBindingTest<true>(); }
    { ViewTest t; t.transposeTest<false>(); }
    { ViewTest t; t.transposeTest<true>(); }
    { ViewTest t; t.permuteTest<false>(); }
    { ViewTest t; t.permuteTest<true>(); } 
    { ViewTest t; t.shiftOperatorTest<false, andres::LastMajorOrder>(); }
    { ViewTest t; t.shiftOperatorTest<false, andres::FirstMajorOrder>(); }
    { ViewTest t; t.shiftOperatorTest<true, andres::LastMajorOrder>(); }
    { ViewTest t; t.shiftOperatorTest<true, andres::FirstMajorOrder>(); } 
    { ViewTest t; t.arithmeticOperatorsTest(); }
    { ViewTest t; t.asStringTest<true>(); } 
    { ViewTest t; t.asStringTest<false>(); } 
    { ViewTest t; t.reshapeTest(); }
    { ViewTest t; t.overlapTreatmentTest(); }
    { ViewTest t; t.compatibilityFunctionsTest(); }

    { IteratorTest t; t.constructorTest<false>(); }
    { IteratorTest t; t.constructorTest<true>(); }
    { IteratorTest t; t.accessIteratorOperations<false>(); }
    { IteratorTest t; t.accessIteratorOperations<true>(); }
    { IteratorTest t; t.arithmeticOperatorsTest<false>(); }
    { IteratorTest t; t.arithmeticOperatorsTest<true>(); }
    { IteratorTest t; t.comparisonOperators<false>(); }
    { IteratorTest t; t.comparisonOperators<true>(); }
    { IteratorTest t; t.hasMoreTest<false>(); }
    { IteratorTest t; t.hasMoreTest<true>(); }
    { IteratorTest t; t.indexTest<false>(); }
    { IteratorTest t; t.indexTest<true>(); }
    { IteratorTest t; t.coordinateTest<false>(); }
    { IteratorTest t; t.coordinateTest<true>(); }

    { MarrayTest t; t.constructorTest(); } 
    { MarrayTest t; t.assignTest(); } 
    { MarrayTest t; t.copyConstructorTest<false>(); }
    { MarrayTest t; t.copyConstructorTest<true>(); }
    { MarrayTest t; t.assignmentOperatorTest<false>(); }
    { MarrayTest t; t.assignmentOperatorTest<true>(); }
    { MarrayTest t; t.reshapeTest(); } 
    { MarrayTest t; t.resizeTest<andres::LastMajorOrder>(); } 
    { MarrayTest t; t.resizeTest<andres::FirstMajorOrder>(); } 

    { ExpressionTemplateTest t; t.constructionAndAssignmentTest(); }
    { ExpressionTemplateTest t; t.arithmeticOperatorsTest(); }

    { DifferingTypesTest t; t.constructionTest(); }
    { DifferingTypesTest t; t.assignmentTest(); }
    { DifferingTypesTest t; t.arithmeticOperatorsTest(); }
    { DifferingTypesTest t; t.nonBasicTypesTest(); }
    { DifferingTypesTest t; t.typePromotionTest(); }

    #ifdef HAVE_CPP0X_INITIALIZER_LISTS
    { Cpp0xTest t; t.test(); }
    #endif

    std::cout << "All tests have been passed." << std::endl;
    return 0;
}       

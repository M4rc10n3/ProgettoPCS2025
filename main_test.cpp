# include <gtest/gtest.h>

#include "GTest/TestUtils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolyhedraLibrary;

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
#include "catch.hpp"
#include "static_analyser.h"
#include <vector>

namespace skrobot {

    TEST_CASE("检查一手牌是不是顺子", "[StaticAnalyserC, IsShunZi]") {
        StaticAnalyserC target;

        SECTION("手牌长度小于５，返回false") {
            int input[4] = {1,2,3,4};
            REQUIRE(target.IsShunZi(input, 4) == false);
        }

        SECTION("手牌长度大于等于５，但牌的点数间有间隔，返回false") {
            int input[5] = {0x21, 0x22, 0x23, 0x25, 0x26};
            REQUIRE(target.IsShunZi(input, 5) == false);
        }

        SECTION("手牌长度大于等于5，且牌都是连贯的，没有财神，返回true") {
            int input[8] = {0x21, 0x22, 0x33, 0x34, 0x27, 0x26, 0x25, 0x18};
            REQUIRE(target.IsShunZi(input, 8) == true);
        }

        SECTION("手牌长度大于等于5，虽然有n张牌不连贯，但有n张财神，返回true") {
            int input[8] = {0x21, 0x22, 0x33, 0x34, 0x27, 0x4E, 0x25, 0x18};
            REQUIRE(target.IsShunZi(input, 8) == true);
            int input2[8] = {0x21, 0x22, 0x33, 0x4F, 0x27, 0x4E, 0x25, 0x18};
            REQUIRE(target.IsShunZi(input2, 8) == true);
        }
    }
    
TEST_CASE("检查一手牌是不是对子", "[skrobot, static_analyser]") {
    StaticAnalyserC target;

    SECTION("数组长度不为２要报错") {
        int wrong_input[3] = {0x21,0x22,0x23};
        REQUIRE(target.IsDouble(wrong_input, 3) == false);
    }

    SECTION("数组长度为２，但点数不同") {
        int input[2] = {0x21, 0x22};
        REQUIRE(target.IsDouble(input, 2) == false);
    }

    SECTION("数组长度为２，且点数相同") {
        int input[2] = {0x21, 0x31};
        REQUIRE(target.IsDouble(input, 2) == true);
    }

    SECTION("数组长度为2，点数不同，但有一张财神，返回true") {
        int input[2] = {0x21, 0x4F};
        int input2[2] = {0x21, 0x4E};
        REQUIRE(target.IsDouble(input ,2) == true);
        REQUIRE(target.IsDouble(input2 ,2) == true);
    }
}

TEST_CASE("检查一手牌是不是三条", "[skrobot, static_analyser]") {
    StaticAnalyserC target;

    SECTION("数组的长度不为３，返回false") {
        int input[2] = {0x21,0x22};
        REQUIRE(target.IsTriple(input, 2) == false);
    }

    SECTION("数组长度为３，但点数不同，返回false") {
        int input[3] = {0x22, 0x32, 0x31};
        REQUIRE(target.IsTriple(input, 3) == false);
    }

    SECTION("数组长度为３，且点数相同，返回true") {
        int input[3] = {0x21, 0x31, 0x11};
        REQUIRE(target.IsTriple(input, 3) == true);
    }

    SECTION("数组长度为３，有一个对子，一张财神，返回true") {
        int input[3] = {0x21, 0x21, 0x4F};
        REQUIRE(target.IsTriple(input, 3) == true);
    }

    SECTION("数组长度为３，一张单牌，两张财神，返回true") {
        int input[3] = {0x4F, 0x4E, 0x21};
        REQUIRE(target.IsTriple(input, 3) == true);
    }
}

TEST_CASE("检查一手牌是否是对连", "[StaticAnalyserC, IsDoubleLink]") {
    StaticAnalyserC target;

    SECTION("数组的长度不被２整除，返回false") {
        int input[7] = {1,2,3,4,5,6,7};
        REQUIRE(target.IsDoubleLink(input, 7) == false);
    }

    SECTION("数组的长度小于６，返回false") {
        int input[4] = {1,2,3,4};
        REQUIRE(target.IsDoubleLink(input, 4) == false);
    }

    SECTION("数组长度符合条件，但存在落单的牌, 且没有财神，返回false") {
        int input[6] = {0x21, 0x31, 0x22, 0x12, 0x33, 0x34};
        REQUIRE(target.IsDoubleLink(input, 6) == false);
    }

    SECTION("数组的长度符合条件，全是对子，但点数不连贯, 且没有财神, 返回false") {
        int input[6] = {0x21, 0x31, 0x22, 0x12, 0x34, 0x34};
        REQUIRE(target.IsDoubleLink(input, 6) == false);
    }

    SECTION("数组的长度符合条件，全是对子，且点数连贯,没有财神, 返回true") {
        int input[6] = {0x21, 0x31, 0x22, 0x12, 0x33, 0x33};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }

    SECTION("数组的长度符合条件，一张单牌，一张财神，点数连贯，返回true") {
        int input[6] = {0x21, 0x21, 0x22, 0x22, 0x33, 0x4E};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }

    SECTION("数组的长度符合条件，两张单牌，两张财神，点数连贯，返回true") {
        int input[6] = {0x21, 0x21, 0x22, 0x4E, 0x33, 0x4E};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }
}

TEST_CASE("检查一手牌是否是三连", "[skrobot, static_analyser]") {
    StaticAnalyserC target;

    SECTION("数组的长度不被3整除，返回false") {
        int input[7] = {1,2,3,4,5,6,7};
        REQUIRE(target.IsTripleLink(input, 7) == false);
    }

    SECTION("数组的长度小于9，返回false") {
        int input[4] = {1,2,3,4};
        REQUIRE(target.IsTripleLink(input, 4) == false);
    }

    SECTION("数组长度符合条件，但存在落单的牌,返回false") {
        int input[9] = {0x21, 0x31, 0x21, 0x16, 0x32, 0x32, 0x35,0x35,0x35};
        REQUIRE(target.IsTripleLink(input, 9) == false);
    }

    SECTION("数组的长度符合条件，全是三条，但点数不连贯, 返回false") {
        int input[9] = {0x21, 0x31, 0x21, 0x12, 0x32, 0x32, 0x14,0x24,0x34};
        REQUIRE(target.IsTripleLink(input, 9) == false);
    }

    SECTION("数组的长度符合条件，全是三条，且点数连贯, 返回true") {
        int input[9] = {0x21, 0x31, 0x21, 0x12, 0x32, 0x32, 0x33, 0x33, 0x33};
        REQUIRE(target.IsTripleLink(input, 9) == true);
    }

    SECTION("数组的长度符合条件，有一张财神，在非财神牌中，有一个点数的牌成双，其余成三条, 返回true") {
        int input[9] = {0x22, 0x22, 0x31,0x21,0x11, 0x33,0x23,0x33, 0x4E};
        REQUIRE(target.IsTripleLink(input , 9) == true);
    }
    SECTION("数组的长度符合条件，有一张财神，在非财神牌中，有一个点数的牌成双，其余成三条, 返回true") {
        int input[9] = {0x22, 0x22, 0x31,0x21,0x11, 0x33,0x23,0x33, 0x4E};
        REQUIRE(target.IsTripleLink(input , 9) == true);
    }
}

TEST_CASE("检查一副手牌是否是炸弹", "[skrobot, static analyser]") {
    StaticAnalyserC target;

    SECTION("如果手牌长度小于4且不等于3,返回false") {
        int input[3] = {1,2,3};
        REQUIRE(target.IsBomb(input, 3, true) == false);
    }

    SECTION("手牌长度为3，且不全是司令，返回false") {
        int input[3] = {0x21, 0x4E, 0x4F};
        REQUIRE(target.IsBomb(input, 3, false) == false);
    }

    SECTION("手牌长度为3，且全部是司令，三王炸，返回true") {
        int input[3] = {0x4E, 0x4E, 0x4F};
        REQUIRE(target.IsBomb(input, 3, false) == true);
    }

    SECTION("手牌长度大于等于４，但有不同点数的牌，返回false") {
        int input[4] = {0x21, 0x22, 0x21, 0x21};
        REQUIRE(target.IsBomb(input, 4, false) == false);
    }

    SECTION("手牌长度大于等于４, 且所有牌的点数相同，返回true") {
        int input[4] = {0x21, 0x21, 0x31, 0x31};
        REQUIRE(target.IsBomb(input, 4, false) == true );
    }

    SECTION("手牌中有一个三条，一张财神，返回true") {
        int input[4] = {0x22, 0x22, 0x22, 0x4F};
        REQUIRE(target.IsBomb(input, 4, false) == true);
    }

    SECTION("手牌中有5张同样的牌，以及正负司令各一个，返回true") {
        int input[7] = {0x22,0x22,0x22,0x22,0x22,0x4F,0x4E};
        REQUIRE(target.IsBomb(input, 7, false) == true);
    }

    SECTION("这幅手牌是个排炸，单个炸弹长度为4, 返回true") {
        int input[8] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22};
        REQUIRE(target.IsBomb(input, 8, false) == true);
    }
}

TEST_CASE("检查一副手牌是否是三王炸", "[static_analyser, IsBomb3W]") {
    StaticAnalyserC target;

    SECTION("手牌数量不为3，返回false") {
        int input[2] = {1,2};
        REQUIRE(target.IsBomb3W(input, 2) == false);
    }

    SECTION("手牌数量为3，但不都是司令，返回false") {
        int input[3] = {0x21, 0x4E, 0x4F};
        REQUIRE(target.IsBomb3W(input, 3) == false);
    }

    SECTION("手牌数量为3，且全是司令，返回true") {
        int input[3] = {0x4E, 0x4E, 0x4F};
        REQUIRE(target.IsBomb3W(input, 3) == true);
    }
}

TEST_CASE("检查一副手牌是否是天王炸", "[static_analyser, IsBombTW]") {
    StaticAnalyserC target;

    SECTION("手牌数量不为4，返回false") {
        int input[2] = {1,2};
        REQUIRE(target.IsBombTW(input, 2) == false);
    }

    SECTION("手牌数量为4，但不都是司令，返回false") {
        int input[4] = {0x21, 0x4E,0x4E, 0x4F};
        REQUIRE(target.IsBombTW(input, 4) == false);
    }

    SECTION("手牌数量为4，且全是司令，返回true") {
        int input[4] = {0x4E, 0x4E, 0x4F, 0x4F};
        REQUIRE(target.IsBombTW(input, 4) == true);
    }
    
}

TEST_CASE("检查一副手牌是否是排炸", "[static_analyser, IsBombLinkj]") {
    StaticAnalyserC target;

    SECTION("如果单个炸弹长度无法整除手牌长度，返回false") {
        int input[9] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x23};
        REQUIRE(target.IsBombLink(input, 9, 4) == false);
    }

    SECTION("如果存在着一个点数的牌不成炸弹，返回false") {
        int input[8] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x23};
        REQUIRE(target.IsBombLink(input, 8, 4) == false);
    }

    SECTION("全是炸弹，但是点数不连续，返回false") {
        int input[8] = {0x21,0x21,0x21,0x21,0x23,0x23,0x23,0x23};
        REQUIRE(target.IsBombLink(input, 8, 4) == false);
    }

    SECTION("全是炸弹，且点数连续，没有财神，返回true") {
        int input[8] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22};
        REQUIRE(target.IsBombLink(input, 8, 4) == true);
        int input2[12] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x23,0x23,0x23,0x23};
        REQUIRE(target.IsBombLink(input2, 12, 4) == true);
    }

    SECTION("全是炸弹，且点数连续，使用了财神") {
        int input[8] = {0x21,0x21,0x21,0x4E,0x22,0x22,0x22,0x22};
        REQUIRE(target.IsBombLink(input, 8, 4) == true);
    }
}

TEST_CASE("获取一组手牌中各种点数的扑克牌的数量", "[static_analyser, DistributionByValue]") {
    StaticAnalyserC target;

    int input[12] = {0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x23,0x23,0x23,0x23,0x23};
    const int type[] = {1, 2, 3};
    const int arr[] = {3, 4, 5};
    std::vector<int> dist(arr, arr+3);
    std::vector<int> type_value(type, type+3);
    std::vector<int> result[2] = {type_value, dist};
    std::vector<int> distribution[2];

    target.DistributionByValue(input, 12, distribution);
    REQUIRE(std::equal(distribution, distribution+2, result) == true);
}

}

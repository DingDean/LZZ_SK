#include "common.h"
#include "catch.hpp"

namespace skrobot {

TEST_CASE("获得扑克的点数", "[skrobot]") {
    REQUIRE(GetCardValue(0x21) == 1);
    REQUIRE(GetCardValue(0x32) == 2);
}

TEST_CASE("获得扑克的花色", "[skrobot]") {
    REQUIRE(GetCardColor(0x21) == 0x20);
    REQUIRE(GetCardColor(0x31) == 0x30);
}

TEST_CASE("获得扑克的逻辑点数", "[comon, GetCardLogicValue]") {
    REQUIRE(GetCardLogicValue(0x21) == 0x0E);
    REQUIRE(GetCardLogicValue(0x22) == 0x0F);
    REQUIRE(GetCardLogicValue(0x23) == 0x03);
    REQUIRE(GetCardLogicValue(0x4E) == 0x10);
    REQUIRE(GetCardLogicValue(0x4F) == 0x11);
}

TEST_CASE("获取手牌中财神的数量", "[common, NumTrump]") {
    int input[5] = {1,2,3, 0x4E, 0x4F};
    REQUIRE(NumTrump(input, 5) == 2);
}

TEST_CASE("获取手牌中点数为具体某个数字的牌的数量", "[common, NumCard]") {
    int input[5] = {0x21, 0x21, 0x22, 0x21, 0x22};
    REQUIRE(NumCardByValue(input, 5, 2) == 2);
    REQUIRE(NumCardByValue(input, 5, 1) == 3);
}

//TEST_CASE("将手牌按照点数排序 SortByValue", "[skrobot, common]") {
    //int input[5] = {0x21, 0x31, 0x22, 0x21, 0x33};
    //int ouput[5] = {0x21, 0x21, 0x31, 0x22, 0x33};
    //SortByValue(input, 5);
    //REQUIRE(input == ouput);
//}


} //namespace skrobot








#include "catch.hpp"
#include "static_analyser.h"
#include "common.h"
#include <vector>

namespace skrobot {

    
TEST_CASE("检查一手牌是不是对子", "[StaticAnalyserC, isDouble]") {
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

TEST_CASE("检查一手牌是不是三条", "[StaticAnalyserC, IsTriple]") {
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

TEST_CASE("检查一手牌是不是顺子", "[StaticAnalyserC, IsShunZi]") {
    StaticAnalyserC target;

    SECTION("手牌长度小于５，返回false") {
        int input[4] = {1,2,3,4};
        REQUIRE(target.IsShunZi(input, 4) == false);
    }

    SECTION("手牌长度大于等于５，但牌的点数间有间隔，返回false") {
        int input[5] = {0x23,0x24,0x25,0x27,0x28};
        REQUIRE(target.IsShunZi(input, 5) == false);
    }

    SECTION("手牌长度大于等于5，且牌都是连贯的，没有财神，返回true") {
        int input[8] = {0x29, 0x2A, 0x33, 0x34, 0x27, 0x26, 0x25, 0x18};
        REQUIRE(target.IsShunZi(input, 8) == true);
    }

    SECTION("手牌长度大于等于5，虽然有n张牌不连贯，但有n张财神，返回true") {
        int input[6] = {0x33, 0x34, 0x27, 0x4E, 0x25, 0x18};
        REQUIRE(target.IsShunZi(input, 6) == true);
    }

    SECTION("手牌长度大于等于5，虽然有n张牌不连贯，但只有n-1张财神，返回false") {
        int input[5] = {0x29, 0x24, 0x25, 0x26, 0x4E};
        REQUIRE(target.IsShunZi(input, 5) == false);
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

    SECTION("2不能出现在对连中") {
        int input[6] = {0x22,0x22,0x21,0x21,0x2F,0x2F};
        REQUIRE(target.IsDoubleLink(input, 6) == false);
    }

    SECTION("数组长度符合条件，但存在落单的牌, 且没有财神，返回false") {
        int input[6] = {0x23, 0x23, 0x24, 0x24, 0x25, 0x26};
        REQUIRE(target.IsDoubleLink(input, 6) == false);
    }

    SECTION("数组的长度符合条件，全是对子，但点数不连贯, 且没有财神, 返回false") {
        int input[6] = {0x21, 0x31, 0x22, 0x12, 0x34, 0x34};
        REQUIRE(target.IsDoubleLink(input, 6) == false);
    }

    SECTION("数组的长度符合条件，全是对子，且点数连贯,没有财神, 返回true") {
        int input[6] = {0x24, 0x34, 0x25, 0x15, 0x33, 0x33};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }

    SECTION("数组的长度符合条件，一张单牌，一张财神，点数连贯，返回true") {
        int input[6] = {0x23, 0x23, 0x24,0x24,0x25,0x4E};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }

    SECTION("数组的长度符合条件，两张单牌，两张财神，点数连贯，返回true") {
        int input[6] = {0x25, 0x25, 0x24, 0x4E, 0x33, 0x4E};
        REQUIRE(target.IsDoubleLink(input, 6) == true);
    }
}

TEST_CASE("检查一手牌是否是三连", "[StaticAnalyserC, IsTripleLink]") {
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
        int input[9] = {0x23,0x23,0x23,0x24,0x24,0x24,0x25,0x25,0x26};
        REQUIRE(target.IsTripleLink(input, 9) == false);
    }

    SECTION("数组的长度符合条件，全是三条，但点数不连贯, 返回false") {
        int input[9] = {0x23,0x23,0x23,0x25,0x25,0x25,0x27,0x27,0x27};
        REQUIRE(target.IsTripleLink(input, 9) == false);
    }

    SECTION("数组的长度符合条件，全是三条，且点数连贯, 返回true") {
        int input[9] = {0x23,0x23,0x23,0x24,0x24,0x24,0x25,0x25,0x25};
        REQUIRE(target.IsTripleLink(input, 9) == true);
    }

    SECTION("数组的长度符合条件，有一张财神，在非财神牌中，有一个点数的牌成双，其余成三条, 返回true") {
        int input[9] = {0x23,0x23,0x23,0x24,0x24,0x24,0x25,0x25,0x4F};
        REQUIRE(target.IsTripleLink(input , 9) == true);
    }
}

TEST_CASE("检查一副手牌是否是炸弹", "[StaticAnalyserC, IsBomb]") {
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
        int input[12] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x1D,0x1D,0x1D,0x1D};
        REQUIRE(target.IsBomb(input, 12, false) == true);
    }

    SECTION("六张牌，一个三条，一个天王炸，返回true") {
        int input[6] = {0x21,0x21,0x21,0x4F,0x4E,0x4E};
        REQUIRE(target.IsBomb(input, 6, false) == true);
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

TEST_CASE("检查一副手牌是否是天王炸", "[StaticAnalyserC, IsBombTW]") {
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

TEST_CASE("检查一副手牌是否是排炸", "[StaticAnalyserC, IsBombLink]") {
    StaticAnalyserC target;

    SECTION("排炸必须至少是三连炸") {
        int input[8] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22};
        REQUIRE(target.IsBombLink(input, 8) == false);
    }

    SECTION("如果存在着一个点数的牌不成炸弹，返回false") {
        int input[12] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x23,0x24,0x24,0x24,0x24};
        REQUIRE(target.IsBombLink(input, 12) == false);
    }

    SECTION("全是炸弹，但是点数不连续，返回false") {
        int input[12] = {0x21,0x21,0x21,0x21,0x23,0x23,0x23,0x23,0x24,0x24,0x24,0x24};
        REQUIRE(target.IsBombLink(input, 12) == false);
    }

    SECTION("全是炸弹，且点数连续，没有财神，返回true") {
        int input[12] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x1D,0x1D,0x1D,0x1D};
        REQUIRE(target.IsBombLink(input, 12) == true);
    }

    SECTION("全是炸弹，且点数连续，使用了财神") {
        int input[12] = {0x21,0x21,0x21,0x4E,0x22,0x22,0x22,0x22,0x1D,0x2D,0x3D,0x1D};
        REQUIRE(target.IsBombLink(input, 12) == true);
    }

    SECTION("千变双扣中，炸弹的长度不一定相同") {
        int input[13] = {0x21,0x21,0x21,0x21,0x4E,0x22,0x22,0x22,0x22,0x1D,0x2D,0x3D,0x1D};
        REQUIRE(target.IsBombLink(input, 13) == true);
    }
}

TEST_CASE("获取一组手牌中各种点数的扑克牌的数量", "[StaticAnalyserC, DistributionByValue]") {
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

TEST_CASE("获取一组手牌的描述符", "[StaticAnalyserC, GenHandDescriptor]") {
    StaticAnalyserC target;

    SECTION("可以识别出顺子") {
        int input[8] = {0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A};
        REQUIRE(target.GenHandDescriptor(input, 8) == true);
        HandDescriptor desc = target.GetDescriptor();
        REQUIRE(desc.hand_type      == kHandTypeShunzi);
        REQUIRE(desc.start_value    == GetCardLogicValue(0x23));
        REQUIRE(desc.block_len      == 1);
        REQUIRE(desc.num_of_blocks  == 8);
    }

    SECTION("可以识别出对子") {
        int input[2] = {0x22, 0x22};
        REQUIRE(target.GenHandDescriptor(input, 2) == true);
        HandDescriptor desc = target.GetDescriptor();
        REQUIRE(desc.hand_type      == kHandTypeDouble);
        REQUIRE(desc.start_value    == GetCardLogicValue(0x22));
        REQUIRE(desc.block_len      == 2);
        REQUIRE(desc.num_of_blocks  == 1);
    }

    SECTION("可以识别出三条") {
        int input[3] = {0x22, 0x22, 0x22};
        REQUIRE(target.GenHandDescriptor(input, 3) == true);
        HandDescriptor desc = target.GetDescriptor();
        REQUIRE(desc.hand_type      == kHandTypeTriple);
        REQUIRE(desc.start_value    == GetCardLogicValue(0x22));
        REQUIRE(desc.block_len      == 3);
        REQUIRE(desc.num_of_blocks  == 1);
    }

    SECTION("可以识别出对连") {
        int input[6] = {0x33,0x33,0x24,0x14,0x15,0x35};
        REQUIRE(target.GenHandDescriptor(input, 6) == true);
        HandDescriptor desc = target.GetDescriptor();
        REQUIRE(desc.hand_type      == kHandTypeDoubleLink);
        REQUIRE(desc.start_value    == GetCardLogicValue(0x33));
        REQUIRE(desc.block_len      == 2);
        REQUIRE(desc.num_of_blocks  == 3);
    }

    SECTION("可以识别出三连") {
        int input[9] = {0x23,0x23,0x23,0x24,0x24,0x24,0x25,0x25,0x25};
        REQUIRE(target.GenHandDescriptor(input, 9) == true);
        HandDescriptor desc = target.GetDescriptor();
        REQUIRE(desc.hand_type      == kHandTypeTripleLink);
        REQUIRE(desc.start_value    == GetCardLogicValue(0x23));
        REQUIRE(desc.block_len      == 3);
        REQUIRE(desc.num_of_blocks  == 3);
    }

    SECTION("可以识别炸弹") {
        SECTION("可以识别三王炸") {
            int input[3] = {0x4E, 0x4E, 0x4F};
            REQUIRE(target.GenHandDescriptor(input, 3) == true);
            HandDescriptor desc = target.GetDescriptor();
            REQUIRE(desc.hand_type      == kHandTypeBomb3W);
            REQUIRE(desc.start_value    == GetCardLogicValue(0x4E));
            REQUIRE(desc.block_len      == 3);
            REQUIRE(desc.num_of_blocks  == 1);
        }

        SECTION("可以识别天王炸") {
            int input[4] = {0x4E, 0x4E, 0x4F, 0x4F};
            REQUIRE(target.GenHandDescriptor(input, 4) == true);
            HandDescriptor desc = target.GetDescriptor();
            REQUIRE(desc.hand_type      == kHandTypeBombTW);
            REQUIRE(desc.start_value    == GetCardLogicValue(0x4E));
            REQUIRE(desc.block_len      == 4);
            REQUIRE(desc.num_of_blocks  == 1);
        }

        SECTION("可以识别排炸") {
            int input[12] = {0x21,0x21,0x21,0x21,0x22,0x22,0x22,0x22,0x1D,0x1D,0x1D,0x1D};
            REQUIRE(target.GenHandDescriptor(input, 12) == true);
            HandDescriptor desc = target.GetDescriptor();
            REQUIRE(desc.hand_type      == kHandTypeBombLink);
            REQUIRE(desc.start_value    == GetCardLogicValue(0x1D));
            REQUIRE(desc.block_len      == 4);
            REQUIRE(desc.num_of_blocks  == 12);
        }
    }
}

TEST_CASE("针对一张牌X，从手牌中获得所有可以压制这张牌的散牌", "[StaticAnalyserC, OptionsSingleCard]") {
    StaticAnalyserC target;

    SECTION("X为2，则只有司令可以压制它") {
        SECTION("有司令") {
            int hand[5] = {0x22, 0x21, 0x23, 0x4E, 0x4F};
            iVector output;
            int card = 0x22;
            int result[2] = {0x4E, 0x4F};
            iVector i_result(result, result+2);
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 2);
            REQUIRE(std::equal(output.begin(), output.end(), i_result.begin()) == true);
        }

        SECTION("没有司令") {
            int hand[5] = {0x22, 0x21, 0x23, 0x24, 0x25};
            iVector output;
            int card = 0x22;
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 0);
        }
    }

    SECTION("X为A，只有司令和2可以压制它") {
        SECTION("有司令，有2") {
            int hand[5] = {0x22, 0x21, 0x23, 0x4E, 0x4F};
            iVector output;
            int card = 0x21;
            int result[3] = {0x22, 0x4E, 0x4F};
            iVector i_result(result, result+3);
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 3);
            REQUIRE(std::equal(output.begin(), output.end(), i_result.begin()) == true);
        }

        SECTION("有司令，没2") {
            int hand[5] = {0x23, 0x21, 0x23, 0x4E, 0x4F};
            iVector output;
            int card = 0x21;
            int result[2] = {0x4E, 0x4F};
            iVector i_result(result, result+2);
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 2);
            REQUIRE(std::equal(output.begin(), output.end(), i_result.begin()) == true);
        }

        SECTION("没司令，有2") {
            int hand[5] = {0x22, 0x21, 0x23, 0x25, 0x2D};
            iVector output;
            int card = 0x21;
            int result[1] = {0x22};
            iVector i_result(result, result+1);
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 1);
            REQUIRE(std::equal(output.begin(), output.end(), i_result.begin()) == true);
        }

        SECTION("没司令，没2") {
            int hand[5] = {0x2A, 0x21, 0x23, 0x25, 0x2D};
            iVector output;
            int card = 0x21;

            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 0);
        }
    }

    SECTION("副司令只能被正司令压制，正司令单张无敌") {

        SECTION("X是副司令, 且有正司令") {
            int hand[5] = {0x22, 0x21, 0x23, 0x4E, 0x4F};
            iVector output;
            int card = 0x4E;
            int result[1] = {0x4F};
            iVector i_result(result, result+1);
            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 1);
            REQUIRE(std::equal(output.begin(), output.end(), i_result.begin()) == true);
        }

        SECTION("X是副司令, 但无正司令") {
            int hand[5] = {0x22, 0x21, 0x23, 0x4E, 0x3A};
            iVector output;
            int card = 0x4E;

            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 0);
        }

        SECTION("X是正司令, 但无正司令") {
            int hand[5] = {0x4F,0x4F,0x4F,0x4F,0x4F};
            iVector output;
            int card = 0x4F;

            REQUIRE(target.OptionsSingleCard(hand, 5, card, &output) == true);
            REQUIRE(output.size() == 0);
        }
    }
}

TEST_CASE("OptionsXples从手牌中获取可以压制对手的对子或者三条", "[StaticAnalyserC, OptionsXples]") {
    StaticAnalyserC target;

    SECTION("获取所有可能的对子") {
        int hand[8] = {0x23,0x24,0x24,0x25,0x25,0x25,0x4E};
        int combo_card = 0x23;
        int combo_len = 2;
        iVector output_options;
        target.OptionsXples(hand, 8, combo_card, combo_len, &output_options);
        REQUIRE(output_options.size() == 12);
    }

    SECTION("获取所有可能的三条") {
        int hand[8] = {0x23,0x24,0x24,0x25,0x25,0x25,0x4E};
        int combo_card = 0x23;
        int combo_len = 3;
        iVector output_options;
        target.OptionsXples(hand, 8, combo_card, combo_len, &output_options);
        REQUIRE(output_options.size() == 12);
    }

    SECTION("能使用OptinonsXples找出手牌中所有的x张") {
        int hand[9] = {0x23,0x23,0x24,0x24,0x25,0x25,0x25,0x4E};
        int combo_card = 0;
        int combo_len = 3;
        iVector output_options;
        target.OptionsXples(hand, 9, combo_card, combo_len, &output_options);
        REQUIRE(output_options.size() == 16);
    }
}

TEST_CASE("OptionsXLinks 从手牌中获取所有可以压制对手的连牌", "[StaticAnalyserC, OptionsXLinks]") {
    StaticAnalyserC target;
    SECTION("获得所有的顺子") {
        int hand[8] = {0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A};
        int link_start_card = 0x23;
        int link_block_len  = 1;
        int link_total_len  = 5;
        iVector output_options;
        target.OptionsXLinks(hand, 8, link_start_card, link_block_len, link_total_len, &output_options);
        REQUIRE(output_options.size() == 18);
    }
}

TEST_CASE("MakeXplesWithTrump将司令和已有的组合整合在一起做出连牌", "[tags]") {
    StaticAnalyserC target;

    SECTION("什么都不制作") {
        TrumpDescriptor trump_desc = {1,1,0};
        int combo_len = 2;
        iVector output_options;
        target.MakeXplesWithTrump(0,0,combo_len, trump_desc, &output_options);
        REQUIRE(output_options.size() == 0);
    }

    SECTION("制作对子") {
        int base = 0x22;
        int base_len = 1;
        int trump_needed = 1;
        TrumpDescriptor trump_desc = {3,2,1};
        iVector output_options;
        target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

        int len = output_options.size();
        REQUIRE(len == 6);
    }

    SECTION("司令不够") {
        int base = 0x22;
        int base_len = 1;
        int trump_needed = 1;
        TrumpDescriptor trump_desc = {0,0,0};
        iVector output_options;
        target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

        int len = output_options.size();
        REQUIRE(len == 0);
    }

    SECTION("制作三条") {
        SECTION("base_len为１, 有四张司令") {
            int base = 0x22;
            int base_len = 1;
            int trump_needed = 2;
            TrumpDescriptor trump_desc = {4,2,2};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 12);
        }
        SECTION("base_len为１, 有三张司令") {
            int base = 0x22;
            int base_len = 1;
            int trump_needed = 2;
            TrumpDescriptor trump_desc = {3,2,1};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 8);
        }

        SECTION("base_len为１, 有两张正司令") {
            int base = 0x22;
            int base_len = 1;
            int trump_needed = 2;
            TrumpDescriptor trump_desc = {2,2,0};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 4);
        }

        SECTION("base_len为１, 有正副司令各一张") {
            int base = 0x22;
            int base_len = 1;
            int trump_needed = 2;
            TrumpDescriptor trump_desc = {2,1,1};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 4);
        }

        SECTION("base_len为2, 有两种司令") {
            int base = 0x22;
            int base_len = 2;
            int trump_needed = 1;
            TrumpDescriptor trump_desc = {4,2,2};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 8);
        }

        SECTION("base_len为2,只有一种司令") {
            int base = 0x22;
            int base_len = 2;
            int trump_needed = 1;
            TrumpDescriptor trump_desc = {2,2,0};
            iVector output_options;
            target.MakeXplesWithTrump(base, base_len, trump_needed, trump_desc,  &output_options);

            int len = output_options.size();
            REQUIRE(len == 4);
        }
    }
}

/*TEST_CASE("找出手牌中所有符合一定要求的炸弹", "[StaticAnalyserC, OptionsBombs]") {*/
    /*StaticAnalyserC target;*/

    /*SECTION("能找出所有的普通炸弹") {*/
        /*int hand[13] = {0x23,0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x25,0x25,0x25,0x25,0x25};*/
        /*iVector output_options;*/
        /*target.OptionsBombs(hand, 13, 0x13, 4, 1, &output_options);*/
        /*REQUIRE(output_options.size() == 16);*/
        /*int result[16] = {*/
            /*4,0x24,0x24,0x24,0x24,*/
            /*4,0x25,0x25,0x25,0x25,*/
            /*5,0x25,0x25,0x25,0x25,0x25*/
        /*};*/
        /*iVector i_result(result, result+16);*/
        /*REQUIRE(std::equal(output_options.begin(), output_options.end(), i_result.begin()));*/
    /*}*/

    /*SECTION("能找出带有司令的炸弹") {*/
        /*int hand[14] = {0x25,0x25,0x25,0x24,0x24,0x24,0x24,0x4E,0x4F};*/
        /*iVector output_options;*/
        /*target.OptionsBombs(hand, 14, 0x13, 4, 1, &output_options);*/
        /*REQUIRE(output_options.size() == 40);*/
        /*int result[40] = {4,0x25,0x25,0x25,0x4E,4,0x25,0x25,0x25,0x4F,5,0x25,0x25,0x25,0x4E,0x4F,4,0x24,0x24,0x24,0x24,5,0x24,0x24,0x24,0x24,0x4E,5,0x24,0x24,0x24,0x24,0x4F,6,0x24,0x24,0x24,0x24,0x4E,0x4F};*/
        /*iVector i_result(result, result+40);*/
        /*REQUIRE(std::equal(output_options.begin(), output_options.end(), i_result.begin()));*/
    /*}*/
/*}*/

TEST_CASE("FindBomb3TW 找到手牌中的三王炸", "[StaticAnalyserC, FindBomb3TW]") {
    StaticAnalyserC target;

    SECTION("手上有两个副司令，一个正司令") {
        int hand[3] = {0x4E,0x4E,0x4F};
        iVector output_options;
        target.FindBomb3TW(hand, 3, &output_options);
        REQUIRE(output_options.size() == 4);
    }

    SECTION("手上有两个正司令，一个副司令") {
        int hand[3] = {0x4F,0x4F,0x4E};
        iVector output_options;
        target.FindBomb3TW(hand, 3, &output_options);
        REQUIRE(output_options.size() == 4);
    }

    SECTION("手上有两个正司令，两个副司令") {
        int hand[4] = {0x4F,0x4F,0x4E,0x4E};
        iVector output_options;
        target.FindBomb3TW(hand, 4, &output_options);
        REQUIRE(output_options.size() == 13);
    }

    SECTION("手上司令数不够三个") {
        int hand[3] = {0x21, 0x4E, 0x4E};
        iVector output_options;
        target.FindBomb3TW(hand, 3, &output_options);
        REQUIRE(output_options.size() == 0);
    }
}

TEST_CASE("FindNormalBomb 找到手牌中所有普通的炸弹", "[StaticAnalyserC, FindNormalBomb]") {
    StaticAnalyserC target;

    SECTION("有司令") {
        int hand[8] = {0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x4E};
        iVector output_options;
        target.FindNormalBomb(hand, 8, &output_options);
        REQUIRE(output_options.size() == 21);
    }

    SECTION("没有司令") {
        int hand[10] = {0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x24,0x24,0x23};
        iVector output_options;
        target.FindNormalBomb(hand, 10, &output_options);
        REQUIRE(output_options.size() == 23);
    }
}


// TODO:2017-02-14
// 完成测试
TEST_CASE("FindBombLink 找到手牌中所有的排炸", "[StaticAnalyserC, FindBombLink]") {
    StaticAnalyserC target;

    SECTION("如果手牌中有一个点数的牌即使配合上所有的司令时都无法成炸弹的话，那肯定就没有排炸") {
        int hand[8] = {0x23,0x4F,0x4E,0x24,0x24,0x24,0x24};
        iVector output_options;
        target.FindBombLink(hand, 8, &output_options);
        REQUIRE(output_options.size() == 0);
    }

    SECTION("如果手牌中有") {
        
    }
}

//// TODO:
//TEST_CASE("用司令来凑普通炸弹", "[StaticAnalyserC, MakeBombsWithTrump]") {
    //StaticAnalyserC target;

    //SECTION("") {
        //int hand[6] = {0x24,0x24,0x24,0x4E,0x4F,0x4E};
        //iVector output_options;
        //TrumpDescriptor trump_desc;
        //NumTrump(hand, 6, &trump_desc);

        //target.MakeBombsWithTrump(hand, 6, 0, 3, 4, 6, trump_desc, &output_options);
        //REQUIRE(output_options.size() == 29);
    //}
//}

}

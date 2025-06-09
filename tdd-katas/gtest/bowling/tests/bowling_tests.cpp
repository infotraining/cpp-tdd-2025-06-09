#include "bowling.hpp"

#include <algorithm>
#include <array>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <numeric>
#include <string>

using namespace std;
using namespace TDD;

TEST(BowlingGameTests, WhenGameStarts_ThenScoreIsZero)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}

struct BowlingGameFixture : ::testing::Test
{
    const int rolls_in_game = 20;
    BowlingGame game;

    void roll_many(uint32_t count, uint32_t pins)
    {
        for (int i = 0; i < count; ++i)
            game.roll(pins);
    }
};

struct BowlingGame_NoEvents : BowlingGameFixture
{};

TEST_F(BowlingGame_NoEvents, WhenAllRollsInGutter_ThenScoreIsZero)
{
    roll_many(rolls_in_game, 0);

    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGame_NoEvents, WhenAllRollsWithNoBonus_ThenScoreIsSumOfPins)
{
    roll_many(rolls_in_game, 1);

    ASSERT_EQ(game.score(), 20);
}

struct BowlingGame_WithEvents : BowlingGameFixture
{
    void roll_spare()
    {
        game.roll(1);
        game.roll(9);
    }

    void roll_strike()
    {
        game.roll(10);
    }
};

TEST_F(BowlingGame_WithEvents, WhenSpare_NextRollIsCountedTwice)
{
    roll_spare();
    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29);
}

TEST_F(BowlingGame_WithEvents, WhenStrike_TwoNextRollsAreCountedTwice)
{
    roll_strike();
    game.roll(6);
    game.roll(2);
    roll_many(16, 1);

    ASSERT_EQ(game.score(), 42);
}

struct BowlingGame_LastFrame : BowlingGame_WithEvents
{
    void SetUp() override
    {
        roll_many(18, 1);
    }
};

TEST_F(BowlingGame_LastFrame, WhenStrike_TwoExtraRollsAreCounted)
{    
    roll_strike();
    roll_many(2, 1);

    ASSERT_EQ(game.score(), 30);
}

TEST_F(BowlingGame_LastFrame, WhenSpare_OneExtraRollIsCounted)
{
    roll_spare();
    game.roll(1);

    ASSERT_EQ(game.score(), 29);
}

struct PerfectGame : BowlingGame_WithEvents
{};

TEST_F(PerfectGame, ScoreIs300)
{
    for(int i = 0; i < 12; ++i)
        roll_strike();

    ASSERT_EQ(game.score(), 300);
}

struct BowlingGameParams
{
    const char* test_description;
    std::vector<uint32_t> rolls;
    size_t expected_score;
};

std::ostream& operator<<(std::ostream& out, const BowlingGameParams& params)
{
    out << params.test_description;
    return out;
}

struct BowlingGameParamTests : ::testing::TestWithParam<BowlingGameParams>
{
    BowlingGame game;
};

TEST_P(BowlingGameParamTests, RealGameExamples)
{
    const BowlingGameParams param = GetParam();

    for(size_t pins : param.rolls)
    {
        game.roll(pins);
    }

    ASSERT_EQ(game.score(), param.expected_score);
}

BowlingGameParams params[] = {
    { "simple game - all rolls one pin", { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 20 },
    { "simple game - different rolls", {0, 8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 27},
    { "strike & spare", {10, 4, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 47},
    { "all spares & strike", {1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 10}, 119},
    { "perfect game", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, 300}
};

INSTANTIATE_TEST_SUITE_P(PackOfBowlingTests, BowlingGameParamTests, ::testing::ValuesIn(params));
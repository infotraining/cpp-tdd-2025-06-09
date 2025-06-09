#include <algorithm>
#include <string>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{
public:
    constexpr BowlingGame() noexcept : score_{0u} {}

    [[nodiscard]] uint32_t score() const noexcept
    {
        return score_;
    }

    void roll(uint32_t pins) noexcept
    {
        score_ += pins;
    }

private:
    uint32_t score_;
};

TEST(BowlingGameTests, WhenGameStarts_ThenScoreIsZero)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}

struct BowlingGame_SimpleRolls : ::testing::Test
{
    const int rolls_in_game = 20;
    BowlingGame game; 

    void roll_many(uint32_t count, uint32_t pins)
    {
        for(int i = 0; i < count; ++i) 
            game.roll(pins);
    }
};

TEST_F(BowlingGame_SimpleRolls, WhenAllRollsInGutter_ThenScoreIsZero)
{    
    roll_many(rolls_in_game, 0);

    ASSERT_EQ(game.score(), 0); 
}

TEST_F(BowlingGame_SimpleRolls, WhenAllRollsWithNoBonus_ThenScoreIsSumOfPins)
{
    roll_many(rolls_in_game, 1);

    ASSERT_EQ(game.score(), 20);
}

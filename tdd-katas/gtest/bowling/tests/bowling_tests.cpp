#include "bowling.hpp"

#include <algorithm>
#include <array>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <numeric>
#include <string>

using namespace std;

class BowlingGame
{
    static constexpr uint32_t MAX_PINS_IN_FRAME = 10;
    static constexpr size_t MAX_ROLLS_IN_GAME = 20;
public:
    constexpr BowlingGame() noexcept { }

    [[nodiscard]] uint32_t score() const noexcept
    {
        uint32_t result = 0;
        for(size_t roll_index = 0; roll_index < pins_.size(); roll_index+=2)
        {
            result += frame_score(roll_index);
            
            if (is_spare(roll_index)) 
                result += spare_bonus(roll_index);        
        }

        return result;
    }

    void roll(uint32_t pins) noexcept
    {
        pins_[roll_index_] = pins;
        ++roll_index_;        
    }

private:
    std::array<uint32_t, MAX_ROLLS_IN_GAME> pins_{};
    size_t roll_index_{};

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1] == MAX_PINS_IN_FRAME;
    }

    uint32_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index+2];
    }

    uint32_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1];
    }
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
        for (int i = 0; i < count; ++i)
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

struct BowlingGame_WithEvents : BowlingGame_SimpleRolls
{
    void roll_spare()
    {
        game.roll(1);
        game.roll(9); 
    }
};

TEST_F(BowlingGame_WithEvents, WhenSpare_NextIsRollIsCountedTwice)
{
    roll_spare();
    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29);
}

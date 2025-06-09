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
        for (size_t roll_index = 0; roll_index < pins_.size(); roll_index += 2)
        {
            result += frame_score(roll_index);

            if(is_strike(roll_index))
                result += strike_bonus(roll_index);
            else if (is_spare(roll_index))
                result += spare_bonus(roll_index);
        }

        return result;
    }

    void roll(uint32_t pins) noexcept
    {
        pins_[roll_index_] = pins;

        if (is_strike(roll_index_))
            ++roll_index_;

        ++roll_index_;
    }

private:
    std::array<uint32_t, MAX_ROLLS_IN_GAME> pins_{};
    size_t roll_index_{};

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1] == MAX_PINS_IN_FRAME;
    }

    bool is_strike(size_t roll_index) const
    {
        return roll_index_ % 2 == 0 && pins_[roll_index] == MAX_PINS_IN_FRAME;
    }

    uint32_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    uint32_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2] + pins_[roll_index + 3];
    }

    uint32_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1];
    }
};

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

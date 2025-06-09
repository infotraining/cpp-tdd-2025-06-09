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
    static constexpr size_t MAX_ROLLS_IN_GAME = 21;

public:
    constexpr BowlingGame() noexcept { }

    [[nodiscard]] uint32_t score() const noexcept
    {
        uint32_t result = 0;
        for (size_t frame_index = 0, roll_index = 0; frame_index < 10; ++frame_index)
        {
            const auto [score_in_frame, frame_size] = frame_score(roll_index);
            result += score_in_frame;

            if(is_strike(roll_index))
                result += strike_bonus(roll_index);
            
            if (is_spare(roll_index))
                result += spare_bonus(roll_index);

            roll_index += frame_size;
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
        return pins_[roll_index] + pins_[roll_index + 1] == MAX_PINS_IN_FRAME;
    }

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == MAX_PINS_IN_FRAME;
    }

    uint32_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    uint32_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 1] + pins_[roll_index + 2];
    }

    std::pair<uint32_t, size_t> frame_score(size_t roll_index) const
    {
        if (is_strike(roll_index))
            return std::pair{MAX_PINS_IN_FRAME, 1};
        
        return std::pair{pins_[roll_index] + pins_[roll_index + 1], 2};
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
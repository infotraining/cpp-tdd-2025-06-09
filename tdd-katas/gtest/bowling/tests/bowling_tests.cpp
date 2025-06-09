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
    constexpr [[nodiscard]] uint32_t score() const noexcept
    {
        return 0;
    }
};

TEST(BowlingGameTests, InitGame_ScoreIsZero)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}
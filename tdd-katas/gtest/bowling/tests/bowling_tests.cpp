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
    [[nodiscard]] uint32_t score() const noexcept
    {
        return 0;
    }

    void roll(uint32_t pins) noexcept
    {        
    }
};

TEST(BowlingGameTests, WhenGameStarts_ThenScoreIsZero)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}

TEST(BowlingGameTests, WhenAllRollsInGutter_ThenScoreIsZero)
{
    const int rolls_in_game = 20;
    
    BowlingGame game; // Arrange / Given
    
    for(int i = 0; i < rolls_in_game; ++i) // Act / When
        game.roll(0);

    ASSERT_EQ(game.score(), 0); // Assert / Then
}
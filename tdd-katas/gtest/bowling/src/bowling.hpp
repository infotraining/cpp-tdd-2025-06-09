#ifndef BOWLING_HPP
#define BOWLING_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <array>

namespace TDD
{
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

                if (is_strike(roll_index))
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

}

#endif
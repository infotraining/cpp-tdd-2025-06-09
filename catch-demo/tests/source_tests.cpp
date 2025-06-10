#include "source.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <deque>
#include <list>
#include <set>

using namespace std;

struct Value
{
    int value;

    bool operator==(const Value& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Value& v)
    {
        out << "Value{" << v.value << "}";
        return out;
    }
};

Value operator+(const Value& v1, const Value& v2)
{
    return Value{v1.value + v2.value};
}

TEST_CASE("adding two numbers - operator+", "[math][operators][Value]")
{
    Value v1{42};
    Value v2{2};

    REQUIRE(v1 + v2 == Value{44});
}

TEST_CASE("Pushing items to vector")
{
    std::vector<int> vec = {1, 2, 3, 4};   // Arrange

    SECTION("push_back")
    {
        vec.push_back(42);  // Act

        SECTION("size is increased")
        {
            REQUIRE(vec.size() == 5);      // Assert
        }

        SECTION("capacity is increased")
        {
            REQUIRE(vec.capacity() >= 5);  // Assert
        }

        SECTION("item is inserted at back")
        {
            REQUIRE(vec.back() == 42);    // Assert
        }
    }
}

SCENARIO("Pushing items to vector - G/W/T")
{
    GIVEN("vector with items")
    {
        std::vector<int> vec = {1, 2, 3, 4};

        WHEN("push_back")
        {
            vec.push_back(42);

            THEN("size is increased")
            {
                REQUIRE(vec.size() == 5);
            }

            AND_THEN("capacity is increased")
            {
                REQUIRE(vec.capacity() >= 5);
            }

            AND_THEN("item is inserted at back")
            {
                REQUIRE(vec.back() == 42);
            }
        }
    }
}

/////////////////////////////////////////
// Parametrized tests

TEST_CASE("adding two numbers", "[math][operators][Value]")
{
    // const auto [a, b, expected] = GENERATE(
    //     table<int, int, int>(
    //         {
    //             {1, 2, 3},
    //             {42, 2, 44},
    //             {53, -1, 52}
    //         }
    //     )
    // );

    const auto [a, b, expected] = GENERATE(
        std::tuple{1, 2, 3},
        std::tuple{42, 2, 44},
        std::tuple{53, -1, 52}
    );

    DYNAMIC_SECTION("" << a << " + " << b << " = " << expected)
    {
        Value v1{a};
        Value v2{b};

        REQUIRE(v1 + v2 == Value{expected});
    }
}

TEMPLATE_TEST_CASE("vector can be resized", "[template]", int, std::string, float)
{
    std::vector<TestType> vec; // This setup will be done 3 times in total, once for each type

    CHECK(vec.size() == 0);
    vec.resize(10);
    REQUIRE(vec.size() == 10);
}

TEMPLATE_TEST_CASE("pushing items to container", "[template]", (std::vector<int>), (std::deque<int>), (std::list<int>))
{
    using Container = TestType;
    Container vec; // This setup will be done 3 times in total, once for each type

    auto pusher = [&vec]()
    {
        for (int i = 0; i < 1'000; ++i)
            vec.push_back(i);
        return vec.size();
    };

    BENCHMARK("pushing")
    {
        return pusher();
    };
}

int fibonacci(int n)
{
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return fibonacci(n - 1) + fibonacci(n - 2);
}

TEST_CASE("fibonacci")
{
    BENCHMARK("fibonacci(4)")
    {
        return fibonacci(4);
    };

    BENCHMARK("fibonacci(20)")
    {
        return fibonacci(20);
    };
}
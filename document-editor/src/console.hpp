#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <string>

class Console
{
public:
    [[no_discard]] virtual std::string read_line() = 0;
    virtual void print(const std::string& line) = 0;
    virtual ~Console() = default;
};

class Terminal : public Console
{
public:
    std::string read_line() override
    {
        std::string line;
        std::getline(std::cin, line);

        return line;
    }

    void print(const std::string& line) override
    {
        std::cout << line << std::endl;
    }
};

#endif // CONSOLE_HPP

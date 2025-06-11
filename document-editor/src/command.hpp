#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "clipboard.hpp"
#include "console.hpp"
#include "document.hpp"

#include <memory>
#include <stack>

class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class PrintCommand : public Command
{
    Document& document_;
    Console& console_;
public:
    PrintCommand(Document& doc, Console& console) : document_{doc}, console_{console}
    {}

    void execute() override
    {
        console_.print("---");
        for(const auto& line : document_.lines())
        {
            console_.print(line);
        }
        console_.print("---");
    }
};

class AddLineCommand : public Command
{
    Document& document_;
    Console& console_;
public:
    AddLineCommand(Document& doc, Console& console) : document_{doc}, console_{console}
    {}

    void execute() override
    {
        console_.print("Add new line:");
        auto line = console_.read_line();
        document_.add_line(std::move(line));
    }
};

#endif // COMMAND_HPP

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <unordered_map>

#include "console.hpp"
#include "command.hpp"

class EditorApp
{
public:
    EditorApp(Console& console)
        : console_(console)
    {
    }
    void run() 
    {
        while(true)
        {
            console_.print("> Enter a command:");
            std::string cmd_name = console_.read_line();

            if (cmd_name == "Exit")
                return;

            if (auto pos = commands_.find(cmd_name); pos != commands_.end())
            {
                auto [cmd_name, cmd] = *pos;
                cmd->execute();
            }
            else
            {
                console_.print("Unknown Command: " + cmd_name);
            }
        }
    }

    void add_command(std::string cmd_name, std::shared_ptr<Command> cmd)
    {
        commands_.emplace(std::move(cmd_name), std::move(cmd));
    }

private:
    Console& console_;
    std::unordered_map<std::string, std::shared_ptr<Command>> commands_;
};

#endif // APPLICATION_HPP

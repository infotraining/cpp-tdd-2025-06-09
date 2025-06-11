#include "application.hpp"
#include "command.hpp"
#include "document.hpp"
#include "mocks/mocks.hpp"
#include <unordered_map>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace std::literals;

class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

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
            console_.print("> Enter a command:"s);
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

TEST(EditorApp_Run, ShowsPromptForCommand)
{
    NiceMock<MockConsole> console;
    ON_CALL(console, read_line()).WillByDefault(Return("Exit"));
    EXPECT_CALL(console, print("> Enter a command:"s));

    EditorApp app{console};
    app.run();
}

TEST(EditorApp_Run, LoopsUntilExitCommand)
{
    NiceMock<MockConsole> console;
    EXPECT_CALL(console, read_line())
        .WillOnce(Return("Print"))
        .WillOnce(Return("Copy"))
        .WillOnce(Return("Exit"));
    
    EditorApp app{console};
    app.run();
}

class MockCommand : public Command
{
public:
    MOCK_METHOD(void, execute, (), (override));
};

TEST(EditorApp_Run, ExecutesCommand)
{
    NiceMock<MockConsole> console;
    EXPECT_CALL(console, read_line())
        .WillOnce(Return("Command"))
        .WillOnce(Return("Exit"));

    auto command = std::make_shared<MockCommand>();
    EXPECT_CALL(*command, execute()).WillOnce(Return());

    EditorApp app{console};
    app.add_command("Command", command);
    app.run();
}

TEST(EditorApp_Run, UnknownCommandPrintsErrorMessage)
{
    NiceMock<MockConsole> console;
    EXPECT_CALL(console, read_line())
        .WillOnce(Return("UnknownCommand"))
        .WillOnce(Return("Exit"));

    EXPECT_CALL(console, print(::testing::_)).WillRepeatedly(Return());
    EXPECT_CALL(console, print("Unknown Command: UnknownCommand"))
        .Times(1);

    EditorApp app{console};    
    app.run();
}

///////////////////////////
// Print command

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

TEST(PrintCommand_Execute, PrintsDocumentContentOnConsole)
{
    Document document{"Line1", "Line2"};
    MockConsole console;

    EXPECT_CALL(console, print("---")).Times(2);
    EXPECT_CALL(console, print("Line1"));
    EXPECT_CALL(console, print("Line2"));

    PrintCommand print_cmd{document, console};
    print_cmd.execute();
}
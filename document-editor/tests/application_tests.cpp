#include "application.hpp"
#include "command.hpp"
#include "document.hpp"
#include "mocks/mocks.hpp"
#include <unordered_map>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace std::literals;


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

TEST(AddLineCommand_Execute, AddsConsoleLineToDocument)
{
    MockConsole console;

    EXPECT_CALL(console, print("Add new line:"));
    EXPECT_CALL(console, read_line())
        .WillOnce(Return("Line3"));

    Document document{"Line1", "Line2"};

    AddLineCommand add_line_cmd{document, console};
    add_line_cmd.execute();

    ASSERT_THAT(document.lines(), ElementsAre("Line1", "Line2", "Line3"));
}
#include <iostream>

#include "application.hpp"
#include "command.hpp"
#include "console.hpp"
#include "document.hpp"
#include "clipboard.hpp"

using namespace std;

int main()
{
    Document doc;
    Terminal term;

    EditorApp app(term);

    auto print_cmd = std::make_shared<PrintCommand>(doc, term);
    auto add_line_cmd = std::make_shared<AddLineCommand>(doc, term);

    app.add_command("Print", print_cmd);
    app.add_command("AddLine", add_line_cmd);

    app.run();
}
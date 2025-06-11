#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using Content = std::vector<std::string>;

namespace Helpers
{
    inline std::string to_upper(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    inline std::string to_lower(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
}

class Document
{
    Content content_;

public:
    Document() noexcept
        : content_{}
    {
    }

    Document(std::initializer_list<std::string> lines)
        : content_{lines}
    {
    }

    std::string text() const
    {
        std::ostringstream oss;
        for (const auto& line : content_)
        {
            oss << line << '\n';
        }
        return oss.str();
    }

    Content lines() const
    {
        return content_;
    }

    void add_line(std::string line)
    {
        content_.push_back(std::move(line));
    }

    void to_upper()
    {
        std::transform(content_.begin(), content_.end(), content_.begin(), [](auto& line)
            { return Helpers::to_upper(line); });
    }

    void to_lower()
    {
        std::transform(content_.begin(), content_.end(), content_.begin(), [](auto& line)
            { return Helpers::to_lower(line); });
    }

    void clear()
    {
        content_.clear();
    }
};

#endif

#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include <mutex>
#include <string>

#include "document.hpp"

class SharedClipboard
{
    Content content_;
    mutable std::mutex content_mtx_;

public:
    static SharedClipboard& instance()
    {
        static SharedClipboard unique_instance;

        return unique_instance;
    }

    Content content() const
    {
        std::lock_guard<std::mutex> lk{content_mtx_};

        return content_;
    }

    void set_content(const Content& content)
    {
        std::lock_guard<std::mutex> lk{content_mtx_};

        content_ = content;
    }
};

#endif // CLIPBOARD_HPP

#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "document.hpp"

using namespace ::testing;

struct DocumentTests : ::testing::Test
{
    Document doc;
};

struct Document_DefaultConstructed : DocumentTests
{
};

TEST_F(Document_DefaultConstructed, Text_ReturnsEmptyString)
{
    ASSERT_THAT(doc.text(), StrEq(""));
}

TEST_F(Document_DefaultConstructed, Lines_ReturnsEmptyContent)
{
    ASSERT_THAT(doc.lines(), IsEmpty());
}

struct Document_ConstructedWithStrings : Test
{
    Document doc{"abc", "def"};
};

TEST_F(Document_ConstructedWithStrings, Text_ReturnsConcatenatedString)
{
    ASSERT_THAT(doc.text(), StrEq("abc\ndef\n"));
}

TEST_F(Document_ConstructedWithStrings, Lines_ReturnsContent)
{
    ASSERT_THAT(doc.lines(), ElementsAre("abc", "def"));
}

struct Document_Clear : Document_ConstructedWithStrings
{
};

TEST_F(Document_Clear, Text_ReturnsEmptyString)
{
    doc.clear();

    ASSERT_THAT(doc.text(), StrEq(""));
}

TEST_F(Document_Clear, Lines_ReturnsEmptyContent)
{
    doc.clear();

    ASSERT_THAT(doc.lines(), IsEmpty());
}

struct Document_AddLine : Document_ConstructedWithStrings
{
};

TEST_F(Document_AddLine, Text_LineIsAppendedAtTheEnd)
{
    doc.add_line("ghi");

    ASSERT_THAT(doc.text(), StrEq("abc\ndef\nghi\n"));
}

TEST_F(Document_AddLine, Lines_LineIsPushedBack)
{
    doc.add_line("ghi");

    ASSERT_THAT(doc.lines(), ElementsAre("abc", "def", "ghi"));
}

struct Document_CaseConversion : Test
{
    Document doc{"aBc", "DeF"};
};

TEST_F(Document_CaseConversion, ToUpperCase)
{
    doc.to_upper();

    ASSERT_THAT(doc.text(), StrEq("ABC\nDEF\n"));
}

TEST_F(Document_CaseConversion, ToLowerCase)
{
    doc.to_lower();

    ASSERT_THAT(doc.text(), StrEq("abc\ndef\n"));
}
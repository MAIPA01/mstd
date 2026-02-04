#include <mstd/terminal.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(TerminalTest, GetSizeValidValues) {
        int w = -1;
        int h = -1;

        get_terminal_size(w, h);

        EXPECT_GE(w, 0);
        EXPECT_GE(h, 0);

        std::cout << "[ INFO ] Detected Terminal Size: " << w << "x" << h << std::endl;
    }

    TEST(TerminalTest, ClearTerminalExecution) {
        EXPECT_NO_THROW({
            clear_terminal();
            });
    }

    TEST(TerminalTest, GetSizeConsistency) {
        int w1 = 0, h1 = 0;
        int w2 = 0, h2 = 0;

        get_terminal_size(w1, h1);
        get_terminal_size(w2, h2);

        EXPECT_EQ(w1, w2);
        EXPECT_EQ(h1, h2);
    }
}
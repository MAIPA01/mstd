#include <mstd/assert.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    struct TestLogger {
        static inline std::vector<std::string> logs;
        static void log(const std::string_view msg) {
            logs.emplace_back(msg);
        }
        static void clear() { logs.clear(); }
    };

    class AssertTest : public ::testing::Test {
    protected:
        void SetUp() override { TestLogger::clear(); }
    };

    TEST_F(AssertTest, LoggerIsCalledOnFailure) {
        bool failed = false;
        auto my_logger = [&](const std::string_view msg) {
            failed = true;
            EXPECT_FALSE(msg.empty());
            EXPECT_TRUE(msg.find("1 == 2") != std::string::npos);
            };

        mstd_log_assert_base(1 == 2, my_logger);

        EXPECT_TRUE(failed);
    }

    TEST_F(AssertTest, FormatLogContent) {
        std::string captured_msg;
        auto logger = [&](const std::string_view msg) { captured_msg = msg; };

        mstd_log_assert_base(2 + 2 == 5, logger);

        EXPECT_NE(captured_msg.find("Assertion failed"), std::string::npos);
        EXPECT_NE(captured_msg.find("2 + 2 == 5"), std::string::npos);
        EXPECT_NE(captured_msg.find("AssertTest"), std::string::npos);
    }

    TEST_F(AssertTest, FormatWithArguments) {
        std::string captured_msg;
        auto logger = [&](const std::string_view msg) { captured_msg = msg; };

        int value = 42;
        mstd_log_assert_base(value == 0, logger, "Value should be 0, but is {}", value);

        EXPECT_NE(captured_msg.find("Value should be 0, but is 42"), std::string::npos);
    }

    TEST_F(AssertTest, DoNotLogOnSuccess) {
        bool logger_called = false;
        auto logger = [&](const std::string_view) { logger_called = true; };

        mstd_log_assert_base(true, logger);

        EXPECT_FALSE(logger_called);
    }

    TEST_F(AssertTest, ConsistencyWithMockedLogger) {
        std::string s = "test";
        int i = 42;
        int call_count = 0;
        auto logger = [&](const std::string_view) { call_count++; };

        mstd_log_assert_base(!s.empty(), logger);
        mstd_log_assert_base(i == 42, logger);

        EXPECT_EQ(call_count, 0);
    }
}
#include <gtest/gtest.h>
#include <mstd/assert.hpp>

namespace mstd::test {
	struct TestLogger {
		static inline std::vector<std::string> logs;

		static void log(std::string_view const msg) { logs.emplace_back(msg); }

		static void clear() { logs.clear(); }
	};

	class AssertTest : public ::testing::Test {
	protected:
		void SetUp() override { TestLogger::clear(); }
	};

	TEST_F(AssertTest, LoggerIsCalledOnFailure) {
		bool failed	   = false;
		auto my_logger = [&](std::string_view const msg) {
			failed = true;
			EXPECT_FALSE(msg.empty());
			EXPECT_TRUE(msg.find("1 == 2") != std::string::npos);
		};

		MSTD_LOG_ASSERT_BASE(1 == 2, my_logger);

		EXPECT_TRUE(failed);
	}

	TEST_F(AssertTest, FormatLogContent) {
		std::string captured_msg;
		auto logger = [&](std::string_view const msg) { captured_msg = msg; };

		MSTD_LOG_ASSERT_BASE(2 + 2 == 5, logger);

		EXPECT_NE(captured_msg.find("Assertion failed"), std::string::npos);
		EXPECT_NE(captured_msg.find("2 + 2 == 5"), std::string::npos);
		EXPECT_NE(captured_msg.find("AssertTest"), std::string::npos);
	}

	TEST_F(AssertTest, FormatWithArguments) {
		std::string captured_msg;
		auto logger = [&](std::string_view const msg) { captured_msg = msg; };

		int value	= 42;
		MSTD_LOG_ASSERT_BASE(value == 0, logger, "Value should be 0, but is {}", value);

		EXPECT_NE(captured_msg.find("Value should be 0, but is 42"), std::string::npos);
	}

	TEST_F(AssertTest, DoNotLogOnSuccess) {
		bool logger_called = false;
		auto logger		   = [&](std::string_view const) { logger_called = true; };

		MSTD_LOG_ASSERT_BASE(true, logger);

		EXPECT_FALSE(logger_called);
	}

	TEST_F(AssertTest, ConsistencyWithMockedLogger) {
		std::string s  = "test";
		int i		   = 42;
		int call_count = 0;
		auto logger	   = [&](std::string_view const) { call_count++; };

		MSTD_LOG_ASSERT_BASE(!s.empty(), logger);
		MSTD_LOG_ASSERT_BASE(i == 42, logger);

		EXPECT_EQ(call_count, 0);
	}
} // namespace mstd::test
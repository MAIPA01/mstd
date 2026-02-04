#include <mstd/events.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(EVENTS_EVENT_HANDLER, BASIC_INVOKE) {
        mstd::unordered_method_event_handler handler;
        int call_count = 0;

        handler += [&]() { call_count++; };
        handler += [&]() { call_count++; };

        handler.invoke();
        EXPECT_EQ(call_count, 2);
    }

    TEST(EVENTS_EVENT_HANDLER, ARGUMENT_PASSING) {
        mstd::unordered_event_handler<int, std::string> handler;
        int received_int = 0;
        std::string received_str = "";

        std::ignore = handler.add_callback([&](int i, std::string s) {
            received_int = i;
            received_str = s;
            });

        handler(10, "Test");
        EXPECT_EQ(received_int, 10);
        EXPECT_EQ(received_str, "Test");
    }

    TEST(EVENTS_EVENT_HANDLER, REENTRANCY_SAFETY_SELF_REMOVAL) {
        mstd::unordered_method_event_handler handler;
        size_t id1;
        bool second_called = false;
        int first_call_count = 0;

        id1 = handler.add_callback([&]() {
            first_call_count++;
            handler.remove_callback(id1);
            });

        std::ignore = handler.add_callback([&]() {
            second_called = true;
            });

        EXPECT_NO_THROW(handler.invoke());
        EXPECT_EQ(first_call_count, 1);
        EXPECT_TRUE(second_called);

        EXPECT_FALSE(handler.remove_callback(id1));
    }

    TEST(EVENTS_EVENT_HANDLER, ORDERED_MAP_EXECUTION_ORDER) {
        mstd::event_handler<std::vector<int>&> handler;

        handler += [](std::vector<int>& v) { v.push_back(1); };
        handler += [](std::vector<int>& v) { v.push_back(2); };
        handler += [](std::vector<int>& v) { v.push_back(3); };

        std::vector<int> results;
        handler.invoke(results);

        std::vector<int> expected = { 1, 2, 3 };
        EXPECT_EQ(results, expected);
    }

    TEST(EVENTS_EVENT_HANDLER, REMOVE_AND_CLEAR) {
        mstd::unordered_method_event_handler handler;
        auto id = handler += []() {};

        EXPECT_TRUE(handler.remove_callback(id));

        handler += []() {};
        handler += []() {};
        handler.remove_all_callbacks();

        int counter = 0;
        handler += [&]() { counter++; };
        handler.remove_all_callbacks();
        handler();
        EXPECT_EQ(counter, 0);
    }
}
#include <Arduino.h>
#include "ml_tendon_commands.hpp"
#include "unity.h"

void test_tests(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_command_factory(void)
{
    
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_tests);
    // RUN_TEST(test_command_factory);
    UNITY_END();
}

void loop() {
    
}
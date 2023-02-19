#include <unity.h>
#include <Timer.hpp>

void Test_InitialState()
{
    Timer timer;
    TEST_ASSERT_FALSE(timer.isRunning());
    TEST_ASSERT_FALSE(timer.expired(0));
}

void Test_StartInterval()
{
    Timer timer;

    timer.setInterval(1000);

    TEST_ASSERT_FALSE(timer.isRunning());
    TEST_ASSERT_FALSE(timer.expired(1100));

    timer.start(5);
    TEST_ASSERT_TRUE(timer.isRunning());
    TEST_ASSERT_FALSE(timer.expired(100));
    TEST_ASSERT_FALSE(timer.expired(1000));
    TEST_ASSERT_TRUE(timer.expired(1005));
    TEST_ASSERT_TRUE(timer.expired(1100));
    TEST_ASSERT_TRUE(timer.isRunning());
}

void Test_StopInterval()
{
    Timer timer;

    timer.setInterval(1000);
    timer.start(100);
    timer.stop();
    TEST_ASSERT_FALSE(timer.isRunning());
    TEST_ASSERT_FALSE(timer.expired(110));
    TEST_ASSERT_FALSE(timer.expired(1200));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_InitialState);
    RUN_TEST(Test_StartInterval);
    RUN_TEST(Test_StopInterval);
    return UNITY_END();
}
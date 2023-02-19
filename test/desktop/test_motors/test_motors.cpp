#include <unity.h>
#include <motors.hpp>

void Test_InitialState()
{
    TEST_ASSERT_EQUAL(FlapStatus::CLOSED, flapGet());
    TEST_ASSERT_EQUAL(FilterState::FS0, filterGet());
}

void Test_FlapSet()
{
    flapSet(FlapStatus::OPENED);
    TEST_ASSERT_EQUAL(FlapStatus::OPENED, flapGet());
    flapSet(FlapStatus::CLOSED);
    TEST_ASSERT_EQUAL(FlapStatus::CLOSED, flapGet());
}

void Test_FilterSelect()
{
    const FilterState filterStates[] = {
        FilterState::FS0,
        FilterState::FS1,
        FilterState::FS2,
        FilterState::FS3,
        FilterState::FS4
    };
    for (const auto filterState: filterStates) {
        filterSet(filterState);
        TEST_ASSERT_EQUAL(filterState, filterGet());
    }
    filterSet(FilterState::FS0);
}

int main()
{
    motorsInit();
    UNITY_BEGIN();
    RUN_TEST(Test_InitialState);
    RUN_TEST(Test_FlapSet);
    RUN_TEST(Test_FilterSelect);
    return UNITY_END();
}
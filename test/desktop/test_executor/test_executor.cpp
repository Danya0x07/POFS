#include <unity.h>
#include <executor.hpp>

Timer timer;

void setUp()
{
    timer.stop();
}

void Test_InitialState()
{
    Executor executor(timer);
    Response response;

    TEST_ASSERT_FALSE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(0, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(1000, response));
}

void Test_FlapCommands()
{
    Executor executor(timer);
    Response response;
    Command command = {.type = CommandType::SET_FLAP};

    command.flapStatus = FlapStatus::OPENED;
    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(5, response));
    TEST_ASSERT_FALSE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));

    command.flapStatus = FlapStatus::CLOSED;
    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(5, response));
    TEST_ASSERT_FALSE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));

    command.flapStatus = FlapStatus::OPENED;
    executor.startExecuting(command, 5);
    executor.stopExecuting();
    TEST_ASSERT_FALSE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(10, response));
}

void Test_FilterCommands()
{
    Executor executor(timer);
    Response response;
    Command command = {.type = CommandType::SET_FLAP};

    const FilterState filterStates[] = {
        FilterState::FS0,
        FilterState::FS1,
        FilterState::FS2,
        FilterState::FS3,
        FilterState::FS4
    };

    for (const auto filterState: filterStates) {
        command.filterState = filterState;
        executor.startExecuting(command, 5);
        TEST_ASSERT_TRUE(executor.isExecuting());
        TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(5, response));
        TEST_ASSERT_FALSE(executor.isExecuting());
        TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));

        executor.startExecuting(command, 5);
        executor.stopExecuting();
        TEST_ASSERT_FALSE(executor.isExecuting());
        TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));
        TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(10, response));
    }
}

void Test_WaitCommand()
{
    Executor executor(timer);
    Response response;
    Command command = {.type = CommandType::WAIT, .waitTime = 1000};

    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(500, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(1000, response));
    TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(1005, response));
    TEST_ASSERT_FALSE(executor.isExecuting());

    executor.startExecuting(command, 5);
    executor.stopExecuting();
    TEST_ASSERT_FALSE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(5, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(1005, response));
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(1100, response));
}

void Test_ResetCommand()
{
    Executor executor(timer);
    Response response;
    Command command = {.type = CommandType::WAIT, .waitTime = 100};

    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());

    command.type = CommandType::RESET;
    executor.startExecuting(command, 10);
    TEST_ASSERT_TRUE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(11, response));
    TEST_ASSERT_FALSE(executor.isExecuting());
}

void Test_NewCommandPreempts()
{
    Executor executor(timer);
    Response response;
    Command command = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};

    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());
    
    command.type = CommandType::WAIT;
    command.waitTime = 100;
    executor.startExecuting(command, 5);
    TEST_ASSERT_TRUE(executor.isExecuting());
    TEST_ASSERT_EQUAL(Executor::NOTHING, executor.run(10, response));
    TEST_ASSERT_EQUAL(Executor::FINISHED, executor.run(105, response));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_InitialState);
    RUN_TEST(Test_FlapCommands);
    RUN_TEST(Test_FilterCommands);
    RUN_TEST(Test_WaitCommand);
    RUN_TEST(Test_ResetCommand);
    RUN_TEST(Test_NewCommandPreempts);
    return UNITY_END();
}
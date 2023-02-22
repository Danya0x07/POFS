#include <unity.h>
#include <dispatcher.hpp>

Command commandBuffer[11];
ProgramMemory programMemory(commandBuffer, sizeof(commandBuffer) / sizeof(commandBuffer[0]));
ProgramRunner programRunner(commandBuffer);

void setUp()
{
    programMemory.reset();
    programRunner.reset();
}

void Test_RealTimeState()
{
    CommandDispatcher dispatcher(programMemory, programRunner);

    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    Command command = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::EXECUTING, dispatcher.getState());
    dispatcher.notifyExecutionFinished();
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    command = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS2};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::EXECUTING, dispatcher.getState());
    dispatcher.notifyExecutionFinished();
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    command = {.type = CommandType::WAIT, .waitTime = 3000};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::EXECUTING, dispatcher.getState());
    dispatcher.notifyExecutionFinished();
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    command = {.type = CommandType::RESET};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    command = {.type = CommandType::EXECUTE_PROGRAM};
    TEST_ASSERT_EQUAL(CommandDispatcher::ERROR, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());

    command = {.type = CommandType::SAVE_PROGRAM, .loop = {.beginMark = 2, .endMark = 2, .numRepetitions = 2}};
    TEST_ASSERT_EQUAL(CommandDispatcher::ERROR, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());
}

static void setupSomeSequence(CommandDispatcher &dispatcher)
{
    Command command = {.type = CommandType::WAIT, .waitTime = 3000};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    command = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS2};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    command = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    command = {.type = CommandType::WAIT, .waitTime = 1000};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    command = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS0};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    command = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());
}

void Test_RecordingStateComplete()
{
    CommandDispatcher dispatcher(programMemory, programRunner);
    Command command = {.type = CommandType::LOADING_MODE};

    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::RECORDING, dispatcher.getState());

    setupSomeSequence(dispatcher);

    command = {.type = CommandType::SAVE_PROGRAM, .loop = {.beginMark = 2, .endMark = 4, .numRepetitions = 3}};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());
}

void Test_ExecutingState()
{
    CommandDispatcher dispatcher(programMemory, programRunner);
    Command command = {.type = CommandType::WAIT, .waitTime = 5000};

    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::EXECUTING, dispatcher.getState());

    command = {.type = CommandType::RESET};
    TEST_ASSERT_EQUAL(CommandDispatcher::OK, dispatcher.dispatch(command));
    TEST_ASSERT_EQUAL(CommandDispatcher::REALTIME, dispatcher.getState());
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_RealTimeState);
    RUN_TEST(Test_RecordingStateComplete);
    RUN_TEST(Test_ExecutingState);
    return UNITY_END();
}
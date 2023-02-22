#include <unity.h>
#include <program_memory.hpp>

Command commandBuffer[11];
const int bufferLength = sizeof(commandBuffer) / sizeof(commandBuffer[0]);
ProgramMemory memory(commandBuffer, bufferLength);

void tearDown()
{
    memory.reset();
}

void Test_InitialState() // should run first
{
    for (const auto &command: commandBuffer) {
        TEST_ASSERT_EQUAL(CommandType::ERROR, command.type);
    }
    TEST_ASSERT_EQUAL(0, memory.getLenUsed());
}

void Test_IncorrectCommandTypes()
{
    Command command;
    CommandType incorrectCommandTypes[] = {
        CommandType::RESET, CommandType::ERROR, 
        CommandType::EXECUTE_PROGRAM, CommandType::LOADING_MODE
    };

    for (CommandType cmdType: incorrectCommandTypes) {
        command.type = cmdType;
        TEST_ASSERT_EQUAL(ProgramMemory::ERROR, memory.store(command));
    }
}

void Test_BufferOverflow()
{
    Command command = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED};

    TEST_ASSERT_EQUAL(ProgramMemory::START, memory.store(command));
    TEST_ASSERT_EQUAL(0, memory.getLenUsed());
    for (int i = 1; i < bufferLength - 1; i++) {
        TEST_ASSERT_EQUAL(ProgramMemory::STORE, memory.store(command));
        TEST_ASSERT_EQUAL(0, memory.getLenUsed());
    }
    TEST_ASSERT_EQUAL(ProgramMemory::ERROR, memory.store(command));
    Test_InitialState();
}

void Test_LoopBounds()
{
    Command correctCommands[] = {
        {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED},
        {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED},
        {.type = CommandType::SET_FILTER, .filterState = FilterState::FS0},
        {.type = CommandType::SET_FILTER, .filterState = FilterState::FS1},
        {.type = CommandType::SET_FILTER, .filterState = FilterState::FS2},
        {.type = CommandType::SET_FILTER, .filterState = FilterState::FS3},
        {.type = CommandType::SET_FILTER, .filterState = FilterState::FS4},
        {.type = CommandType::WAIT, .waitTime = 3000}
    };
    const int buffLen = sizeof(correctCommands) / sizeof(correctCommands[0]);

    TEST_ASSERT_EQUAL(ProgramMemory::START, memory.store(correctCommands[0]));
    for (int i = 1; i < buffLen - 1; i++) {
        TEST_ASSERT_EQUAL(ProgramMemory::STORE, memory.store(correctCommands[i]));
    }

    Command command = {.type = CommandType::SAVE_PROGRAM, .loop = {.beginMark = 5, .endMark = 4, .numRepetitions = 3}};
    TEST_ASSERT_EQUAL(ProgramMemory::ERROR, memory.store(command));

    command.loop.beginMark = 20;
    command.loop.endMark = 23;
    TEST_ASSERT_EQUAL(ProgramMemory::ERROR, memory.store(command));
    TEST_ASSERT_EQUAL(0, memory.getLenUsed());

    command.loop.beginMark = 3;
    command.loop.endMark = 6;
    TEST_ASSERT_EQUAL(ProgramMemory::COMPLETE, memory.store(command));
    TEST_ASSERT_EQUAL(0, memory.getLenUsed());

    command.loop.beginMark = 0;
    command.loop.endMark = 0;
    TEST_ASSERT_EQUAL(ProgramMemory::COMPLETE, memory.store(command));
    TEST_ASSERT_EQUAL(buffLen, memory.getLenUsed());
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_InitialState);
    RUN_TEST(Test_IncorrectCommandTypes);
    RUN_TEST(Test_BufferOverflow);
    RUN_TEST(Test_LoopBounds);
    return UNITY_END();
}
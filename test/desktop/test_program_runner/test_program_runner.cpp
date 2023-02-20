#include <unity.h>
#include <program_runner.hpp>
#include <string.h>

const Command commandBufferFull[15] = {
    [0] = {.type = CommandType::SAVE_PROGRAM, .loop = {.beginMark = 6, .endMark = 9, .numIterations = 4}},
    
    [1] = {.type = CommandType::WAIT, .waitTime = 1000},

    [2] = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS1},
    [3] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED},
    [4] = {.type = CommandType::WAIT, .waitTime = 2000},
    [5] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED},

    [6] = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS2},
    [7] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED},
    [8] = {.type = CommandType::WAIT, .waitTime = 3000},
    [9] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED},

    [10] = {.type = CommandType::SET_FILTER, .filterState = FilterState::FS0},
    [11] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED},
    [12] = {.type = CommandType::WAIT, .waitTime = 4000},
    [13] = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED},

    [14] = {.type = CommandType::WAIT, .waitTime = 5000},
};

void Test_FullCommandSequence()
{
    ProgramRunner programRunner(commandBufferFull);
    Command command;
    int sequenceLen = sizeof(commandBufferFull) / sizeof(commandBufferFull[0]) - 1;

    ProgramRunner::Status status = programRunner.setupCommandSequence(sequenceLen);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    TEST_ASSERT_EQUAL(26, programRunner.commandsAvailable());

    for (int i = 1; i <= 5; i++) {
        status = programRunner.readCommand(command);
        TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
        sequenceLen--;
        TEST_ASSERT_EQUAL_MEMORY(&commandBufferFull[i], &command, sizeof(Command));
        TEST_ASSERT_EQUAL(sequenceLen, programRunner.commandsAvailable());
    }

    for (int j = 0; j < 4; j++) {
        for (int i = 6; i <= 9; i++) {
            status = programRunner.readCommand(command);
            TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
            sequenceLen--;
            TEST_ASSERT_EQUAL_MEMORY(&commandBufferFull[i], &command, sizeof(Command));
            TEST_ASSERT_EQUAL(sequenceLen, programRunner.commandsAvailable());
        }
    }

    for (int i = 10; i <= 14; i++) {
        status = programRunner.readCommand(command);
        TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
        sequenceLen--;
        TEST_ASSERT_EQUAL_MEMORY(&commandBufferFull[i], &command, sizeof(Command));
        TEST_ASSERT_EQUAL(sequenceLen, programRunner.commandsAvailable());
    }
    TEST_ASSERT_EQUAL(0, sequenceLen);
    status = programRunner.readCommand(command);
    TEST_ASSERT_EQUAL(ProgramRunner::ERROR, status);
}

void Test_SequenceWithoutLoop()
{
    Command cmdBuffer[10];

    memcpy(cmdBuffer, commandBufferFull, 10 * sizeof(Command));
    cmdBuffer[0].loop.beginMark = 0;
    cmdBuffer[0].loop.endMark = 0;
    cmdBuffer[0].loop.numIterations = 0;

    ProgramRunner programRunner(cmdBuffer);
    int sequenceLen = sizeof(cmdBuffer) / sizeof(cmdBuffer[0]) - 1;

    ProgramRunner::Status status = programRunner.setupCommandSequence(sequenceLen);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    TEST_ASSERT_EQUAL(9, programRunner.commandsAvailable());

    Command command;
    for (int i = 1; i <= 9; i++) {
        status = programRunner.readCommand(command);
        TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
        sequenceLen--;
        TEST_ASSERT_EQUAL_MEMORY(&cmdBuffer[i], &command, sizeof(Command));
        TEST_ASSERT_EQUAL(sequenceLen, programRunner.commandsAvailable());
    }
    TEST_ASSERT_EQUAL(0, sequenceLen);
    status = programRunner.readCommand(command);
    TEST_ASSERT_EQUAL(ProgramRunner::ERROR, status);
}

void Test_ExtraordinaryCommand()
{
    ProgramRunner programRunner(commandBufferFull);
    Command exCmd = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};
    int sequenceLen = sizeof(commandBufferFull) / sizeof(commandBufferFull[0]) - 1;

    ProgramRunner::Status status = programRunner.setupCommandSequence(sequenceLen);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    programRunner.setExtraordinaryCommand(exCmd);
    TEST_ASSERT_EQUAL(26 + 1, programRunner.commandsAvailable());

    Command command;
    programRunner.readCommand(command);
    TEST_ASSERT_EQUAL_MEMORY(&exCmd, &command, sizeof(Command));

    programRunner.readCommand(command);
    TEST_ASSERT_EQUAL_MEMORY(&commandBufferFull[1], &command, sizeof(Command));
}

void Test_UrgentCommand()
{
    ProgramRunner programRunner(commandBufferFull);
    Command urgCmd = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};
    Command exCmd = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED};
    int sequenceLen = sizeof(commandBufferFull) / sizeof(commandBufferFull[0]) - 1;

    ProgramRunner::Status status = programRunner.setupCommandSequence(sequenceLen);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    
    TEST_ASSERT_FALSE(programRunner.hasUrgentCommand());
    programRunner.setUrgentCommand(urgCmd);
    TEST_ASSERT_TRUE(programRunner.hasUrgentCommand());
    TEST_ASSERT_EQUAL(26 + 1, programRunner.commandsAvailable());

    programRunner.setExtraordinaryCommand(exCmd);
    TEST_ASSERT_EQUAL(26 + 2, programRunner.commandsAvailable());
    TEST_ASSERT_TRUE(programRunner.hasUrgentCommand());

    Command command;

    status = programRunner.readCommand(command);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    TEST_ASSERT_EQUAL_MEMORY(&urgCmd, &command, sizeof(Command));
    TEST_ASSERT_FALSE(programRunner.hasUrgentCommand());

    status = programRunner.readCommand(command);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    TEST_ASSERT_EQUAL_MEMORY(&exCmd, &command, sizeof(Command));

    status = programRunner.readCommand(command);
    TEST_ASSERT_EQUAL(ProgramRunner::OK, status);
    TEST_ASSERT_EQUAL_MEMORY(&commandBufferFull[1], &command, sizeof(Command));

    TEST_ASSERT_EQUAL(26 - 1, programRunner.commandsAvailable());
}

void Test_Reset()
{
    ProgramRunner programRunner(commandBufferFull);
    Command urgCmd = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::OPENED};
    Command exCmd = {.type = CommandType::SET_FLAP, .flapStatus = FlapStatus::CLOSED};
    int sequenceLen = sizeof(commandBufferFull) / sizeof(commandBufferFull[0]) - 1;
    ProgramRunner::Status status = programRunner.setupCommandSequence(sequenceLen);

    programRunner.setExtraordinaryCommand(exCmd);
    programRunner.setUrgentCommand(urgCmd);
    programRunner.reset();

    TEST_ASSERT_FALSE(programRunner.hasUrgentCommand());
    TEST_ASSERT_EQUAL(0, programRunner.commandsAvailable());
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_FullCommandSequence);
    RUN_TEST(Test_SequenceWithoutLoop);
    RUN_TEST(Test_ExtraordinaryCommand);
    RUN_TEST(Test_UrgentCommand);
    RUN_TEST(Test_Reset);
    return UNITY_END();
}
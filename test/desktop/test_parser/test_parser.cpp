#include <unity.h>
#include <parser.hpp>

void Test_IncorectCommands()
{
    const char *rawStrings[] = {
        "Z",
        "Z7",
        "Z2,3",
        "A B",
        "G2", "G-1",
        "F7", "F-7",
        "W 2000 ",
        "L5", "L -6",
        "P6,2,3", "P2,3", "P3", "P", "P2,6,3,4", "PZ",
        "E5", "E -6",
        "R5", "R -6"
    };
    ParsingStatus status;
    Command cmd;

    for (const char *rawString: rawStrings) {
        status = parse(rawString, cmd);
        TEST_ASSERT_EQUAL_MESSAGE(ParsingStatus::ERROR, status, rawString);
    }
}

void Test_CorrectCommands()
{
    ParsingStatus status;
    Command cmd;

    status = parse("G0", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FLAP, cmd.type);
    TEST_ASSERT_EQUAL(FlapStatus::CLOSED, cmd.flapStatus);

    status = parse("G1", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FLAP, cmd.type);
    TEST_ASSERT_EQUAL(FlapStatus::OPENED, cmd.flapStatus);

    status = parse("F0", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FILTER, cmd.type);
    TEST_ASSERT_EQUAL(FilterState::FS0, cmd.filterState);

    status = parse("F1", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FILTER, cmd.type);
    TEST_ASSERT_EQUAL(FilterState::FS1, cmd.filterState);

    status = parse("F2", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FILTER, cmd.type);
    TEST_ASSERT_EQUAL(FilterState::FS2, cmd.filterState);

    status = parse("F3", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FILTER, cmd.type);
    TEST_ASSERT_EQUAL(FilterState::FS3, cmd.filterState);

    status = parse("F4", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SET_FILTER, cmd.type);
    TEST_ASSERT_EQUAL(FilterState::FS4, cmd.filterState);

    status = parse("W120000", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::WAIT, cmd.type);
    TEST_ASSERT_EQUAL(120000, cmd.waitTime);

    status = parse("L", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::LOADING_MODE, cmd.type);

    status = parse("P3,6,5", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::SAVE_PROGRAM, cmd.type);
    TEST_ASSERT_EQUAL(3, cmd.loop.beginMark);
    TEST_ASSERT_EQUAL(6, cmd.loop.endMark);
    TEST_ASSERT_EQUAL(5, cmd.loop.numRepetitions);

    status = parse("E", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::EXECUTE_PROGRAM, cmd.type);

    status = parse("R", cmd);
    TEST_ASSERT_EQUAL(ParsingStatus::OK, status);
    TEST_ASSERT_EQUAL(CommandType::RESET, cmd.type);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_IncorectCommands);
    RUN_TEST(Test_CorrectCommands);
    return UNITY_END();
}
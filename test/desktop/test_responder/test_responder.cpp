#include <unity.h>
#include <responder.hpp>

char buffer[8];

void Test_ReplyCorrectString()
{
    reply(buffer, Response::PARSING_OK);
    TEST_ASSERT_EQUAL_STRING("O\n", buffer);

    reply(buffer, Response::PARSING_ERR);
    TEST_ASSERT_EQUAL_STRING("1\n", buffer);

    reply(buffer, Response::DISPATCH_ERR);
    TEST_ASSERT_EQUAL_STRING("2\n", buffer);

    reply(buffer, Response::EXEC_FINISH);
    TEST_ASSERT_EQUAL_STRING("3\n", buffer);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_ReplyCorrectString);
    return UNITY_END();
}
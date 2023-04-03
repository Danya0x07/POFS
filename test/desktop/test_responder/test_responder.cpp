#include <unity.h>
#include <responder.hpp>
#include <string.h>

char buffer[64];

void Test_ReplyCorrectString()
{
    Response response;

    response.type = ResponseType::PARSING_OK;
    reply(buffer, response);
    TEST_ASSERT_EQUAL_STRING("0\n", buffer);

    response.type = ResponseType::PARSING_ERR;
    reply(buffer, response);
    TEST_ASSERT_EQUAL_STRING("1\n", buffer);

    response.type = ResponseType::DISPATCH_ERR;
    reply(buffer, response);
    TEST_ASSERT_EQUAL_STRING("2\n", buffer);

    uint8_t angles[NUM_MOTORS];
    response.type = ResponseType::DATA;
    memset(angles, 23, sizeof(angles));
    memcpy(response.angles.opened, angles, sizeof(angles));
    memset(angles, 32, sizeof(angles));
    memcpy(response.angles.closed, angles, sizeof(angles));
    reply(buffer, response);
    TEST_ASSERT_EQUAL_STRING("4,23 32,23 32,23 32,23 32,23 32\n", buffer);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(Test_ReplyCorrectString);
    return UNITY_END();
}
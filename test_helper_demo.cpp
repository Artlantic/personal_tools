#include "test_helper.h"
using namespace test_helper;

int func(char a, std::string b) {
    std::cout << "print:" << a << " " << b << " end.\n";
    return 1;
}
int func1(int a, std::string b, std::string c) {
    std::cout << "print:" << a << " " << b << " " << c << " end.\n";
    return 2;
}

TEST_INIT_BEGIN
ADD_TEST_FUNCTION("echo", func)
ADD_TEST_FUNCTION("echo1", func1)
TEST_INIT_END

int main() {
    std::vector<std::string> req;
    std::string res;
    req = {"1", "str"};
    CALL_TEST("echo", req, res)
    req = {"3", "str2", "123"};
    CALL_TEST("echo1", req, res)
    req = {"ff", "str1", ""};
    CALL_TEST("echo1", req, res)
}

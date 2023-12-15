#include "test_helper.h"
#include <functional>
using namespace test_helper;

int func(const char& a, const std::string& b) {
    std::cout << "print0:" << a << " " << b << " end.\n";
    return 0;
}
int func1(int a, std::string b, std::string c) {
    std::cout << "print1:" << a << " " << b << " " << c << " end.\n";
    return 1;
}

class c {
public:
    static int func2(int a) {
        std::cout << "print2:" << a << " end.\n";
        return 3;
    }
    int func3(int a) {
        std::cout << "print3:" << a << " end.\n";
        return 2;
    }
};


void test(const std::string& name_, const std::vector<std::string>& arg_, std::string& result_) {
    TEST_INIT(name_, arg_, result_)
    c ci;
    auto bind3                    = std::bind(&c::func3, &ci, std::placeholders::_1);
    std::function<int(int)> func3 = bind3;
    ADD_TEST_STD_FUNCTION("echo3", func3)

    ADD_TEST_FUNCTION("echo", func)
    ADD_TEST_FUNCTION("echo1", func1)
    ADD_TEST_FUNCTION("echo2", c::func2)
}

int main() {
    std::vector<std::string> req;
    std::string res;
    req = {"c", "str"};
    test("echo", req, res);
    req = {"3", "str2", "123"};
    test("echo1", req, res);
    req = {"1ff", "str1", ""};
    test("echo3", req, res); 
}

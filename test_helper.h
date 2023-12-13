#pragma once
#include <functional>
#include <tuple>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

// 不支持复杂类型参数
namespace test_helper {

template <typename T>
struct function_traits {};

template <typename R, typename... Args>
struct function_traits<std::function<R(Args...)>> {
    // typedef
    typedef R result_type;
    typedef std::tuple<Args...> request_type;

    // struct members
    static const size_t nargs = sizeof...(Args);

    // parameters instructor
    template <size_t i>
    struct arg {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        arg() {}
    };

    // parameters parser
    const static size_t end_index = 0;
    template <size_t N>
    static typename std::enable_if<N == end_index>::type parse(request_type& t,
                                                               std::vector<std::string> input) {}
    template <size_t N>
    static typename std::enable_if<N != end_index>::type parse(request_type& t,
                                                               std::vector<std::string> input) {
        std::stringstream ss;
        ss << (input[N - 1]);
        typename arg<N - 1>::type& v = std::get<N - 1>(t);
        ss >> v;
        if (N > 1) parse<N - 1>(t, input);
    }
    static void parse(request_type& t, std::vector<std::string> input) { parse<nargs>(t, input); }

    // function invoke
    template <typename Fn, size_t... N>
    static result_type invokeImpl(Fn fn, request_type& pa, std::index_sequence<N...>) {
        return fn(std::get<N>(pa)...);
    }
    template <typename Fn>
    static result_type invoke(Fn fn, request_type& t) {
        return invokeImpl(fn, t, std::make_index_sequence<std::tuple_size<request_type>::value> {});
    }
};

#define TEST_INIT_BEGIN                                                                            \
    void test_helper_func(std::string name, std::vector<std::string> arg,                          \
                          std::string& result) {
#define TEST_INIT_END }

#define CALL_TEST(name, arg, result) test_helper_func(name, arg, result);

#define ADD_TEST_FUNCTION(funcName, func)                                                          \
    if (funcName == name) {                                                                        \
        typedef std::function<decltype(func)> ft;                                                  \
        typedef function_traits<ft> ftt;                                                           \
        ftt::request_type req;                                                                     \
        ftt::result_type res;                                                                      \
        if (ftt::nargs > arg.size()) {std::cout<<"invalid param."; return;}                          \
        ftt::parse(req, arg);                                                                      \
        res = ftt::invoke(func, req);                                                              \
        std::stringstream ss;                                                                      \
        ss << res;                                                                                 \
        result = ss.str();                                                            \
    }


} // namespace test_helper

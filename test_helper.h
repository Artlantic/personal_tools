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
    // typedef typename std::remove_reference<Args...>::type Args1...;
    typedef R result_type;
    // typedef std::tuple<Args...> request_type;
    typedef std::tuple<typename std::remove_reference<Args>::type...> request_type1;

    typedef std::tuple<typename std::decay_t<Args>...> request_type;

    // using Args_no_reference... = typename std::remove_reference<Args>::type...;

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
        ss >> const_cast<typename std::tuple_element<N - 1, request_type>::type&>(std::get<N - 1>(t));
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

#define TEST_INIT(name, arg, result)                                                               \
    std::string _name             = name;                                                          \
    std::vector<std::string> _arg = arg;                                                           \
    std::string& _result          = result;

#define ADD_TEST_FUNCTION_WITH_TYPE(funcName, funcType, func)                                      \
    if (funcName == _name) {                                                                       \
        typedef function_traits<funcType> ftt;                                                     \
        ftt::request_type req;                                                                     \
        ftt::result_type res;                                                                      \
        if (ftt::nargs > _arg.size()) {                                                            \
            std::cout << "invalid param.";                                                         \
            return;                                                                                \
        }                                                                                          \
        ftt::parse(req, _arg);                                                                     \
        res = ftt::invoke(func, req);                                                              \
        std::stringstream ss;                                                                      \
        ss << res;                                                                                 \
        _result = ss.str();                                                                        \
        return;                                                                                    \
    }

#define ADD_TEST_STD_FUNCTION(funcName, func)                                                      \
    {                                                                                              \
        typedef decltype(func) funcType;                                                           \
        ADD_TEST_FUNCTION_WITH_TYPE(funcName, funcType, func)                                      \
    }

#define ADD_TEST_FUNCTION(funcName, func)                                                          \
    {                                                                                              \
        typedef std::function<decltype(func)> ft;                                                  \
        ADD_TEST_FUNCTION_WITH_TYPE(funcName, ft, func)                                            \
    }

} // namespace test_helper

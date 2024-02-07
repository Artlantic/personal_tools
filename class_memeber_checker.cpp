#include <iostream>
#include <type_traits>

#define DECLARE_PB_MEMBER_CHECKER(MEMBER)                                                     \
    template <typename T, typename = void>                                                    \
    struct MEMBER##_checker : std::false_type                                                 \
    {                                                                                         \
    };                                                                                        \
    template <typename T>                                                                     \
    struct MEMBER##_checker<T, decltype(std::declval<T>().clear_##MEMBER())> : std::true_type \
    {                                                                                         \
    };

#define IF_HAS_MEMBER(PB, MEMBER) \
    MEMBER##_checker<PB>::value

#define DECLARE_PB_MEMBER_SETTER(MEMBER)                                                                  \
    template <typename PB, typename V>                                                                    \
    typename std::enable_if<IF_HAS_MEMBER(PB, MEMBER)>::type MEMBER##_setter(PB& pb, const V& v)          \
    {                                                                                                     \
        pb.set_##MEMBER(v);                                                                               \
    }                                                                                                     \
    template <typename PB, typename V>                                                                    \
    typename std::enable_if<IF_HAS_MEMBER(PB, MEMBER) == false>::type MEMBER##_setter(PB& pb, const V& v) \
    {                                                                                                     \
        ;                                                                                                 \
    }

#define SET_IF_HAS_MEMBER(pb, MEMBER, v) \
    MEMBER##_setter<decltype(pb), decltype(v)>(pb, v);

#define DECLARE_PB_MEMBER_CHECKER_AND_SETTER(MEMBER) \
    DECLARE_PB_MEMBER_CHECKER(MEMBER)                \
    DECLARE_PB_MEMBER_SETTER(MEMBER)

class c
{
public:
    void a() { std::cout << "a\n"; }
    void clear_a() { std::cout << "clear_a\n"; }
    void set_a(float u) { std::cout << "set_a:" << u << std::endl; }
};
DECLARE_PB_MEMBER_CHECKER_AND_SETTER(b)
DECLARE_PB_MEMBER_CHECKER_AND_SETTER(a)

int main()
{
    c ct;
    int val = 1;
    std::cout << IF_HAS_MEMBER(decltype(ct), b);
    std::cout << IF_HAS_MEMBER(decltype(ct), a);
    SET_IF_HAS_MEMBER(ct, a, 1)
}

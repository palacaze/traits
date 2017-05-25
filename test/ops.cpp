#include <type_traits>
#include <tuple>
#include <traits/type/list.h>
#include <traits/type/ops.h>
using namespace pal::trait;

template <typename...>
struct arbitrary_container {};

template <typename U, typename T>
void assert_same() {
    static_assert(std::is_same<U, T>::value, "");
}

template <template <typename...> class C>
void test() {
    using t = C<int, char, float>;

    // size
    static_assert(size_v<t> == 3, "");
    static_assert(size_v<C<>> == 0, "");

    // create_empty
    assert_same<C<>, create_empty<t>>();

    // join
    assert_same<C<int, float>, join<C<int>, C<float>>>();
    assert_same<C<int, float, char>, join<C<int>, C<float>, C<char>>>();
    assert_same<C<int, float>, join<C<int>, C<>, C<float>>>();
    assert_same<C<>, join<C<>, C<>>>();

    // push_front
    assert_same<t, push_front<int, C<char, float>>>();
    assert_same<C<int>, push_front<int, C<>>>();

    // push_back
    assert_same<t, push_back<float, C<int, char>>>();
    assert_same<C<int>, push_back<int, C<>>>();

    // pop_front
    assert_same<C<char, float>, pop_front<t>>();
    assert_same<C<>, pop_front<C<int>>>();
    assert_same<C<>, pop_front<C<>>>();

    // pop_back
    assert_same<C<int, char>, pop_back<t>>();
    assert_same<C<>, pop_back<C<int>>>();
    assert_same<C<>, pop_back<C<>>>();

    // take
    assert_same<C<int, char>, take<2, t>>();
    assert_same<t, take<3, t>>();
    assert_same<C<>, take<0, t>>();
    // assert_same<C<>, take<4, t>>();  // asserts false

    // skip
    assert_same<C<float>, skip<2, t>>();
    assert_same<C<>, skip<3, t>>();
    assert_same<t, skip<0, t>>();
}

int main() {
    test<trait::list>();
    test<std::tuple>();
    test<arbitrary_container>();
}


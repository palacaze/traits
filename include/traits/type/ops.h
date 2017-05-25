#pragma once

#include <cstddef>
#include "integral.h"
#include "nothing.h"

// Fundamental operations over arbitrary lists of types, container agnostic.

namespace pal {
namespace trait {
namespace detail {

    template <typename>
    struct size_impl;

    template <template <typename...> class C, typename... T>
    struct size_impl<C<T...>>
        : public integral_constant<size_t, sizeof...(T)> {};

    template <typename...>
    struct create_empty_impl;

    template <template <typename...> class C, typename... T>
    struct create_empty_impl<C<T...>> {
        using type = C<>;
    };

    template <typename...>
    struct join_impl;

    template <template <typename...> class C, typename... T>
    struct join_impl<C<T...>> {
        using type = C<T...>;
    };

    template <template <typename...> class C, typename... U, typename... T, typename... L>
    struct join_impl<C<U...>, C<T...>, L...> {
        using type = typename join_impl<C<U..., T...>, L...>::type;
    };

    template <typename, typename...>
    struct push_front_impl;

    template <template <typename...> class C, typename E, typename... T>
    struct push_front_impl<E, C<T...>> {
        using type = C<E, T...>;
    };

    template <typename, typename...>
    struct push_back_impl;

    template <template <typename...> class C, typename E, typename... T>
    struct push_back_impl<E, C<T...>> {
        using type = C<T..., E>;
    };

    template <typename>
    struct pop_front_impl;

    template <template <typename...> class C>
    struct pop_front_impl<C<>> {
        using type = nothing;
        using rest = C<>;
    };

    template <template <typename...> class C, typename E, typename... T>
    struct pop_front_impl<C<E, T...>> {
        using type = E;
        using rest = C<T...>;
    };

    template <typename, typename...>
    struct pop_back_impl;

    template <template <typename...> class C>
    struct pop_back_impl<C<>, C<>> {
        using type = nothing;
        using rest = C<>;
    };

    template <template <typename...> class C, typename T, typename... U>
    struct pop_back_impl<C<T>, C<U...>> {
        using type = T;
        using rest = C<U...>;
    };

    template <template <typename...> class C, typename E, typename... T, typename... U>
    struct pop_back_impl<C<E, T...>, C<U...>> {
        using s = pop_back_impl<C<T...>, C<U..., E>>;
        using type = typename s::type;
        using rest = typename s::rest;
    };

    template <size_t N, typename L, typename R>
    struct take_impl {
        using tmp = pop_front_impl<L>;
        using type = typename take_impl<N-1,
                         typename tmp::rest,
                         typename push_back_impl<typename tmp::type, R>::type>::type;
    };

    template <size_t N, template <typename...> class C, typename R>
    struct take_impl<N, C<>, R> {
        static_assert(N == 0, "Index out of bounds");
        using type = R;
    };

    template <typename R, typename L>
    struct take_impl<0, L, R> {
        using type = R;
    };

    template <template <typename...> class C, typename R>
    struct take_impl<0, C<>, R> {
        using type = R;
    };

    template <size_t N, typename L>
    struct skip_impl {
        using type = typename skip_impl<N-1, typename pop_front_impl<L>::rest>::type;
    };

    template <typename L>
    struct skip_impl<0, L> {
        using type = L;
    };

    template <size_t N, typename L>
    struct at_impl {
        using type = typename at_impl<N-1, typename pop_front_impl<L>::rest>::type;
    };

    template <typename L>
    struct at_impl<0, L> {
        using type = typename pop_front_impl<L>::type;
    };

} // detail

template <typename L>
using size = typename detail::size_impl<L>;

template <typename L>
constexpr size_t size_v = size<L>::value;

template <typename L>
using create_empty = typename detail::create_empty_impl<L>::type;

template <typename... L>
using join = typename detail::join_impl<L...>::type;

template <size_t I, typename L>
using take = typename detail::take_impl<I, L, create_empty<L>>::type;

template <size_t I, typename L>
using skip = typename detail::skip_impl<I, L>::type;

template <typename E, typename L>
using push_front = typename detail::push_front_impl<E, L>::type;

template <typename E, typename L>
using push_back = typename detail::push_back_impl<E, L>::type;

template <typename L>
using pop_front = typename detail::pop_front_impl<L>::rest;

template <typename L>
using pop_back = typename detail::pop_back_impl<L, create_empty<L>>::rest;

template <size_t I, typename L>
using at = typename detail::at_impl<I, L>::type;

} // trait
} // pal


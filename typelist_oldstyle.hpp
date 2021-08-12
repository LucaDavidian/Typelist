#ifndef TYPELIST_OLD_H
#define TYPELIST_OLD_H

#include <cstddef>

#define TYPELIST_1(T1)           Typelist<T1, NullType>
#define TYPELIST_2(T1,T2)        Typelist<T1, Typelist<T2, NullType>>
#define TYPELIST_3(T1,T2,T3)     Typelist<T1, Typelist<T2, Typelist<T3, NullType>>>
#define TYPELIST_4(T1,T2,T3,T4)  Typelist<T1, Typelist<T2, Typelist<T3, Typelist<T4, NullType>>>>

/**** null type ****/
struct NullType {};

/**** typelist definition ****/
template <typename T, typename U>
struct Typelist
{
    typedef T Head;
    typedef U Tail;
};

/**** get list length ****/
// template <typename List>
// struct Length
// {
//     enum { value = 1 + Length<typename List::Tail>::value };
// };

// template <>
// struct Length<NullType>
// {
//     enum { value = 0 };
// };

template <typename List>
struct Length;

template <typename T, typename U>
struct Length<Typelist<T,U>>
{
    enum { value = 1 + Length<U>::value };
};

template <>
struct Length<NullType>
{
    enum { value = 0 };
};

/**** get list length - constexpr function version (C++11) ****/
template <typename T, typename U>
constexpr std::size_t ComputeLength(Typelist<T,U> const &list)
{
    return 1 + ComputeLength(U());
}

constexpr std::size_t ComputeLength(NullType const &null)
{
    return 0;
}

/**** get list element by integer index ****/
template <typename List, std::size_t N>
struct Element;

template <typename U, typename V, std::size_t N>
struct Element<Typelist<U,V>,N>
{
    typedef typename Element<V,N - 1>::type type;
};

template <typename U, typename V>
struct Element<Typelist<U,V>,0>
{
    typedef U type;
};

/**** append to typelist ****/


#endif  // TYPELIST_OLD_H
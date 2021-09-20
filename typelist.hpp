#ifndef TYPELIST_H
#define TYPELIST_H

#include "../traits/traits.hpp"

/******** typelist definition ********/
template <typename... Types>
struct Typelist
{
};

/**** is empty metafunction returns true if typelist is empty, false otherwise ****/
template <typename List>
struct IsEmpty
{
};

template <typename... Types>
struct IsEmpty<Typelist<Types...>> : Traits::FalseType
{
    // static constexpr bool Value = false;
};

template <>
struct IsEmpty<Typelist<>> : Traits::TrueType
{
    // static constexpr bool Value = true;
};

template <typename List>
constexpr bool IsEmptyV = IsEmpty<List>::Value;

/**** front metafunction retrieves the first type in the list ****/
template <typename List>
struct Front;

template <typename Head, typename... Tail>
struct Front<Typelist<Head, Tail...>>
{
    using Type = Head;
};

template <typename List>      
using FrontT = typename Front<List>::Type;

/**** pop front metafunction produces a new typelist without the first element ****/
template <typename List>
struct PopFront;

template <typename Head, typename... Tail>
struct PopFront<Typelist<Head, Tail...>>
{
    using Type = Typelist<Tail...>;
};

template <typename List>    
using PopFrontT = typename PopFront<List>::Type;

/**** push front metafunction produces a new typelist pushing a type at the beginning of list ****/
template <typename List, typename NewType>
struct PushFront;

template <typename... Types, typename NewType>
struct PushFront<Typelist<Types...>, NewType>
{
    using Type = Typelist<NewType, Types...>;
};

template <typename List, typename NewType>  
using PushFrontT = typename PushFront<List, NewType>::Type;

/**** push back metafunction produces a new typelist by appending a type to the end of the list ****/
template <typename List, typename NewType>
struct PushBack;

template <typename... Types, typename NewType>
struct PushBack<Typelist<Types...>, NewType>
{
    using Type = Typelist<Types..., NewType>;
};

template <typename List, typename NewType>
using PushBackT = typename PushBack<List, NewType>::Type;

/**** push back metafunction using primitive operations (FrontT, PopFrontT, PushFront and IsEmpty) ****/


/**** replace front metafunction produces a new list by replacing first element ****/
template <typename List, typename NewType>
struct ReplaceFront //: PushFrontT<PopFrontT<List>, NewType>
{
    using Type = PushFrontT<PopFrontT<List>, NewType>;
};

// template <typename List, typename NewType>
// using ReplaceFrontT = typename ReplaceFront<List, NewType>::Type;

template <typename T, typename NewType>
using ReplaceFrontT = typename PushFront<typename PopFront<T>::Type, NewType>::Type;

/**** indexing a typelist ****/
// template <typename T, unsigned int N>
// struct NthElement;

// template <typename Head, typename... Tail, unsigned int N>
// struct NthElement<Typelist<Head, Tail...>,N>
// {
//     using Type = typename NthElement<Typelist<Tail...>, N - 1>::Type;
// };

// template <typename Head, typename... Tail>
// struct NthElement<Typelist<Head, Tail...>, 0U>
// {
//     using Type = Head;
// };

template <typename T, unsigned int N>
struct NthElement : NthElement<typename PopFront<T>::Type, N - 1>
{
};

template <typename T>
struct NthElement<T, 0U> : Front<T>
{
};

template <typename T, unsigned int N>
using NthElementT = typename NthElement<T, N>::Type;

/**** find the largest type (useful to allocate storage for variant) ****/
template <typename T>
struct LargestType
{
private:
    using First = FrontT<T>;
    using Rest = PopFrontT<T>;
public:
    using Type = Traits::ConditionalT<sizeof(First) >= sizeof(typename LargestType<Rest>::Type), First, typename LargestType<Rest>::Type>;
};

template <>
struct LargestType<Typelist<>>
{
    using Type = char;
};

template <typename T>   // convenience alias template
using LargestTypeT = typename LargestType<T>::Type;

/**** find the index of a type in a typelist (starting from N) ****/
template <typename Type, typename T, unsigned int N = 0, bool EMPTY = IsEmpty<T>::Value>
struct IndexOfType;

template <typename Type, typename List, unsigned int N>
struct IndexOfType<Type, List, N, false> : Traits::ConditionalT<Traits::IsSame<Type, FrontT<List>>::Value,
                                                        Traits::IntegralConstant<unsigned int, N>,
                                                        IndexOfType<Type, PopFrontT<List>, N + 1>
                                                        >
{
};

template <typename Type, typename List, unsigned int N>
struct IndexOfType<Type, List, N, true>
{
    // no Value 
};

template <typename Type, typename List, unsigned int N = 0>
static constexpr unsigned int IndexOfTypeV = IndexOfType<Type, List, N>::Value;

/**** length metafunction returns the list's length ****/
// template <typename T>
// struct Length;

// template <typename Head, typename... Tail>
// struct Length<Typelist<Head, Tail...>>
// {
//     static constexpr unsigned int Value = Length<Typelist<Tail...>> + 1;
// };

// template <>
// struct Length<Typelist<>>
// {
//     static constexpr unsigned int Value = 0U;
// };

template <typename T, bool Empty = IsEmptyV<T>>
struct Length;

template <typename T>
struct Length<T, false>
{
    static constexpr unsigned int Value = Length<typename PopFront<T>::Type>::Value + 1;
};

template <typename T>
struct Length<T, true>
{
    static constexpr unsigned int Value = 0U;
};

template <typename T>   
constexpr unsigned int LengthV = Length<T>::Value;

#include <cstddef>

/**** get list length - constexpr function version (C++11) ****/
template <typename Head, typename... Tail>
constexpr std::size_t ComputeLength(Typelist<Head, Tail...> const &list)
{
    return 1 + ComputeLength(Typelist<Tail...>());
}

constexpr std::size_t ComputeLength(Typelist<> const &null)
{
    return 0;
}

/**** reverse metafunction returns a reversed typelist ****/
template <typename T, bool Empty = IsEmpty<T>::Value>
struct Reverse;

template <typename T>
struct Reverse<T, false>
{
    using Type = typename PushBack<typename Reverse<PopFrontT<T>>::Type, FrontT<T>>::Type;
};

template <typename T>
struct Reverse<T, true>
{
    using Type = T;
};

template <typename T>
using ReverseT = typename Reverse<T>::Type;

/**** back metafunction retrieves the last element of the list ****/
template <typename T>
struct Back : Front<ReverseT<T>>
{
};

template <typename T>
using BackT = typename Back<T>::Type;

/**** pop back metafunction removes the last element of the list ****/
template <typename T>
struct PopBack : Reverse<PopFrontT<ReverseT<T>>>
{
};

template <typename T>
using PopBackT = typename PopBack<T>::Type;




/******** valuelist alias template ********/

template <typename T, T Val>
struct IntegralConstant
{
    static constexpr T Value = Val;
};

// template <typename T, T... Values>
// using Valuelist = Typelist<IntegralConstant<T, Values>...>;

/**** value list type ****/
template <typename T, T... Values>
struct Valuelist
{
};

/**** core operations for value lists (class template partial specializations for value lists) ****/

// template <typename T, T... Values>
// struct IsEmpty<Valuelist<T, Values...>>  // : Traits::FalseType
// {
//     static constexpr bool Value = false;
// };

// template <typename T>
// struct IsEmpty<Valuelist<T>> // : Traits::TrueType
// {
//     static constexpr bool Value = true;
// };

template <typename T, T... Values>
struct IsEmpty<Valuelist<T, Values...>>
{
    static constexpr bool Value = sizeof...(Values) == 0U;
};

template <typename T, T Head, T... Tail>
struct Front<Valuelist<T, Head, Tail...>>
{
    using Type = IntegralConstant<T, Head>;
    static constexpr T Value = Head;
};

template <typename T, T... Values, T NewValue>
struct PushFront<Valuelist<T, Values...>, IntegralConstant<T, NewValue>>
{
    using Type = Valuelist<T, NewValue, Values...>;
};

template <typename T, T Head, T... Tail>
struct PopFront<Valuelist<T, Head, Tail...>>
{
    using Type = Valuelist<T, Tail...>;
};

template <typename T, T... Values, T NewValue>
struct PushBack<Valuelist<T, Values...>, IntegralConstant<T, NewValue>>
{
    using Type = Valuelist<T, Values..., NewValue>;
};

#endif  // TYPELIST_H
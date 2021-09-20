#ifndef TYPELIST_H
#define TYPELIST_H

#define TYPELIST_1(t1) TypeList<t1, NullType>
#define TYPELIST_2(t1, t2) TypeList<t1, TYPELIST_1(t2)>
#define TYPELIST_3(t1, t2, t3) TypeList<t1, TYPELIST_2(t2, t3)>
#define TYPELIST_4(t1, t2, t3, t4) TypeList<t1, TYPELIST_3(t2, t3, t4)>
#define TYPELIST_5(t1, t2, t3, t4, t5) TypeList<t1, TYPELIST_4(t2, t3, t4, t5)>
#define TYPELIST_6(t1, t2, t3, t4, t5, t6) TypeList<t1, TYPELIST_5(t2, t3, t4, t5, t6)>
#define TYPELIST_7(t1, t2, t3, t4, t5, t6, t7) TypeList<t1, TYPELIST_6(t2, t3, t4, t5, t6, t7)>
#define TYPELIST_8(t1, t2, t3, t4, t5, t6, t7, t8) TypeList<t1, TYPELIST_7(t2, t3, t4, t5, t6, t7, t8)>

struct NullType {};  // sentinel type - end of typelist

template <class T, class TList = NullType>
struct TypeList
{
    typedef T Head;
    typedef TList Tail;
};

/* typelist algorithms */

// length
template <typename TList>
struct Length;

template <class T, class U>
struct Length<TypeList<T, U>>
{
    //enum { Result = 1 + Length<typename TypeList<T, U>::Tail>::Result };
    enum { Result = 1 + Length<U>::Result };
};

template <>
struct Length<NullType>
{
    enum { Result = 0 };
};

// indexed access
template <typename TList, unsigned int I>
struct TypeAt;

template <class T, class U, unsigned int I>
struct TypeAt<TypeList<T, U>, I>
{
    //typedef typename TypeAt<typename TypeList<T, U>::Tail, I - 1>::Result Result;
    typedef typename TypeAt<U, I - 1>::Result Result;
};

template <class T, class U>
struct TypeAt<TypeList<T, U>, 0U>
{
    //typedef typename TypeList<T, U>::Head Result;
    typedef T Result;
};

// pop front 
template <typename TList>
struct PopFront
{
    typedef typename TList::Tail Result;
};

// push front
template <typename TList, typename T>
struct PushFront
{
    typedef TypeList<T, TList> Result;
};

// append
template <class TList, class T>
struct Append;

template <class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T>
{
    typedef TypeList<Head, TypeList<Tail, T>> Result;
};

template <class T>
struct Append<NullType, T>
{
    typedef TypeList<T, NullType> Result;
};

// search (return index of first type found, or -1)
template <class TList, class Type>
struct IndexOf;

template <class Head, class Tail, class Type>
struct IndexOf<TypeList<Head, Tail>, Type>
{
private:
    //enum { Temp = Search<typename TypeList<Head, Tail>::Tail, Type>::Result; };
    enum { Temp = IndexOf<Tail, Type>::Result };
public:
    enum { Result = Temp == -1 ? -1 : 1 + Temp };
};

template <class Head, class Tail>
struct IndexOf<TypeList<Head, Tail>, Head>
{
    enum { Result = 0 };
};

template <class Type>
struct IndexOf<NullType, Type>
{
    enum { Result = -1 };
};

#endif  // TYPELIST_H
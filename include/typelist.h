#ifndef TYPELIST_H
#define TYPELIST_H


#include "wrappers.h"
#include "typetraits.h"
#include "if.h"


struct NilType {};

template<typename HeadT, typename TailT> 
struct TypeList
{
    typedef HeadT head_type;
    typedef TailT tail_type;
};


#define TYPELIST_1(t1) TypeList<t1, NilType>
#define TYPELIST_2(t1, t2) TypeList<t1, TYPELIST_1(t2) >
#define TYPELIST_3(t1, t2, t3) TypeList<t1, TYPELIST_2(t2, t3) >
#define TYPELIST_4(t1, t2, t3, t4) TypeList<t1, TYPELIST_3(t2, t3, t4) >
#define TYPELIST_5(t1, t2, t3, t4, t5) TypeList<t1, TYPELIST_4(t2, t3, t4, t5) >
#define TYPELIST_6(t1, t2, t3, t4, t5, t6) TypeList<t1, TYPELIST_5(t2, t3, t4, t5, t6) >
#define TYPELIST_7(t1, t2, t3, t4, t5, t6, t7) TypeList<t1, TYPELIST_6(t2, t3, t4, t5, t6, t7) >
#define TYPELIST_8(t1, t2, t3, t4, t5, t6, t7, t8) TypeList<t1, TYPELIST_7(t2, t3, t4, t5, t6, t7, t8) >
#define TYPELIST_9(t1, t2, t3, t4, t5, t6, t7, t8, t9) TypeList<t1, TYPELIST_8(t2, t3, t4, t5, t6, t7, t8, t9) >
#define TYPELIST_10(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10) TypeList<t1, TYPELIST_9(t2, t3, t4, t5, t6, t7, t8, t9, t10) >
#define TYPELIST_11(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11) TypeList<t1, TYPELIST_10(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11) >
#define TYPELIST_12(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12) TypeList<t1, TYPELIST_11(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12) >
#define TYPELIST_13(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13) TypeList<t1, TYPELIST_12(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13) >
#define TYPELIST_14(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14) TypeList<t1, TYPELIST_13(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14) >
#define TYPELIST_15(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15) TypeList<t1, TYPELIST_14(t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15) >


// ---------------------------------- size ------------------------------------------

/// calculate the size of a given typelist
template<typename ListT> 
struct Size;

template<typename HeadT, typename TailT> 
struct Size<TypeList<HeadT, TailT> >
{
    enum { value = Size<TailT>::value + 1 };
};

template<typename HeadT> 
struct Size<TypeList<HeadT, NilType> >
{
    enum { value = 1 };
};

// ------------------------------ push/pop front -------------------------------------

template<typename ListT, typename InsertT>
struct PushFront
{
   typedef TypeList<InsertT, ListT> type;
};

template<typename ListT>
struct PopFront;

template<typename HeadT, typename TailT>
struct PopFront<TypeList<HeadT, TailT> >
{
   typedef TailT type;
};

template<>
struct PopFront<NilType>
{
   typedef NilType type;
};

// -------------------------------popfrontN ------------------------------------
// TODO this should be a generic Erase algorihtm instead

template<int N, typename ListT>
struct PopFrontN;

template<int N, typename HeadT, typename TailT>
struct PopFrontN<N, TypeList<HeadT, TailT> >
{
   typedef typename PopFrontN<N-1, TailT>::type type;
};

template<typename HeadT, typename TailT>
struct PopFrontN<0, TypeList<HeadT, TailT> >
{
   typedef TypeList<HeadT, TailT> type;
};

template<int N>
struct PopFrontN<N, NilType>
{
   typedef NilType type;
};

// -------------------------------push/pop back ------------------------------------

template<typename ListT, typename InsertT>
struct PushBack;

template<typename HeadT, typename TailT, typename InsertT>
struct PushBack<TypeList<HeadT, TailT>, InsertT>
{
   typedef TypeList<HeadT, typename PushBack<TailT, InsertT>::type> type;
};

template<typename HeadT, typename InsertT>
struct PushBack<TypeList<HeadT, NilType>, InsertT>
{
   typedef TypeList<HeadT, TypeList<InsertT, NilType> > type;
};


template<typename ListT>
struct PopBack;

template<typename HeadT, typename TailT>
struct PopBack<TypeList<HeadT, TailT> >
{
   typedef TypeList<HeadT, typename PopBack<TailT>::type> type;
};

template<typename HeadT>
struct PopBack<TypeList<HeadT, NilType> >
{
   typedef NilType type;
};

template<>
struct PopBack<NilType>
{
   // FIXME maybe generate a compiler error here?!
   typedef NilType type;
};

// --------------------------------------- find ------------------------------------------------

// forward decl
template<typename SearchT, typename ListT, int N=0>
struct Find;

template<typename SearchT, typename HeadT, typename TailT, int N>
struct Find<SearchT, TypeList<HeadT, TailT>, N>
{
    typedef typename if_<(int)is_same<SearchT, HeadT>::value, int_<N>, Find<SearchT, TailT, N+1> >::type type_;
    static const int value = type_::value;
};

template<typename SearchT, typename HeadT, int N>
struct Find<SearchT, TypeList<HeadT, NilType>, N>
{
    typedef typename if_<(int)is_same<SearchT, HeadT>::value, int_<N>, int_<-1> >::type type_;    
    static const int value = type_::value;
};

template<typename SearchT, int N>
struct Find<SearchT, NilType, N>
{
    enum { value = -1 };
};

// -------------------------------- type at -------------------------------------------

/// extract type at given position in typelist; position must be within bounds
template<int N, typename ListT> 
struct TypeAt;

template<int N, typename HeadT, typename TailT> 
struct TypeAt<N, TypeList<HeadT, TailT> >
{
    typedef typename TypeAt<N-1, TailT>::type type;
    typedef const typename TypeAt<N-1, TailT>::type const_type;
};

template<typename HeadT, typename TailT> 
struct TypeAt<0, TypeList<HeadT, TailT> >
{
    typedef HeadT type;
    typedef const HeadT const_type;
};

// ------------------- type at (relaxed, does not matter if boundary was crossed) ---------------------

/// extract type at given position in typelist; if position is out of
/// typelist bounds it always returns the type NilType
template<int N, typename ListT> 
struct RelaxedTypeAt;

template<int N> 
struct RelaxedTypeAt<N, NilType>
{
    typedef NilType type;   ///< save access over end of typelist
    typedef const NilType const_type;
};

template<int N, typename HeadT, typename TailT> 
struct RelaxedTypeAt<N, TypeList<HeadT, TailT> >
{
    typedef typename RelaxedTypeAt<N-1, TailT>::type type;
    typedef const typename RelaxedTypeAt<N-1, TailT>::type const_type;
};

template<typename HeadT, typename TailT> 
struct RelaxedTypeAt<0, TypeList<HeadT, TailT> >
{
    typedef HeadT type;
    typedef const HeadT const_type;
};

// --------------------------- reverse the sequence ------------------------------------

namespace detail
{
   
template<typename ListT, int N>
struct ReverseHelper
{
   typedef TypeList<typename TypeAt<N - 1, ListT>::type, typename ReverseHelper<ListT, N - 1>::type> type;
};

template<typename ListT>
struct ReverseHelper<ListT, 1>
{
   typedef TypeList<typename TypeAt<0, ListT>::type, NilType> type;
};

}   // namespace detail


/// reverse the sequence
template<typename ListT>
struct Reverse
{
   typedef typename detail::ReverseHelper<ListT, Size<ListT>::value>::type type;
};

// ------------------------ Count a certain type within the typelist ---------------------------------

template<typename SearchT, typename ListT>
struct Count;

template<typename SearchT, typename HeadT, typename TailT>
struct Count<SearchT, TypeList<HeadT, TailT> >
{
   enum { value = is_same<SearchT, HeadT>::value + Count<SearchT, TailT>::value };
};

template<typename SearchT>
struct Count<SearchT, NilType>
{
   enum { value = 0 };
};

#endif // TYPELIST_H


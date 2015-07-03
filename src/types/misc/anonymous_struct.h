#ifndef ANONYMOUS_STRUCT_H
#define ANONYMOUS_STRUCT_H

#include <iostream>
/*
template<typename... Values>
struct TValues
{
    TValues(Values... v)
      : TValues<Values...>(v...)
    {

    }
};

template<typename T0>
struct TValues<T0>
{
    TValues<T0>(T0 _v0)
      : v0(_v0) { }
    T0 v0;
};

template<typename T0, typename T1>
struct TValues<T0, T1>
{
    TValues<T0, T1>(T0 _v0, T1 _v1)
      : v0(_v0), v1(_v1) { }
    T0 v0;
    T1 v1;
};

template<typename T0, typename T1, typename T2>
struct TValues<T0, T1, T2>
{
    TValues<T0, T1, T2>(T0 _v0, T1 _v1, T2 _v2)
      : v0(_v0), v1(_v1), v2(_v2) { }
    T0 v0; T1 v1; T2 v2;
};

template<typename T0, typename T1, typename T2, typename T3>
struct TValues<T0, T1, T2, T3>
{
    TValues<T0, T1, T2, T3>(T0 _v0, T1 _v1, T2 _v2, T3 _v3)
      : v0(_v0), v1(_v1), v2(_v2) { }
    T0 v0; T1 v1; T2 v2; T3 v3;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct TValues<T0, T1, T2, T3, T4>
{
    TValues<T0, T1, T2, T3, T4>(T0 _v0, T1 _v1, T2 _v2, T3 _v3, T4 _v4)
      : v0(_v0), v1(_v1), v2(_v2), v3(_v3), v4(_v4) { }
    T0 v0; T1 v1; T2 v2; T3 v3; T4 v4;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
struct TValues<T0, T1, T2, T3, T4, T5>
{
    TValues<T0, T1, T2, T3, T4, T5>(T0 _v0, T1 _v1, T2 _v2, T3 _v3, T4 _v4, T5 _v5)
      : v0(_v0), v1(_v1), v2(_v2), v3(_v3), v4(_v4), v5(_v5) { }
    T0 v0; T1 v1; T2 v2; T3 v3; T4 v4; T5 v5;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct TValues<T0, T1, T2, T3, T4, T5, T6>
{
    TValues<T0, T1, T2, T3, T4, T5, T6>(T0 _v0, T1 _v1, T2 _v2, T3 _v3, T4 _v4, T5 _v5, T6 _v6)
      : v0(_v0), v1(_v1), v2(_v2), v3(_v3), v4(_v4), v5(_v5), v6(_v6) { }
    T0 v0; T1 v1; T2 v2; T3 v3; T4 v4; T5 v5; T6 v6;
};
*/

struct inaccessible
{
    inaccessible(int i) {}
    template <typename T, typename V, void* fail = 0>
    T& operator =(const V& x)
    {
      static_assert(fail == 0, "undefined type");
    }
};

template<typename T0 = inaccessible, typename T1 = inaccessible, typename T2 = inaccessible, typename T3 = inaccessible, typename T4 = inaccessible, typename T5 = inaccessible, typename T6 = inaccessible, typename T7 = inaccessible>
struct TValues
{
    TValues(T0 _v0 = 0, T1 _v1 = 0, T2 _v2 = 0, T3 _v3 = 0, T4 _v4 = 0, T5 _v5 = 0, T6 _v6 = 0, T7 _v7 = 0)
      : v0(_v0), v1(_v1), v2(_v2), v3(_v3), v4(_v4), v5(_v5), v6(_v6), v7(_v7) { }
    T0 v0; T1 v1; T2 v2; T3 v3; T4 v4; T5 v5; T6 v6; T7 v7;
};


#endif // ANONYMOUS_STRUCT_H

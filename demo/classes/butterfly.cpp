
#include "Butterfly.hpp"
#pragma once
#ifndef _Butterfly_CPP_
#define _Butterfly_CPP_

using namespace GMlib;

template <typename T>
inline Butterfly<T>::Butterfly()
: PCurve<T,3>(20, 0, 0)
{
}

template <typename T>
inline bool Butterfly<T>::isClosed() const {
  return true;
}

template <typename T>
inline void Butterfly<T>::eval(T t, int d, bool /*l*/) const
{
  this->_p.setDim( d + 1 );

  const T x = sin(t) * (pow(M_E, cos(t)) - 2 * cos(4 * t) - pow(sin(t/12), 5));
  const T y = cos(t) * (pow(M_E, cos(t)) - 2 * cos(4 * t) - pow(sin(t/12), 5));
  const T z = 0;
  this->_p[0][0] = x;
  this->_p[0][1] = y;
  this->_p[0][2] = z;

}

template <typename T>
inline T Butterfly<T>::getStartP() const {
  return T(0);
}

template <typename T>
inline T Butterfly<T>::getEndP()const {
  return T( 12 * M_PI );
}



#endif // _Butterfly_C

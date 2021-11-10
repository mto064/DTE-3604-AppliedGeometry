
#include "CleliaCurve.hpp"
#pragma once
#ifndef _CleliaCurve_CPP_
#define _CleliaCurve_CPP_

using namespace GMlib;

template <typename T>
inline CleliaCurve<T>::CleliaCurve(T radius, T c)
: PCurve<T,3>(20, 0, 0), _r(radius), _c(c)
{
}

template <typename T>
inline bool CleliaCurve<T>::isClosed() const {
  return true;
}

template <typename T>
inline void CleliaCurve<T>::eval(T t, int d, bool /*l*/) const
{
  this->_p.setDim( d + 1 );

  const T x = _r * cos(t) * cos(_c * t);
  const T y = _r * cos(t) * sin(_c * t);
  const T z = _r * sin(t);
  this->_p[0][0] = x;
  this->_p[0][1] = y;
  this->_p[0][2] = z;

}

template <typename T>
inline T CleliaCurve<T>::getStartP() const {
  return T(0);
}

template <typename T>
inline T CleliaCurve<T>::getEndP()const {
  return T( 2 * M_PI );
}



#endif // _CleliaCurve_C

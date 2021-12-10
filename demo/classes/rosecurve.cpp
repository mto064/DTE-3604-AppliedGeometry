
#include "RoseCurve.hpp"
#pragma once
#ifndef _RoseCurve_CPP_
#define _RoseCurve_CPP_

using namespace GMlib;

template <typename T>
inline RoseCurve<T>::RoseCurve(T radius, int num, int den)
: PCurve<T,3>(20, 0, 0), _r(radius), _num(num), _den(den)
{
}

template <typename T>
inline bool RoseCurve<T>::isClosed() const {
  return true;
}

template <typename T>
inline void RoseCurve<T>::eval(T t, int d, bool /*l*/) const
{
  this->_p.setDim( d + 1 );

  const T k = T(_num /(_den * 1.0));
  const T x = _r * cos(t) * cos(k * t);
  const T y = _r * sin(t) * cos(k * t);
  const T z = 0;
  this->_p[0][0] = x;
  this->_p[0][1] = y;
  this->_p[0][2] = z;

}

template <typename T>
inline T RoseCurve<T>::getStartP() const {
  return T(0);
}

template <typename T>
inline T RoseCurve<T>::getEndP()const {
  if (_den % 2 == 0)
    return T( _den * 2 * M_PI );
  else
    return T(_den * M_PI);
}



#endif // _RoseCurve_C

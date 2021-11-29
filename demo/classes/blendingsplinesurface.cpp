#include "blendingsplinesurface.hpp"
#pragma once
#ifndef _BLENDINGSPLINESURFACE_CPP_
#define _BLENDINGSPLINESURFACE_CPP_

template <typename T>
inline BlendingSplineSurface<T>::BlendingSplineSurface()
{

}

template <typename T>
bool BlendingSplineSurface<T>::isClosedU() const {
  return false;
}

template <typename T>
bool BlendingSplineSurface<T>::isClosedV() const {
  return false;
}

template <typename T>
T BlendingSplineSurface<T>::getStartPU() const {
  return T(0);
}

template <typename T>
T BlendingSplineSurface<T>::getEndPU() const {
  return T(1);
}

template <typename T>
T BlendingSplineSurface<T>::getStartPV() const {
  return T(0);
}

template <typename T>
T BlendingSplineSurface<T>::getEndPV() const {
  return T(1);
}

#endif

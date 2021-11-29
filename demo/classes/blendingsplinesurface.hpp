#pragma once
#ifndef BLENDINGSPLINESURFACE_HPP
#define BLENDINGSPLINESURFACE_HPP

#include "parametrics/gmpsurf.h"

using namespace GMlib;

template <typename T>
class BlendingSplineSurface : public PSurf<T,3>
{
GM_SCENEOBJECT(BlendingSplineSurface)
public:
  BlendingSplineSurface();


// from PSurf
  bool          isClosedU() const override;
  bool          isClosedV() const override;

protected:
  // Virtual function from PSurf that has to be implemented locally
  void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
  T             getStartPU() const override;
  T             getEndPU()   const override;
  T             getStartPV() const override;
  T             getEndPV()   const override;


};

#include "blendingsplinesurface.cpp"
#endif

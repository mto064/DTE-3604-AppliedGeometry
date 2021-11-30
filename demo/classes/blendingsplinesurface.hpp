#pragma once
#ifndef BLENDINGSPLINESURFACE_HPP
#define BLENDINGSPLINESURFACE_HPP

#include <parametrics/gmpsurf.h>
#include "psimplesubsurf.hpp"
#include <vector>

using namespace GMlib;

template <typename T>
class BlendingSplineSurface : public PSurf<T,3>
{
GM_SCENEOBJECT(BlendingSplineSurface)
public:
  BlendingSplineSurface(PSurf<T,3>* surf, int nu, int nv);


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

private:
  void  generateKnotVectorU(T parDelta, int n);
  void  generateKnotVectorV(T parDelta, int n);
  void  generateSubSurfaces(PSurf<T,3>* surf, int nu, int nv);
  int   getKnotIndex(const std::vector<T>& knotVector, const T t) const;

  T     calcW(const std::vector<T>& knot, T t, int i, int d) const;
  T     calcBlending(T w) const;

  bool _closedU;
  bool _closedV;

  std::vector<std::vector<PSimpleSubSurf<T>*>> _subSurfaces;
  std::vector<T>  _knotU;
  std::vector<T>  _knotV;

  T               _startPU;
  T               _endPU;
  T               _startPV;
  T               _endPV;

};

#include "blendingsplinesurface.cpp"
#endif

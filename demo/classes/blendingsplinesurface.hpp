#pragma once
#ifndef BLENDINGSPLINESURFACE_HPP
#define BLENDINGSPLINESURFACE_HPP

#include <parametrics/gmpsurf.h>
#include "subpatch.h"
#include <vector>

using namespace GMlib;

template <typename T>
class BlendingSplineSurface : public PSurf<T,3>
{
GM_SCENEOBJECT(BlendingSplineSurface)
public:
  BlendingSplineSurface(PSurf<T,3>* surf, int nu, int nv);

  DMatrix<SubPatch<T>*> getSubSurfaces() const;

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
  void          localSimulate(double dt) override;
  void          edit(SceneObject* lp) override;

private:
  void  generateKnotVectorU(T parDelta, int n);
  void  generateKnotVectorV(T parDelta, int n);
  void  generateKnotVector(std::vector<T>& knot, T start, T end, bool closed, int n);
  void  generateSubSurfaces(PSurf<T,3>* surf, int nu, int nv);
  int   getKnotIndex(const std::vector<T>& knotVector, const T t) const;


  T     calcW(const std::vector<T>& knot, T t, int i, int d) const;
  Vector<T,2>     calcBlending(T w) const;

  bool _closedU;
  bool _closedV;

  PSurf<T,3>*     _surf;
  //std::vector<std::vector<SubPatch<T>*>> _subSurfaces;
  DMatrix<SubPatch<T>*>      _subSurfaces;
  std::vector<T>  _knotU;
  std::vector<T>  _knotV;

  T               _startPU;
  T               _endPU;
  T               _startPV;
  T               _endPV;

  bool runSimulate = false;

};

#include "blendingsplinesurface.cpp"
#endif

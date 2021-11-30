#include "blendingsplinesurface.hpp"
//#include <vector>
#pragma once
#ifndef _BLENDINGSPLINESURFACE_CPP_
#define _BLENDINGSPLINESURFACE_CPP_

using namespace GMlib;

template <typename T>
inline BlendingSplineSurface<T>::BlendingSplineSurface(PSurf<T,3>* surf, int nu, int nv)
{
  _closedU = surf->isClosedU();
  _closedV = surf->isClosedV();
  _startPU = surf->getParStartU();
  _endPU = surf->getParEndU();
  _startPV = surf->getParStartV();
  _endPV = surf->getParEndV();

  _knotU.clear();
  _knotV.clear();
  generateKnotVectorU(surf->getParDeltaU(), nu);
  generateKnotVectorV(surf->getParDeltaV(), nv);
  generateSubSurfaces(surf, nu, nv);
}

template <typename T>
inline void BlendingSplineSurface<T>::generateKnotVectorU(T parDelta, int n)
{
  int intervals = n;
  T intervalIncr;
  int nKnots = n + 2;

  intervalIncr = parDelta / T(n - 1);

  for (int i = 0; i < nKnots; i++) {
    if (i < 2)
      _knotU.push_back(_startPU);
    else if (i < n)
      _knotU.push_back(_knotU[i-1] + intervalIncr);
    else
      _knotU.push_back(_endPU);
  }

}

template <typename T>
void BlendingSplineSurface<T>::generateKnotVectorV(T parDelta, int n)
{
  int intervals = n;
  T intervalIncr;
  int nKnots = n + 2;

  intervalIncr = parDelta / T(n - 1);

  for (int i = 0; i < nKnots; i++) {
    if (i < 2)
      _knotV.push_back(_startPV);
    else if (i < n)
      _knotV.push_back(_knotV[i-1] + intervalIncr);
    else
      _knotV.push_back(_endPV);
  }

}

template <typename T>
void BlendingSplineSurface<T>::generateSubSurfaces(PSurf<T,3>* surf, int nu, int nv)
{
  _subSurfaces.resize(nu);

  for (int i = 0; i < nu; i++) {
    _subSurfaces[i].resize(nv);
    for (int j = 0; j < nv; j++) {
      _subSurfaces[i][j] = new PSimpleSubSurf<T>(surf,
            _knotU[i], _knotU[i+2], _knotU[i+1],
            _knotV[j], _knotV[j+2], _knotV[j+1]);
    }
  }

}

template <typename T>
inline int BlendingSplineSurface<T>::getKnotIndex(const std::vector<T>& knotVector, const T t) const
{
  int index = 2;
  int nControlPoints = knotVector.size() - 2;

  for (int i = index + 1; i < nControlPoints; i++) {
    if (knotVector[i] == t)
      return i - 1;
    else if (knotVector[i] > t)
      return index;
    index++;
  }
  return index;

}


template <typename T>
void BlendingSplineSurface<T>::eval(T u, T v, int d1, int d2, bool lu, bool lv) const
{

  int iu = getKnotIndex(_knotU, u);
  int iv = getKnotIndex(_knotV, v);

  auto A = _subSurfaces[iu][iv]->getMatrix();

  T b_i_u = calcBlending(calcW(_knotU, u, iu, d1));


}

template <typename T>
inline T BlendingSplineSurface<T>::calcW(const std::vector<T>& knot,T t, int i, int d) const
{
  T temp1 = t - knot[i];
  T temp2 = knot[i+d] - knot[i];
  return temp1 / temp2;
}

template <typename T>
inline T BlendingSplineSurface<T>::calcBlending(T w) const
{
  return (3 * w * w) - (2 * w * w * w);
}


template <typename T>
bool BlendingSplineSurface<T>::isClosedU() const {
  return _closedU;
}

template <typename T>
bool BlendingSplineSurface<T>::isClosedV() const {
  return _closedV;
}

template <typename T>
T BlendingSplineSurface<T>::getStartPU() const {
  return _knotU[2];
}

template <typename T>
T BlendingSplineSurface<T>::getEndPU() const {
  return T(_knotU[_knotU.size() - 2]);
}

template <typename T>
T BlendingSplineSurface<T>::getStartPV() const {
  return _knotV[2];
}

template <typename T>
T BlendingSplineSurface<T>::getEndPV() const {
  return _knotV[_knotV.size() - 2];
}

#endif

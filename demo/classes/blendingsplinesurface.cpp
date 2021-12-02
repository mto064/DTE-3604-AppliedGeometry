#include "blendingsplinesurface.hpp"
//#include <vector>
#pragma once
#ifndef _BLENDINGSPLINESURFACE_CPP_
#define _BLENDINGSPLINESURFACE_CPP_

using namespace GMlib;

template <typename T>
inline BlendingSplineSurface<T>::BlendingSplineSurface(PSurf<T,3>* surf, int nu, int nv) : _surf(surf)
{
  _closedU = surf->isClosedU();
  _closedV = surf->isClosedV();
  _startPU = surf->getParStartU();
  _endPU = surf->getParEndU();
  _startPV = surf->getParStartV();
  _endPV = surf->getParEndV();


  _knotU.clear();
  _knotV.clear();
//  generateKnotVectorU(_endPU - _startPU, nu);
//  generateKnotVectorV(_endPV - _startPV, nv);
  generateKnotVector(_knotU, _startPU, _endPU, _closedU, nu);
  generateKnotVector(_knotV, _startPV, _endPV, _closedV, nv);
  generateSubSurfaces(surf, nu, nv);
}

template <typename T>
inline void BlendingSplineSurface<T>::generateKnotVectorU(T parDelta, int n)
{
  int intervals = n;
  T intervalIncr;
  int nKnots = n + 2;

  if (_closedU) {
    nKnots++;
  }

  intervalIncr = parDelta / T(n - (_closedU ? 0 : 1));

  if (_closedU) {
    for (int i = 0; i < nKnots; i++) {
      if (i < 2)
        _knotU.push_back(_startPU);
      else if (i < n+1)
        _knotU.push_back(_knotU[i-1] + intervalIncr);
      else
        _knotU.push_back(_endPU);
    }
    _knotU[0] = _knotU[1] - intervalIncr;
    _knotU[_knotU.size() - 1] = _knotU[_knotU.size() - 2] + intervalIncr;
  }
  else {
    for (int i = 0; i < nKnots; i++) {
    if (i < 2)
      _knotU.push_back(_startPU);
    else if (i < n)
      _knotU.push_back(_knotU[i-1] + intervalIncr);
    else
      _knotU.push_back(_endPU);
    }
  }


}

template <typename T>
void BlendingSplineSurface<T>::generateKnotVectorV(T parDelta, int n)
{
  int intervals = n;
  T intervalIncr;
  int nKnots = n + 2;

  if (_closedV) {
    nKnots++;
  }

  intervalIncr = parDelta / T(n - (_closedV ? 0 : 1));


  if (_closedV) {
    for (int i = 0; i < nKnots; i++) {
      if (i < 2)
        _knotV.push_back(_startPV);
      else if (i < n+1)
        _knotV.push_back(_knotV[i-1] + intervalIncr);
      else
        _knotV.push_back(_endPV);
    }
    _knotV[0] = _knotV[1] - intervalIncr;
    _knotV[_knotV.size() - 1] = _knotV[_knotV.size() - 2] + intervalIncr;
  }
  else {
    for (int i = 0; i < nKnots; i++) {
    if (i < 2)
      _knotV.push_back(_startPV);
    else if (i < n)
      _knotV.push_back(_knotV[i-1] + intervalIncr);
    else
      _knotV.push_back(_endPV);
    }
  }
}

template <typename T>
void BlendingSplineSurface<T>::generateKnotVector(std::vector<T>& knot, T start, T end, bool closed, int n)
{
  int intervals;
  T intervalIncr;
  int nKnots;

  if (closed) {
    nKnots = n + 3;
    intervals = n;
  }
  else {
    nKnots = n + 2;
    intervals = n - 1;
  }
  T parDelta = end - start;
  intervalIncr = parDelta / T(intervals);

  for (int i = 0; i < nKnots; i++) {
    if (i < 2)
      knot.push_back(start);
    else if (i < nKnots - 2)
      knot.push_back(knot[i-1] + intervalIncr);
    else
      knot.push_back(end);
  }
  if (closed) {
    knot[0] = knot[1] - intervalIncr;
    knot[knot.size() - 1] = knot[knot.size() - 2] + intervalIncr;
  }
}

template <typename T>
void BlendingSplineSurface<T>::generateSubSurfaces(PSurf<T,3>* surf, int nu, int nv)
{
  if (_closedU && _closedV)
    _subSurfaces.setDim(nu+1, nv+1);
  else if (_closedU)
    _subSurfaces.setDim(nu+1, nv);
  else if (_closedV)
    _subSurfaces.setDim(nu, nv+1);
  else
    _subSurfaces.setDim(nu, nv);

  for (int i = 0; i < nu; i++) {
    for (int j = 0; j < nv; j++) {
      _subSurfaces[i][j] = new SubPatch<T>(surf,
            _knotU[i+1],
            _knotV[j+1]);
      this->insert(_subSurfaces[i][j]);
      _subSurfaces[i][j]->setCollapsed(true);
    }
    if (_closedV)
      _subSurfaces[i][nv] = _subSurfaces[i][0];
  }

  if (_closedU) {
    for (int j = 0; j < _subSurfaces.getDim2(); j++) {
      _subSurfaces[_subSurfaces.getDim1() - 1][j] = _subSurfaces[0][j];
    }
  }



}

template <typename T>
inline int BlendingSplineSurface<T>::getKnotIndex(const std::vector<T>& knotVector, const T t) const
{
  int index = 1;
  int nControlPoints = knotVector.size() - 1;

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
  this->_p.setDim( d1+1, d2+1 );
  int iu = getKnotIndex(_knotU, u);
  int iv = getKnotIndex(_knotV, v);

  HqMatrix<float,3> A_00 = _subSurfaces(iu-1)(iv-1)->getMatr();
  HqMatrix<float,3> A_10 = _subSurfaces(iu)(iv-1)->getMatr();
  HqMatrix<float,3> A_01 = _subSurfaces(iu-1)(iv)->getMatr();
  HqMatrix<float,3> A_11 = _subSurfaces(iu)(iv)->getMatr();

  //std::cout << A_11 << std::endl;

  HqMatrix<float,3> A_1 = A_10 - A_00;
  HqMatrix<float,3> A_2 = A_01 - A_00;
  HqMatrix<float,3> A_3 = A_11 - A_01 - A_1;


  Vector<T,2> b_u = calcBlending(calcW(_knotU, u, iu, 1));
  Vector<T,2> b_v = calcBlending(calcW(_knotV, v, iv, 1));

//  HqMatrix<float,3> test = A_1 * b_u[0];
  HqMatrix<float,3> A = A_00 + A_1 * b_u[0] + A_2 * b_v[0] + A_3 * (b_u[0] * b_v[0]);
  HqMatrix<float,3> Au = A_1 * b_u[1] + A_3 * (b_u[1] * b_v[0]);
  HqMatrix<float,3> Av = A_2 * b_v[1] + A_3 * (b_u[0] * b_v[1]);



  DMatrix<Vector<T,3> > S = _surf->evaluate(u, v, d1, d2);

  Point<float,3> p = S[0][0];
  Vector<float,3> Su = S[1][0];
  Vector<float,3> Sv = S[0][1];
  this->_p[0][0] = A * p;
  this->_p[1][0] = Au * p + A * Su;
  this->_p[0][1] = Av * p + A * Sv;

}

template <typename T>
void BlendingSplineSurface<T>::localSimulate(double dt)
{
  if (runSimulate) {
//      static double t1 = 0;
//      static double t2 = 0;

//      t1 += dt;
//      t2 -= dt;
////    for (int i = 0; i < _subSurfaces[0].size(); i++) {
////      _subSurfaces[1][i]->rotate(cos(t * 0.2), Vector<double,3>(0, 1, 0));
////    }
//    _subSurfaces[1][1]->translate(Vector<double,3>(0, 0, dt));
    this->sample(10, 10, 1,1);
    this->setEditDone();
  }

}



template <typename T>
inline T BlendingSplineSurface<T>::calcW(const std::vector<T>& knot,T t, int i, int d) const
{
  T temp1 = t - knot[i];
  T temp2 = knot[i+d] - knot[i];
  return temp1 / temp2;
}

template <typename T>
inline Vector<T,2> BlendingSplineSurface<T>::calcBlending(T w) const
{
  return Vector<T,2>((3 * w * w) - (2 * w * w * w), (6 * w) - (6 * w * w));
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
  return _knotU[1];
}

template <typename T>
T BlendingSplineSurface<T>::getEndPU() const {
  return T(_knotU[_knotU.size() - 2]);
}

template <typename T>
T BlendingSplineSurface<T>::getStartPV() const {
  return _knotV[1];
}

template <typename T>
T BlendingSplineSurface<T>::getEndPV() const {
  return _knotV[_knotV.size() - 2];
}

//template <typename T>
//std::vector<std::vector<SubPatch<T>*>> BlendingSplineSurface<T>::getSubSurfaces() const
//{
//  return _subSurfaces;
//}

#endif

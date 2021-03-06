#include "blendingspline.hpp"
#pragma once
#ifndef _BLENDINGSPLINE_CPP_
#define _BLENDINGSPLINE_CPP_

using namespace GMlib;

template <typename T>
BlendingSpline<T>::BlendingSpline(PCurve<T,3>* curve, int n)
  : PCurve<T,3>(20, 0, 0)
{
  _degree = 1;
  _order = 2;

  _closed = curve->isClosed();

  _startP = curve->getParStart();
  _endP = curve->getParEnd();
  _t.clear();
  generateKnotVector(curve->getParDelta(), n);
  generateControlCurves(curve, n);


  std::random_device rd;
  std::mt19937 gen(rd());
  _gen = gen;
  std::uniform_real_distribution<> dis(0, 1.0);
  _dis = dis;
}


template <typename T>
inline void BlendingSpline<T>::generateKnotVector(T parDelta, int n)
{

  int intervals;
  T intervalIncr;
  int nKnots;

  if (_closed) {
    nKnots = n + _order + 1;
    intervals = n;
  }
  else {
    nKnots = n + _order;
    intervals = n - 1;
  }
  intervalIncr = parDelta / T(intervals);

  for (int i = 0; i < nKnots; i++) {
    if (i < _order)
      _t.push_back(_startP);
    else if (i < nKnots - _order)
      _t.push_back(_t[i-1] + intervalIncr);
    else
      _t.push_back(_endP);
  }

  if (_closed) {
    _t[0] = _t[1] - intervalIncr;
    _t[_t.size()-1] = _t[_t.size()-2] + intervalIncr;
  }

}

template <typename T>
inline void BlendingSpline<T>::generateControlCurves(PCurve<T,3>* curve, int n)
{
  if (_closed)
    _c.resize(n + 1);
  else
    _c.resize(n);

  for (int i = 0; i < n; i++) {
    _c[i] = new GMlib::PSubCurve<T>(curve, _t[i], _t[i+2], _t[i+1]);
    this->insert(_c[i]);
    //_c[i]->toggleDefaultVisualizer();
    _c[i]->sample(100,1);
    //_c[i]->setCollapsed(true);
  }
  if (_closed)
    _c[n] = _c[0];


}

template <typename T>
inline int BlendingSpline<T>::getKnotIndex(const T t, int nControlPoints) const
{
  int index = _degree;
  for (int i = index + 1; i < nControlPoints; i++) {
    if (_t[i] == t)
      return i - 1;
    else if (_t[i] > t)
      return index;
    index++;
  }
  return index;
}

template <typename T>
inline void BlendingSpline<T>::eval(T t, int d, bool /*l*/) const
{
  this->_p.setDim( d + 1 );

  int i = getKnotIndex(t, _t.size() - 1);

  auto c_i =_c[i]->evaluateParent(t, d)[0];
  auto c_i1 = _c[i-1]->evaluateParent(t, d)[0];
  T w_1_i = calcW(t, i, 1);
  auto delta_c = c_i - c_i1;
  auto b = calcBlending(w_1_i);

  auto c_t = c_i1 + b * delta_c;
  this->_p[0] = c_t;

}

template <typename T>
inline void BlendingSpline<T>::localSimulate(double dt)
{
  static double t = 0;
  static int count = 0;
  t += dt;

  if (count == 0)
    this->setColor(Color(_dis(_gen), _dis(_gen), _dis(_gen)));

  //std::cout << t << std::endl;
  int changeDir = (1.0 / _animSpeed) * 4;
  if (count < changeDir) {
    T circlePart = 13;
    for (int i = 2; i < _c.size() - 1; i+= 4) {

      auto dir = Vector<T,3>(cos(circlePart * M_PI / 7), sin(circlePart * M_PI / 7), 0);
      //float speed = 0.01;
      _c[i]->translate( _animSpeed * dir);
      circlePart -= 2;
    }
  }
  else if (count >= changeDir) {
    T circlePart = 13;
    for (int i = 2; i < _c.size() - 1; i+= 4) {

      auto dir = Vector<T,3>(cos(circlePart * M_PI / 7), sin(circlePart * M_PI / 7), 0);
      //float speed = 0.01;
      _c[i]->translate( _animSpeed * -dir);
      circlePart -= 2;
    }
  }
  count += 1;
  if (count >= changeDir * 2)
    count = 0;

  for (int i = 0; i < _c.size() - 1; i+= 4) {
    auto dir = _c[i]->getGlobalPos() - Vector3(0, 0, 0);
    _c[i]->rotateParent(_rotSpeed, dir.normalize());
  }


  this->resample();
  this->setEditDone();
}

template <typename T>
inline T BlendingSpline<T>::calcW(T t, int i, int d) const
{
  T temp1 = t - _t[i];
  T temp2 = _t[i+d] - _t[i];
  return temp1 / temp2;
}

template <typename T>
inline T BlendingSpline<T>::calcBlending(T w) const
{
  return (3 * w * w) - (2 * w * w * w);
}

template <typename T>
inline bool BlendingSpline<T>::isClosed() const {
  return _closed;
}

template <typename T>
inline T BlendingSpline<T>::getStartP() const {
  return _t[_degree];
}

template <typename T>
inline T BlendingSpline<T>::getEndP() const {
  return _t[_t.size() - _order];
}

template <typename T>
inline std::vector<PCurve<T,3>*> BlendingSpline<T>::getControlCurves() const
{
  return _c;
}


#endif  // _BLENDINGSPLINE_CPP_

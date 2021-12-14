#pragma once
#ifndef BLENDINGSPLINE_HPP
#define BLENDINGSPLINEHPP

#include <parametrics/gmpcurve.h>
#include <parametrics/curves/gmperbscurve.h>
#include <vector>
#include <random>

using namespace GMlib;

template <typename T>
class BlendingSpline : public PCurve<T,3> {
  GM_SCENEOBJECT(BlendingSpline)
public:
  BlendingSpline(PCurve<T,3>* curve, int n); // n is number of subcurves

  virtual ~BlendingSpline(){}

  // from PCurve
  bool    isClosed() const override;

  void    setBlending(bool blend);
  bool    getBlending() const;

  std::vector<PCurve<T,3>*> getControlCurves() const;

protected:
  // Virtual functions from PCurve, which have to be implemented locally
  void            eval(T t, int d, bool l) const override;
  T               getStartP() const override;
  T               getEndP()   const override;
  void            localSimulate(double dt) override;

private:
  void  generateKnotVector(T parDelta, int n);
  void  generateControlCurves(PCurve<T,3>* curve, int n);
  int   getKnotIndex(const T t, int nControlPoints) const;
  T     calcW(T t, int i, int d) const;
  T     calcBlending(T w) const;

  std::vector<PCurve<T,3>*>  _c;  // control points
  std::vector<T>        _t;  // knot vectors
  int                   _degree;
  int                   _order;
  bool                  _closed;
  T                     _startP;
  T                     _endP;
  float                 _animSpeed = 0.02;
  float                 _rotSpeed = 0.05;

  //std::random_device rd;
  std::mt19937 _gen;
  std::uniform_real_distribution<> _dis;

};


#include "blendingspline.cpp"
#endif  // BLENDINGSPLINE_HPP

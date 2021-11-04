#pragma once
#ifndef BSPLINE_HPP
#define BSPLINE_HPP

#include <parametrics/gmpcurve.h>
#include <vector>

using namespace GMlib;

template <typename T>
class BSpline : public PCurve<T,3> {
  GM_SCENEOBJECT(BSpline)
public:
  BSpline(const DVector< Vector<T, 3>> &c, const T startP, const T endP, bool closed=false);
  BSpline(const DVector< Vector<T, 3>> &p, int n, bool closed =false);  // n control points we want
  BSpline(const BSpline<T>& copy);
  virtual ~BSpline(){}

  // from PCurve
  bool    isClosed() const override;

  void    setBlending(bool blend);
  bool    getBlending() const;
  DVector<Vector<T,3>> getControlPoints() const;

protected:
  // Virtual functions from PCurve, which have to be implemented locally
  void            eval(T t, int d, bool l) const override;
  T               getStartP() const override;
  T               getEndP()   const override;


private:
  void        generateKnotVector(int nControlPoints);
  void        generateControlPoints(const DVector<Vector<T,3>> &p, int n);
  int         getKnotIndex(const T t, int nControlPoints) const;
  T           calcW(T t, int i, int d) const;
  Vector<T,3> calcB(T t, int i) const;
  T           calcBlending(T w) const;

  DVector<Vector<T,3>>  _c;  // control points
  std::vector<T>        _t;  // knot vectors
  int                   _d;  // Polynomial degree
  int                   _k;  // order of Bspline
  T                     _startP;
  T                     _endP;
  bool                  _closed;
  bool                  _blend;

};


#include "bspline.cpp"
#endif // BSPLINE_H

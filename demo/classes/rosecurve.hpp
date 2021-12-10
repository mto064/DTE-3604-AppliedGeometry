#pragma once
#ifndef RoseCurve_HPP
#define RoseCurve_HPP

#include <parametrics/gmpcurve.h>

using namespace GMlib;

template <typename T>
class RoseCurve : public PCurve<T,3> {
  GM_SCENEOBJECT(RoseCurve)
public:
  RoseCurve(T radius, int num, int den);
  virtual ~RoseCurve(){}

  // from PCurve
  bool            isClosed() const override;

protected:
  // Virtual functions from PCurve, which have to be implemented locally
  void            eval(T t, int d, bool l) const override;
  T               getStartP() const override;
  T               getEndP()   const override;

  //void            computeSurroundingSphere( const std::vector<DVector<Vector<T,3>>>& /*p*/, Sphere<double,3>& s ) const override;


  T           _r;
  int         _num;
  int         _den;

};


#include "RoseCurve.cpp"
#endif // RoseCurve_HPP

#pragma once
#ifndef CLELIACURVE_HPP
#define CLELIACURVE_HPP

#include <parametrics/gmpcurve.h>

using namespace GMlib;

template <typename T>
class CleliaCurve : public PCurve<T,3> {
  GM_SCENEOBJECT(CleliaCurve)
public:
  CleliaCurve(T radius, T c);
  virtual ~CleliaCurve(){}

  // from PCurve
  bool            isClosed() const override;

protected:
  // Virtual functions from PCurve, which have to be implemented locally
  void            eval(T t, int d, bool l) const override;
  T               getStartP() const override;
  T               getEndP()   const override;

  //void            computeSurroundingSphere( const std::vector<DVector<Vector<T,3>>>& /*p*/, Sphere<double,3>& s ) const override;


  T         _r;
  T         _c;

};


#include "CleliaCurve.cpp"
#endif // CLELIACURVE_HPP

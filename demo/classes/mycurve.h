#pragma once
#ifndef MYCURVE_H
#define MYCURVE_H

#include <parametrics/gmpcurve.h>

using namespace GMlib;

  template <typename T>
  class MyCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(MyCurve)
  public:
    MyCurve(T radius);
    virtual ~MyCurve(){}

    // from PCurve
    bool            isClosed() const override;

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d, bool l) const override;
    T               getStartP() const override;
    T               getEndP()   const override;

    //void            computeSurroundingSphere( const std::vector<DVector<Vector<T,3>>>& /*p*/, Sphere<double,3>& s ) const override;


    T               _rx;
    T               _ry;

  };


#include "mycurve.c"
#endif // MYCURVE_H

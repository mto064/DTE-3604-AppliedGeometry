#pragma once
#include <parametrics/gmpcurve.h>

namespace GMlib {

  //template <typename T>
  class MyCurve : public PCurve<double,3> {
    GM_SCENEOBJECT(MyCurve)
  public:
    MyCurve(double radius);


    // from PCurve
    bool            isClosed() const override;

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(double t, int d, bool l) const override;
    double               getStartP() const override;
    double               getEndP()   const override;

    //void            computeSurroundingSphere( const std::vector<DVector<Vector<double,3>>>& /*p*/, Sphere<double,3>& s ) const override;


    double               _rx;
    double               _ry;

  };
}

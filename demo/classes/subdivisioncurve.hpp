#pragma once
#ifndef SUBDIVUSIONCURVE_HPP
#define SUBDIVISIONCURVE_HPP

#include <parametrics/gmpcurve.h>
#include <vector>

using namespace GMlib;

template <typename T>
class SubDivisionCurve : public PCurve<T,3>
{
  GM_SCENEOBJECT(SubDivisionCurve)
public:
  SubDivisionCurve(const DVector< Vector<T, 3>> &p);

  virtual ~SubDivisionCurve(){}

  // from PCurve
  bool    isClosed() const override;
  void    resample( std::vector<DVector<Vector<T,3>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const override;

protected:
  // Virtual functions from PCurve, which have to be implemented locally
  void            eval(T t, int d, bool l) const override;
  T               getStartP() const override;
  T               getEndP()   const override;




private:
  void doublePoints(std::vector<DVector<Vector<T,3>>>& p, int n) const;
  void smoothPoints(std::vector<DVector<Vector<T,3>>>& p, int n) const;

  DVector<Vector<T,3>>  _points;
  bool                  _closed;
  int                   _d;

};

#include "subdivisioncurve.cpp"
#endif // SUBDIVISIONCURVE_HPP

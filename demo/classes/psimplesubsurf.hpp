/**********************************************************************************
**
** Copyright (C) 1994 2017 UiT - The Arctic University of Norway
** Contact: GMlib Online Portal
**
** This file is not a part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/


#ifndef GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_HPP
#define GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_HPP

#include <parametrics/gmpsurf.h>
//#include <gmCoreModule>
//#include <gmParametricsModule>



  template <typename T>
  class PSimpleSubSurf : public GMlib::PSurf<T,3> {
    GM_SCENEOBJECT(PSimpleSubSurf)
  public:
  // Constructors and destructor
    PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T sv, T ev);
    PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v);
    PSimpleSubSurf( const PSimpleSubSurf<T>& copy );

    virtual ~PSimpleSubSurf() {}

  protected:
    // Virtual functions from PSurf, which have to be implemented locally
    void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
    T             getStartPU() const override;
    T             getEndPU()   const override;
    T             getStartPV() const override;
    T             getEndPV()   const override;


    // Protected data for the surface
    GMlib::PSurf<T,3>*     _s;  // The original surface
    T                      _su; // Start parameter value in u-direction
    T                      _sv; // Start parameter value in v-direction
    T                      _eu; // End parameter value in u-direction
    T                      _ev; // End parameter value in v-direction
    T                      _u;  // Center parameter value in u-direction
    T                      _v;  // Center parameter value in v-direction
    GMlib::Vector<float,3> _trans;// Translation so center is origin in local coordinate system

  private:

    // Private help functions
    void    set(GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v);

  }; // END class PSimpleSubSurf


#include "psimplesubsurf.cpp"



#endif // GM_PARAMETRICS_SURFACES_PSIMPLESUBSURF_H

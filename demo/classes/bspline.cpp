
#include "bspline.hpp"
#pragma once
#ifndef _BSPLINE_CPP_
#define _BSPLINE_CPP_

using namespace GMlib;

  template <typename T>
  inline BSpline<T>::BSpline(const DVector< Vector<T, 3>> &c,
    const T startP, const T endP, bool closed) : PCurve<T,3>(20, 0, 0)
  {
    _k = 3;
    _d = 2;
    _c = c;
    _startP = startP;
    _endP = endP;
    generateKnotVector(_c.getDim());
    _closed = closed;
    _blend = false;
  }

  template <typename T>
  inline BSpline<T>::BSpline(const DVector< Vector<T, 3>> &p, int n, bool closed) : PCurve<T,3>(20, 0, 0)
  {
    _k = 3;
    _d = 2;
    _blend = false;
    generateControlPoints(p, n);
    _closed = closed;
  }

  template <typename T>
  inline BSpline<T>::BSpline(const BSpline<T>& copy) : PCurve<T, 3> (copy)
  {
  }

  template < typename T>
  void BSpline<T>::generateKnotVector(int nControlPoints)
  {
    //int points = _c.getDim();
    //int knots = points + 3;
    int intervals = nControlPoints - _d;
    T intervalIncr = (_endP - _startP) / T(intervals);

    _t.clear();
//    int i = 0;
//    for (i; i < _k; i++)        // set the 3 first knots to 0(startP)
//        _t.push_back(_startP);
//    double j = 1.0;             // represents current interval
//    for (i; i < _k + nControlPoints; i++) {
//        _t.push_back(_t[i-1] + intervalIncr);
//    }
    for (int i = 0; i <_k + nControlPoints; i++) {
        if (i < _k)
            _t.push_back(_startP);
        else if (i < nControlPoints)
            _t.push_back(_t[i-1] + intervalIncr);
        else
            _t.push_back(_endP);
    }
  }

  template <typename T>
  void BSpline<T>::generateControlPoints(const DVector<Vector<T,3>> &p, int n)
  {
    std::vector<T> x = {0};
    for (int i = 1; i < p.getDim(); i++) {
        x.push_back((p[i] - p[i-1]).getLength() + x.back());
    }
    _startP = x[0];
    _endP = x[x.size() - 1];
    generateKnotVector(n);

    DMatrix<T> A(p.getDim(), n, T(0)); // initialize m*n matrix with 0's
    for (int i = 0; i < x.size(); i++) {
      int j = getKnotIndex(x[i], n);
      auto b = calcB(x[i], j);
      A[i][j - 2] = b[0];
      A[i][j - 1] = b[1];
      A[i][j] = b[2];
    }

    auto A_t = A;
    A_t.transpose();

    auto B_mat = A_t * A;
    auto y = A_t * p;
    B_mat.invert();
    _c = B_mat * y;

  }

  template <typename T>
  inline bool BSpline<T>::isClosed() const {
    return _closed;
  }

  template <typename T>
  inline void BSpline<T>::eval(T t, int d, bool /*l*/) const
  {
    this->_p.setDim( d + 1 );

    int i = getKnotIndex(t, _c.getDim());

    Vector<T,3> b = calcB(t, i);


    this->_p[0] = b[0] * _c[i-2] +
                  b[1] * _c[i-1] +
                  b[2] * _c[i];

    //std::cout << b[0] << " " << b[1] << " " << b[2] << std::endl;
    //std::cout << "p0 = " << this->_p[0] << std::endl;
  }

  template<typename T>
  inline int BSpline<T>::getKnotIndex(const T t, int nControlPoints) const
  {
    int index = _d;
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
  inline T BSpline<T>::calcW(T t, int i, int d) const
  {
    T temp1 = t - _t[i];
    T temp2 = _t[i+d] - _t[i];
    return temp1 / temp2;
  }

  template <typename T>
  inline Vector<T,3> BSpline<T>::calcB(T t, int i) const
  {
    T w_1_i = calcW(t, i, 1);
    T w_2_i = calcW(t, i, 2);
    T w_2_i1 = calcW(t, i - 1, 2);

    if (_blend) {
        w_1_i = calcBlending(w_1_i);
        w_2_i = calcBlending(w_2_i);
        w_2_i1 = calcBlending(w_2_i1);
    }

    T b1 = (1 - w_1_i) * (1 - w_2_i1);
    T b2 = (1 - w_1_i) * (w_2_i1) + (w_1_i) * (1 - w_2_i);
    T b3 = (w_1_i) * (w_2_i);


    return Vector<T,3>(b1, b2, b3);
  }

  template <typename T>
  inline T BSpline<T>::calcBlending(T w) const
  {
    return (3 * w * w) - (2 * w * w * w);
  }

  template <typename T>
  inline T BSpline<T>::getStartP() const {
    return _t[2];
  }

  template <typename T>
  inline T BSpline<T>::getEndP()const {
    return _t[_t.size() - _k];
  }

  template <typename T>
  inline void BSpline<T>::setBlending(bool blend){
    _blend = blend;
  }

  template <typename T>
  inline bool BSpline<T>::getBlending() const {
    return _blend;
  }

  template <typename T>
  inline DVector<Vector<T,3>> BSpline<T>::getControlPoints() const {
    return _c;
  }




#endif // _BSpline_C

#include "mycurve.hpp"


namespace GMlib {

  //template <typename T>
  MyCurve::MyCurve(double radius) : PCurve<double,3>(20, 0, 7)
  {
    _rx = radius;
    _ry = radius;
  }

  //template <typename T>
  bool MyCurve::isClosed() const {
    return true;
  }

  //template <typename T>
  void MyCurve::eval(double t, int d, bool /*l*/) const
  {
    this->_p.setDim( d + 1 );

    const double ct_x = _rx * (6 * cos(t) - cos(6*t));
    const double st_y = _ry * (6 * sin(t) - sin(6*t));

    this->_p[0][0] = ct_x;
    this->_p[0][1] = st_y;
    this->_p[0][2] = double(0);

  }

  //template <typename T>
  double MyCurve::getStartP() const {
    return double(0);
  }

  //template <typename T>
  double MyCurve::getEndP()const {
    return double( M_2PI );
  }

  //template <typename T>
//  void MyCurve::computeSurroundingSphere( const std::vector<DVector<Vector<double,3>>>& /*p*/, Sphere<double,3>& s ) const {

//     s.resetPos(Point<double,3>(double(0)));
//     if(_rx > _ry)
//       s.resetRadius(_rx);
//     else
//       s.resetRadius(_ry);
//  }

}

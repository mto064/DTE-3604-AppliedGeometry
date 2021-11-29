//***********************************************************
  // Include PSimpleSubSurf class function implementations ****
  //       sometimes located in a simplesubsurf.c file     ****
  //***********************************************************

  //*****************************************
  // Constructors and destructor           **
  //*****************************************

template <typename T>
inline
PSimpleSubSurf<T>::PSimpleSubSurf( GMlib::PSurf<T,3>* s, T su, T eu, T sv, T ev )
{
  set(s, su, eu, (su+eu)/2, sv, ev, (sv+ev)/2);
  // Set local coordinate system, origin in center point
  GMlib::DMatrix<GMlib::Vector<T,3> > tr = _s->evaluateParent( _u, _v, 0, 0 );
  _trans = tr[0][0];
  this->translateParent(_trans);
}


template <typename T>
inline
PSimpleSubSurf<T>::PSimpleSubSurf( PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v )
{
  set(s, su, eu, u, sv, ev, v);
  // Set local coordinate system, origin in center point
  GMlib::DMatrix<GMlib::Vector<T,3> > tr = _s->evaluateParent( _u, _v, 0, 0 );
  _trans = tr[0][0];
  this->translateParent( _trans );
}


template <typename T>
inline
PSimpleSubSurf<T>::PSimpleSubSurf( const PSimpleSubSurf<T>& copy ) : GMlib::PSurf<T,3>( copy )
{
  set(copy._s, copy._su, copy._eu, copy._u, copy._sv, copy._ev, copy._v);
  // Set local coordinate system, origin in center point
  _trans = copy._trans;
}




//*****************************************************
// Overrided (protected) virtual functons from PSurf **
//*****************************************************

template <typename T>
void PSimpleSubSurf<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/) const {

  this->_p.setDim(3,3);
  this->_p = _s->evaluateParent( u, v, d1, d2 );
  this->_p[0][0] -=  _trans;
}


template <typename T>
T PSimpleSubSurf<T>::getStartPU() const {
  return _su;
}


template <typename T>
T PSimpleSubSurf<T>::getEndPU() const {
  return _eu;
}


template <typename T>
T PSimpleSubSurf<T>::getStartPV() const {
  return _sv;
}


template <typename T>
T PSimpleSubSurf<T>::getEndPV() const {
  return _ev;
}



//***************************
// Private help functions  **
//***************************

template <typename T>
inline
void PSimpleSubSurf<T>::set(GMlib::PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v) {
  _s  = s;
  _su = su;
  _sv = sv;
  _eu = eu;
  _ev = ev;
  _u  = u;
  _v  = v;
}

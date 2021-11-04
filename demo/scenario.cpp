
#include <iostream>

#include "scenario.h"
#include "testtorus.h"


// hidmanager
#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <scene/light/gmpointlight.h>
#include <scene/sceneobjects/gmpathtrack.h>
#include <scene/sceneobjects/gmpathtrackarrows.h>
#include <parametrics/curves/gmpline.h>
#include <parametrics/surfaces/gmpsphere.h>

#include <parametrics/curves/gmpbsplinebasiscurve.h>
#include <parametrics/curves/gmpcircle.h>
#include "classes/mycurve.h"
#include "classes/bspline.hpp"
#include "classes/subdivisioncurve.hpp"
//#include "classes/mycurve.cpp"

// qt
#include <QQuickItem>


template <typename T>
inline
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  out << v.size() << std::endl;
  for(uint i=0; i<v.size(); i++) out << " " << v[i];
  out << std::endl;
  return out;
}

using Vector3 = GMlib::Vector<double,3>;


void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight( light, false );

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3>  init_cam_pos( 0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  1.0f, 0.0f, 0.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


  /***************************************************************************
   *                                                                         *
   * Standar example, including path track and path track arrows             *
   *                                                                         *
   ***************************************************************************/

  GMlib::Material mm(GMlib::GMmaterial::polishedBronze());
  mm.set(45.0);

  bool showAxis = true;

  if (showAxis) {
    // x-coord = red
    auto xLine = new GMlib::PLine<float>(GMlib::Point<float,3>(0,0,0),
                                  GMlib::Point<float,3>(50,0,0));
    xLine->toggleDefaultVisualizer();
    xLine->sample(100, 100);
    this->scene()->insert(xLine);

    // y-coord = green
    auto yLine = new GMlib::PLine<float>(GMlib::Point<float,3>(0,0,0),
                                  GMlib::Point<float,3>(0,50,0));
    yLine->toggleDefaultVisualizer();
    yLine->sample(100, 100);
    yLine->setColor(GMlib::Color(0, 255, 0));
    this->scene()->insert(yLine);

    // z-coord = blue
    auto zLine = new GMlib::PLine<float>(GMlib::Point<float,3>(0,0,0),
                                  GMlib::Point<float,3>(0,0,50));
    zLine->toggleDefaultVisualizer();
    zLine->sample(100, 100);
    zLine->setColor(GMlib::Color(0, 0, 255));
    this->scene()->insert(zLine);
  }


  Vector3 points[] {
    Vector3(4.0, 0.0, 0.0),
    Vector3(4.0, 5.0, 0.0),
    Vector3(8.0, 8.0, 0.0),
    Vector3(16.0, 8.0, 0.0),
    Vector3(20.0, 5.0, 0.0),
    Vector3(20.0, 0.0, 0.0)
  };

//  Vector3 points[] {
//    Vector3(0.0, 0.0, 0.0),
//    Vector3(-2.0, 5.0, 0.0),
//    Vector3(3.0, 8.0, 0.0),
//    Vector3(8.0, 5.0, 0.0),
//    Vector3(10.0, 2.0, 0.0),
//    Vector3(7.0, 0.0, 0.0),
//    Vector3(6.0, 4.0, 0.0),
//    Vector3(2.0, 4.0, 0.0),
//    Vector3(2.0, 1.0, 0.0),
//    Vector3(3.0, 0.0, 0.0)
//  };

  int size = sizeof(points) / sizeof(points[0]);
//  int size2 = sizeof(points2) / sizeof(points2[0]);

//  for (int i = 0; i <= size; i++) {
//    auto sphere = new GMlib::PSphere<double>(0.2);
//    sphere->translate(points[i]);
//    sphere->sample(100, 100, 1, 1);
//    sphere->toggleDefaultVisualizer();
//    this->scene()->insert(sphere);
//  }


  auto controlPoints = GMlib::DVector<Vector3>(size, points);
//  auto bspline = new BSpline<double>(controlPoints, 0.0, 15.0, false);
//  bspline->sample(40, 1);
//  bspline->toggleDefaultVisualizer();
//  bspline->setColor(GMlib::Color(80, 154, 235));
//  this->scene()->insert(bspline);

//  auto bspline2 = new BSpline<double>(controlPoints, 0.0, 15.0, false);
//  bspline2->setBlending(true);
//  bspline2->sample(40, 1);
//  bspline2->toggleDefaultVisualizer();
//  bspline2->setColor(GMlib::Color(0, 200, 30));
//  this->scene()->insert(bspline2);

  // Least Square contructor test  //

  GMlib::DVector<GMlib::Vector<double,3>> circle_p(12);
  for (int i  =0; i < 12; i++){
      circle_p[i] = GMlib::Vector<double,3>
        (10*cos(i * M_PI/6), 10*sin(i * M_PI/6), 0.0);
  }
  for (int i = 0; i < 12; i++) {
    auto sphere = new GMlib::PSphere<double>(0.2);
    sphere->translate(circle_p[i]);
    sphere->sample(100, 100, 1, 1);
    sphere->toggleDefaultVisualizer();
    this->scene()->insert(sphere);
  }

  auto bspline3 = new BSpline<double>(circle_p, 10);
  bspline3->sample(40, 1);
  bspline3->toggleDefaultVisualizer();
  bspline3->setColor(GMlib::GMcolor::blueViolet());
  this->scene()->insert(bspline3);

  auto cp = bspline3->getControlPoints();
  for (int i = 0; i < cp.getDim(); i++) {
    std::cout << cp[i][0] << " " << cp[i][1] << " " << cp[i][2] << std::endl;
    auto s = new GMlib::PSphere<double>(0.2);
    s->translate(cp[i]);
    s->sample(100, 100, 1, 1);
    s->toggleDefaultVisualizer();
    s->setMaterial(GMlib::GMmaterial::emerald());
    this->scene()->insert(s);
  }


  //  SubDivision Curve  //

//  Vector3 div_p[] {
//    Vector3(0.0, 0.0, 0.0),
//    Vector3(0.0, 4.0, 0.0),
//    Vector3(4.0, 4.0, 0.0),
//    Vector3(4.0, 0.0, 0.0)
//  };
//  int div_size = sizeof(div_p) / sizeof(div_p[0]);
//  auto div_points = GMlib::DVector<Vector3>(div_size, div_p);
//  auto subdiv = new SubDivisionCurve<double>(div_points);
//  subdiv->sample(10, 1);
//  subdiv->toggleDefaultVisualizer();
//  this->scene()->insert(subdiv);

}




void Scenario::cleanupScenario() {

}




void Scenario::callDefferedGL() {

  GMlib::Array< const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for(int i=0; i < e_obj.getSize(); i++)
    if(e_obj(i)->isVisible()) e_obj[i]->replot();
}


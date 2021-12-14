
#include <iostream>

#include "scenario.h"
#include "testtorus.h"


// hidmanager
#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <scene/light/gmpointlight.h>
#include <scene/sceneobjects/gmspotlightg.h>
#include <scene/sceneobjects/gmpointlightg.h>
#include <scene/sceneobjects/gmpathtrack.h>
#include <scene/sceneobjects/gmpathtrackarrows.h>
#include <parametrics/curves/gmpline.h>
#include <parametrics/surfaces/gmpsphere.h>

#include <parametrics/curves/gmpbsplinebasiscurve.h>
#include <parametrics/curves/gmpcircle.h>
#include <parametrics/curves/gmpsubcurve.h>
#include <parametrics/surfaces/gmpplane.h>
#include <parametrics/surfaces/gmpcylinder.h>
#include "classes/mycurve.hpp"
#include "classes/bspline.hpp"
#include "classes/subdivisioncurve.hpp"
#include "classes/blendingspline.hpp"
#include "classes/cleliacurve.hpp"
#include "classes/butterfly.hpp"
#include "classes/blendingsplinesurface.hpp"
#include "classes/rosecurve.hpp"

#include "classes/psimplesubsurf.hpp"

// qt
#include <QQuickItem>

#include <random>

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
  GMlib::PointLightG *light = new GMlib::PointLightG(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight( light, false );

  // Insert Sun
  this->scene()->insertSun();

  // insert pointlight
  //this->scene()->insert(light);

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

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1.0);

  bool showAxis = false;
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


  //  Model Curve //
  bool showModelBlending = false;
  if (showModelBlending)
  {
    auto mycurve = new MyCurve<double>(1.0);
//    mycurve->sample(500, 1);
//    mycurve->toggleDefaultVisualizer();
//    mycurve->setColor(GMlib::GMcolor::crimson());
//    this->scene()->insert(mycurve);

    auto blendspline = new BlendingSpline<double>(mycurve, 15);
    blendspline->sample(500, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::blueViolet());
    blendspline->translate(Vector3(0, 0, 4));
    this->scene()->insert(blendspline);
    // show control curves
    auto cc = blendspline->getControlCurves();
    for (auto& curve : cc) {
      curve->sample(50, 1);
      curve->toggleDefaultVisualizer();
      this->scene()->insert(curve);
    }
  }

  bool showClelia = false;
  if (showClelia)
  {
  float r = 3.0;
    auto clelia = new CleliaCurve<double>(r, (7.0/5.0));
    clelia->sample(300, 1);
//    clelia->toggleDefaultVisualizer();
//    clelia->setColor(GMlib::GMcolor::darkBlue());
//    this->scene()->insert(clelia);

    auto blendspline = new BlendingSpline<double>(clelia, 28);
    blendspline->sample(200, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::blueViolet());
    //blendspline->translate(Vector3(0, 0, 4));
    this->scene()->insert(blendspline);
    // show control curves
    auto cc = blendspline->getControlCurves();

    for (int i = 0; i < cc.size(); i++) {
      if (i % 4 == 2) {
        cc[i]->setCollapsed(true);
      }
    }
//    cc[2]->translate(Vector3(r * cos(13 * M_PI / 7), r * sin(13 * M_PI / 7), 0));
//    cc[6]->translate(Vector3(r * cos(11 * M_PI / 7), r * sin(11 * M_PI / 7), 0));
//    cc[10]->translate(Vector3(r * cos(9 * M_PI / 7), r * sin(9 * M_PI / 7), 0));
//    cc[14]->translate(Vector3(r * cos(7 * M_PI / 7), r * sin(7 * M_PI / 7), 0));
//    cc[18]->translate(Vector3(r * cos(5 * M_PI / 7), r * sin(5 * M_PI / 7), 0));
//    cc[22]->translate(Vector3(r * cos(3 * M_PI / 7), r * sin(3 * M_PI / 7), 0));
//    cc[26]->translate(Vector3(r * cos(1 * M_PI / 7), r * sin(1 * M_PI / 7), 0));
  }

  bool showButterfly = false;
  if (showButterfly)
  {
    auto butterfly = new Butterfly<double>();
//    butterfly->sample(500, 1);
//    butterfly->toggleDefaultVisualizer();
//    butterfly->setColor(GMlib::GMcolor::aliceBlue());
//    this->scene()->insert(butterfly);

    auto blendspline = new BlendingSpline<double>(butterfly, 100);
    blendspline->sample(500, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::darkGreen());
    blendspline->translate(Vector3(0, 0, 4));
    this->scene()->insert(blendspline);



    auto cc = blendspline->getControlCurves();
    for (auto& curve : cc) {
      curve->sample(100, 1);
      curve->toggleDefaultVisualizer();
      curve->setColor(Color(dis(gen), dis(gen), dis(gen)));
      this->scene()->insert(curve);
    }
  }

  bool showRoseCurve = false;
  if (showRoseCurve)
  {
    auto rose = new RoseCurve<double>(3.0, 7.0, 4.0);
//    rose->sample(300, 1);
//    rose->toggleDefaultVisualizer();
//    rose->setColor(GMlib::GMcolor::darkBlue());
//    this->scene()->insert(rose);

    auto blendspline = new BlendingSpline<double>(rose, 56);
    blendspline->sample(400, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::red());
    //blendspline->translate(Vector3(0, 0, 4));
    this->scene()->insert(blendspline);

    auto cc = blendspline->getControlCurves();

//    for (auto& curve : cc) {
//      curve->sample(100, 1);
//      curve->toggleDefaultVisualizer();
//      //curve->setColor(Color(dis(gen), dis(gen), dis(gen)));
//      this->scene()->insert(curve);
//    }
//     for (int i = 0; i < cc.size(); i++) {
//      if ( i % 4 == 0) {
//        cc[i]->setColor(GMlib::GMcolor::blue());
//        //cc[i]->rotate(-M_PI_2, Vector3(0, 1, 0));
//      }
//    }
//    cc[0]->rotate(M_PI, Vector3(1, 0, 0));
//    auto dir = cc[4]->getGlobalPos() - Vector3(0, 0, 0);
//    cc[4]->rotateParent(M_PI, dir.normalize());
//    std::cout << cc[0]->getGlobalPos() << std::endl;
//    std::cout << cc[4]->getGlobalPos() << std::endl;
//    cc[55]->setColor(GMlib::GMcolor::black());


    cc[0]->setColor(GMlib::GMcolor::black());
    cc[1]->setColor(GMlib::GMcolor::black());
    cc[2]->setColor(GMlib::GMcolor::black());
    cc[3]->setColor(GMlib::GMcolor::black());
  }

  bool showRoseCurve2 = true;
  if (showRoseCurve2)
  {
    auto rose = new RoseCurve<double>(3.0, 7, 5);

    auto blendspline = new BlendingSpline<double>(rose, 28);
    blendspline->sample(400, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::red());
    this->scene()->insert(blendspline);

    auto cc = blendspline->getControlCurves();

    // part 1 : move middle curves,
    double circlePart = 13;
    for (int i = 2; i < cc.size() - 1; i+= 4) {

      auto dir = Vector3(cos(circlePart * M_PI / 7), sin(circlePart * M_PI / 7), 0);
      float speed = 1.0;
      cc[i]->translate( speed * -dir);
      cc[i]->setCollapsed(true);
      circlePart -= 2;
    }

    // part 2
//    auto dir = Vector3(cos(1 * M_PI / 7), sin(1 * M_PI / 7), 0);
//    cc[1]->translateGlobal(dir * 1.5);
//    cc[23]->translateGlobal(dir * 1.5);

//    for (int i = 0; i < cc.size() - 1; i+=4) {
//      auto dir = cc[i]->getGlobalPos() - Vector3(0, 0, 0);
//      cc[i]->rotateParent(M_PI_2, dir.normalize());
//      cc[i]->setCollapsed(true);
//    }



//    cc[0]->rotate(M_PI, Vector3(1, 0, 0));
//    auto dir = cc[4]->getGlobalPos() - Vector3(0, 0, 0);
//    cc[4]->rotateParent(M_PI, dir.normalize());
//    std::cout << cc[0]->getGlobalPos() << std::endl;
//    std::cout << cc[4]->getGlobalPos() << std::endl;
//    cc[55]->setColor(GMlib::GMcolor::black());

//    float x = std::cos((2.0 * M_PI * 3.0)/28.0);
//    float y = std::sin((2.0 * M_PI * 3.0)/28.0);
//    auto newPos = Vector3(x, y, 0);
//    auto trans = Vector3(0, 0, 0) - cc[0]->getGlobalPos();
//    cc[0]->translate(trans);
//    cc[0]->rotate(45, Vector3(0, 0, 1));
//    cc[0]->translate(-trans);
  }


  bool showTestCircle = false;
  if (showTestCircle)
  {
    auto circle = new GMlib::PLine<float>(GMlib::Point<float,3>(0,0,0), GMlib::Point<float,3>(0,20,0));
    circle->sample(100, 1);
    circle->toggleDefaultVisualizer();
    //this->scene()->insert(circle);

    auto blendspline = new BlendingSpline<float>(circle, 4);
    blendspline->sample(100, 1);
    blendspline->toggleDefaultVisualizer();
    blendspline->setColor(GMlib::GMcolor::blueViolet());
    //blendspline->translate(Vector3(0, 0, 4));
    this->scene()->insert(blendspline);

//    auto cc = blendspline->getControlCurves();

//    for (auto& curve : cc) {
//      curve->sample(100, 1);
//      curve->toggleDefaultVisualizer();
//      curve->setColor(Color(dis(gen), dis(gen), dis(gen)));
//      this->scene()->insert(curve);
//    }
//    cc[0]->rotate(M_PI, Vector3(0, 1, 0));

  }

  // ***  B-splines with different b-functions  *** //
  bool showBSplines = false;
  if (showBSplines)
  {
//    Vector3 points[] {
//      Vector3(4.0, 0.0, 0.0),
//      Vector3(4.0, 5.0, 0.0),
//      Vector3(8.0, 8.0, 0.0),
//      Vector3(16.0, 8.0, 0.0),
//      Vector3(20.0, 5.0, 0.0),
//      Vector3(20.0, 0.0, 0.0)
//    };

    Vector3 points[] {
      Vector3(4.0, 0.0, 0.0),
      Vector3(1.0, 5.0, 0.0),
      Vector3(5.0, 10.0, 0.0),
      Vector3(9.0, 7.0, 0.0),
      Vector3(14.0, 11.0, 0.0),
      Vector3(16.0, 5.0, 0.0),
      Vector3(15.0, -1.0, 0.0),
      Vector3(8.0, 1.0, 0.0),
      Vector3(6.0, 4.0, 0.0),
      Vector3(11.0, 5.0, 0.0),
    };

    int size = sizeof(points) / sizeof(points[0]);
    // visualize the control points
    //std::vector<GMlib::PSphere<double>>
//    for (int i = 0; i <= size; i++) {
//      auto sphere = new GMlib::PSphere<double>(0.2);
//      sphere->translate(points[i]);
//      sphere->sample(100, 100, 1, 1);
//      sphere->toggleDefaultVisualizer();
//      this->scene()->insert(sphere);
//    }
//    // draw the lines between points
//    for (int i = 0; i < size; i++) {
//      auto line = new PLine<double>(points[i].toPoint(), points[i+1].toPoint());
//      line->sample(20, 1);
//      line->toggleDefaultVisualizer();
//      this->scene()->insert(line);
//    }

    // create b-spline with no blending
    auto controlPoints = GMlib::DVector<Vector3>(size, points);
    auto bspline = new BSpline<double>(controlPoints, 3.0, 15.0, false);
    bspline->sample(100, 1);
    bspline->toggleDefaultVisualizer();
    bspline->setColor(GMlib::GMcolor::aqua());
    this->scene()->insert(bspline);

    // create a second bspline with blending = true
    auto bspline2 = new BSpline<double>(controlPoints, 0.0, 15.0, false);
    bspline2->setBlending(true);
    bspline2->sample(100, 1);
    bspline2->toggleDefaultVisualizer();
    bspline2->setColor(GMlib::GMcolor::green());
    this->scene()->insert(bspline2);

//    auto blendspline = new BlendingSpline<double>(bspline, 4);
//    blendspline->sample(20, 1);
//    blendspline->toggleDefaultVisualizer();
//    blendspline->setColor(GMlib::GMcolor::brown());
//    this->scene()->insert(blendspline);
  }


  // ***  Least Square contructor test  *** //
  bool showLeastSquare = false;
  if (showLeastSquare)
  {
    // create initial points along the circle
    GMlib::DVector<GMlib::Vector<double,3>> circle_p(12);
    for (int i  =0; i < 12; i++){
        circle_p[i] = GMlib::Vector<double,3>
          (10*cos(i * M_PI/6), 10*sin(i * M_PI/6), 0.0);
    }
    // insert the initial points in the scene
    for (int i = 0; i < 12; i++) {
      auto sphere = new GMlib::PSphere<double>(0.2);
      sphere->translate(circle_p[i]);
      sphere->sample(100, 100, 1, 1);
      sphere->toggleDefaultVisualizer();
      this->scene()->insert(sphere);
    }
    // create a B-spline using least squares contructor
    auto bspline3 = new BSpline<double>(circle_p, 10);
    bspline3->sample(40, 1);
    bspline3->toggleDefaultVisualizer();
    bspline3->setColor(GMlib::GMcolor::blueViolet());
    this->scene()->insert(bspline3);
    // show the generated control points
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
    // draw lines between control points
    for (int i = 0; i < cp.getDim() - 1; i++) {
      auto line = new PLine<double>(cp[i].toPoint(), cp[i+1].toPoint());
      line->sample(20, 1);
      line->toggleDefaultVisualizer();
      this->scene()->insert(line);
    }
  }


  // ***  SubDivision Curve  *** //
  bool showSubDiv = false;
  if (showSubDiv) {
    Vector3 div_p[] {
      Vector3(0.0, 0.0, 0.0),
      Vector3(0.0, 8.0, 0.0),
      Vector3(8.0, 8.0, 0.0),
      Vector3(8.0, 0.0, 0.0)
    };
    int div_size = sizeof(div_p) / sizeof(div_p[0]);
    // draw lines between points
//    for (int i = 0; i < div_size - 1; i++) {
//      auto line = new PLine<double>(div_p[i].toPoint(), div_p[i+1].toPoint());
//      line->sample(20, 1);
//      line->toggleDefaultVisualizer();
//      this->scene()->insert(line);
//    }
//    auto line = new PLine<double>(div_p[0].toPoint(), div_p[3].toPoint());
//    line->sample(20, 1);
//    line->toggleDefaultVisualizer();
//    this->scene()->insert(line);
    auto div_points = GMlib::DVector<Vector3>(div_size, div_p);
    auto subdiv = new SubDivisionCurve<double>(div_points);
    subdiv->sample(3, 1);
    subdiv->toggleDefaultVisualizer();
    this->scene()->insert(subdiv);
  }

  // ***   Blending spline surface   *** //
  bool showBlendSurface = false;
  if (showBlendSurface) {
    auto plane = new GMlib::PPlane<float>(GMlib::Point<float,3>(0, 0, 0), Vector3(6, 0, 0), Vector3(0, 6, 0));
//    plane->sample(10, 10, 1, 1);
//    plane->toggleDefaultVisualizer();
//    plane->setMaterial(GMlib::GMmaterial::emerald());
//    this->scene()->insert(plane);

    auto blendSurf = new BlendingSplineSurface<float>(plane, 3, 3);
    auto subs = blendSurf->getSubSurfaces();
//    for (int i = 0; i < 6; i++) {
//      subs(2)(i)->rotateGlobal(45, GMlib::Vector<float,3>(0, 1, 0));
//    }
//    subs(1)(1)->rotateGlobal(160, Vector3(0, 0, 1));
//    subs(1)(1)->translate(Vector3(0, 0, 4));

    blendSurf->sample(20, 20, 1, 1);
    blendSurf->toggleDefaultVisualizer();
    this->scene()->insert(blendSurf);
  }

  bool showBlendSurf2 = false;
  if (showBlendSurf2) {
    auto cylinder = new GMlib::PCylinder<float>(2, 2, 5);
    cylinder->sample(100, 100, 1, 1);
    cylinder->toggleDefaultVisualizer();
    //cylinder->translate(Vector3(0, 0, 5));
    this->scene()->insert(cylinder);

    auto blendSurf = new BlendingSplineSurface<float>(cylinder, 4, 10);
    blendSurf->sample(100, 100, 1, 1);
    blendSurf->toggleDefaultVisualizer();
    blendSurf->setMaterial(GMlib::GMmaterial::emerald());
    this->scene()->insert(blendSurf);
  }

  bool showBlendTorus = false;
  if (showBlendTorus) {
    auto torus = new GMlib::PTorus<float>(3, 1, 1);
//    torus->sample(100, 100, 1,1 );
//    torus->toggleDefaultVisualizer();
//    scene()->insert(torus);

    auto blendSurf = new BlendingSplineSurface<float>(torus, 5, 8);

    blendSurf->toggleDefaultVisualizer();
    blendSurf->setMaterial(GMlib::GMmaterial::ruby());
    this->scene()->insert(blendSurf);
    auto subs = blendSurf->getSubSurfaces();
//    for (int i = 0; i < 8; i++) {
//      subs(0)(i)->translate(Vector3(3, 0, 0));
//      subs(0)(i)->rotateGlobal(140, Vector3(0, 1, 0));
//      subs(0)(i)->translate(Vector3(3, 0, 0));
//    }

    //subs(0)(1)->rotate(60, Vector3(0, 1, 1));
    blendSurf->sample(100, 100, 1, 1);

  }

}



void Scenario::cleanupScenario() {

}




void Scenario::callDefferedGL() {

  GMlib::Array< const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for(int i=0; i < e_obj.getSize(); i++)
    if(e_obj(i)->isVisible()) e_obj[i]->replot();
}


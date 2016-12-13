#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Camera>
#include <osg/Material>
/*!
 * \brief Main file that adds tester and reference geometries to the scene graph.
 * \author Victoria Rudakova
 * \date 2016-2017
 * \copyright MIT License
*/

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgGA/TrackballManipulator>

#include "SVMData.h"
#include "DraggableWire.h"
#include "EventHandler.h"

const int OSG_WIDTH = 1280;
const int OSG_HEIGHT = 960;

osg::Node* createReferenceNode()
{
    osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3( 0.f, 0.f, 0.f ), 0.25f, 0.5f);
    osg::ShapeDrawable* sd = new osg::ShapeDrawable( cylinder );
    sd->setColor( osg::Vec4( 0.8f, 0.5f, 0.2f, 1.f ) );
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(sd);

    return geode;
}

int main(int, char**)
{
#ifdef _WIN32
    ::SetProcessDPIAware();
#endif

    std::cout << "Line, point and virtual plane intersectors demo." << std::endl;

    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group> root = new osg::Group();

    // create a tester node
    SVMData* svm = new SVMData();
    svm->setTransformFloor(osg::Matrix::identity() * osg::Matrix::translate(0,1.5,0));
    svm->setTransformWall(osg::Matrix::rotate(3.14157*0.5, 1, 0, 0));

    root->addChild(svm);
    osg::Node* node = createReferenceNode();
    root->addChild(node);

    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    root->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    root->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(100,100,OSG_WIDTH, OSG_HEIGHT);
    viewer.addEventHandler(new EventHandler());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    return viewer.run();
}


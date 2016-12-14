/*!
 * \brief Virtual plane intersection class.
 * \details This file is a part of osgIntersectors example program. See more details:
 * https://github.com/vicrucann/osg-intersectors-example
 * \autor Victoria Rudakova
 * \date 2016-2017
 * \copyright MIT License
*/

#ifndef VIRTUALPLANEINTERSECTOR_H
#define VIRTUALPLANEINTERSECTOR_H

#include <tuple>

#include <osg/Referenced>
#include <osg/observer_ptr>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "DraggableWire.h"


/*! \class VirtualPlaneIntersector
 * \brief Performs a raycast intersection with a virtual plane where there geometry is located.
 * This class is not a standard OSG-based intersector. It assumes that all the points of the given
 * geometry lie within the same plane in 3D - virtual plane. Then it performs the raycast of the
 * current camera position and finds an intersection point with the virtual plane. The intersection
 * point is returned in local coordiante system: it has a format `(u,v)`, assuming the z-coordinate is
 * zero.
 * For the math details, refer to http://vicrucann.github.io/tutorials/osg-raycast/
*/
class VirtualPlaneIntersector : public osg::Referenced
{
public:
    typedef std::tuple<double, double, bool> Intersection;

    VirtualPlaneIntersector(DraggableWire* wire);

protected:
    bool getIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, double& u, double& v);

public:
    Intersection getIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
    bool getViewProjectionWorld(osgGA::GUIActionAdapter &aa, osg::Matrix &VPW, osg::Matrix &invVPW);

    void getFarNear(double x, double y, const osg::Matrix &invVPW, osg::Vec3f &near, osg::Vec3f &far);

    bool getRayPlaneIntersection(const osg::Plane &plane, const osg::Vec3f &center, const osg::Vec3f &nearPoint, const osg::Vec3f &farPoint, osg::Vec3f &P);

private:
    osg::observer_ptr<DraggableWire> m_planeGeometry;
};

#endif // VIRTUALPLANEINTERSECTOR_H

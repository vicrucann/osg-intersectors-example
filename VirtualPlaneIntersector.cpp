#include "VirtualPlaneIntersector.h"

#include <assert.h>
#include <iostream>
#include <osg/Plane>
#include <osgViewer/Viewer>
#include <osg/Camera>

VirtualPlaneIntersector::VirtualPlaneIntersector(DraggableWire *wire)
    : osg::Referenced(),
      m_planeGeometry(wire)
{
}

bool VirtualPlaneIntersector::getIntersection(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, double &u, double &v)
{
    osg::Matrix VPW, invVPW;
    if (!this->getViewProjectionWorld(aa, VPW, invVPW)) return false;

    /* get far and near in global 3D coords */
    osg::Vec3f nearPoint, farPoint;

    this->getFarNear(ea.getX(), ea.getY(), invVPW, nearPoint, farPoint);

    /* get intersection point in global 3D coords */
    osg::Vec3f P;
    osg::Plane plane = m_planeGeometry->getPlane();
    osg::Vec3f center = m_planeGeometry->getCenter3D();
    if (!this->getRayPlaneIntersection(plane,center, nearPoint, farPoint, P))
        return false;

    /* get model matrix and its inverse */
    osg::Matrix M = m_planeGeometry->getMatrix();
    osg::Matrix invM;
    if (!invM.invert(M)) return false;

    /* obtain intersection in local 2D point */
    osg::Vec3f p = P * invM;
    assert(std::fabs( p.z()) < 0.000001 );
    u=p.x();
    v=p.y();

    return true;
}

VirtualPlaneIntersector::Intersection VirtualPlaneIntersector::getIntersection(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    double u=0,v=0;
    bool success = this->getIntersection(ea, aa, u, v);
    return std::make_tuple(u,v,success);
}

bool VirtualPlaneIntersector::getViewProjectionWorld(osgGA::GUIActionAdapter &aa, osg::Matrix &VPW, osg::Matrix &invVPW)
{
    if (!m_planeGeometry.get()) return false;
    osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
    if (!viewer) return false;
    osg::Camera* camera = viewer->getCamera();
    if (!camera) return false;
    if (!camera->getViewport()) return false;

    /* get far and near points of the ray */
    VPW = camera->getViewMatrix()
            * camera->getProjectionMatrix()
            * camera->getViewport()->computeWindowMatrix();
    if (!invVPW.invert(VPW)) return false;

    return true;
}

void VirtualPlaneIntersector::getFarNear(double x, double y, const osg::Matrix &invVPW, osg::Vec3f &near, osg::Vec3f &far)
{
    near = osg::Vec3f(x, y, 0.f) * invVPW;
    far = osg::Vec3f(x, y, 1.f) * invVPW;
}

bool VirtualPlaneIntersector::getRayPlaneIntersection(const osg::Plane &plane, const osg::Vec3f &center, const osg::Vec3f &nearPoint, const osg::Vec3f &farPoint, osg::Vec3f &P)
{

    std::vector<osg::Vec3f> ray(2);
    ray[0] = nearPoint;
    ray[1] = farPoint;
    if (plane.intersect(ray)) { // 1 or -1 means no intersection
        std::cerr << "rayPlaneIntersection: not intersection with ray" << std::endl;
        return false;
    }

    osg::Vec3f dir = farPoint-nearPoint;
    if (!plane.dotProductNormal(dir)){
        std::cerr << "rayPlaneIntersection: projected ray is almost parallel to plane. "
                     "Change view point." << std::endl;
        return false;
    }

    if (! plane.dotProductNormal(center-nearPoint)){
        std::cerr << "rayPlaneIntersection: plane contains the line. "
                     "Change view point" << std::endl;
        return false;
    }

    double len = plane.dotProductNormal(center-nearPoint) / plane.dotProductNormal(dir);
    P = dir * len + nearPoint;

    return true;
}

#ifndef VIRTUALPLANEINTERSECTOR_H
#define VIRTUALPLANEINTERSECTOR_H

#include <tuple>
#include <osg/observer_ptr>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include "DraggableWire.h"

class VirtualPlaneIntersector
{
public:
    VirtualPlaneIntersector(DraggableWire* wire);

    bool isVirtualIntersector() const;

    bool getIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, double& u, double& v);

protected:
    bool getViewProjectionWorld(osgGA::GUIActionAdapter &aa, osg::Matrix &VPW, osg::Matrix &invVPW);

    void getFarNear(double x, double y, const osg::Matrix &invVPW, osg::Vec3f &near, osg::Vec3f &far);

    bool getRayPlaneIntersection(const osg::Plane &plane, const osg::Vec3f &center, const osg::Vec3f &nearPoint, const osg::Vec3f &farPoint, osg::Vec3f &P);

private:
    osg::observer_ptr<DraggableWire> m_wire;
};

#endif // VIRTUALPLANEINTERSECTOR_H

#ifndef DRAGGABLEWIRE_H
#define DRAGGABLEWIRE_H

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Switch>
#include <osg/StateSet>
#include <osg/Point>
#include <osg/LineWidth>

class DraggableWire : public osg::Geode
{
public:
    DraggableWire();

    void setColorDefaults();
    void setColorWireSelected();
    void setColorWire(osg::Vec4f color);
    void setColorPoints(osg::Vec4f c1, osg::Vec4f c2, osg::Vec4f c3, osg::Vec4f c4);

    const osg::Vec4f CLR_POINTS = osg::Vec4f(0.4f,0.8f,0.2f,1.f);
    const osg::Vec4f CLR_WIRE = osg::Vec4f(0.6f, 0.6f, 0.6f, 1.f);
    const osg::Vec4f CLR_RED = osg::Vec4f(1,0,0,1);

protected:
    void updateGeometry(osg::Geometry* geom);

private:
    osg::Geometry*      m_wire;
    osg::Geometry*      m_points;
};

#endif // DRAGGABLEWIRE_H

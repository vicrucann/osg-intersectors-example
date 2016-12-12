#include "DraggableWire.h"

#include "assert.h"
#include <iostream>

DraggableWire::DraggableWire()
    : osg::MatrixTransform()
    , m_geode(new osg::Geode)
    , m_wire(new osg::Geometry)
    , m_points(new osg::Geometry)
    , m_selectedPoint(-1)
{
    this->addChild(m_geode);
    m_geode->addDrawable(m_wire);
    m_geode->addDrawable(m_points);

    m_wire->setUseDisplayList(false);
    m_wire->setName("Wire");
    m_points->setUseDisplayList(false);
    m_points->setName("Point");

    osg::Vec3Array* verts = new osg::Vec3Array;
    verts->push_back(osg::Vec3f(1.f, 1.f, 0.f));
    verts->push_back(osg::Vec3f(-1.f, 1.f, 0.f));
    verts->push_back(osg::Vec3f(-1.f, -1.f, 0.f));
    verts->push_back(osg::Vec3f(1.f, -1.f, 0.f));

    osg::Vec4Array* clrWire = new osg::Vec4Array(4);
    m_wire->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, verts->size()));
    m_wire->setVertexArray(verts);
    m_wire->setColorArray(clrWire, osg::Array::BIND_PER_VERTEX);
    this->setColorWire(CLR_WIRE);

    osg::Vec4Array* clrPts = new osg::Vec4Array(4);
    m_points->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, verts->size()));
    m_points->setVertexArray(verts);
    m_points->setColorArray(clrPts, osg::Array::BIND_PER_VERTEX);
    this->setColorPointsDefaults();

    m_points->getOrCreateStateSet()->setAttribute(new osg::Point(20.f), osg::StateAttribute::ON);
    m_wire->getOrCreateStateSet()->setAttribute(new osg::LineWidth(3.5f), osg::StateAttribute::ON);
}

osg::Vec3f DraggableWire::getCenter3D() const
{
    return osg::Vec3f(0,0,0) * this->getMatrix();
}

osg::Plane DraggableWire::getPlane() const
{
    osg::Vec3f P1 = osg::Vec3f(1,0,0) * this->getMatrix();
    osg::Vec3f P2 = osg::Vec3f(1,1,0) * this->getMatrix();
    osg::Vec3f P3 = osg::Vec3f(0,1,0) * this->getMatrix();
    return osg::Plane(P1, P2, P3);
}

const osg::Geode *DraggableWire::getGeode() const
{
    return m_geode;
}

void DraggableWire::editPick(double u, double v)
{
    if (m_selectedPoint < 0 || m_selectedPoint>3) return;
    osg::Vec3Array* vertsWire = static_cast<osg::Vec3Array*> (m_wire->getVertexArray());
    osg::Vec3Array* vertsPoints = static_cast<osg::Vec3Array*> (m_points->getVertexArray());
    assert(vertsWire && vertsPoints);
    (*vertsWire)[m_selectedPoint] = osg::Vec3f(u,v,0);
    (*vertsPoints)[m_selectedPoint] = osg::Vec3f(u,v,0);
    this->updateGeometry(m_wire);
    this->updateGeometry(m_points);
}

void DraggableWire::unselect()
{
    this->setColorWire(CLR_WIRE);
    this->setColorPointsDefaults();
    m_selectedPoint = -1;
}

void DraggableWire::select()
{
    this->setColorWire(CLR_WIRE_HOVER);
    m_selectedPoint = -1;
}

void DraggableWire::pick(int index)
{
    if (index<0 || index > 3) return;
    this->select();
    for (int i=0; i<4; ++i){
        osg::Vec4f clr = i==index? CLR_POINTS_HOVER : CLR_POINTS;
        this->setColorPoint(i, clr);
    }
    m_selectedPoint = index;
}

void DraggableWire::unpick()
{
    this->setColorPointsDefaults();
    m_selectedPoint = -1;
}

void DraggableWire::drag()
{
    if (m_selectedPoint < 0 || m_selectedPoint>3) return;
    this->setColorPointWire(m_selectedPoint, CLR_DRAG);
}

void DraggableWire::dragStop()
{
    if (m_selectedPoint < 0 || m_selectedPoint>3) return;
    this->pick(m_selectedPoint);
}

void DraggableWire::setColorPointsDefaults()
{
    this->setColorPoint(0, CLR_POINTS);
    this->setColorPoint(1, CLR_POINTS);
    this->setColorPoint(2, CLR_POINTS);
    this->setColorPoint(3, CLR_POINTS);

    m_selectedPoint = -1;
}

void DraggableWire::setColorWire(osg::Vec4f color)
{
    osg::Vec4Array* clr = static_cast<osg::Vec4Array*> (m_wire->getColorArray());
    assert(clr);
    assert(clr->size() == 4);
    (*clr)[0] = color;
    (*clr)[1] = color;
    (*clr)[2] = color;
    (*clr)[3] = color;
    this->updateGeometry(m_wire);
}

void DraggableWire::setColorPoint(int index, osg::Vec4f color)
{
    assert(index>=0 && index<4);
    osg::Vec4Array* clr = static_cast<osg::Vec4Array*>(m_points->getColorArray());
    assert(clr);
    (*clr)[index] = color;

    this->updateGeometry(m_points);
}

void DraggableWire::setColorPointWire(int index, osg::Vec4f color)
{
    this->setColorPoint(index, color);

    assert(index>=0 && index<4);
    osg::Vec4Array* clr = static_cast<osg::Vec4Array*>(m_wire->getColorArray());
    assert(clr);
    (*clr)[index] = color;

    this->updateGeometry(m_wire);
}

void DraggableWire::updateGeometry(osg::Geometry *geom)
{
    geom->dirtyDisplayList();
    geom->dirtyBound();
}

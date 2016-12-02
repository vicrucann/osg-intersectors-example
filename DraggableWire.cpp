#include "DraggableWire.h"

#include "assert.h"

DraggableWire::DraggableWire()
    : osg::Geode()
    , m_wire(new osg::Geometry)
    , m_points(new osg::Geometry)
{
    this->addDrawable(m_wire);
    this->addDrawable(m_points);

    m_wire->setName("Wire");
    m_points->setName("Point");

    osg::Vec3Array* verts = new osg::Vec3Array;
    verts->push_back(osg::Vec3f(1.f, 1.f, 0.f));
    verts->push_back(osg::Vec3f(-1.f, 1.f, 0.f));
    verts->push_back(osg::Vec3f(-1.f, -1.f, 0.f));
    verts->push_back(osg::Vec3f(1.f, -1.f, 0.f));

    osg::Vec4Array* clrWire = new osg::Vec4Array(1);
    m_wire->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, verts->size()));
    m_wire->setVertexArray(verts);
    m_wire->setColorArray(clrWire, osg::Array::BIND_OVERALL);

    osg::Vec4Array* clrPts = new osg::Vec4Array(4);
    m_points->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, verts->size()));
    m_points->setVertexArray(verts);
    m_points->setColorArray(clrPts, osg::Array::BIND_PER_VERTEX);

    m_points->getOrCreateStateSet()->setAttribute(new osg::Point(10.f), osg::StateAttribute::ON);
    m_wire->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.f), osg::StateAttribute::ON);
}

void DraggableWire::setColorDefaults()
{
    this->setColorWire(CLR_WIRE);
    this->setColorPoints(CLR_POINTS, CLR_POINTS, CLR_POINTS, CLR_POINTS);
}

void DraggableWire::setColorWireSelected()
{
    this->setColorWire(CLR_RED);
}

void DraggableWire::setColorWire(osg::Vec4f color)
{
    osg::Vec4Array* clr = static_cast<osg::Vec4Array*> (m_wire->getColorArray());
    assert(clr);
    (*clr)[0] = color;
    this->updateGeometry(m_wire);
}

void DraggableWire::setColorPoints(osg::Vec4f c1, osg::Vec4f c2, osg::Vec4f c3, osg::Vec4f c4)
{
    osg::Vec4Array* clr = static_cast<osg::Vec4Array*>(m_points->getColorArray());
    assert(clr);
    (*clr)[0] = c1;
    (*clr)[1] = c2;
    (*clr)[2] = c3;
    (*clr)[3] = c4;

    this->updateGeometry(m_wire);
}

void DraggableWire::updateGeometry(osg::Geometry *geom)
{
    geom->dirtyDisplayList();
    geom->dirtyBound();
}

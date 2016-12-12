#include "SVMData.h"

SVMData::SVMData()
    : osg::Group()
    , m_switch(new osg::Switch())
    , m_wire1(new DraggableWire())
    , m_wire2(new DraggableWire())
{
    this->addChild(m_switch);
    m_switch->addChild(m_wire1);
    m_switch->addChild(m_wire2);
}

void SVMData::setTransformWall(osg::Matrix m)
{
    m_wire1->setMatrix(m);
}

void SVMData::setTransformFloor(osg::Matrix m)
{
    m_wire2->setMatrix(m);
}

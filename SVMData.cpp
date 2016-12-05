#include "SVMData.h"

SVMData::SVMData()
    : osg::Group()
    , m_switch(new osg::Switch())
    , m_T1(new osg::MatrixTransform())
    , m_T2(new osg::MatrixTransform())
    , m_wire1(new DraggableWire())
    , m_wire2(new DraggableWire())
{
    this->addChild(m_switch);
    m_switch->addChild(m_T1);
    m_switch->addChild(m_T2);
    m_switch->setChildValue(m_T1, true);
    m_switch->setChildValue(m_T2, true);


    m_T1->addChild(m_wire1);
    m_T2->addChild(m_wire2);
}

void SVMData::setTransformPhoto(osg::Matrix m)
{
    m_T1->setMatrix(m);
}

void SVMData::setTransformFloor(osg::Matrix m)
{
    m_T2->setMatrix(m);
}

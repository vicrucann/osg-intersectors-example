#ifndef SVMDATA_H
#define SVMDATA_H

#include <osg/Group>
#include <osg/Switch>
#include <osg/MatrixTransform>

#include "DraggableWire.h"

class SVMData : public osg::Group
{
public:
    SVMData();

    void setTransformPhoto(osg::Matrix m);

    void setTransformFloor(osg::Matrix m);

private:
    osg::Switch*            m_switch;
    osg::MatrixTransform*   m_T1;
    osg::MatrixTransform*   m_T2;
    DraggableWire*          m_wire1;
    DraggableWire*          m_wire2;
};

#endif // SVMDATA_H

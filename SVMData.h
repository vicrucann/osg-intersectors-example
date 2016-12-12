#ifndef SVMDATA_H
#define SVMDATA_H

#include <osg/Group>
#include <osg/Switch>

#include "DraggableWire.h"

/*! \class SVMData
 * \brief A tester class that contains two DraggableWire geometries.
*/

class SVMData : public osg::Group
{
public:
    SVMData();

    void setTransformPhoto(osg::Matrix m);

    void setTransformFloor(osg::Matrix m);

private:
    osg::Switch*            m_switch;
    DraggableWire*          m_wire1;
    DraggableWire*          m_wire2;
};

#endif // SVMDATA_H

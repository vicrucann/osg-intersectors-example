/*!
 * \brief A tester class.
 * \details This file is a part of osgIntersectors example program. See more details:
 * https://github.com/vicrucann/osg-intersectors-example
 * \autor Victoria Rudakova
 * \date 2016-2017
 * \copyright MIT License
*/

#ifndef SVMDATA_H
#define SVMDATA_H

#include <osg/Group>
#include <osg/Switch>

#include "DraggableWire.h"

/*! \class SVMData
 * \brief A tester class that contains two DraggableWire geometries which are called "Wall" and "Floor".
*/

class SVMData : public osg::Group
{
public:
    SVMData();

    void setTransformWall(osg::Matrix m);

    void setTransformFloor(osg::Matrix m);

private:
    osg::Switch*            m_switch;
    DraggableWire*          m_wire1;
    DraggableWire*          m_wire2;
};

#endif // SVMDATA_H

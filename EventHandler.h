/*!
 * \brief OSG-based event handler.
 * \details This file is a part of osgIntersectors example program. See more details:
 * https://github.com/vicrucann/osg-intersectors-example
 * \autor Victoria Rudakova
 * \date 2016-2017
 * \copyright MIT License
*/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <tuple>

#include <osgGA/EventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventHandler>
#include <osg/Geode>
#include <osg/observer_ptr>
#include <osg/Drawable>

#include "DraggableWire.h"
#include "LineIntersector.h"
#include "PointIntersector.h"
#include "VirtualPlaneIntersector.h"

enum EDIT_MODE
{
    MOUSE_IDLE = 0x000,
    MOUSE_HOVER_WIRE = 0x100,
    MOUSE_DRAG_WIRE = 0x110,
    MOUSE_HOVER_POINT = 0x200,
    MOUSE_DRAG_POINT = 0x210,

    DRAG_MASK = 0x010
};

/*! \class EventHandler
 * \brief Class that handles events for scene.
 * This class is designed to be used in conjuction with DraggableWire class. The wire
 * plays role of "selection" - something that changes geometry color based on mouse events.
 * For example, the user can select the DraggableWire by hovering over its frame (wire); they can
 * also select a point by hovering over one of the points; at last it is possible to perform a drag
 * operation. For the latter the push, drag and release mouse events are used.
 *
 * The class uses three types of intersectors:
 *
 * 1. Intersector of a raycast and a line - refer to LineIntersector
 * 2. Intersector of a raycast and a point - refer to PointIntersector
 * 3. Intersector of a raycast and a virtual plane - refer to VirtualPlaneIntersector
 *
 * The intersector results help to determine which EDIT_MODE is currently on. Depending on the EDIT_MODE,
 * the visual appearence of geometry and the mouse behaviour change.
*/
class EventHandler : public osgGA::GUIEventHandler
{
public:
    /*! Default constructor */
    EventHandler();

    /*! Re-defined OSG method that behaves depending on the mouse events and current edit mode. */
    bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

protected:
    void doIdleMouse(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);
    void doHoverWire(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);
    void doHoverPoint(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);
    void doDragPoint(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

protected:
    DraggableWire* getDraggableWire(const LineIntersector::Intersection& result);
    int getSelectedPoint(const PointIntersector::Intersection& result);

    EDIT_MODE getModeFromName(const osgUtil::LineSegmentIntersector::Intersection& result, EDIT_MODE mode_default) const;
    EDIT_MODE getModeFromEvent(EDIT_MODE mode, const osgGA::GUIEventAdapter& ea);

    void updateWireGeometry(const LineIntersector::Intersection& intersection);
    void updatePointGeometry(const PointIntersector::Intersection& intersection);
    void updateDragPointGeometry(const std::tuple<double, double, bool>& intersection, const osgGA::GUIEventAdapter& ea);

protected:
    template <typename TypeIntersection, typename TypeIntersector>
    bool getIntersection(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, TypeIntersection &resultIntersection);

    template <typename TResult, typename TIntersector>
    std::tuple<bool, TResult> setMouseState(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, EDIT_MODE modeDefault);

private:
    osg::observer_ptr<DraggableWire>    m_selection; /*!< current selection, if any */
    EDIT_MODE                           m_mode; /*!< current state of the selection in relation to mouse events */
};

#endif // EVENTHANDLER_H

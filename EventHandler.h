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
*/
class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler();

    bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doIdleMouse(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doHoverWire(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doHoverPoint(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doDragPoint(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

protected:
    template <typename TypeIntersection, typename TypeIntersector>
    bool getIntersection(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, TypeIntersection &resultIntersection);

    template <typename TResult, typename TIntersector>
    std::tuple<bool, TResult> setMouseState(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, EDIT_MODE modeDefault);

    template <typename T>
    EDIT_MODE getMouseMode(const T& result, EDIT_MODE mode_default) const;

    EDIT_MODE getMouseSubMode(EDIT_MODE mode, const osgGA::GUIEventAdapter& ea);

    DraggableWire* getDraggableWire(const LineIntersector::Intersection& result);

    int getSelectedPoint(const PointIntersector::Intersection& result);

    void setWireState(const LineIntersector::Intersection& intersection);

    void setPointState(const PointIntersector::Intersection& intersection);

private:
    osg::observer_ptr<DraggableWire>    m_selection;
    EDIT_MODE                           m_mode;
};

#endif // EVENTHANDLER_H

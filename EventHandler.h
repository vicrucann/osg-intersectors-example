#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <osgGA/EventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventHandler>
#include <osg/Geode>
#include <osg/observer_ptr>
#include <osg/Drawable>

#include "DraggableWire.h"
#include "LineIntersector.h"

enum EDIT_MODE
{
    MOUSE_IDLE,
    MOUSE_HOVER_WIRE,
    MOUSE_HOVER_POINT,
    MOUSE_DRAG_WIRE,
    MOUSE_DRAG_POINT
};

class EventHandler : public osgGA::GUIEventHandler
{
public:
    EventHandler();

    bool handle(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doIdleMouse(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

    void doHoverWire(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa);

private:
    template <typename TypeIntersection, typename TypeIntersector>
    bool getIntersection(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, TypeIntersection &resultIntersection);

    template <typename TResult, typename TIntersector>
    bool setEditMode(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, EDIT_MODE modeDefault);

    template <typename T>
    EDIT_MODE getMouseMode(const T& result, EDIT_MODE mode_default) const;

    DraggableWire* getDraggableWire(const LineIntersector::Intersection& result);

    template <typename T>
    void setColorFromMode(const T& selection);

    osg::observer_ptr<DraggableWire>    m_selection;
    EDIT_MODE                           m_mode;
};

#endif // EVENTHANDLER_H

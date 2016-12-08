#include "EventHandler.h"

#include <assert.h>
#include <iostream>
#include <osgViewer/View>
#include <osg/Camera>
#include <osg/ref_ptr>

#include "LineIntersector.h"
#include "SVMData.h"

EventHandler::EventHandler()
    : osgGA::GUIEventHandler()
    , m_selection(0)
    , m_mode(MOUSE_IDLE)
{
}

bool EventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    switch(m_mode){
    case MOUSE_HOVER_WIRE:
        this->doHoverWire(ea, aa);
        break;
    case MOUSE_HOVER_POINT:
        this->doHoverPoint(ea, aa);
        break;
    case MOUSE_DRAG_POINT:
        break;
    case MOUSE_DRAG_WIRE:
    case MOUSE_IDLE:
    default:
        this->doIdleMouse(ea, aa);
        break;
    }
    return false;
}

void EventHandler::doIdleMouse(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::MOVE)
        return;
    std::cout << "idle" << std::endl;

    bool isModeSame = true;
    LineIntersector::Intersection intersection;
    std::tie(isModeSame, intersection) = this->setMouseState<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
    this->setWireState(intersection);
}

void EventHandler::doHoverWire(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::MOVE)
        return;
    std::cout << "hover-wire" << std::endl;

    bool isModeSame = true;
    LineIntersector::Intersection intersectionLine;
    std::tie(isModeSame, intersectionLine) = this->setMouseState<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
    this->setWireState(intersectionLine);
    if (m_mode == MOUSE_IDLE) return;

    PointIntersector::Intersection intersectionPoint;
    std::tie(isModeSame, intersectionPoint) =
            this->setMouseState<PointIntersector::Intersection, PointIntersector>(ea, aa, MOUSE_HOVER_WIRE);
    this->setPointState(intersectionPoint);
}

void EventHandler::doHoverPoint(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::MOVE)
        return;
    std::cout << "hover-points" << std::endl;

    bool isModeSame = true;
    PointIntersector::Intersection intersectionPoint;
    std::tie(isModeSame, intersectionPoint) =
            this->setMouseState<PointIntersector::Intersection, PointIntersector>(ea, aa, MOUSE_HOVER_WIRE);
    this->setPointState(intersectionPoint);
}

DraggableWire* EventHandler::getDraggableWire(const osgUtil::LineSegmentIntersector::Intersection& result)
{
    if (!result.drawable.get()) return NULL;
    osg::Group* parent = result.drawable->getParent(0);
    return parent? dynamic_cast<DraggableWire*>(parent) : NULL;
}

int EventHandler::getSelectedPoint(const osgUtil::LineSegmentIntersector::Intersection &result)
{
    if (!result.drawable.get()) return -1;

    return result.primitiveIndex;
}

void EventHandler::setPointState(const PointIntersector::Intersection &intersection)
{
    if (intersection.drawable.get()){
        // pick a point
        if (!m_selection.get()){
            DraggableWire* wire = this->getDraggableWire(intersection);
            if (!wire) return;
            wire->select();
        }
        if (!m_selection->containsDrawable(intersection.drawable.get())) return;
        int index = this->getSelectedPoint(intersection);
        m_selection->pick(index);
    }
    else {
        // unpick the point
        if (m_selection.get()){
            m_selection->unpick();
        }
    }
}

void EventHandler::setWireState(const LineIntersector::Intersection &intersection)
{
    if (intersection.drawable.get()){
        if (!m_selection.get()){
            DraggableWire* wire = this->getDraggableWire(intersection);
            if (!wire) return;
            wire->select();
            m_selection = wire;
        }
    }
    else {
        if (m_selection.get()){
            m_selection->unselect();
            m_selection = 0;
        }
    }
}

template <typename TypeIntersection, typename TypeIntersector>
bool EventHandler::getIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, TypeIntersection& resultIntersection)
{
    osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
    if (!viewer) return false;
    osg::ref_ptr<TypeIntersector> intersector = new TypeIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
    osgUtil::IntersectionVisitor iv(intersector);
    osg::Camera* cam = viewer->getCamera();
    if (!cam) return false;
    cam->accept(iv);
    if (!intersector->containsIntersections()) return false;

    resultIntersection = intersector->getFirstIntersection();
    return true;
}

template <typename TResult, typename TIntersector>
std::tuple<bool, TResult> EventHandler::setMouseState(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, EDIT_MODE modeDefault)
{
    bool isModeSame = true;
    TResult intersection;
    bool inter_occured = this->getIntersection<TResult, TIntersector>(ea,aa, intersection);

    /* if mouse is hovering over certain drawable, set the corresponding mode */
    if (inter_occured){
        EDIT_MODE mode = this->getMouseMode<TResult>(intersection, modeDefault);
        isModeSame = mode == m_mode;
        m_mode = mode;
    }
    /* if not, or if the mouse left the drawable area, make sure it is in entity select mode */
    else{
        isModeSame = m_mode == modeDefault;
        m_mode = modeDefault;
    }

    return std::make_tuple(isModeSame, intersection);
}

template <typename T>
EDIT_MODE EventHandler::getMouseMode(const T& result, EDIT_MODE mode_default) const
{
    std::string name = result.drawable->getName();
    if (name == "Wire")
        return MOUSE_HOVER_WIRE;
    else if (name == "Point")
        return MOUSE_HOVER_POINT;
    return mode_default;
}



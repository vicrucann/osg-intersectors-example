#include "EventHandler.h"

#include <assert.h>
#include <iostream>
#include <osgViewer/View>
#include <osg/Camera>
#include <osg/ref_ptr>
#include <osg/Plane>

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
    bool handled = true;
    switch(m_mode){
    case MOUSE_HOVER_WIRE:
    case MOUSE_DRAG_WIRE:
        this->doHoverWire(ea, aa);
        break;
    case MOUSE_HOVER_POINT:
        this->doHoverPoint(ea, aa);
        break;
    case MOUSE_DRAG_POINT:
        this->doDragPoint(ea, aa);
        break;
    case MOUSE_IDLE:
    default:
        this->doIdleMouse(ea, aa);
        handled = false;
        break;
    }
    return handled;
}

void EventHandler::doIdleMouse(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::MOVE)
        return;

    bool isModeSame = true;
    LineIntersector::Intersection intersection;
    std::tie(isModeSame, intersection) = this->setMouseState<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
    this->updateWireGeometry(intersection);
}

void EventHandler::doHoverWire(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::MOVE)
        return;

    bool isModeSame = true;
    LineIntersector::Intersection intersectionLine;
    std::tie(isModeSame, intersectionLine) = this->setMouseState<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
    this->updateWireGeometry(intersectionLine);
    if (!isModeSame) return;

    PointIntersector::Intersection intersectionPoint;
    std::tie(isModeSame, intersectionPoint) =
            this->setMouseState<PointIntersector::Intersection, PointIntersector>(ea, aa, MOUSE_HOVER_WIRE);
    this->updatePointGeometry(intersectionPoint);
}

void EventHandler::doHoverPoint(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if ( !(ea.getEventType() == osgGA::GUIEventAdapter::MOVE ||
           (ea.getEventType() == osgGA::GUIEventAdapter::DRAG && ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)) )
        return;

    bool isModeSame = true;
    PointIntersector::Intersection intersectionPoint;
    std::tie(isModeSame, intersectionPoint) =
            this->setMouseState<PointIntersector::Intersection, PointIntersector>(ea, aa, MOUSE_HOVER_WIRE);
    this->updatePointGeometry(intersectionPoint);
}

void EventHandler::doDragPoint(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if (!( (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButtonMask()== osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
           || (ea.getEventType() == osgGA::GUIEventAdapter::DRAG && ea.getButtonMask()== osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
           || (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton()==osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
           ))
        return;

    /* obtain new location of the dragging point and edit the selection */
    osg::ref_ptr<VirtualPlaneIntersector> vpi = new VirtualPlaneIntersector(m_selection.get());
    VirtualPlaneIntersector::Intersection intersection = vpi->getIntersection(ea, aa);
    this->updateDragPointGeometry(intersection, ea);
}

DraggableWire *EventHandler::getDraggableWire(const osgUtil::LineSegmentIntersector::Intersection &result)
{
    if (!result.drawable.get()) return NULL;
    osg::Group* group = result.drawable->getParent(0);
    if (!group) return NULL;
    osg::Group* parent = group->getParent(0);
    return parent? dynamic_cast<DraggableWire*>(parent) : NULL;
}

int EventHandler::getSelectedPoint(const osgUtil::LineSegmentIntersector::Intersection &result)
{
    if (!result.drawable.get()) return -1;

    return result.primitiveIndex;
}

EDIT_MODE EventHandler::getModeFromName(const osgUtil::LineSegmentIntersector::Intersection &result, EDIT_MODE mode_default) const
{
    std::string name = result.drawable->getName();
    if (name == "Wire")
        return MOUSE_HOVER_WIRE;
    else if (name == "Point")
        return MOUSE_HOVER_POINT;
    return mode_default;
}

EDIT_MODE EventHandler::getModeFromEvent(EDIT_MODE mode, const osgGA::GUIEventAdapter &ea)
{
    return static_cast<EDIT_MODE>((ea.getEventType() == osgGA::GUIEventAdapter::DRAG)?
                                      (mode | EDIT_MODE::DRAG_MASK) : mode);
}

void EventHandler::updateWireGeometry(const osgUtil::LineSegmentIntersector::Intersection &intersection)
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

void EventHandler::updatePointGeometry(const osgUtil::LineSegmentIntersector::Intersection &intersection)
{
    if (intersection.drawable.get()){
        // pick a point
        if (!m_selection.get()){
            DraggableWire* wire = this->getDraggableWire(intersection);
            if (!wire) return;
            wire->select();
        }
        if (!m_selection->getGeode()->containsDrawable(intersection.drawable.get())) return;
        int index = this->getSelectedPoint(intersection);
        if (! (m_mode & EDIT_MODE::DRAG_MASK))
            m_selection->pick(index);
        else
            m_selection->drag();
    }
    else {
        // unpick the point
        if (m_selection.get()){
            m_selection->unpick();
        }
    }
}

void EventHandler::updateDragPointGeometry(const std::tuple<double, double, bool> &intersection, const osgGA::GUIEventAdapter &ea)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE || !std::get<2>(intersection)){
        m_selection->dragStop();
        m_mode = MOUSE_HOVER_POINT;
    }
    else {
        m_selection->editPick(std::get<0>(intersection), std::get<1>(intersection));
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
        EDIT_MODE mode = this->getModeFromName(intersection, modeDefault);
        mode = this->getModeFromEvent(mode, ea);
        isModeSame = (mode == m_mode);
        m_mode = mode;
    }
    /* if not, or if the mouse left the drawable area, make sure it is in entity select mode */
    else{
        isModeSame = m_mode == modeDefault;
        m_mode = modeDefault;
    }

    return std::make_tuple(isModeSame, intersection);
}

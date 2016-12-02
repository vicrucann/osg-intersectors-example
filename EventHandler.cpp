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
    // check if mouse is hovering  over the wires on scene
    this->setEditMode<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
}

void EventHandler::doHoverWire(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    this->setEditMode<LineIntersector::Intersection, LineIntersector>(ea, aa, MOUSE_IDLE);
}

DraggableWire* EventHandler::getDraggableWire(const osgUtil::LineSegmentIntersector::Intersection& result)
{
    if (!result.drawable.get()) return NULL;
    osg::Group* parent = result.drawable->getParent(0);
    return parent? dynamic_cast<DraggableWire*>(parent) : NULL;
}

template <typename T>
void EventHandler::setColorFromMode(const T& selection)
{
    if (!selection.drawable.get() && m_selection.get()) {
        /* when mouse is not hovering over anything
         * set the color to defaults and de-select the geometries. */
        std::cout << "Deselect the geometry" << std::endl;
        m_selection->setColorDefaults();
        m_selection = 0;
    }
    else{
        DraggableWire* wire = this->getDraggableWire(selection);
        if (!wire) return;
        std::cout << "Setting select colors for geometry" << std::endl;
        wire->setColorWireSelected();
        m_selection = wire;
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
bool EventHandler::setEditMode(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, EDIT_MODE modeDefault)
{
    bool result = true;
    if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE){
//      if (selected){
        TResult result_drawable;
        bool inter_occured = this->getIntersection<TResult, TIntersector>(ea,aa, result_drawable);

        /* if mouse is hovering over certain drawable, set the corresponding mode */
        if (inter_occured){
            EDIT_MODE mode = this->getMouseMode<TResult>(result_drawable, modeDefault);
            result = mode == m_mode;
            m_mode = mode;
        }
        /* if not, or if the mouse left the drawable area, make sure it is in entity select mode */
        else{
            result = m_mode == modeDefault;
            m_mode = modeDefault;
        }
        this->setColorFromMode(result_drawable);
//      }
    }
    return result;
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

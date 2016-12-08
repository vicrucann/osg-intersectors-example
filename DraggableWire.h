#ifndef DRAGGABLEWIRE_H
#define DRAGGABLEWIRE_H

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Switch>
#include <osg/StateSet>
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/MatrixTransform>

class DraggableWire : public osg::MatrixTransform
{
public:
    DraggableWire();

    /*! \return global center of the wire plane */
    osg::Vec3f getCenter3D() const;

    osg::Plane getPlane() const;

    const osg::Geode* getGeode() const;

    void editPick(double u, double v);

    /*! A method to set up defult colors for wire and point geometries. */
    void unselect();

    /*! A method to set up selected color for all the wire geometry. */
    void select();

protected:
    /*! A method to set up default colors for all the wire geometry. */
    void setColorWireDefaults();

public:
    /*! A method to set up selected color to a specified point.
     * \param index is the point index at which the color will be assigned. */
    void pick(int index);

    /*! A method to set up point selected color to defaults. */
    void unpick();

    /*! A method to set up gragged colors to a point and adjacent edges of the wire. */
    void drag();

    void dragStop();

protected:
    /*! A method to set up default colors to all the points. */
    void setColorPointsDefaults();

    /*! Set a given color to the whole wire geometry. \param color is the color input. */
    void setColorWire(osg::Vec4f color);

    /*! A method to assign a color to a specific point. \param index is the point index, \param color is the input color. */
    void setColorPoint(int index, osg::Vec4f color);

    /*! A method to assign a color to a specific point and wire index. \sa setColorPoint(). */
    void setColorPointWire(int index, osg::Vec4f color);

private:
    const osg::Vec4f CLR_POINTS = osg::Vec4f(0.2f,0.4f,0.1f,1.f);
    const osg::Vec4f CLR_WIRE = osg::Vec4f(0.6f, 0.6f, 0.6f, 1.f);
    const osg::Vec4f CLR_WIRE_HOVER = osg::Vec4f(0.8f,0.4f,0.9f,1.f);
    const osg::Vec4f CLR_POINTS_HOVER = osg::Vec4f(0.4f,0.8f,0.2f,1.f);
    const osg::Vec4f CLR_DRAG = osg::Vec4f(1.f,0.8f,0.4f,1.f);

protected:
    /*! A method to update geometries after new colors are assigned. */
    void updateGeometry(osg::Geometry* geom);

private:
    osg::Geode*         m_geode;
    osg::Geometry*      m_wire; /*!< pointer on a geometry for wireframe */
    osg::Geometry*      m_points; /*!< pointer on a geometry for points */
    int                 m_selectedPoint; /*!< index of a selected point, in a range [0,3]; if none, it is -1 */
    bool                m_selected; /*!< flag to indicate whether the wire is currently selected or not. */
    bool                m_dragged; /*!< flag to indicate whether the wire is currently being dragged or not. */
};

#endif // DRAGGABLEWIRE_H

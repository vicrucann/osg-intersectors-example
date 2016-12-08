#ifndef POINTINTERSECTOR_H
#define POINTINTERSECTOR_H

#include <vector>
#include "LineIntersector.h"
#include <osgUtil/LineSegmentIntersector>
#include <osg/Drawable>

class PointIntersector : public LineIntersector
{
public:
    PointIntersector();
    PointIntersector(const osg::Vec3& start,  const osg::Vec3& end);
    PointIntersector(CoordinateFrame cf, double x, double y);
    PointIntersector(CoordinateFrame cf, const osg::Vec3d& start, const osg::Vec3d& end);

    virtual Intersector* clone(osgUtil::IntersectionVisitor &iv);
    virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable);

};

#endif // POINTINTERSECTOR_H

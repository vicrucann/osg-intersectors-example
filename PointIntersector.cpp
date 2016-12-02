#include "PointIntersector.h"

#include <osg/Geometry>

PointIntersector::PointIntersector()
    : LineIntersector()
{
}

PointIntersector::PointIntersector(const osg::Vec3 &start, const osg::Vec3 &end)
    : LineIntersector(start, end)
{
}

PointIntersector::PointIntersector(osgUtil::Intersector::CoordinateFrame cf, double x, double y)
    : LineIntersector(cf, x, y)
{
}

void PointIntersector::intersect(osgUtil::IntersectionVisitor &iv, osg::Drawable *drawable)
{
    osg::BoundingBox bb = drawable->getBoundingBox();
    bb.xMin() -= m_offset; bb.xMax() += m_offset;
    bb.yMin() -= m_offset; bb.yMax() += m_offset;
    bb.zMin() -= m_offset; bb.zMax() += m_offset;

    osg::Vec3d s(_start), e(_end);
    if (!intersectAndClip(s, e, bb)) return;
    if (iv.getDoDummyTraversal()) return;

    osg::Geometry* geometry = drawable->asGeometry();
    if (geometry)
    {
        osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
        if (!vertices) return;

        osg::Vec3d dir = e - s;
        double invLength = 1.0 / dir.length();
        for (unsigned int i=0; i<vertices->size(); ++i)
        {
            double distance = std::fabs( (((*vertices)[i] - s)^dir).length() );
            distance *= invLength;
            if ( m_offset<distance ) continue;

            Intersection hit;
            hit.ratio = distance;
            hit.nodePath = iv.getNodePath();
            hit.drawable = drawable;
            hit.matrix = iv.getModelMatrix();
            hit.localIntersectionPoint = (*vertices)[i];
            m_hitIndices.push_back(i);
            insertIntersection(hit);
        }
    }
}

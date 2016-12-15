# Line, point and virtual plane intersectors using C++11 and OpenSceneGraph

[![Build Status](https://travis-ci.org/vicrucann/osg-intersectors-example.svg?branch=master)](https://travis-ci.org/vicrucann/osg-intersectors-example)

![Intersectors example: screenshots](https://github.com/vicrucann/osg-intersectors-example/blob/master/images/intersectors.gif)

This is an example project that demonstrates the work of three types of intersectors: **line**, **point** and **virtual plane**. While the line and point intersectors are based on standard OSG intersector, the virtual plane intersector utilizes ray casting algorithm - when the intersection is calculated between the ray and a virtual plane.

## Requirements

* Compiler that supports C++11, e.g., GCC-4.8
* OpenSceneGraph library (>=3.4.0)
* CMake (>= 2.8.11)

## Intersectors

The project is composed of two wire rectangles in two different 3D planes. User can select each of the wires by hovering above them and then drag the wire outline by dragging the corresponding corner point. 

Each intersector is responsible for one part:

1. Line intersector is used to determine over what wire the mouse is hovering. Based on the intersection result, that wire gets "selected" which is depicted by wire's color change. **Magenta** color is used to denote the line intersector is in action.

2. Point intersector is used to determine whether a mouse is hovering over any point of the wire or not. If it is hovering, that point gets "selected" (depicted as color change) and it also enables point dragging in case if mouse was pushed. **Bright green** color for a point geometry is used to denote the intersector is in action.

3. Virtual plane intersector is used to determine the intersection between the ray which is cast from the current camera view and virtual plane which contains the selected wire. This intersector is used for dragging functionality and helps to determine the new position of the drag point in its 3D plane. **Yellow** color is used for a point geometry and a for a part of a line geometry to denote the virtual plane intersector is in action.

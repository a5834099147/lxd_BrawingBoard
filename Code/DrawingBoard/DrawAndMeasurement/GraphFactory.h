#pragma once

#include "shape.h"

class GraphFactory
{
public:
    GraphFactory(void);
    ~GraphFactory(void);

    Shape* CreateGraph(Shape::Code type);

};


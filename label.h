#ifndef LABEL_H
#define LABEL_H

#include <QtDataVisualization/QScatterDataProxy>
#include "const.h"
using namespace QtDataVisualization;

class Label
{
public:
    Label(){}
    Label(float init_x0, float init_y0, float init_z0, float(*init_p)(float*));

    float x0, y0, z0;
    // probability function
    float(*p)(float*); //array: x0 y0 z0 phy x y z
    // directional diagram
    float(*dd)(float angle);
    float field[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];

    void updateField(float phy);
    QScatterDataArray* getArray(float level);
    float getValue(int x, int y, int z)\
        {return field[x*SIZE_Y*PPM*SIZE_Z*PPM + y*SIZE_Z*PPM + z];}
};

#endif // LABEL_H

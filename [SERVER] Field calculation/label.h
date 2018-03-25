#ifndef LABEL_H
#define LABEL_H

#include <QtDataVisualization/QScatterDataProxy>
#include "const.h"
using namespace QtDataVisualization;

class Label
{
public:
    Label(){}
    Label(float init_x0, float init_y0, float init_z0, float init_alfa, float init_beta, float(*init_p)(float*, float(*)(float)), float(*init_dd)(float));

    float x0, y0, z0, alfa, beta; // 0 <= alfa <= PI (from 0X); 0 <= beta <= 2PI (from 0Y in Y0Z)
    float xdd, ydd, zdd; // vector of directional diagram
    // probability function
    float(*p)(float*, float(*)(float)); //array: x0 y0 z0 cos sinsin sincos phy x y z
    // directional diagram
    float(*dd)(float);
    float field[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];

    void updateField(float phy);
    QScatterDataArray* getArray(float level);
    float getValue(int x, int y, int z)\
        {return field[x*SIZE_Y*PPM*SIZE_Z*PPM + y*SIZE_Z*PPM + z];}
};

#endif // LABEL_H

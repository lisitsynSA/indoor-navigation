#ifndef LABEL_H
#define LABEL_H

#include <QtDataVisualization/QScatterDataProxy>
using namespace QtDataVisualization;

class Label
{
public:
    Label(float init_x0, float init_y0, float init_z0, float(*init_p)(float*));

    float x0, y0, z0;
    float(*p)(float*); //array: x0 y0 z0 phy x y z
    float field[40*4*20*4*40*4];

    updateField(float phy);
    QScatterDataArray* getArray(float level);
};

#endif // LABEL_H

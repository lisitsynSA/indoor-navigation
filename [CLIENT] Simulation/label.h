#ifndef LABEL_H
#define LABEL_H

#include <QtDataVisualization/QScatterDataProxy>
#include "const.h"
using namespace QtDataVisualization;

class Label
{
public:
    Label(){}
    Label(int init_number, float init_x0, float init_y0, float init_z0,\
          float init_alfa, float init_beta, float(*init_ddFunc)(float), int init_ddNumber);

    int number;
    float x0, y0, z0, alfa, beta; // 0 <= alfa <= PI (from 0X); 0 <= beta <= 2PI (from 0Y in Y0Z)
    float xdd, ydd, zdd; // vector of directional diagram
    float rssi; // current rssi, -1 is non value
    // directional diagram
    float(*ddFunc)(float);
    int ddNumber;

    void setRSSI(float value);
};

#endif // LABEL_H

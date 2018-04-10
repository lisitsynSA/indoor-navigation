#ifndef LABEL_H
#define LABEL_H

#include <QtDataVisualization/QScatterDataProxy>
using namespace QtDataVisualization;

class Label
{
public:
    Label(){}
    Label(int init_number, float init_x0, float init_y0, float init_z0,\
          float init_alfa, float init_beta, float(*init_p)(float*, float(*)(float)), float(*init_dd)(float));
    ~Label();

    int sizeX, sizeY, sizeZ;// room size
    int PPM;// point per meter
    float PPM_f;// point per meter

    int number;
    float x0, y0, z0, alfa, beta; // 0 <= alfa <= PI (from 0X); 0 <= beta <= 2PI (from 0Y in Y0Z)
    float xdd, ydd, zdd; // vector of directional diagram
    float rssi; // current rssi, -1 is non value
    int time;
    bool calculated;
    // probability function
    float(*p)(float*, float(*)(float)); //array: x0 y0 z0 cos sinsin sincos phy x y z
    // directional diagram
    float(*dd)(float);
    float* field;//[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];

    void updateMemAlloc(int x, int y, int z, int ppm);
    void setRSSI(float value, int RSSItime);
    int updateField();
    QScatterDataArray* getArray(float fieldLevel);
    float getValue(int x, int y, int z);
};

#endif // LABEL_H

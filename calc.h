#ifndef CALC_H
#define CALC_H

#include <QObject>
#include <QVector>
#include "label.h"
#include "data.h"
#include "const.h"

class Calc : public QObject
{
    Q_OBJECT
public:
    Calc(Data* init_data);
    ~Calc();
    Data *graphData;
    QTimer *timer;
    float field[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];
    float value;
    float startValue;
    float endValue;
    QVector<Label*> labels;
    float x, y, z;
    QScatterDataArray* getArray(float level);
    QScatterDataArray* getLabels();
    void updateField();
    float distance(float x1, float y1, float z1, float x2, float y2, float z2)\
        { return pow((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2), 0.5); }

public slots:
    void showField();
    void showFieldEnd(double value);
    void showFieldStart(double value);
    void updateSource();
    void setX(double value) { x = value;}
    void setY(double value) { y = value;}
    void setZ(double value) { z = value;}
    void start();
    void stop();

};

#endif // CALC_H

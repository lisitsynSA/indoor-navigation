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
    float startValue;
    float endValue;
    QVector<Label*> labels;
    QScatterDataArray* getArray(float level);
    QScatterDataArray* getLabels();
    void updateField();

public slots:
    void showField();
    void showFieldEnd(double value);
    void showFieldStart(double value);
    void start();

};

#endif // CALC_H

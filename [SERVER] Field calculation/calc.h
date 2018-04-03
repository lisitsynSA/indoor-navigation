#ifndef CALC_H
#define CALC_H

#include <QObject>
#include <QVector>
#include "label.h"
#include "data.h"
#include "const.h"
#include "message.h"
#include "qcustomplot.h"
#include <QSpinBox>
#include <QTimer>

enum AXIS {X = 0, Y = 1, Z = 2};

class Calc : public QObject
{
    Q_OBJECT
public:
    Calc(Data* init_data, QCustomPlot* init_plot, QCPColorMap* init_colorMap, QCPColorScale *init_colorScale, QSpinBox *init_valuePlot);
    ~Calc();

    Data *graphData;
    QTimer *timer;
    float field[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];
    float fieldValue;
    float startValue;
    float endValue;
    QVector<Label*> labels;
    AXIS typePlot;

    float distance(float x1, float y1, float z1, float x2, float y2, float z2);
    void updateRoom();
    void updateLabels();
    int updateField();
    void updateSource(float x, float y, float z);
    void updateBeacon(float x, float y, float z);
    QScatterDataArray* getArray(float level);

    QCustomPlot* plot;
    QCPColorMap* colorMap;
    QCPColorScale *colorScale;
    QSpinBox *valuePlot;

public slots:
    void onMessageRecieved(Message msg);
    void drawPlot();
    void setTypePlot(int type);
    void setValuePlot();
    double showValueFunc(double value);
    void showField();
    void showFieldEnd(double value);
    void showFieldStart(double value);
    void start();
    void stop();

};

#endif // CALC_H

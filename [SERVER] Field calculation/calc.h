#ifndef CALC_H
#define CALC_H

#include <QObject>
#include <QVector>
#include "label.h"
#include "data.h"
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

    int sizeX, sizeY, sizeZ;// room size
    int PPM;// point per meter
    float PPM_f;// point per meter

    Data *graphData;
    QTimer *timer;
    float* field;//[SIZE_X*PPM*SIZE_Y*PPM*SIZE_Z*PPM];
    float fieldValue;
    float startValue;
    float endValue;
    QVector<Label*> labels;
    AXIS typePlot;

    void updateMemAlloc(int x, int y, int z, int ppm);
    float distance(float x1, float y1, float z1, float x2, float y2, float z2);
    void updateRoom(float x, float y, float z);
    void updateLabels();
    int updateField();
    void updateSource();
    void updateBeacon();
    QScatterDataArray* getArray(float level);

    QCustomPlot* plot;
    QCPColorMap* colorMap;
    QCPColorScale *colorScale;
    QSpinBox *valuePlot;


    float sourceX, sourceY, sourceZ;
    float beaconX, beaconY, beaconZ;
    int32_t beaconTime, cameraTime;
    void calcError();

signals:
    sendCalcError(double err);

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

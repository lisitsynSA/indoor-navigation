#ifndef CALC_H
#define CALC_H

#include <QObject>
#include <QVector>
#include "label.h"
#include "const.h"
#include <QSpinBox>
#include <QTimer>
#include "message.h"

class Calc : public QObject
{
    Q_OBJECT
public:
    Calc();
    ~Calc();
    QTimer *timer;
    QVector<Label*> labels;
    float step;
    float x, y, z;

    void setRSSI(Label* label);
    float distance(float x1, float y1, float z1, float x2, float y2, float z2);

signals:
    void sendMessage(Message msg);

public slots:
    void start();
    void timerHandler();
    void stop();

};

#endif // CALC_H

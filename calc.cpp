#include "calc.h"

//       0  1  2  3   4      5      6   7 8 9
//array: x0 y0 z0 cos sinsin sincos phy x y z
float init_p(float* data, float(*dd)(float))
{
    float x = data[7] - data[0];
    float y = data[8] - data[1];
    float z = data[9] - data[2];
    if (x == 0 && y == 0 && z == 0)
        return 0;
    float vecLen = pow(x*x + y*y + z*z, 0.5);
    float angle = 0;
    float cos_angle = (x*data[3] + y*data[4] + z*data[5])/vecLen;
    if (cos_angle > 1)
        angle = 0;
    else if (cos_angle < -1)
        angle = M_PI;
    else
        angle = acos(cos_angle);
    float R = (1.0/data[6])*(*dd)(angle);
    /*if ((*dd)(angle) < 1)
        qDebug() << x << y << z << "\t"
                 << data[3] << data[4] << data[5] << "\t" << angle\
                 << vecLen << (x*data[3] + y*data[4] + z*data[5])\
                 << (x*data[3] + y*data[4] + z*data[5])/vecLen;*/
    //qDebug() << "phi = " << data[3] << "; R = " << R << "; vecLen = " << vecLen;
    //return pow(M_E, -(R - vecLen)*(R - vecLen)/2.0);
    if (vecLen > R)
    {
        //qDebug() << ">" << 1/(vecLen - R + 1);
        //return 1/(vecLen - R + 1);
        return sin(M_PI/(2*(vecLen - R + 1)));
    }else{
        //qDebug() << ">" << (1 - cos(vecLen*M_PI/R))/2;
        //return (1 - cos(vecLen*M_PI/(R*2)));  // cos(M_PI/2)
        return (1 - cos(vecLen*M_PI/R))/2;      // cos(M_PI)
    }

}

// 0 <= angle  <= PI
float init_dd(float angle)
{
    //return 1; // circle
    //return (1.0 + cos(angle)*cos(angle)*2.0)/3.0;
    return cos(angle)*cos(angle);
    /*if (angle <= M_PI/6)
        return 1;
    else
        return 0;*/ // sector
}

Calc::Calc(Data* init_data, QCustomPlot *init_plot, QCPColorMap *init_colorMap, QCPColorScale *init_colorScale):
    graphData(init_data),
    plot(init_plot),
    colorMap(init_colorMap),
    colorScale(init_colorScale)
{
    startValue = 0;
    endValue = 0;
    Label *newLabel = new Label(0,0,0,atan(pow(2, 0.5)),M_PI/4,&init_p, &init_dd);
    labels.push_back(newLabel);
    newLabel = new Label(SIZE_X,0,SIZE_Z,M_PI - atan(pow(2, 0.5)),3*M_PI/4,&init_p, &init_dd);
    labels.push_back(newLabel);
    newLabel = new Label(0,SIZE_Y,SIZE_Z,atan(pow(2, 0.5)),5*M_PI/4,&init_p, &init_dd);
    labels.push_back(newLabel);
    newLabel = new Label(SIZE_X,SIZE_Y,0,M_PI - atan(pow(2, 0.5)),7*M_PI/4,&init_p, &init_dd);
    labels.push_back(newLabel);

    x = X0;
    y = Y0;
    z = Z0;
    timer = NULL;
    updateLabels();
    updateSource();
    graphData->toggleRanges();
    drawPlot(3, 0);
}

Calc::~Calc()
{
    qDeleteAll(labels);
    labels.clear();
}

void Calc::drawPlot(int type, int value)
{
    float minVal = 1;
    float maxVal = 0;
    if (type == 3)
    {
        // give the axes some labels:
        plot->xAxis->setLabel("Axis X");
        plot->yAxis->setLabel("Axis Y");

        /*colorMap->data()->setSize(50, 50);
        colorMap->data()->setRange(QCPRange(0, 2), QCPRange(0, 2));
        for (int x = 0; x < 50; ++x)
          for (int y = 0; y < 50; ++y)
            colorMap->data()->setCell(x, y, qCos(x/10.0)+qSin(y/10.0));
        colorMap->rescaleDataRange(true);*/

        colorMap->data()->setSize(SIZE_X*PPM, SIZE_Y*PPM);
        colorMap->data()->setRange(QCPRange(0, SIZE_X), QCPRange(0, SIZE_Y));

        for (int nx = 0; nx < SIZE_X*PPM; nx++)
        for (int ny = 0; ny < SIZE_Y*PPM; ny++)
        {
            colorMap->data()->setCell(nx, ny, field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + value*PPM]);
            minVal = std::min(minVal, field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + value*PPM]);
            maxVal = std::max(maxVal, field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + value*PPM]);
        }

        colorScale->setDataRange(QCPRange(minVal, maxVal));
        colorMap->rescaleDataRange(true);
        plot->rescaleAxes();
        plot->replot();
    }
}


void Calc::updateLabels()
{
    // room
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->push_back(QVector3D(SIZE_X, SIZE_Y, SIZE_Z));
    dataArray->push_back(QVector3D(SIZE_X, SIZE_Y, 0));
    dataArray->push_back(QVector3D(SIZE_X, 0, SIZE_Z));
    dataArray->push_back(QVector3D(SIZE_X, 0, 0));
    dataArray->push_back(QVector3D(0, SIZE_Y, SIZE_Z));
    dataArray->push_back(QVector3D(0, SIZE_Y, 0));
    dataArray->push_back(QVector3D(0, 0, SIZE_Z));
    dataArray->push_back(QVector3D(0, 0, 0));
    graphData->updateData(3, dataArray);

    // Labels
    dataArray = new QScatterDataArray;
    for (int i = 0; i < labels.size(); ++i)
        dataArray->push_back(QVector3D(labels.at(i)->x0, labels.at(i)->y0, labels.at(i)->z0));
    graphData->updateData(0, dataArray);

    // direction of diagram
    dataArray = new QScatterDataArray;
    for (int i = 0; i < labels.size(); ++i)
        dataArray->push_back(QVector3D(labels.at(i)->x0 + cos(labels.at(i)->alfa),\
               labels.at(i)->y0 + sin(labels.at(i)->alfa)*sin(labels.at(i)->beta),\
               labels.at(i)->z0 + sin(labels.at(i)->alfa)*cos(labels.at(i)->beta)));
    graphData->updateData(2, dataArray);
}

void Calc::updateSource()
{
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->push_back(QVector3D(x, y, z));
    graphData->updateData(1, dataArray);
    updateField();

    drawPlot(3, 0);
}

float Calc::getPhy(Label* label)
{
    float x0 = x - label->x0;
    float y0 = y - label->y0;
    float z0 = z - label->z0;

    float R = pow(x0*x0 + y0*y0 + z0*z0, 0.5);
    float angle = 0;
    float cos_angle = (x0*label->xdd + y0*label->ydd + z0*label->zdd)/R;
    if (cos_angle > 1)
        angle = 0;
    else if (cos_angle < -1)
        angle = M_PI;
    else
        angle = acos(cos_angle);
    return (1/R)*init_dd(angle);
}

void Calc::updateField()
{
    for (int nx = 0; nx < SIZE_X*PPM; nx++)
    for (int ny = 0; ny < SIZE_Y*PPM; ny++)
    for (int nz = 0; nz < SIZE_Z*PPM; nz++)
    {
        field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] = 1;//0;
    }
    for (int i = 0; i < labels.size(); ++i)
    {
        qDebug() << "Update Label's field:" << i;
        labels.at(i)->updateField(getPhy(labels.at(i)));
        //2/pow((SIZE_X*SIZE_X + SIZE_Y*SIZE_Y + SIZE_Z*SIZE_Z),0.5));
        for (int nx = 0; nx < SIZE_X*PPM; nx++)
        for (int ny = 0; ny < SIZE_Y*PPM; ny++)
        for (int nz = 0; nz < SIZE_Z*PPM; nz++)
        {
            // +=
            field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] *=\
                    labels.at(i)->getValue(nx, ny, nz);
        }
    }
}

void Calc::start()
{
    qDebug() << "Start";
    if (timer == NULL)
        timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showField()));
    timer->start(100); // И запустим таймер
    value = startValue;
}

void Calc::stop()
{
    qDebug() << "End";
    if (timer != NULL)
        timer->stop();
    value = 0;
    QScatterDataArray *dataArray = new QScatterDataArray;
    graphData->updateData(4, dataArray); // clear Field

}

double Calc::showValueFunc(double value)
{
    //return value + (endValue - startValue)/100;
    return value == 0 ? 0.0001 : 1.2*value;
}

void Calc::showField()
{
    graphData->updateData(4, getArray(value));
    value = showValueFunc(value);
    if (value > endValue)
        value = startValue;
}

void Calc::showFieldEnd(double value)
{
    endValue = value;
    graphData->updateData(4, getArray((float)value));
}

void Calc::showFieldStart(double value)
{
    startValue = value;
    graphData->updateData(4, getArray((float)value));
}

QScatterDataArray* Calc::getArray(float level)
{
    QScatterDataArray *dataArray = new QScatterDataArray;

    int check = 0;
    for (int nx = 0; nx < SIZE_X*PPM - 1; nx++)
    for (int ny = 0; ny < SIZE_Y*PPM - 1; ny++)
    for (int nz = 0; nz < SIZE_Z*PPM - 1; nz++)
    {
        check = 0;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] > level;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + (nz + 1)] > level;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + nz] > level;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + (nz + 1)] > level;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] > level;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + (nz + 1)] > level;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + nz] > level;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + (nz + 1)] > level;
        if (check != 0 && check != 8)
        {
            dataArray->push_back(QVector3D((nx + 0.5)/PPM_f, (ny + 0.5)/PPM_f, (nz + 0.5)/PPM_f));
        }
    }

    qDebug() << "Count for value " << level << "; SIZE " << dataArray->size();
    return dataArray;
}

#include "calc.h"

float init_p(float* data)
{
    //qDebug() << "";
    float R = 1.0/data[3];
    float vecLen = pow((data[0] - data[4])*(data[0] - data[4]) + (data[1] - data[5])*(data[1] - data[5]) + (data[2] - data[6])*(data[2] - data[6]), 0.5);
    //qDebug() << data[0] << data[1] << data[2];
    //qDebug() << data[4] << data[5] << data[6];
    //qDebug() << "phi = " << data[3] << "; R = " << R << "; vecLen = " << vecLen;
    if (vecLen > R)
    {
        //() << ">" << 1/(vecLen - R + 1);
        return 1/(vecLen - R + 1);
    }else{
        //qDebug() << ">" << (1 - cos(vecLen*M_PI/R))/2;
        return (1 - cos(vecLen*M_PI/(R*2)));
    }

}

Calc::Calc(Data* init_data):
    graphData(init_data)
{
    startValue = 0;
    endValue = 0;
    Label *newLabel = new Label(0,0,0,&init_p);
    labels.push_back(newLabel);
    newLabel = new Label(SIZE_X,0,SIZE_Z,&init_p);
    labels.push_back(newLabel);
    newLabel = new Label(0,SIZE_Y,SIZE_Z,&init_p);
    labels.push_back(newLabel);
    newLabel = new Label(SIZE_X,SIZE_Y,0,&init_p);
    labels.push_back(newLabel);

    x = X0;
    y = Y0;
    z = Z0;
    updateSource();

    graphData->updateData(0, getLabels());
}

Calc::~Calc()
{
    qDeleteAll(labels);
    labels.clear();
}


QScatterDataArray* Calc::getLabels()
{
    QScatterDataArray *dataArray = new QScatterDataArray;
    for (int i = 0; i < labels.size(); ++i)
        dataArray->push_back(QVector3D(labels.at(i)->x0, labels.at(i)->y0, labels.at(i)->z0));
    return dataArray;
}

void Calc::updateSource()
{
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->push_back(QVector3D(x, y, z));
    graphData->updateData(1, dataArray);
    updateField();
}

void Calc::updateField()
{
    for (int nx = 0; nx < SIZE_X*PPM; nx++)
    for (int ny = 0; ny < SIZE_Y*PPM; ny++)
    for (int nz = 0; nz < SIZE_Z*PPM; nz++)
    {
        field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] = 0;
    }
    for (int i = 0; i < labels.size(); ++i)
    {
        qDebug() << "Update Label's field:" << i;
        labels.at(i)->updateField(1/distance(x, y, z, labels.at(i)->x0, labels.at(i)->y0, labels.at(i)->z0));
        //2/pow((SIZE_X*SIZE_X + SIZE_Y*SIZE_Y + SIZE_Z*SIZE_Z),0.5));
        for (int nx = 0; nx < SIZE_X*PPM; nx++)
        for (int ny = 0; ny < SIZE_Y*PPM; ny++)
        for (int nz = 0; nz < SIZE_Z*PPM; nz++)
        {
            field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] += \
                    labels.at(i)->getValue(nx, ny, nz);
        }
    }
}

void Calc::start()
{
    qDebug() << "Start";
    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showField()));
    timer->start(100); // И запустим таймер
    value = startValue;
}

void Calc::stop()
{
    qDebug() << "End";
    timer->stop();
    delete timer;
    value = 0;
    QScatterDataArray *dataArray = new QScatterDataArray;
    graphData->updateData(4, dataArray); // clear Field

}

void Calc::showField()
{
    graphData->updateData(4, getArray(value));
    value += (endValue - startValue)/100;
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

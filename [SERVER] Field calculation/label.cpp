#include "label.h"

Label::Label(int init_number, float init_x0, float init_y0, float init_z0,\
             float init_alfa, float init_beta, float(*init_p)(float*, float(*)(float)), float(*init_dd)(float)):
    number(init_number),
    x0(init_x0),
    y0(init_y0),
    z0(init_z0),
    alfa(init_alfa),
    beta(init_beta),
    p(init_p),
    dd(init_dd)
{
    xdd = (float)cos(alfa);
    ydd = (float)(sin(alfa)*sin(beta));
    zdd = (float)(sin(alfa)*cos(beta));
    rssi = -1;
    calculated = false;
}

void Label::setRSSI(float value)
{
    if (rssi != value)
    {
        rssi = value;
        calculated = false;
    }
}

int Label::updateField()
{
    if (rssi == -1)
        return - 1;
    if (calculated)
        return 0;
    float data[10] = {x0, y0, z0, xdd, ydd, zdd, rssi};
    for (int nx = 0; nx < SIZE_X*PPM; nx++)
    for (int ny = 0; ny < SIZE_Y*PPM; ny++)
    for (int nz = 0; nz < SIZE_Z*PPM; nz++)
    {
        data[7] = nx/PPM_f;
        data[8] = ny/PPM_f;
        data[9] = nz/PPM_f;
        field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] = (*p)(data, dd);
    }
    calculated = true;
    return 0;
}


QScatterDataArray* Label::getArray(float fieldLevel)
{
    QScatterDataArray *dataArray = new QScatterDataArray;
    int check = 0;
    for (int nx = 0; nx < SIZE_X*PPM - 1; nx++)
    for (int ny = 0; ny < SIZE_Y*PPM - 1; ny++)
    for (int nz = 0; nz < SIZE_Z*PPM - 1; nz++)
    {
        check = 0;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] > fieldLevel;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + (nz + 1)] > fieldLevel;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + nz] > fieldLevel;
        check += field[nx*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + (nz + 1)] > fieldLevel;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + nz] > fieldLevel;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + ny*SIZE_Z*PPM + (nz + 1)] > fieldLevel;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + nz] > fieldLevel;
        check += field[(nx + 1)*SIZE_Y*PPM*SIZE_Z*PPM + (ny + 1)*SIZE_Z*PPM + (nz + 1)] > fieldLevel;
        if (check != 0 && check != 8)
            dataArray->push_back(QVector3D((nx + 0.5)/PPM_f, (ny + 0.5)/PPM_f, (nz + 0.5)/PPM_f));
    }

    return dataArray;
}

float Label::getValue(int x, int y, int z)
{
    return field[x*SIZE_Y*PPM*SIZE_Z*PPM + y*SIZE_Z*PPM + z];
}

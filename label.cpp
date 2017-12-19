#include "label.h"

Label::Label(float init_x0, float init_y0, float init_z0, float(*init_p)(float*)):
    x0(init_x0),
    y0(init_y0),
    z0(init_z0),
    p(init_p)
{}

Label::updateField(float phy)
{
    float data[7] = {x0, y0, z0, phy};
    for (int nx = 0; nx < 40*4; nx++)
    for (int ny = 0; ny < 40*4; ny++)
    for (int nz = 0; nz < 40*4; nz++)
    {
        data[4] = nx*0.25;
        data[5] = ny*0.25;
        data[6] = nz*0.25;
        field[nx*20*4*40*4 + ny*40*4 + nz] = (*p)(data);
    }
}


QScatterDataArray* Label::getArray(float level)
{
    QScatterDataArray *dataArray = new QScatterDataArray;
    int check = 0;
    for (int nx = 0; nx < 40*4; nx++)
    for (int ny = 0; ny < 40*4; ny++)
    for (int nz = 0; nz < 40*4; nz++)
    {
        check = 0;
        check += field[nx*20*4*40*4 + ny*40*4 + nz] > level;
        check += field[nx*20*4*40*4 + ny*40*4 + (nz + 1)] > level;
        check += field[nx*20*4*40*4 + (ny + 1)*40*4 + nz] > level;
        check += field[nx*20*4*40*4 + (ny + 1)*40*4 + (nz + 1)] > level;
        check += field[(nx + 1)*20*4*40*4 + ny*40*4 + nz] > level;
        check += field[(nx + 1)*20*4*40*4 + ny*40*4 + (nz + 1)] > level;
        check += field[(nx + 1)*20*4*40*4 + (ny + 1)*40*4 + nz] > level;
        check += field[(nx + 1)*20*4*40*4 + (ny + 1)*40*4 + (nz + 1)] > level;
        if (check != 0 && check != 8)
            dataArray->push_back(QVector3D(nx*0.25 + 0.125, ny*0.25 + 0.125, nz*0.25 + 0.125));
    }

    return dataArray;
}

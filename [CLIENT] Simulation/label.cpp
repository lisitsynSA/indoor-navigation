#include "label.h"

Label::Label(int init_number, float init_x0, float init_y0, float init_z0, \
             float init_alfa, float init_beta, float(*init_ddFunc)(float), int init_ddNumber):
    number(init_number),
    x0(init_x0),
    y0(init_y0),
    z0(init_z0),
    alfa(init_alfa),
    beta(init_beta),
    ddFunc(init_ddFunc),
    ddNumber(init_ddNumber)
{
    xdd = (float)cos(alfa);
    ydd = (float)(sin(alfa)*sin(beta));
    zdd = (float)(sin(alfa)*cos(beta));
    rssi = -1;
}

void Label::setRSSI(float value)
{
    rssi = value;
}

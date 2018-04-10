#include "calc.h"
#include "qmath.h"

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

Calc::Calc()
{
    Label *newLabel = new Label(1,0,10,20,0,0,&init_dd, 1);
    labels.push_back(newLabel);
    newLabel = new Label(2,SIZE_X,0,SIZE_Z,M_PI - atan(pow(2, 0.5)),3*M_PI/4,&init_dd, 1);
    labels.push_back(newLabel);
    newLabel = new Label(3,0,SIZE_Y,SIZE_Z,atan(pow(2, 0.5)),5*M_PI/4,&init_dd, 1);
    labels.push_back(newLabel);
    newLabel = new Label(4,SIZE_X,SIZE_Y,0,M_PI - atan(pow(2, 0.5)),7*M_PI/4,&init_dd, 1);
    labels.push_back(newLabel);

    x = X0;
    y = Y0;
    z = Z0;
    timer = NULL;
}

Calc::~Calc()
{
    qDeleteAll(labels);
    labels.clear();
}

float Calc::distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return pow((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2), 0.5);
}

void Calc::setRSSI(Label* label)
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
    label->setRSSI((1/R)*label->ddFunc(angle));
}

void Calc::start()
{
    qDebug() << "Start";
    if (timer == NULL)
        timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerHandler()));
    timer->start(1000); // И запустим таймер

    // MSG room
    Message room(SIZE_X, SIZE_Y, SIZE_Z, X0, Y0, Z0);
    emit sendMessage(room);

    // MSG cameras
    Label *temp;
    for (int i = 0; i < labels.size(); ++i)
    {
        temp = labels.at(i);
        Message camera(temp->number, temp->x0, temp->y0, temp->z0,\
                       temp->alfa, temp->beta, temp->ddNumber);
        emit sendMessage(camera);
    }

    step = 0;
}

void Calc::timerHandler()
{
    // Update beacon position
    x = X0 + 5*sin(step/5.0);
    y = Y0 + sin(step);
    z = Z0 + 5*cos(step/5.0);
    step += 1;

    // MSG beacon
    Message beacon(x, y, z, step % 1000);
    emit sendMessage(beacon);

    // MSG values
    for (int i = 0; i < labels.size(); ++i)
    {
        setRSSI(labels.at(i));
        Message value(labels.at(i)->number, labels.at(i)->rssi, step % 1000);
        emit sendMessage(value);
    }
}

void Calc::stop()
{
    qDebug() << "Stop";
    if (timer != NULL)
        timer->stop();
    step = 0;
}

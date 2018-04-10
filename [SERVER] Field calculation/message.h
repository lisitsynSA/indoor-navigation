#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QString>

enum MSGType{Room = 0, Camera = 1, Value = 2, Beacon = 3};

struct msgRoom{
    float sizeX, sizeY, sizeZ;
    float startX, startY, startZ;
};

struct msgCamera{
    int32_t cameraNumber;
    float x0, y0, z0, alfa, beta;
    // 0 <= alfa <= PI (from 0X); 0 <= beta <= 2PI (from 0Y in Y0Z)
    int32_t dd; // directional diagram number
};

struct msgValue{
    int32_t camera;
    float value;
    int32_t time;
};

struct msgBeacon{
    float X, Y, Z;
    int32_t time;
};

class Message
{
public:
    explicit Message(QByteArray initMsg);
    Message(float initSizeX, float initSizeY, float initSizeZ,\
            float initStartX, float initStartY, float initStartZ); // Room
    Message(int32_t initNumber, float initX0, float initY0, float initZ0,\
            float initAlfa, float initBeta, int32_t initDD); // Camera
    Message(int32_t initNumber, float initValue, int32_t initTime); // Value
    Message(float initX, float initY, float initZ, int32_t initTime); // Beacon

    MSGType getType();
    QString getInfo();

    msgCamera* getCamera();
    msgRoom* getRoom();
    msgValue* getValue();
    msgBeacon* getBeacon();
    QByteArray getByteArray();

private:
    QByteArray msg;
};

#endif // MESSAGE_H

#include "message.h"

Message::Message(QByteArray initMsg) : msg(initMsg)
{}

Message::Message(float initSizeX, float initSizeY, float initSizeZ,\
        float initStartX, float initStartY, float initStartZ) // Room
{
    msgRoom room = {initSizeX, initSizeY, initSizeZ,\
                    initStartX, initStartY, initStartZ};
    msg.append((char) Room);
    msg.append((const char *) &room, sizeof(msgRoom));
}

Message::Message(int32_t initNumber, float initX0, float initY0, float initZ0,\
        float initAlfa, float initBeta, int32_t initDD) // Camera
{
    msgCamera camera = {initNumber, initX0, initY0, initZ0,\
                        initAlfa, initBeta, initDD};
    msg.append((char) Camera);
    msg.append((const char *) &camera, sizeof(msgCamera));
}

Message::Message(int32_t initNumber, float initValue, int32_t initTime) // Value
{
    msgValue value = {initNumber, initValue, initTime};
    msg.append((char) Value);
    msg.append((const char *) &value, sizeof(msgValue));
}

Message::Message(float initX, float initY, float initZ, int32_t initTime) // Beacon
{
    msgBeacon beacon = {initX, initY, initZ, initTime};
    msg.append((char) Beacon);
    msg.append((const char *) &beacon, sizeof(msgBeacon));
}

MSGType Message::getType()
{
    return (MSGType)msg.at(0);
}

QString Message::getInfo()
{
    msgRoom* room = NULL;
    msgCamera* camera = NULL;
    msgValue* value = NULL;
    msgBeacon* beacon = NULL;
    QString info;

    switch (getType()) {
    case Room:
        info += "Room";
        room = getRoom();
        info += " sizeX=" + QString::number(room->sizeX);
        info += " sizeY=" + QString::number(room->sizeY);
        info += " sizeZ=" + QString::number(room->sizeZ);
        info += " startX=" + QString::number(room->startX);
        info += " startY=" + QString::number(room->startY);
        info += " startZ=" + QString::number(room->startZ);
        break;
    case Camera:
        info += "Camera";
        camera = getCamera();
        info += " cameraNumber=" + QString::number(camera->cameraNumber);
        info += " x0=" + QString::number(camera->x0);
        info += " y0=" + QString::number(camera->y0);
        info += " z0=" + QString::number(camera->z0);
        info += " alfa=" + QString::number(camera->alfa);
        info += " beta=" + QString::number(camera->beta);
        info += " dd=" + QString::number(camera->dd);

        break;
    case Value:
        info += "Value";
        value = getValue();
        info += " camera=" + QString::number(value->camera);
        info += " value=" + QString::number(value->value);
        info += " time=" + QString::number(value->time);
        break;
    case Beacon:
        info += "Beacon";
        beacon = getBeacon();
        info += " X=" + QString::number(beacon->X);
        info += " Y=" + QString::number(beacon->Y);
        info += " Z=" + QString::number(beacon->Z);
        info += " time=" + QString::number(beacon->time);
        break;
    default:
        info += "WRONG MESSAGE";
        break;
    }
    return info;
}

msgCamera* Message::getCamera()
{
    return (msgCamera*)(msg.data() + 1);
}

msgRoom* Message::getRoom()
{
    return (msgRoom*)(msg.data() + 1);
}

msgValue* Message::getValue()
{
    return (msgValue*)(msg.data() + 1);
}

msgBeacon* Message::getBeacon()
{
    return (msgBeacon*)(msg.data() + 1);
}

QByteArray Message::getByteArray()
{
    return msg;
}

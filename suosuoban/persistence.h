#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "qmypathitem.h"



class ScenePersistence{
private:

    void pathItemToJson(const QMyPathItem* item,QJsonObject &jsonObject);
    void pathItemsToJson(const QList<QMyPathItem*>& items, QJsonArray& array);
public:
    bool saveToFile(QString fileName,const QList<QMyPathItem*> &pathItems);
    bool loadFromFile(QString fileName, QList<QMyPathItem*>& pathItems );

};


#endif // PERSISTENCE_H

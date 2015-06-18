#include "persistence.h"
#include "config.h"
#include <QPen>
#include <QFile>
void ScenePersistence::pathItemToJson(const QMyPathItem *item, QJsonObject &jsonObject)
{
    QJsonArray pointArray;
    for (int i=0;i< item->points.size();i++){
        QJsonObject jsonPoint;
        jsonPoint["x"]=item->points[i].x();
        jsonPoint["y"]=item->points[i].y();
        pointArray.append(jsonPoint);
    }
    jsonObject["polyline"]=pointArray;
    jsonObject["color"]=colorToHexStr( item->pen().color() );
}


void ScenePersistence::pathItemsToJson(const QList<QMyPathItem *> &items, QJsonArray &array)
{
    for (int i=0; i<items.size();i++){
        QMyPathItem* item = items[i];
        QJsonObject itemObject;
        pathItemToJson(item,itemObject);
        array.append(itemObject);
    }
}


bool ScenePersistence::saveToFile(QString fileName, const QList<QMyPathItem *> &pathItems)
{
    QJsonArray itemsArray;
    pathItemsToJson(pathItems,itemsArray);

    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }


    QJsonDocument saveDoc(itemsArray);
    saveFile.write(saveDoc.toJson(QJsonDocument::Compact));


    return true;
}


bool ScenePersistence::loadFromFile(QString fileName, QList<QMyPathItem *> &pathItems)
{

}

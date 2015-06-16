#include <cassert>
#include "config.h"


Config* Config::config=NULL;

QString widthTwo(QString str){
    if (str.length()==0){
        return "00";
    }
    if (str.length()==1){
        return "0"+ str;
    }
    if (str.length()==2){
        return str;
    }

    return str.left(2);


}

QColor hexStrToColor(QString strColor){
    bool ok;
    uint uintColor=strColor.toUInt(&ok,16);
    uint red,green,blue,alpha;
    red = (uintColor >> 24) & 0xFF;
    green = (uintColor >> 16) & 0xFF;
    blue = (uintColor >> 8) & 0xFF;
    alpha = uintColor &  0xFF;
    return QColor(red,green,blue,alpha);
}



QString colorToHexStr(QColor color){
    QString red = widthTwo(QString::number( color.red(),16));
    QString green = widthTwo( QString::number( color.green(),16) );
    QString blue = widthTwo(QString::number( color.blue(),16));
    QString alpha = widthTwo( QString::number( color.alpha(),16) );
    return red + green + blue + alpha;
}

Config::Config()
{
    settings = new
            QSettings ("suosuoban.ini",QSettings::IniFormat);
}

Config::~Config()
{
    delete settings;
}

int Config::contourPadding()
{
    return settings->value("geometry/contourpadding",20).toInt();
}

QColor Config::clusterColor()
{
    QString strColor=settings->value("canvas/clustercolor","E0999964").toString();
    return hexStrToColor(strColor);
}

void Config::clusterColor(QColor val)
{
    settings->setValue("canvas/clustercolor",colorToHexStr(val));
}

QColor Config::penColor()
{
    QString strColor=settings->value("canvas/pencolor","000000FF").toString();
    return hexStrToColor(strColor);
}

void Config::penColor(QColor val)
{
    settings->setValue("canvas/pencolor",colorToHexStr(val));
}

QColor Config::backColor()
{
    QString strColor=settings->value("canvas/backgroundcolor","FFFFFFFF").toString();
    return hexStrToColor(strColor);
}

void Config::backColor(QColor val)
{
    settings->setValue("canvas/backgroundcolor",colorToHexStr(val));
}

int Config::numHistory()
{
    return settings->value("history/num",50).toInt();
}

void Config::numHistory(int val)
{
    settings->setValue("history/num",val);
}

int Config::penWidth(){
    return settings->value("canvas/penwidth",5).toInt();
}

double Config::minGap()
{
    return settings->value("geometry/mingap",6).toDouble();
}

double Config::minAngle()
{
    return settings->value("geometry/minangle",9).toDouble();
}

Config* Config::instance(){
    if (!config){
        config = new Config();
    }
    return config;
}

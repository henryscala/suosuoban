#include "config.h"

Config* Config::config=NULL;


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
    QString strColor=settings->value("canvas/clustercolor","E0FFFF64").toString();
    bool ok;
    uint uintColor=strColor.toUInt(&ok,16);
    uint red,green,blue,alpha;
    red = (uintColor >> 24) & 0xFF;
    green = (uintColor >> 16) & 0xFF;
    blue = (uintColor >> 8) & 0xFF;
    alpha = uintColor &  0xFF;
    QColor color(red,green,blue,alpha);
    return color;
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

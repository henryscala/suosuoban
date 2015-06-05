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

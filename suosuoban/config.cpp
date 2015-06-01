#include "config.h"

Config* Config::config=NULL;


Config::Config()
{
    settings = new
            QSettings ("suosuoban.ini",QSettings::IniFormat);
}

Config::~Config()
{

}

int Config::defaultPenWidth(){
    return settings->value("canvas/penwidth",3).toInt();
}

double Config::minGap()
{
    return settings->value("geometry/mingap",6).toDouble();
}

Config* Config::instance(){
    if (!config){
        config = new Config();
    }
    return config;
}

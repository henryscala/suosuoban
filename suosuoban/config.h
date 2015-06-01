#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>

class Config
{
private:
    QSettings *settings;
    static Config* config;
    Config();
    ~Config();
public:
    double minGap();
    int defaultPenWidth();
    static Config *instance();
};

#endif // CONFIG_H

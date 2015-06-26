#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QColor>
#include <QString>

QColor hexStrToColor(QString strColor);
QString colorToHexStr(QColor color);


class Config
{
private:
    QSettings *settings;
    static Config* config;
    Config();
    ~Config();
public:
    static void reload();
    static Config *instance();

    double minGap();
    void minGap(double val);

    double minAngle();//in degree not radian
    void minAngle(double val);

    int penWidth();
    void penWidth(int val);

    int contourPadding();
    void contourPadding(int val);

    QColor clusterColor();
    void clusterColor(QColor val);

    QColor penColor();
    void penColor(QColor val);

    QColor backColor();
    void backColor(QColor val);

    int numHistory();
    void numHistory(int val);
    static const char* CONFIG_FILE_NAME;
};



#endif // CONFIG_H

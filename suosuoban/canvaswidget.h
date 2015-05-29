#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include "geom.h"
namespace Ui {
class CanvasWidget;
}

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = 0);
    ~CanvasWidget();
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::CanvasWidget *ui;
    bool isMouseDown;
    PolyLine *currPath;
    PolyLineCluster *currCluster;

};

#endif // CANVASWIDGET_H

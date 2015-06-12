#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets>
#include "canvasscene.h"
#include "grid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);

public slots:
    void test();
    void canvasModeChange();
    void canvasColorChange();
protected:
    void createActions();
    void createMenus();
    QIcon createIcon(QColor color);

private:
    Ui::MainWindow *ui;

    CanvasScene* scene;
    QGraphicsView* view;

    QAction *colorPenAction;
    QAction *colorBackAction;
    QAction *colorClusterAction;

    QAction *drawModeAction;
    QAction *clusterModeAction;
    QAction *eraseModeAction;


};




#endif // MAINWINDOW_H

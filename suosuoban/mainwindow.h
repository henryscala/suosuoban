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
protected:
    void createActions();
    void createMenus();
protected slots:
    void test();
private:
    Ui::MainWindow *ui;

    CanvasScene* scene;
    QGraphicsView* view;

    QAction *testAction;
    QAction *drawModeAction;
    QAction *clusterModeAction;
    QAction *eraseModeAction;


};




#endif // MAINWINDOW_H

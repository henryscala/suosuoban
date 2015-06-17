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

    void canvasModeChange();
    void canvasColorChange();
    void showClusterChange();
    void delCluster();
    void undoRedo();
    void updateActionsState();
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

    QAction *showHideClusterAction;

    QAction *delClusterAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *saveAction;
    QAction *saveAsAction;
    QAction *openAction;
    QAction *exitAction;
};




#endif // MAINWINDOW_H

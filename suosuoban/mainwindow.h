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

    void selectAll();
    void delCluster();
    void undoRedo();
    void updateActionsState();
    void saveFile();
    void readFile();
    void changeSceneSize();
    void helpAbout();
protected:

    void createActions();
    void createMenus();
    void createToolBar();
    QIcon createIcon(QColor color);

    void contextMenuEvent(QContextMenuEvent *event);

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
    QAction *incHeightAction;
    QAction *decHeightAction;
    QAction *incWidthAction;
    QAction *decWidthAction;

    QAction *selectAllAction;
    QAction *delClusterAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *saveAction;
    QAction *openAction;
    QAction *exitAction;

    QAction *helpAboutAction;
};




#endif // MAINWINDOW_H

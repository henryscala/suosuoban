#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"
#include <iostream>
#include <cassert>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const int SIZEX = 5000;//temporarily
    const int SIZEY = 5000;
    ui->setupUi(this);

    scene = new CanvasScene(this);
    scene->setSceneRect(QRectF(0, 0, SIZEX, SIZEY));



    view = new QGraphicsView(scene);

    setCentralWidget(view);
    setWindowTitle(tr("Suosuoban, by Qinmishu.org"));
    setUnifiedTitleAndToolBarOnMac(true);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::resizeEvent(QResizeEvent *event){

}

void MainWindow::createActions()
{
    //testAction = new QAction(tr("&Test"),this);
    //connect(testAction,SIGNAL(triggered()),this,SLOT(test()));


    drawModeAction = new QAction(tr("&Draw"),this);
    drawModeAction->setCheckable(true);
    drawModeAction->setChecked(true);
    drawModeAction->setData((int)MODE_DRAW);
    connect(drawModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));

    eraseModeAction = new QAction(tr("&Erase"),this);
    eraseModeAction->setCheckable(true);
    eraseModeAction->setData((int)MODE_ERASE);
    connect(eraseModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));

    clusterModeAction = new QAction(tr("&Cluster"),this);
    clusterModeAction->setCheckable(true);
    clusterModeAction->setData((int)MODE_CLUSTER);
    connect(clusterModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));


}

void MainWindow::createMenus()
{
    //QMenu* testMenu = menuBar()->addMenu(tr("&Test"));
    //testMenu->addAction(testAction);

    QMenu* modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(drawModeAction);
    modeMenu->addAction(eraseModeAction);
    modeMenu->addAction(clusterModeAction);
}

void MainWindow::test()
{
    cout << "test clicked" << endl;
}

void MainWindow::canvasModeChange()
{
    QAction * action=(QAction*)sender();
    bool ok;

    int canvasMode=(CanvasMode) action->data().toInt(&ok);
    scene->canvasModeChange(canvasMode);

    drawModeAction->setChecked(false);
    clusterModeAction->setChecked(false);
    eraseModeAction->setChecked(false);
    if (drawModeAction == action) {
        drawModeAction->setChecked(true);
    } else if (clusterModeAction == action) {
        clusterModeAction->setChecked(true);
    } else if (eraseModeAction == action) {
        eraseModeAction->setChecked(true);
    } else {
        assert(false);
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"
#include <iostream>
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
    setWindowTitle(tr("Suosuoban"));
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
    testAction = new QAction(tr("&Test"),this);
    connect(testAction,SIGNAL(triggered()),this,SLOT(test()));


    drawModeAction = new QAction(tr("&Draw"),this);
    drawModeAction->setData((int)MODE_DRAW);
    connect(drawModeAction,SIGNAL(triggered()),scene,SLOT(canvasModeChange()));

    eraseModeAction = new QAction(tr("&Erase"),this);
    eraseModeAction->setData((int)MODE_ERASE);
    connect(eraseModeAction,SIGNAL(triggered()),scene,SLOT(canvasModeChange()));

    clusterModeAction = new QAction(tr("&Cluster"),this);
    clusterModeAction->setData((int)MODE_CLUSTER);
    connect(clusterModeAction,SIGNAL(triggered()),scene,SLOT(canvasModeChange()));


}

void MainWindow::createMenus()
{
    QMenu* testMenu = menuBar()->addMenu(tr("&Test"));
    testMenu->addAction(testAction);

    QMenu* modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(drawModeAction);
    modeMenu->addAction(eraseModeAction);
    modeMenu->addAction(clusterModeAction);
}

void MainWindow::test()
{
    cout << "test clicked" << endl;
}


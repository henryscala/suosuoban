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
}

void MainWindow::createMenus()
{
    QMenu* testMenu = menuBar()->addMenu(tr("&Test"));
    testMenu->addAction(testAction);
}

void MainWindow::test()
{
    cout << "test clicked" << endl;
}


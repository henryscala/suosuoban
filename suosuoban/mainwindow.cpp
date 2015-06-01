#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new CanvasScene(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));


    view = new QGraphicsView(scene);

    setCentralWidget(view);
    setWindowTitle(tr("Suosuoban"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){

}

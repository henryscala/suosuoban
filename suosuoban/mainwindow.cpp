#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvasWidget = new CanvasWidget();




    //scrollArea = new QScrollArea();
    //scrollArea->setWidget(canvasWidget);

    //this->setCentralWidget(scrollArea);

    setCentralWidget(ui->graphicsView);
    //QVBoxLayout* vBoxLayout = new QVBoxLayout();
    //vBoxLayout->addWidget(canvasWidget,0,Qt::AlignVCenter);
    //setLayout(vBoxLayout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    canvasWidget->setGeometry( this->geometry() );
}

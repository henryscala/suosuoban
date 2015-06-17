#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "persistence.h"
#include <QtWidgets>
#include <iostream>
#include <cassert>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    scene = new CanvasScene(this);
    scene->setSceneRect(QRectF(0, 0, this->geometry().width(), this->geometry().height()));



    view = new QGraphicsView(scene);

    setCentralWidget(view);
    setWindowTitle(tr("Suosuoban, by Qinmishu.org"));
    setUnifiedTitleAndToolBarOnMac(true);

    createActions();
    updateActionsState();
    createMenus();

    connect(scene,SIGNAL(sceneChanged()),this,SLOT(updateActionsState()));
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::resizeEvent(QResizeEvent *event){

}

void MainWindow::createActions()
{



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

    showHideClusterAction = new QAction(tr("&Show Cluster"),this);
    showHideClusterAction->setCheckable(true);
    showHideClusterAction->setChecked(true);//checked means Showing Cluster
    connect(showHideClusterAction,SIGNAL(triggered()),this,SLOT(showClusterChange()));

    colorPenAction = new QAction(tr("&Pen"),this);
    colorPenAction->setData((int)COLOR_TYPE_PEN);
    colorPenAction->setIcon(createIcon(Config::instance()->penColor()));
    connect(colorPenAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));

    colorBackAction = new QAction(tr("&Background"),this);
    colorBackAction->setData((int)COLOR_TYPE_BACK);
    colorBackAction->setIcon(createIcon(Config::instance()->backColor()));
    connect(colorBackAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));

    colorClusterAction = new QAction(tr("&Cluster"),this);
    colorClusterAction->setData((int)COLOR_TYPE_CLUSTER);
    colorClusterAction->setIcon(createIcon(Config::instance()->clusterColor()));
    connect(colorClusterAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));

    delClusterAction = new QAction(tr("&Delete"),this);
    connect(delClusterAction,SIGNAL(triggered()),this,SLOT(delCluster()));

    undoAction =  new QAction(tr("&Undo"),this);
    connect(undoAction,SIGNAL(triggered()),this, SLOT(undoRedo()));
    redoAction =  new QAction(tr("&Redo"),this);
    connect(redoAction,SIGNAL(triggered()),this, SLOT(undoRedo()));

    saveAction = new QAction(tr("&Save"),this);
    openAction = new QAction(tr("&Open"),this);
    saveAsAction = new QAction(tr("Save &As..."),this);
    exitAction = new QAction(tr("e&Xit"),this);
}

void MainWindow::updateActionsState()
{
    undoAction->setEnabled(history::isUndoable());
    redoAction->setEnabled(history::isRedoable());

    eraseModeAction->setEnabled(scene->pathClusters.size()>0);

    bool isModeCluster = clusterModeAction->isChecked();
    delClusterAction->setEnabled(isModeCluster && scene->selectedClusterIndices.size() > 0);

    bool showCluster = showHideClusterAction->isChecked();
    clusterModeAction->setEnabled(showCluster);
    if (!showCluster){
        if (clusterModeAction->isChecked()){
            drawModeAction->activate(QAction::Trigger);//trigger action by program
        }
    }
}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(delClusterAction);

    QMenu* modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(drawModeAction);
    modeMenu->addAction(eraseModeAction);
    modeMenu->addAction(clusterModeAction);
    modeMenu->addSeparator();
    modeMenu->addAction( showHideClusterAction );


    QMenu* colorMenu = menuBar()->addMenu(tr("&Color"));
    colorMenu->addAction(colorPenAction);
    colorMenu->addAction(colorBackAction);
    colorMenu->addAction(colorClusterAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

}

QIcon MainWindow::createIcon(QColor color)
{
    QPixmap bmp(32,32);
    bmp.fill(color);
    return QIcon(bmp);
}



void MainWindow::canvasModeChange()
{
    QAction * action=(QAction*)sender();
    bool ok;

    CanvasMode canvasMode=(CanvasMode) action->data().toInt(&ok);
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

    updateActionsState();
}

void MainWindow::canvasColorChange()
{

    QAction * action=(QAction*)sender();
    bool ok;

    CanvasColorType colorType=(CanvasColorType) action->data().toInt(&ok);
    QColor outputColor;
    switch (colorType){
    case COLOR_TYPE_PEN:
        outputColor = QColorDialog::getColor(Config::instance()->penColor(),this,tr("Pen Color"),QColorDialog::ShowAlphaChannel);
        break;
    case COLOR_TYPE_BACK:
        outputColor = QColorDialog::getColor(Config::instance()->backColor(),this,tr("Background Color"),QColorDialog::ShowAlphaChannel);
        break;
    case COLOR_TYPE_CLUSTER:
        outputColor = QColorDialog::getColor(Config::instance()->clusterColor(),this,tr("Cluster Color"),QColorDialog::ShowAlphaChannel);
        break;
    default:
        assert(false);
    }

    action->setIcon(createIcon(outputColor));
    scene->canvasColorChange(colorType,outputColor);
}

void MainWindow::showClusterChange()
{
    //isChecked is the afterward state after click
    bool isChecked = showHideClusterAction->isChecked();

    scene->setShowCluster(isChecked);

    updateActionsState();
}

void MainWindow::delCluster()
{
    scene->delCluster();

    updateActionsState();
}

void MainWindow::undoRedo()
{
    QAction * action=(QAction*)sender();
    if (action == undoAction){
        cout << " undo clicked" << endl;

        if (!history::isUndoable()){
            return;
        }

        const history::PolyLineOp &op=history::undo();
        scene->undo(op);
        updateActionsState();
        return;
    }
    if (action == redoAction){
        cout << " redo clicked" << endl;

        if (!history::isRedoable()){
            return;
        }

        const history::PolyLineOp &op=history::redo();
        scene->redo(op);
        updateActionsState();
        return;
    }
    assert(false);


}




#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "history.h"
#include <QtWidgets>
#include "configdialog.h"

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
    toolBar = this->addToolBar("Tools");

    setCentralWidget(view);
    setWindowTitle(tr("Suosuoban, by Qinmishu.org"));
    setUnifiedTitleAndToolBarOnMac(true);

    createActionsUi();


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

    QString icons = QApplication::applicationDirPath() + "/icons";


    drawModeAction = new QAction(tr("&Draw"),this);
    drawModeAction->setCheckable(true);
    drawModeAction->setChecked(true);
    drawModeAction->setData((int)MODE_DRAW);
    drawModeAction->setShortcut(tr("D"));
    drawModeAction->setIcon(QIcon(icons+"/pen.png"));
    connect(drawModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));
    allActions << drawModeAction;

    eraseModeAction = new QAction(tr("&Erase"),this);
    eraseModeAction->setCheckable(true);
    eraseModeAction->setData((int)MODE_ERASE);
    eraseModeAction->setShortcut(tr("E"));
    eraseModeAction->setIcon(QIcon(icons+"/erase.png"));
    connect(eraseModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));
    allActions << eraseModeAction ;

    clusterModeAction = new QAction(tr("&Cluster"),this);
    clusterModeAction->setCheckable(true);
    clusterModeAction->setData((int)MODE_CLUSTER);
    clusterModeAction->setShortcut(tr("C"));
    clusterModeAction->setIcon(QIcon(icons+"/bubble.png"));
    connect(clusterModeAction,SIGNAL(triggered()),this,SLOT(canvasModeChange()));
    allActions << clusterModeAction;

    showHideClusterAction = new QAction(tr("&Show Cluster"),this);
    showHideClusterAction->setCheckable(true);
    showHideClusterAction->setChecked(true);//checked means Showing Cluster
    showHideClusterAction->setShortcut(tr("H"));
    showHideClusterAction->setIcon(QIcon(icons+"/eye.png"));
    connect(showHideClusterAction,SIGNAL(triggered()),this,SLOT(showClusterChange()));
    allActions << showHideClusterAction;

    incHeightAction =  new QAction(tr("&Increase Height"),this);
    incHeightAction->setShortcut(tr("]"));
    connect(incHeightAction,SIGNAL(triggered()),this,SLOT(changeSceneSize()));
    allActions << incHeightAction;

    decHeightAction =  new QAction(tr("&Decrease Height"),this);
    decHeightAction->setShortcut(tr("["));
    connect(decHeightAction,SIGNAL(triggered()),this,SLOT(changeSceneSize()));
    allActions << decHeightAction;

    incWidthAction =  new QAction(tr("I&ncrease Width"),this);
    connect(incWidthAction,SIGNAL(triggered()),this,SLOT(changeSceneSize()));
    allActions << incWidthAction;

    decWidthAction =  new QAction(tr("D&ecrease Width"),this);
    connect(decWidthAction,SIGNAL(triggered()),this,SLOT(changeSceneSize()));
    allActions << decWidthAction;

    settingsAction = new QAction(tr("Settings..."),this);
    connect(settingsAction,SIGNAL(triggered()),this,SLOT(settings()));
    allActions << settingsAction;

    colorPenAction = new QAction(tr("&Pen"),this);
    colorPenAction->setData((int)COLOR_TYPE_PEN);
    colorPenAction->setIcon(createIcon(Config::instance()->penColor()));
    colorPenAction->setShortcut(tr("P"));
    connect(colorPenAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));
    allActions <<colorPenAction;

    colorBackAction = new QAction(tr("&Background"),this);
    colorBackAction->setData((int)COLOR_TYPE_BACK);
    colorBackAction->setIcon(createIcon(Config::instance()->backColor()));
    connect(colorBackAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));
    allActions << colorBackAction;

    colorClusterAction = new QAction(tr("&Cluster"),this);
    colorClusterAction->setData((int)COLOR_TYPE_CLUSTER);
    colorClusterAction->setIcon(createIcon(Config::instance()->clusterColor()));
    connect(colorClusterAction,SIGNAL(triggered()),this,SLOT(canvasColorChange()));
    allActions <<colorClusterAction;

    selectAllAction = new QAction(tr("Select &All"),this);
    selectAllAction->setShortcut(tr("A"));
    selectAllAction->setIcon(QIcon(icons+"/hand.png"));
    connect(selectAllAction,SIGNAL(triggered()),this, SLOT(selectAll()));
    allActions <<selectAllAction;

    delClusterAction = new QAction(tr("&Delete"),this);
    delClusterAction->setShortcut(tr("X"));
    delClusterAction->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    connect(delClusterAction,SIGNAL(triggered()),this,SLOT(delCluster()));
    allActions <<delClusterAction;

    undoAction =  new QAction(tr("&Undo"),this);
    undoAction->setIcon( style()->standardIcon(QStyle::SP_ArrowBack) );
    undoAction->setShortcut(tr("U"));
    connect(undoAction,SIGNAL(triggered()),this, SLOT(undoRedo()));
    allActions <<undoAction;

    redoAction =  new QAction(tr("&Redo"),this);
    redoAction->setIcon( style()->standardIcon(QStyle::SP_ArrowForward) );
    redoAction->setShortcut(tr("R"));
    connect(redoAction,SIGNAL(triggered()),this, SLOT(undoRedo()));
    allActions <<redoAction;

    saveAction = new QAction(tr("&Save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setIcon( style()->standardIcon(QStyle::SP_DialogSaveButton) );
    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveFile()));
    allActions <<saveAction;

    openAction = new QAction(tr("&Open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setIcon( style()->standardIcon(QStyle::SP_DialogOpenButton) );
    connect(openAction,SIGNAL(triggered()),this,SLOT(readFile()));
    allActions <<openAction;

    exitAction = new QAction(tr("e&Xit"),this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
    allActions <<exitAction;

    helpAboutAction = new QAction(tr("&About..."),this);
    connect(helpAboutAction,SIGNAL(triggered()),this,SLOT(helpAbout()));
    allActions <<helpAboutAction;
}

void MainWindow::updateActionsState()
{
    undoAction->setEnabled(history::isUndoable());
    redoAction->setEnabled(history::isRedoable());

    eraseModeAction->setEnabled(scene->pathClusters.size()>0);

    bool isModeCluster = clusterModeAction->isChecked();
    delClusterAction->setEnabled(isModeCluster && scene->selectedClusterIndices.size() > 0);
    selectAllAction->setEnabled(isModeCluster && scene->pathClusters.size()>0);

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
    //fileMenu->addAction(saveAsAction);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);


    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAction);
    editMenu->addAction(delClusterAction);

    QMenu* modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(drawModeAction);
    modeMenu->addAction(eraseModeAction);
    modeMenu->addAction(clusterModeAction);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction( showHideClusterAction );
    viewMenu->addSeparator();
    viewMenu->addAction(incHeightAction);
    viewMenu->addAction(decHeightAction);
    viewMenu->addAction(incWidthAction);
    viewMenu->addAction(decWidthAction);
    viewMenu->addSeparator();
    viewMenu->addAction(settingsAction);



    QMenu* colorMenu = menuBar()->addMenu(tr("&Color"));
    colorMenu->addAction(colorPenAction);
    colorMenu->addAction(colorBackAction);
    colorMenu->addAction(colorClusterAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAboutAction);

}

void MainWindow::createToolBar()
{

    toolBar->addAction(colorPenAction);
    toolBar->addAction(drawModeAction);
    toolBar->addAction(eraseModeAction);
    toolBar->addAction(clusterModeAction);
    toolBar->addAction(showHideClusterAction);
    toolBar->addAction(selectAllAction);
    toolBar->addAction(delClusterAction);
    toolBar->addAction(undoAction);
    toolBar->addAction(redoAction);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contextMenu(this);

    contextMenu.addAction(colorPenAction);
    contextMenu.addAction(drawModeAction);

    contextMenu.addAction(eraseModeAction);
    contextMenu.addAction(clusterModeAction);
    contextMenu.addAction(showHideClusterAction);
    contextMenu.addAction(selectAllAction);
    contextMenu.addAction(delClusterAction);
    contextMenu.addAction(undoAction);
    contextMenu.addAction(redoAction);

    contextMenu.exec(event->globalPos());
}

void MainWindow::changeEvent(QEvent *event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            qDebug() << "language changed";
            ui->retranslateUi(this);
            break;

            // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            qDebug() << "locale changed";
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    qDebug() << "file name " << filename;
    if(translator.load(filename)){
        qDebug() << "file name load success " << filename;
        qApp->installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString &rLanguage)
{
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());

        QString langDir = m_langPath + "/";
        switchTranslator(m_translator, langDir + QString("lang_%1.qm").arg(rLanguage));

        qDebug() << "Current Language changed to " << languageName << " " << m_currLang;

        //regenerate ui
        for (int i=0;i<this->allActions.size();i++){
            delete allActions[i];
        }
        allActions.clear();
        menuBar()->clear();
        toolBar->clear();
        createActionsUi();

    }
}

//refer to https://wiki.qt.io/How_to_create_a_multi_language_application
void MainWindow::createLanguageMenu()
{


    QMenu* langMenu = menuBar()->addMenu(tr("&Language"));
    QActionGroup* langGroup = new QActionGroup(langMenu);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    defaultLocale = QLocale::system().name(); // e.g. "de_DE"

    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"
    qDebug() << "system language " << defaultLocale;

    m_langPath = QApplication::applicationDirPath() + "/languages" ;

    qDebug() << "lang path = " << m_langPath;
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("lang*.qm"));


    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "TranslationExample_de.qm"
        locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
        locale.remove(0, locale.indexOf('_') + 1); // "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        qDebug() << "language option " << lang << " " << locale;
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);
        action->setData(locale);


        langMenu->addAction(action);
        langGroup->addAction(action);
        allActions << action;


        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }

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

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    scene->saveFile(fileName);
    updateActionsState();
}

void MainWindow::readFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    scene->readFile(fileName);
    updateActionsState();
}

void MainWindow::changeSceneSize()
{
    QAction * action=(QAction*)sender();
    bool isHeight = false;
    int sign = 1;
    if (action == incHeightAction){
        isHeight = true;
        sign = 1;
    } else if (action == decHeightAction){
        isHeight = true;
        sign = -1;
    } else if (action == incWidthAction){
        isHeight = false;
        sign = 1;
    } else if (action == decWidthAction){
        isHeight = false;
        sign = - 1;
    } else {
        Q_ASSERT(false);
    }

    qreal curX = scene->sceneRect().x();
    qreal curY = scene->sceneRect().y();
    qreal curWidth = scene->sceneRect().width();
    qreal curHeight = scene->sceneRect().height();
    qreal newWidth=curWidth;
    qreal newHeight=curHeight;
    if (isHeight){
        newHeight += sign * this->geometry().height();
        if (newHeight <= 0){
            newHeight=curHeight;
        }
    } else {
        newWidth  += sign * this->geometry().width();
        if (newWidth <=0){
            newWidth=curWidth;
        }
    }
    scene->setSceneRect(curX,curY,newWidth,newHeight);

}

void MainWindow::helpAbout()
{
    QString text =tr(
            "Suosuoban is an illustrator software bring to you by Qinmishu.org."
            "It is a free and opensourced software without any warrant."
            "Visit our web site <a href=\"http://Qinmishu.org\">http://Qinmishu.org</a> "
            "and the project site <a href=\"https://github.com/henryscala/suosuoban\">https://github.com/henryscala/suosuoban</a> "
            "to find more."
                );
    QMessageBox::about(this,tr("Suosuoban, by Qinmishu.org"),text);
}

void MainWindow::settings()
{
    qDebug() << "settings called";
    ConfigDialog* dialog = new ConfigDialog(this);


    QFile loadFile(Config::CONFIG_FILE_NAME);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open  file.");
        return ;
    }

    QByteArray data = loadFile.readAll();
    loadFile.close();
    QString strData(data);
    dialog->setText(strData);

    dialog->exec();

    if (dialog->result() == QDialog::Accepted){
        qDebug() << "save config " ;
        QFile saveFile(Config::CONFIG_FILE_NAME);

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return ;
        }

        strData=dialog->getText();


        saveFile.write(strData.toUtf8());
        saveFile.close();
        Config::reload();

        scene->configChange();

    } else {
        qDebug() << "not save config " ;
    }


    delete dialog;
}

void MainWindow::slotLanguageChanged(QAction *action)
{
    if(NULL != action) {
        // load the language dependant on the action content
        loadLanguage(action->data().toString());

    }
}

void MainWindow::createActionsUi()
{
    createActions();
    updateActionsState();
    createMenus();


    createToolBar();
    createLanguageMenu();
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

void MainWindow::selectAll()
{
    scene->selectAll();
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


        if (!history::isUndoable()){
            return;
        }

        const history::PolyLineOp &op=history::undo();
        scene->undoRedo(op,true);
        updateActionsState();
        return;
    }
    if (action == redoAction){


        if (!history::isRedoable()){
            return;
        }

        const history::PolyLineOp &op=history::redo();
        scene->undoRedo(op,false);
        updateActionsState();
        return;
    }
    assert(false);


}




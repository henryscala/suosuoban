#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets>
#include "canvasscene.h"
#include "grid.h"
#include "config.h"
#include "mylocale.h"

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
    QString defaultLocale;
    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);
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
    void settings();
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);
protected:
    void createActionsUi();
    void createActions();
    void createMenus();
    void createToolBar();
    QIcon createIcon(QColor color);

    void contextMenuEvent(QContextMenuEvent *event);
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);



    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);

private:
    Ui::MainWindow *ui;

    CanvasScene* scene;
    QGraphicsView* view;
    QToolBar *toolBar;

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
    QAction *settingsAction;

    QAction *selectAllAction;
    QAction *delClusterAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *saveAction;
    QAction *openAction;
    QAction *exitAction;

    QAction *helpAboutAction;
    QList<QAction*> allActions;


    QTranslator m_translator; // contains the translations for this application

    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.

};




#endif // MAINWINDOW_H

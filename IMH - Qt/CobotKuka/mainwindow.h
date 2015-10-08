#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cobotkuka.h"

namespace Ui {
class MainWindow;
}

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString laChaineJSON="";
    //CobotKuka cobot;

public slots:
    void colorPickTriggered();
    void on_actionActionEnvoyerTrame_triggered();

private:
    MainWindowPrivate *d;
};

#endif // MAINWINDOW_H

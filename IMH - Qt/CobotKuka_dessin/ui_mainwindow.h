/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionColor_Pick;
    QAction *actionActionEnvoyerTrame;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuColor;
    QMenu *menuEnvoyer_la_trame;
    QStatusBar *statusbar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        actionColor_Pick = new QAction(MainWindow);
        actionColor_Pick->setObjectName(QStringLiteral("actionColor_Pick"));
        actionActionEnvoyerTrame = new QAction(MainWindow);
        actionActionEnvoyerTrame->setObjectName(QStringLiteral("actionActionEnvoyerTrame"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuColor = new QMenu(menubar);
        menuColor->setObjectName(QStringLiteral("menuColor"));
        menuEnvoyer_la_trame = new QMenu(menubar);
        menuEnvoyer_la_trame->setObjectName(QStringLiteral("menuEnvoyer_la_trame"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menubar->addAction(menuColor->menuAction());
        menubar->addAction(menuEnvoyer_la_trame->menuAction());
        menuColor->addAction(actionColor_Pick);
        menuEnvoyer_la_trame->addAction(actionActionEnvoyerTrame);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionColor_Pick->setText(QApplication::translate("MainWindow", "Color Pick", 0));
        actionActionEnvoyerTrame->setText(QApplication::translate("MainWindow", "actionEnvoyerTrame", 0));
        actionActionEnvoyerTrame->setShortcut(QApplication::translate("MainWindow", "Return", 0));
        menuColor->setTitle(QApplication::translate("MainWindow", "color", 0));
        menuEnvoyer_la_trame->setTitle(QApplication::translate("MainWindow", "Envoyer la trame", 0));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

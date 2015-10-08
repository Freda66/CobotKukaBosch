#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawwidget.h"
#include <QColorDialog>
#include <QLayout>
#include <QDebug>

class MainWindowPrivate {
public:
    MainWindowPrivate(MainWindow *owner);
    ~MainWindowPrivate();

    DrawWidget* getDrawWidget();
    void pickColor();

private slots:
    void on_pushButton_clicked();

    void on_actionActionEnvoyerTrame_triggered();

private:
    DrawWidget *m_drawWidget;
    MainWindow *m_Owner;
    Ui::MainWindow *m_ui;
};

MainWindowPrivate::MainWindowPrivate(MainWindow *owner) :
    m_Owner(owner),
    m_drawWidget(new DrawWidget(owner)),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(m_Owner);
    m_ui->centralwidget->setLayout(new QVBoxLayout(m_Owner));
    m_ui->centralwidget->layout()->addWidget(m_drawWidget);

}

DrawWidget* MainWindowPrivate::getDrawWidget(){
    return m_drawWidget;
}

void MainWindowPrivate::on_pushButton_clicked()
{
    qDebug("ZZ");
}
void MainWindowPrivate::pickColor()
{
    QColor color = QColorDialog::getColor(
                m_drawWidget->drawColor(),
                m_Owner,
                QString("Select a draw color"),
                QColorDialog::ShowAlphaChannel
                );
    m_drawWidget->setDrawColor(color);
}

MainWindowPrivate::~MainWindowPrivate(){
    delete m_ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    d(new MainWindowPrivate(this))
{
}

MainWindow::~MainWindow()
{
    delete d;
}

void MainWindow::colorPickTriggered()
{
    d->pickColor();
}

void MainWindow::on_actionActionEnvoyerTrame_triggered()
{
    d->getDrawWidget()->jsonToSend += "]}";
//    d->getDrawWidget()->jsonToSend = d->getDrawWidget()->jsonToSend.replace("],]", "]]");
    d->getDrawWidget()->jsonToSend = d->getDrawWidget()->jsonToSend.replace(",]", "]");
    d->getDrawWidget()->jsonToSend = d->getDrawWidget()->jsonToSend.replace("\n", "");
    d->getDrawWidget()->jsonToSend = d->getDrawWidget()->jsonToSend.replace("\r", "");
    d->getDrawWidget()->jsonToSend = d->getDrawWidget()->jsonToSend.replace("\t", "");

    //if(cobot.connectToServer()){
        //qDebug() <<"jsonToSend :    " << d->getDrawWidget()->jsonToSend;
//        qDebug() << this->laChaineJSON;
        //Once the connection is established, send the JSON chain to the server.
       //cobot.writeJSONToServer(d->getDrawWidget()->jsonToSend);
        //cobot.setJson(d->getDrawWidget()->jsonToSend);
        laChaineJSON = d->getDrawWidget()->jsonToSend;
    //}

//    cobot->activate_OK_pushButton();
      //cobot->getJsonFromSketch();

    this->close();
}

/********************************************************************************
** Form generated from reading UI file 'cobotkuka.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COBOTKUKA_H
#define UI_COBOTKUKA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CobotKuka
{
public:
    QAction *actionQuit;
    QAction *actionAbout;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_7;
    QGridLayout *global_gridLayout;
    QPushButton *ok_pushButton;
    QPushButton *stop_pushButton;
    QPushButton *send_pushButton;
    QGroupBox *mod_groupBox;
    QGridLayout *gridLayout;
    QGridLayout *mod_gridLayout;
    QGroupBox *svg_groupBox;
    QVBoxLayout *svg_verticalLayout;
    QRadioButton *svg_radioButton;
    QGroupBox *svg_actions_groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *svg_file_label;
    QLineEdit *svg_file_lineEdit;
    QPushButton *svg_file_pushButton;
    QGroupBox *text_groupBox;
    QVBoxLayout *text_verticalLayout;
    QRadioButton *text_radioButton;
    QGroupBox *text_actions_groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *text_font_horizontalLayout;
    QLabel *text_font_label;
    QFontComboBox *text_font_fontComboBox;
    QHBoxLayout *text_text_horizontalLayout;
    QLabel *text_text_label;
    QLineEdit *text_text_lineEdit;
    QGroupBox *picture_groupBox;
    QVBoxLayout *picture_verticalLayout;
    QRadioButton *picture_radioButton;
    QGroupBox *picture_actions_groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *picture_file_horizontalLayout;
    QRadioButton *picture_file_radioButton;
    QLineEdit *picture_file_lineEdit;
    QPushButton *picture_file_pushButton;
    QHBoxLayout *picture_webcam_horizontalLayout;
    QRadioButton *picture_webcam_radioButton;
    QPushButton *picture_webcam_pushButton;
    QGroupBox *sketch_groupBox;
    QVBoxLayout *sketch_verticalLayout;
    QRadioButton *sketch_radioButton;
    QGroupBox *sketch_actions_groupBox;
    QVBoxLayout *verticalLayout_3;
    QPushButton *sketch_pushButton;
    QHBoxLayout *connect_horizontalLayout;
    QGroupBox *connect_groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *connect_ip_label;
    QLineEdit *connect_ip_lineEdit;
    QLabel *connect_port_label;
    QLineEdit *connect_port_lineEdit;
    QSpacerItem *connect_horizontalSpacer;
    QLabel *connect_status_label;
    QPushButton *connect_pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusBar;
    QButtonGroup *buttonGroup_Picture;
    QButtonGroup *buttonGroup_Mod;

    void setupUi(QMainWindow *CobotKuka)
    {
        if (CobotKuka->objectName().isEmpty())
            CobotKuka->setObjectName(QStringLiteral("CobotKuka"));
        CobotKuka->resize(703, 428);
        CobotKuka->setAutoFillBackground(true);
        CobotKuka->setStyleSheet(QStringLiteral(""));
        actionQuit = new QAction(CobotKuka);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionAbout = new QAction(CobotKuka);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(CobotKuka);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_7 = new QHBoxLayout(centralWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        global_gridLayout = new QGridLayout();
        global_gridLayout->setSpacing(6);
        global_gridLayout->setObjectName(QStringLiteral("global_gridLayout"));
        global_gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        ok_pushButton = new QPushButton(centralWidget);
        ok_pushButton->setObjectName(QStringLiteral("ok_pushButton"));
        ok_pushButton->setEnabled(true);

        global_gridLayout->addWidget(ok_pushButton, 2, 0, 1, 3);

        stop_pushButton = new QPushButton(centralWidget);
        stop_pushButton->setObjectName(QStringLiteral("stop_pushButton"));
        stop_pushButton->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stop_pushButton->sizePolicy().hasHeightForWidth());
        stop_pushButton->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        stop_pushButton->setFont(font);
        stop_pushButton->setStyleSheet(QStringLiteral(""));

        global_gridLayout->addWidget(stop_pushButton, 2, 3, 2, 1);

        send_pushButton = new QPushButton(centralWidget);
        send_pushButton->setObjectName(QStringLiteral("send_pushButton"));
        send_pushButton->setEnabled(true);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        send_pushButton->setFont(font1);

        global_gridLayout->addWidget(send_pushButton, 3, 0, 1, 3);

        mod_groupBox = new QGroupBox(centralWidget);
        mod_groupBox->setObjectName(QStringLiteral("mod_groupBox"));
        mod_groupBox->setAutoFillBackground(false);
        mod_groupBox->setStyleSheet(QStringLiteral(""));
        mod_groupBox->setFlat(false);
        gridLayout = new QGridLayout(mod_groupBox);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        mod_gridLayout = new QGridLayout();
        mod_gridLayout->setSpacing(6);
        mod_gridLayout->setObjectName(QStringLiteral("mod_gridLayout"));
        mod_gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        svg_groupBox = new QGroupBox(mod_groupBox);
        svg_groupBox->setObjectName(QStringLiteral("svg_groupBox"));
        svg_groupBox->setAutoFillBackground(false);
        svg_groupBox->setStyleSheet(QStringLiteral("background: rgb(246, 255, 240)"));
        svg_groupBox->setFlat(true);
        svg_verticalLayout = new QVBoxLayout(svg_groupBox);
        svg_verticalLayout->setSpacing(5);
        svg_verticalLayout->setContentsMargins(11, 11, 11, 11);
        svg_verticalLayout->setObjectName(QStringLiteral("svg_verticalLayout"));
        svg_verticalLayout->setContentsMargins(2, 2, 2, 2);
        svg_radioButton = new QRadioButton(svg_groupBox);
        buttonGroup_Mod = new QButtonGroup(CobotKuka);
        buttonGroup_Mod->setObjectName(QStringLiteral("buttonGroup_Mod"));
        buttonGroup_Mod->addButton(svg_radioButton);
        svg_radioButton->setObjectName(QStringLiteral("svg_radioButton"));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        svg_radioButton->setFont(font2);
        svg_radioButton->setChecked(true);

        svg_verticalLayout->addWidget(svg_radioButton, 0, Qt::AlignHCenter);

        svg_actions_groupBox = new QGroupBox(svg_groupBox);
        svg_actions_groupBox->setObjectName(QStringLiteral("svg_actions_groupBox"));
        svg_actions_groupBox->setFlat(true);
        horizontalLayout_2 = new QHBoxLayout(svg_actions_groupBox);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        svg_file_label = new QLabel(svg_actions_groupBox);
        svg_file_label->setObjectName(QStringLiteral("svg_file_label"));

        horizontalLayout_2->addWidget(svg_file_label);

        svg_file_lineEdit = new QLineEdit(svg_actions_groupBox);
        svg_file_lineEdit->setObjectName(QStringLiteral("svg_file_lineEdit"));
        svg_file_lineEdit->setAutoFillBackground(false);
        svg_file_lineEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(svg_file_lineEdit);

        svg_file_pushButton = new QPushButton(svg_actions_groupBox);
        svg_file_pushButton->setObjectName(QStringLiteral("svg_file_pushButton"));
        svg_file_pushButton->setAutoFillBackground(false);

        horizontalLayout_2->addWidget(svg_file_pushButton);


        svg_verticalLayout->addWidget(svg_actions_groupBox);


        mod_gridLayout->addWidget(svg_groupBox, 0, 0, 1, 1);

        text_groupBox = new QGroupBox(mod_groupBox);
        text_groupBox->setObjectName(QStringLiteral("text_groupBox"));
        text_groupBox->setAutoFillBackground(false);
        text_groupBox->setStyleSheet(QStringLiteral("background: rgb(255, 240, 255)"));
        text_groupBox->setFlat(true);
        text_verticalLayout = new QVBoxLayout(text_groupBox);
        text_verticalLayout->setSpacing(6);
        text_verticalLayout->setContentsMargins(11, 11, 11, 11);
        text_verticalLayout->setObjectName(QStringLiteral("text_verticalLayout"));
        text_verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        text_verticalLayout->setContentsMargins(1, -1, 1, -1);
        text_radioButton = new QRadioButton(text_groupBox);
        buttonGroup_Mod->addButton(text_radioButton);
        text_radioButton->setObjectName(QStringLiteral("text_radioButton"));
        text_radioButton->setFont(font2);

        text_verticalLayout->addWidget(text_radioButton, 0, Qt::AlignHCenter);

        text_actions_groupBox = new QGroupBox(text_groupBox);
        text_actions_groupBox->setObjectName(QStringLiteral("text_actions_groupBox"));
        text_actions_groupBox->setFlat(true);
        verticalLayout = new QVBoxLayout(text_actions_groupBox);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        text_font_horizontalLayout = new QHBoxLayout();
        text_font_horizontalLayout->setSpacing(6);
        text_font_horizontalLayout->setObjectName(QStringLiteral("text_font_horizontalLayout"));
        text_font_label = new QLabel(text_actions_groupBox);
        text_font_label->setObjectName(QStringLiteral("text_font_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(text_font_label->sizePolicy().hasHeightForWidth());
        text_font_label->setSizePolicy(sizePolicy2);
        text_font_label->setMinimumSize(QSize(144, 0));

        text_font_horizontalLayout->addWidget(text_font_label);

        text_font_fontComboBox = new QFontComboBox(text_actions_groupBox);
        text_font_fontComboBox->setObjectName(QStringLiteral("text_font_fontComboBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(text_font_fontComboBox->sizePolicy().hasHeightForWidth());
        text_font_fontComboBox->setSizePolicy(sizePolicy3);

        text_font_horizontalLayout->addWidget(text_font_fontComboBox);


        verticalLayout->addLayout(text_font_horizontalLayout);

        text_text_horizontalLayout = new QHBoxLayout();
        text_text_horizontalLayout->setSpacing(6);
        text_text_horizontalLayout->setObjectName(QStringLiteral("text_text_horizontalLayout"));
        text_text_label = new QLabel(text_actions_groupBox);
        text_text_label->setObjectName(QStringLiteral("text_text_label"));

        text_text_horizontalLayout->addWidget(text_text_label);

        text_text_lineEdit = new QLineEdit(text_actions_groupBox);
        text_text_lineEdit->setObjectName(QStringLiteral("text_text_lineEdit"));
        text_text_lineEdit->setAutoFillBackground(false);

        text_text_horizontalLayout->addWidget(text_text_lineEdit);


        verticalLayout->addLayout(text_text_horizontalLayout);


        text_verticalLayout->addWidget(text_actions_groupBox);


        mod_gridLayout->addWidget(text_groupBox, 0, 1, 1, 1);

        picture_groupBox = new QGroupBox(mod_groupBox);
        picture_groupBox->setObjectName(QStringLiteral("picture_groupBox"));
        picture_groupBox->setAutoFillBackground(false);
        picture_groupBox->setStyleSheet(QStringLiteral("background: rgb(240, 255, 255)"));
        picture_groupBox->setFlat(true);
        picture_verticalLayout = new QVBoxLayout(picture_groupBox);
        picture_verticalLayout->setSpacing(5);
        picture_verticalLayout->setContentsMargins(11, 11, 11, 11);
        picture_verticalLayout->setObjectName(QStringLiteral("picture_verticalLayout"));
        picture_verticalLayout->setContentsMargins(2, 2, 2, 2);
        picture_radioButton = new QRadioButton(picture_groupBox);
        buttonGroup_Mod->addButton(picture_radioButton);
        picture_radioButton->setObjectName(QStringLiteral("picture_radioButton"));
        picture_radioButton->setFont(font2);

        picture_verticalLayout->addWidget(picture_radioButton, 0, Qt::AlignHCenter);

        picture_actions_groupBox = new QGroupBox(picture_groupBox);
        picture_actions_groupBox->setObjectName(QStringLiteral("picture_actions_groupBox"));
        picture_actions_groupBox->setFlat(true);
        verticalLayout_2 = new QVBoxLayout(picture_actions_groupBox);
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        picture_file_horizontalLayout = new QHBoxLayout();
        picture_file_horizontalLayout->setSpacing(6);
        picture_file_horizontalLayout->setObjectName(QStringLiteral("picture_file_horizontalLayout"));
        picture_file_radioButton = new QRadioButton(picture_actions_groupBox);
        buttonGroup_Picture = new QButtonGroup(CobotKuka);
        buttonGroup_Picture->setObjectName(QStringLiteral("buttonGroup_Picture"));
        buttonGroup_Picture->addButton(picture_file_radioButton);
        picture_file_radioButton->setObjectName(QStringLiteral("picture_file_radioButton"));
        picture_file_radioButton->setChecked(true);

        picture_file_horizontalLayout->addWidget(picture_file_radioButton);

        picture_file_lineEdit = new QLineEdit(picture_actions_groupBox);
        picture_file_lineEdit->setObjectName(QStringLiteral("picture_file_lineEdit"));
        picture_file_lineEdit->setAutoFillBackground(false);
        picture_file_lineEdit->setReadOnly(true);

        picture_file_horizontalLayout->addWidget(picture_file_lineEdit);

        picture_file_pushButton = new QPushButton(picture_actions_groupBox);
        picture_file_pushButton->setObjectName(QStringLiteral("picture_file_pushButton"));

        picture_file_horizontalLayout->addWidget(picture_file_pushButton);


        verticalLayout_2->addLayout(picture_file_horizontalLayout);

        picture_webcam_horizontalLayout = new QHBoxLayout();
        picture_webcam_horizontalLayout->setSpacing(6);
        picture_webcam_horizontalLayout->setObjectName(QStringLiteral("picture_webcam_horizontalLayout"));
        picture_webcam_radioButton = new QRadioButton(picture_actions_groupBox);
        buttonGroup_Picture->addButton(picture_webcam_radioButton);
        picture_webcam_radioButton->setObjectName(QStringLiteral("picture_webcam_radioButton"));

        picture_webcam_horizontalLayout->addWidget(picture_webcam_radioButton);

        picture_webcam_pushButton = new QPushButton(picture_actions_groupBox);
        picture_webcam_pushButton->setObjectName(QStringLiteral("picture_webcam_pushButton"));
        sizePolicy3.setHeightForWidth(picture_webcam_pushButton->sizePolicy().hasHeightForWidth());
        picture_webcam_pushButton->setSizePolicy(sizePolicy3);
        picture_webcam_pushButton->setAutoFillBackground(false);

        picture_webcam_horizontalLayout->addWidget(picture_webcam_pushButton);


        verticalLayout_2->addLayout(picture_webcam_horizontalLayout);


        picture_verticalLayout->addWidget(picture_actions_groupBox);


        mod_gridLayout->addWidget(picture_groupBox, 1, 0, 1, 1);

        sketch_groupBox = new QGroupBox(mod_groupBox);
        sketch_groupBox->setObjectName(QStringLiteral("sketch_groupBox"));
        sketch_groupBox->setAutoFillBackground(false);
        sketch_groupBox->setStyleSheet(QStringLiteral("background: rgb(255, 255, 240)"));
        sketch_groupBox->setFlat(true);
        sketch_verticalLayout = new QVBoxLayout(sketch_groupBox);
        sketch_verticalLayout->setSpacing(5);
        sketch_verticalLayout->setContentsMargins(11, 11, 11, 11);
        sketch_verticalLayout->setObjectName(QStringLiteral("sketch_verticalLayout"));
        sketch_verticalLayout->setContentsMargins(2, 2, 2, 2);
        sketch_radioButton = new QRadioButton(sketch_groupBox);
        buttonGroup_Mod->addButton(sketch_radioButton);
        sketch_radioButton->setObjectName(QStringLiteral("sketch_radioButton"));
        sketch_radioButton->setFont(font2);

        sketch_verticalLayout->addWidget(sketch_radioButton, 0, Qt::AlignHCenter);

        sketch_actions_groupBox = new QGroupBox(sketch_groupBox);
        sketch_actions_groupBox->setObjectName(QStringLiteral("sketch_actions_groupBox"));
        sketch_actions_groupBox->setFlat(true);
        verticalLayout_3 = new QVBoxLayout(sketch_actions_groupBox);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        sketch_pushButton = new QPushButton(sketch_actions_groupBox);
        sketch_pushButton->setObjectName(QStringLiteral("sketch_pushButton"));
        sketch_pushButton->setAutoFillBackground(false);

        verticalLayout_3->addWidget(sketch_pushButton);


        sketch_verticalLayout->addWidget(sketch_actions_groupBox);


        mod_gridLayout->addWidget(sketch_groupBox, 1, 1, 1, 1);


        gridLayout->addLayout(mod_gridLayout, 0, 0, 1, 1);


        global_gridLayout->addWidget(mod_groupBox, 1, 0, 1, 4);

        connect_horizontalLayout = new QHBoxLayout();
        connect_horizontalLayout->setSpacing(5);
        connect_horizontalLayout->setObjectName(QStringLiteral("connect_horizontalLayout"));
        connect_horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        connect_horizontalLayout->setContentsMargins(0, -1, -1, -1);
        connect_groupBox = new QGroupBox(centralWidget);
        connect_groupBox->setObjectName(QStringLiteral("connect_groupBox"));
        connect_groupBox->setFlat(true);
        horizontalLayout = new QHBoxLayout(connect_groupBox);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        connect_ip_label = new QLabel(connect_groupBox);
        connect_ip_label->setObjectName(QStringLiteral("connect_ip_label"));

        horizontalLayout->addWidget(connect_ip_label);

        connect_ip_lineEdit = new QLineEdit(connect_groupBox);
        connect_ip_lineEdit->setObjectName(QStringLiteral("connect_ip_lineEdit"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(connect_ip_lineEdit->sizePolicy().hasHeightForWidth());
        connect_ip_lineEdit->setSizePolicy(sizePolicy4);
        connect_ip_lineEdit->setMaxLength(15);
        connect_ip_lineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);
        connect_ip_lineEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(connect_ip_lineEdit);

        connect_port_label = new QLabel(connect_groupBox);
        connect_port_label->setObjectName(QStringLiteral("connect_port_label"));

        horizontalLayout->addWidget(connect_port_label);

        connect_port_lineEdit = new QLineEdit(connect_groupBox);
        connect_port_lineEdit->setObjectName(QStringLiteral("connect_port_lineEdit"));
        sizePolicy4.setHeightForWidth(connect_port_lineEdit->sizePolicy().hasHeightForWidth());
        connect_port_lineEdit->setSizePolicy(sizePolicy4);
        connect_port_lineEdit->setInputMethodHints(Qt::ImhDigitsOnly);
        connect_port_lineEdit->setMaxLength(5);
        connect_port_lineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);
        connect_port_lineEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(connect_port_lineEdit);


        connect_horizontalLayout->addWidget(connect_groupBox);

        connect_horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        connect_horizontalLayout->addItem(connect_horizontalSpacer);

        connect_status_label = new QLabel(centralWidget);
        connect_status_label->setObjectName(QStringLiteral("connect_status_label"));
        connect_status_label->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
        connect_status_label->setTextFormat(Qt::AutoText);

        connect_horizontalLayout->addWidget(connect_status_label);

        connect_pushButton = new QPushButton(centralWidget);
        connect_pushButton->setObjectName(QStringLiteral("connect_pushButton"));
        sizePolicy3.setHeightForWidth(connect_pushButton->sizePolicy().hasHeightForWidth());
        connect_pushButton->setSizePolicy(sizePolicy3);
        connect_pushButton->setCheckable(true);
        connect_pushButton->setAutoDefault(false);
        connect_pushButton->setDefault(true);
        connect_pushButton->setFlat(false);

        connect_horizontalLayout->addWidget(connect_pushButton);


        global_gridLayout->addLayout(connect_horizontalLayout, 0, 0, 1, 4);


        horizontalLayout_7->addLayout(global_gridLayout);

        CobotKuka->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CobotKuka);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 703, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        CobotKuka->setMenuBar(menuBar);
        statusBar = new QStatusBar(CobotKuka);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CobotKuka->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        svg_file_label->setBuddy(svg_file_lineEdit);
        text_font_label->setBuddy(text_font_fontComboBox);
        text_text_label->setBuddy(text_text_lineEdit);
        connect_ip_label->setBuddy(connect_ip_lineEdit);
        connect_port_label->setBuddy(connect_port_lineEdit);
        connect_status_label->setBuddy(connect_pushButton);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(connect_ip_lineEdit, connect_port_lineEdit);
        QWidget::setTabOrder(connect_port_lineEdit, connect_pushButton);
        QWidget::setTabOrder(connect_pushButton, svg_radioButton);
        QWidget::setTabOrder(svg_radioButton, svg_file_lineEdit);
        QWidget::setTabOrder(svg_file_lineEdit, svg_file_pushButton);
        QWidget::setTabOrder(svg_file_pushButton, text_radioButton);
        QWidget::setTabOrder(text_radioButton, text_font_fontComboBox);
        QWidget::setTabOrder(text_font_fontComboBox, text_text_lineEdit);
        QWidget::setTabOrder(text_text_lineEdit, picture_radioButton);
        QWidget::setTabOrder(picture_radioButton, picture_file_radioButton);
        QWidget::setTabOrder(picture_file_radioButton, picture_file_lineEdit);
        QWidget::setTabOrder(picture_file_lineEdit, picture_file_pushButton);
        QWidget::setTabOrder(picture_file_pushButton, picture_webcam_radioButton);
        QWidget::setTabOrder(picture_webcam_radioButton, picture_webcam_pushButton);
        QWidget::setTabOrder(picture_webcam_pushButton, sketch_radioButton);
        QWidget::setTabOrder(sketch_radioButton, sketch_pushButton);
        QWidget::setTabOrder(sketch_pushButton, ok_pushButton);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionQuit);
        menuAbout->addAction(actionAbout);

        retranslateUi(CobotKuka);

        QMetaObject::connectSlotsByName(CobotKuka);
    } // setupUi

    void retranslateUi(QMainWindow *CobotKuka)
    {
        CobotKuka->setWindowTitle(QApplication::translate("CobotKuka", "CobotKuka", 0));
        actionQuit->setText(QApplication::translate("CobotKuka", "Quit", 0));
#ifndef QT_NO_TOOLTIP
        actionQuit->setToolTip(QApplication::translate("CobotKuka", "Disconnect from the server and exit the program", 0));
#endif // QT_NO_TOOLTIP
        actionQuit->setShortcut(QApplication::translate("CobotKuka", "Esc", 0));
        actionAbout->setText(QApplication::translate("CobotKuka", "About", 0));
        ok_pushButton->setText(QApplication::translate("CobotKuka", "> OK <", 0));
        stop_pushButton->setText(QApplication::translate("CobotKuka", "STOP", 0));
        send_pushButton->setText(QApplication::translate("CobotKuka", "-> Send <-", 0));
        mod_groupBox->setTitle(QString());
        svg_radioButton->setText(QApplication::translate("CobotKuka", "SVG", 0));
        svg_actions_groupBox->setTitle(QString());
        svg_file_label->setText(QApplication::translate("CobotKuka", "file : ", 0));
        svg_file_pushButton->setText(QApplication::translate("CobotKuka", "Open...", 0));
        text_radioButton->setText(QApplication::translate("CobotKuka", "TEXT", 0));
        text_actions_groupBox->setTitle(QString());
        text_font_label->setText(QApplication::translate("CobotKuka", "Font : ", 0));
        text_text_label->setText(QApplication::translate("CobotKuka", "Your text : ", 0));
        picture_radioButton->setText(QApplication::translate("CobotKuka", "PICTURE", 0));
        picture_actions_groupBox->setTitle(QString());
        picture_file_radioButton->setText(QApplication::translate("CobotKuka", "file : ", 0));
        picture_file_pushButton->setText(QApplication::translate("CobotKuka", "Open...", 0));
        picture_webcam_radioButton->setText(QApplication::translate("CobotKuka", "webcam : ", 0));
        picture_webcam_pushButton->setText(QApplication::translate("CobotKuka", "Open webcam", 0));
        sketch_radioButton->setText(QApplication::translate("CobotKuka", "SKETCH", 0));
        sketch_actions_groupBox->setTitle(QString());
        sketch_pushButton->setText(QApplication::translate("CobotKuka", "Open Sketch Window", 0));
        connect_groupBox->setTitle(QString());
        connect_ip_label->setText(QApplication::translate("CobotKuka", "IP : ", 0));
        connect_ip_lineEdit->setText(QApplication::translate("CobotKuka", "172.30.1.125", 0));
        connect_port_label->setText(QApplication::translate("CobotKuka", "Port : ", 0));
        connect_port_lineEdit->setText(QApplication::translate("CobotKuka", "9191", 0));
        connect_status_label->setText(QApplication::translate("CobotKuka", "Disconnected", 0));
        connect_pushButton->setText(QApplication::translate("CobotKuka", "Connect", 0));
        menuFile->setTitle(QApplication::translate("CobotKuka", "File", 0));
        menuAbout->setTitle(QApplication::translate("CobotKuka", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class CobotKuka: public Ui_CobotKuka {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COBOTKUKA_H

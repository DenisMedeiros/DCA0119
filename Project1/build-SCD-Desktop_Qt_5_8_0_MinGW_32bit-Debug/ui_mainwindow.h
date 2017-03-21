/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionMode1;
    QAction *actionMode2;
    QAction *actionSCD;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QFrame *chartFrame;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QMenuBar *menuBar;
    QMenu *menuSystem;
    QMenu *menuOperation;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(463, 335);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/resources/scd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionStart = new QAction(MainWindow);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/resources/start.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon1);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/resources/stop.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon2);
        actionMode1 = new QAction(MainWindow);
        actionMode1->setObjectName(QStringLiteral("actionMode1"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/resources/mode1.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionMode1->setIcon(icon3);
        actionMode2 = new QAction(MainWindow);
        actionMode2->setObjectName(QStringLiteral("actionMode2"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/resources/mode2.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionMode2->setIcon(icon4);
        actionSCD = new QAction(MainWindow);
        actionSCD->setObjectName(QStringLiteral("actionSCD"));
        actionSCD->setIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        chartFrame = new QFrame(centralWidget);
        chartFrame->setObjectName(QStringLiteral("chartFrame"));
        chartFrame->setFrameShape(QFrame::Box);
        chartFrame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(chartFrame);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setFont(font);
        lineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit);

        horizontalLayout->setStretch(0, 20);

        horizontalLayout_3->addLayout(horizontalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setFont(font);
        lineEdit_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_2->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEdit_2);

        horizontalLayout_2->setStretch(0, 20);

        horizontalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3->setStretch(0, 40);
        horizontalLayout_3->setStretch(1, 20);
        horizontalLayout_3->setStretch(2, 40);

        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(0, 90);
        verticalLayout->setStretch(1, 10);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 463, 21));
        menuSystem = new QMenu(menuBar);
        menuSystem->setObjectName(QStringLiteral("menuSystem"));
        menuOperation = new QMenu(menuBar);
        menuOperation->setObjectName(QStringLiteral("menuOperation"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSystem->menuAction());
        menuBar->addAction(menuOperation->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuSystem->addAction(actionStart);
        menuSystem->addAction(actionStop);
        menuOperation->addAction(actionMode1);
        menuOperation->addAction(actionMode2);
        menuAbout->addAction(actionSCD);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SCD - v0.1", Q_NULLPTR));
        actionStart->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionStart->setToolTip(QApplication::translate("MainWindow", "This will start the system", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionStart->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", Q_NULLPTR));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionStop->setToolTip(QApplication::translate("MainWindow", "This will stop the system", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionStop->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", Q_NULLPTR));
        actionMode1->setText(QApplication::translate("MainWindow", "Mode 1", Q_NULLPTR));
        actionMode1->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", Q_NULLPTR));
        actionMode2->setText(QApplication::translate("MainWindow", "Mode 2", Q_NULLPTR));
        actionMode2->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", Q_NULLPTR));
        actionSCD->setText(QApplication::translate("MainWindow", "SCD", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Sensor", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("MainWindow", "2.90", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "PWM", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("MainWindow", "3.49", Q_NULLPTR));
        menuSystem->setTitle(QApplication::translate("MainWindow", "System", Q_NULLPTR));
        menuOperation->setTitle(QApplication::translate("MainWindow", "Operation", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

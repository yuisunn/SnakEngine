/********************************************************************************
** Form generated from reading UI file 'footedit.ui'
**
** Created: Thu Feb 16 14:05:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOOTEDIT_H
#define UI_FOOTEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FootEditClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionCopy;
    QAction *actionLog;
    QWidget *pushButton_2;
    QTabWidget *propertyTabWidget;
    QWidget *objectTab;
    QToolButton *toolButton;
    QPushButton *pushButton;
    QRadioButton *radioButton;
    QTableView *tableView;
    QWidget *tab_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QPushButton *pushButton_3;
    QWidget *page_2;
    QToolBox *toolBox;
    QWidget *page_3;
    QWidget *page_4;
    QDialogButtonBox *buttonBox;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QDockWidget *propertyWidget;
    QWidget *dockWidgetContents;
    QDockWidget *sceneWidget;
    QWidget *dockWidgetContents_2;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_3;
    QDockWidget *operatorWidget;
    QWidget *dockWidgetContents_4;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_6;

    void setupUi(QMainWindow *FootEditClass)
    {
        if (FootEditClass->objectName().isEmpty())
            FootEditClass->setObjectName(QString::fromUtf8("FootEditClass"));
        FootEditClass->resize(706, 549);
        actionNew = new QAction(FootEditClass);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(FootEditClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(FootEditClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(FootEditClass);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionExit = new QAction(FootEditClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(FootEditClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionCopy = new QAction(FootEditClass);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionLog = new QAction(FootEditClass);
        actionLog->setObjectName(QString::fromUtf8("actionLog"));
        pushButton_2 = new QWidget(FootEditClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        propertyTabWidget = new QTabWidget(pushButton_2);
        propertyTabWidget->setObjectName(QString::fromUtf8("propertyTabWidget"));
        propertyTabWidget->setGeometry(QRect(170, 180, 131, 201));
        propertyTabWidget->setTabPosition(QTabWidget::South);
        objectTab = new QWidget();
        objectTab->setObjectName(QString::fromUtf8("objectTab"));
        toolButton = new QToolButton(objectTab);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(50, 100, 37, 18));
        pushButton = new QPushButton(objectTab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 50, 75, 23));
        radioButton = new QRadioButton(objectTab);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(30, 80, 89, 16));
        tableView = new QTableView(objectTab);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(-90, -20, 256, 192));
        propertyTabWidget->addTab(objectTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        propertyTabWidget->addTab(tab_2, QString());
        stackedWidget = new QStackedWidget(pushButton_2);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(150, 50, 120, 80));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        pushButton_3 = new QPushButton(page);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 40, 75, 23));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        toolBox = new QToolBox(pushButton_2);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setGeometry(QRect(430, 120, 69, 121));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, 0, 69, 69));
        toolBox->addItem(page_3, QString::fromUtf8("Page 1"));
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        page_4->setGeometry(QRect(0, 0, 100, 30));
        toolBox->addItem(page_4, QString::fromUtf8("Page 2"));
        buttonBox = new QDialogButtonBox(pushButton_2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 180, 156, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textEdit = new QTextEdit(pushButton_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(60, 340, 104, 71));
        lineEdit = new QLineEdit(pushButton_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(50, 270, 113, 20));
        FootEditClass->setCentralWidget(pushButton_2);
        statusBar = new QStatusBar(FootEditClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FootEditClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(FootEditClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 706, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        FootEditClass->setMenuBar(menuBar);
        propertyWidget = new QDockWidget(FootEditClass);
        propertyWidget->setObjectName(QString::fromUtf8("propertyWidget"));
        propertyWidget->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        propertyWidget->setWidget(dockWidgetContents);
        FootEditClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), propertyWidget);
        sceneWidget = new QDockWidget(FootEditClass);
        sceneWidget->setObjectName(QString::fromUtf8("sceneWidget"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        sceneWidget->setWidget(dockWidgetContents_2);
        FootEditClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), sceneWidget);
        dockWidget = new QDockWidget(FootEditClass);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        dockWidget->setWidget(dockWidgetContents_3);
        FootEditClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget);
        operatorWidget = new QDockWidget(FootEditClass);
        operatorWidget->setObjectName(QString::fromUtf8("operatorWidget"));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        operatorWidget->setWidget(dockWidgetContents_4);
        FootEditClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), operatorWidget);
        dockWidget_3 = new QDockWidget(FootEditClass);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        dockWidget_3->setWidget(dockWidgetContents_6);
        FootEditClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_3);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionCopy);
        menuWindow->addAction(actionLog);
        menuHelp->addAction(actionAbout);

        retranslateUi(FootEditClass);

        propertyTabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FootEditClass);
    } // setupUi

    void retranslateUi(QMainWindow *FootEditClass)
    {
        FootEditClass->setWindowTitle(QApplication::translate("FootEditClass", "FootEdit", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("FootEditClass", "New", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("FootEditClass", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("FootEditClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("FootEditClass", "Save As", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("FootEditClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("FootEditClass", "About", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("FootEditClass", "Copy", 0, QApplication::UnicodeUTF8));
        actionLog->setText(QApplication::translate("FootEditClass", "Log", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("FootEditClass", "...", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FootEditClass", "PushButton", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("FootEditClass", "RadioButton", 0, QApplication::UnicodeUTF8));
        propertyTabWidget->setTabText(propertyTabWidget->indexOf(objectTab), QApplication::translate("FootEditClass", "Object", 0, QApplication::UnicodeUTF8));
        propertyTabWidget->setTabText(propertyTabWidget->indexOf(tab_2), QApplication::translate("FootEditClass", "Tab 2", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("FootEditClass", "PushButton", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("FootEditClass", "Page 1", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("FootEditClass", "Page 2", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("FootEditClass", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("FootEditClass", "Edit", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("FootEditClass", "Window", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("FootEditClass", "Help", 0, QApplication::UnicodeUTF8));
        propertyWidget->setWindowTitle(QApplication::translate("FootEditClass", "property", 0, QApplication::UnicodeUTF8));
        sceneWidget->setWindowTitle(QApplication::translate("FootEditClass", "Scene", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FootEditClass: public Ui_FootEditClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOOTEDIT_H

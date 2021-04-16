/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>

/* project includes */
#include <qcustomplot.h>
#include <generalplot.h>
#include <generalsetup.h>
#include <centralwidget.h>

class MainWindow : public QMainWindow
{
        Q_OBJECT
public:
        explicit MainWindow(QWidget *parent = nullptr);

private:
        /* Main menu */
        QMenuBar *main_menu_bar;
        struct _s_main_menu{
                QMenu *file;
        } s_main_menu;
        void buildMainMenu();

protected slots:
        void selectWorkDirectory();


signals:

};

#endif // MAINWINDOW_H

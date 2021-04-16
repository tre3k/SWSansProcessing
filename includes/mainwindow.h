/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qcustomplot.h>
#include <generalplot.h>

class MainWindow : public QMainWindow
{
        Q_OBJECT
public:
        explicit MainWindow(QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H

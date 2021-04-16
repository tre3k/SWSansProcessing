/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/


#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        this->setWindowTitle("SW SANS Processing");

        buildMainMenu();

        auto cw = new CentralWidget();


        this->setCentralWidget(cw);

}

void MainWindow::buildMainMenu(){
        main_menu_bar = new QMenuBar();

        s_main_menu.file = new QMenu("&file");
        s_main_menu.file->addAction("select work directory",this,SLOT(selectWorkDirectory()));
        s_main_menu.file->addAction("&quit",this,SLOT(close()));

        main_menu_bar->addMenu(s_main_menu.file);
        this->setMenuBar(main_menu_bar);
}


void MainWindow::selectWorkDirectory(){
        GeneralSetup::work_direcotry =  QFileDialog::getExistingDirectory(nullptr,"select work directory","./");
}


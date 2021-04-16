/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/

#include "generalplot.h"

GeneralPlot::GeneralPlot()
{
        this->setInteractions(QCP::iRangeZoom |
                              QCP::iRangeDrag |
                              QCP::iMultiSelect |
                              QCP::iSelectLegend |
                              QCP::iSelectPlottables |
                              QCP::iSelectAxes);

        this->xAxis2->setVisible(true);
        this->yAxis2->setVisible(true);

        connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),this->xAxis2, SLOT(setRange(QCPRange)));
        connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));


        // Enable cutom context menu
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuReq(QPoint)));



        QLocale locale("en_EN.UTF-8");                  //чтобы была точка заместо запятой в числах на графике
        this->setLocale(locale);
}

void GeneralPlot::contextMenuReq(QPoint p){
        QMenu *menu = new QMenu(this);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        QMenu *menu_export = new QMenu("Export");
        menu_export->setAttribute(Qt::WA_DeleteOnClose);
        menu->addMenu(menu_export);

        menu_export->addAction("bmp",this,SLOT(exportToBMP()));
        menu_export->addAction("jpg",this,SLOT(exportToJPG()));
        menu_export->addAction("pdf",this,SLOT(exportToPDF()));
        menu_export->addAction("png",this,SLOT(exportToPNG()));

        if(!x_log){
                menu->addAction("x log scale",this,SLOT(setXLog()));
        }else{
                menu->addAction("x linear scale",this,SLOT(setXLog()));
        }

        if(!y_log){
                menu->addAction("y log scale",this,SLOT(setYLog()));
        }else{
                menu->addAction("y linear scale",this,SLOT(setYLog()));
        }

        menu->popup(this->mapToGlobal(p));
}



void GeneralPlot::setXLog(){
        x_log = !x_log;
        if(x_log){
                this->xAxis->setScaleType(QCPAxis::stLogarithmic);
                this->xAxis2->setScaleType(QCPAxis::stLogarithmic);
        }else{
                this->xAxis->setScaleType(QCPAxis::stLinear);
                this->xAxis2->setScaleType(QCPAxis::stLinear);
        }
        this->replot();
}

void GeneralPlot::setYLog(){
        y_log = !y_log;
        if(y_log){
                this->yAxis->setScaleType(QCPAxis::stLogarithmic);
                this->yAxis2->setScaleType(QCPAxis::stLogarithmic);
        }else{
                this->yAxis->setScaleType(QCPAxis::stLinear);
                this->yAxis2->setScaleType(QCPAxis::stLinear);
        }
        this->replot();
}



void GeneralPlot::exportToBMP(){
        auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".bmp","(*.bmp *.BMP)");
        if(filename!=nullptr) this->saveBmp(filename);
}

void GeneralPlot::exportToJPG(){
        auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".jpg","(*.jpg *.JPG)");
        if(filename!=nullptr) this->saveJpg(filename);
}

void GeneralPlot::exportToPDF(){
        auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".pdf","(*.pdf *.PDF)");
        if(filename!=nullptr) this->savePdf(filename);
}

void GeneralPlot::exportToPNG(){
        auto filename = QFileDialog::getSaveFileName(nullptr,"Save",".png","(*.png *.PNG)");
        if(filename!=nullptr) this->savePng(filename);
}


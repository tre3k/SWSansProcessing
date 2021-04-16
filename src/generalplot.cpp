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

        connect(this,SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),this,SLOT(sAxies_drag_zoom(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));
        connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(full_drag_zoom(QMouseEvent*)));
        connect(this,SIGNAL(selectionChangedByUser()),this,SLOT(selectionChanged()));


        // Enable cutom context menu
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuReq(QPoint)));



        QLocale locale(GeneralSetup::number_locale);
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



void GeneralPlot::sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event){
        this->axisRect()->setRangeDrag(sAxis->orientation());
        this->axisRect()->setRangeZoom(sAxis->orientation());
        return;
}

void GeneralPlot::full_drag_zoom(QMouseEvent *mouseEvent){
        this->axisRect()->setRangeDrag(this->xAxis->orientation()|
                                       this->yAxis->orientation());
        this->axisRect()->setRangeZoom(this->xAxis->orientation()|
                                       this->yAxis->orientation());
        return;
}

void GeneralPlot::selectionChanged(){
        if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
                        this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
        {
                this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
                this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        }

        // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
        if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
                        this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
        {
                this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
                this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        }

        // synchronize selection of graphs with selection of corresponding legend items:
        for (int i=0; i<this->graphCount(); ++i)
        {
                QCPGraph *graph = this->graph(i);
                QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
                if (item->selected() || graph->selected())
                {
                        item->setSelected(true);
                        graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
                }
        }
        return;
}

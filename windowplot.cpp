#include "windowplot.h"

windowPlot::windowPlot(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumWidth(600);
    this->setMinimumHeight(350);

    menuBar = new QMenuBar;

    /* menu File */
    menuFile = new QMenu("&File");

    menuExport = new QMenu("&Export");
    menuFile->addMenu(menuExport);
    actionExportTxt = new QAction("Export to txt");
    menuExport->addAction(actionExportTxt);
    actionExportQti = new QAction("Export to QtiPlot file format");
    menuExport->addAction(actionExportQti);
    actionExportPNG = new QAction("Export to PNG");
    menuExport->addAction(actionExportPNG);
    actionExportPDF = new QAction("Export to PDF");
    menuExport->addAction(actionExportPDF);
    actionExportAll = new QAction("Export all");
    menuExport->addAction(actionExportAll);

    menuFile->addSeparator();
    actionClose = new QAction("&Close");
    menuFile->addAction(actionClose);


    /* menu X axies */
    menuXaxis = new QMenu("X axis");
    menuXUnits = new QMenu("x units");
    menuXaxis->addMenu(menuXUnits);
    actionXunitPix = new QAction("pix");
    actionXunitPix->setCheckable(true);
    actionXunitPix->setChecked(true);
    menuXUnits->addAction(actionXunitPix);
    actionXunitQA = new QAction("Q (1/A)");
    actionXunitQA->setCheckable(true);
    menuXUnits->addAction(actionXunitQA);
    actionXunitQnm = new QAction("Q (1/nm)");
    menuXUnits->addAction(actionXunitQnm);
    actionXunitQnm->setCheckable(true);
    actionXunitRad = new QAction("θ (mrad)");
    menuXUnits->addAction(actionXunitRad);
    actionXunitRad->setCheckable(true);
    menuXaxis->addSeparator();
    actionXlogScale = new QAction("x log scale");
    menuXaxis->addAction(actionXlogScale);
    actionXlinScale = new QAction("x lin scale");
    menuXaxis->addAction(actionXlinScale);

    /* menu Y axies */
    menuYaxis = new QMenu("Y axis");
    menuYUnits = new QMenu("y units");
    menuYaxis->addMenu(menuYUnits);
    actionYunitN = new QAction("N (counts)");
    menuYUnits->addAction(actionYunitN);
    actionYunitN->setCheckable(true);
    actionYunitN->setChecked(true);
    actionYunitNs = new QAction("N/time (counts/sec)");
    menuYUnits->addAction(actionYunitNs);
    actionYunitNs->setCheckable(true);
    menuYaxis->addSeparator();
    actionYlogScale = new QAction("y log scale");
    menuYaxis->addAction(actionYlogScale);
    actionYlinScale = new QAction("y lin scale");
    menuYaxis->addAction(actionYlinScale);


    /* menu Plot */
    menuPlot = new QMenu("Plot");

    actionAutoscale = new QAction("Autoscale");
    menuPlot->addAction(actionAutoscale);
    menuPlot->addSeparator();
    actionPlotLeft = new QAction("show/hide plot Left");
    actionPlotLeft->setCheckable(true);
    actionPlotLeft->setChecked(true);
    menuPlot->addAction(actionPlotLeft);
    actionPlotRight = new QAction("show/hide plot Right");
    actionPlotRight->setCheckable(true);
    actionPlotRight->setChecked(true);
    menuPlot->addAction(actionPlotRight);
    actionPlotAmount = new QAction("show/hide plot Amount");
    actionPlotAmount->setCheckable(true);
    actionPlotAmount->setChecked(true);
    menuPlot->addAction(actionPlotAmount);

    /* menu De interpolate */
    menuDeInterpolate = new QMenu("Deinterpolate");
    actionDeInterpolateAverage = new QAction("Average");
    menuDeInterpolate->addAction(actionDeInterpolateAverage);
    actionDeInterpolateThinning = new QAction("Thinning");
    menuDeInterpolate->addAction(actionDeInterpolateThinning);

    /* Set menus */

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuXaxis);
    menuBar->addMenu(menuYaxis);
    menuBar->addMenu(menuPlot);
    //menuBar->addMenu(menuDeInterpolate);


    connect(actionClose,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionClose()));
    connect(actionExportTxt,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionExportTxt()));
    connect(actionExportQti,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionExportQti()));
    connect(actionExportPNG,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionExportPNG()));
    connect(actionExportPDF,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionExportPDF()));
    connect(actionExportAll,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionExportAll()));
    connect(actionAutoscale,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionAutoscale()));
    connect(actionPlotLeft,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionPlotLeft(bool)));
    connect(actionPlotRight,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionPlotRigh(bool)));
    connect(actionPlotAmount,SIGNAL(triggered(bool)),
            this,SLOT(slot_actionPlotAmount(bool)));
    connect(actionXunitPix,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectXUnitPix(bool)));
    connect(actionXunitQA,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectXUnitQA(bool)));
    connect(actionXunitQnm,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectXUnitQnm(bool)));
    connect(actionXunitRad,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectXUnitRad(bool)));
    connect(actionYunitN,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectYUnitN(bool)));
    connect(actionYunitNs,SIGNAL(toggled(bool)),
            this,SLOT(slot_selectYUnitNs(bool)));
    connect(actionXlinScale,SIGNAL(triggered(bool)),
            this,SLOT(slot_setXlinScale()));
    connect(actionXlogScale,SIGNAL(triggered(bool)),
            this,SLOT(slot_setXlogScale()));
    connect(actionYlinScale,SIGNAL(triggered(bool)),
            this,SLOT(slot_setYlinScale()));
    connect(actionYlogScale,SIGNAL(triggered(bool)),
            this,SLOT(slot_setYlogScale()));


    /* PLOT */
    plot = new iQCustomPlot;
    errorBar1 = new QCPErrorBars(plot->xAxis,plot->yAxis);
    errorBar2 = new QCPErrorBars(plot->xAxis,plot->yAxis);
    errorBar_s = new QCPErrorBars(plot->xAxis,plot->yAxis);

    errorBar1->removeFromLegend();
    errorBar1->setPen(QColor("green"));
    errorBar2->removeFromLegend();
    errorBar2->setPen(QColor("red"));
    errorBar_s->removeFromLegend();
    errorBar_s->setPen(QColor("black"));

    plot->legend->setVisible(true);
    plot->legend->setBrush(QBrush(QColor(255,255,255,50)));

    this->setMenuBar(menuBar);
    this->setCentralWidget(plot);
}

void windowPlot::slot_loadVariables(sWindowPlotVariables var){
    sVar = var;

    updatePlot();
    plot->rescaleAxes(true);
    plot->replot();
    return;
}

void windowPlot::updatePlot(){

    int minIndex;
    vErr1.clear(); vErr2.clear(); vErr_s.clear();
    vX1.clear(); vX2.clear(); vXs.clear();
    vY1.clear(); vY2.clear(); vYs.clear();

    switch(unit_x){
    case UNITX_PIX:
        plot->xAxis->setLabel("pix");
        for(int i=0;i<sVar.vX1->size();i++) vX1.append(sVar.vX1->at(i));
        for(int i=0;i<sVar.vX2->size();i++) vX2.append(sVar.vX2->at(i));
        break;
    case UNITX_Q_A:
        plot->xAxis->setLabel("Q, 1/A");
        for(int i=0;i<sVar.vX1->size();i++) vX1.append(pixToQ(sVar.vX1->at(i)));
        for(int i=0;i<sVar.vX2->size();i++) vX2.append(pixToQ(sVar.vX2->at(i)));
        break;
    case UNITX_Q_nm:
        plot->xAxis->setLabel("Q, 1/nm");
        for(int i=0;i<sVar.vX1->size();i++) vX1.append(10*pixToQ(sVar.vX1->at(i)));
        for(int i=0;i<sVar.vX2->size();i++) vX2.append(10*pixToQ(sVar.vX2->at(i)));
        break;
    case UNITX_Rad:
        plot->xAxis->setLabel("θ, mrad");
        for(int i=0;i<sVar.vX1->size();i++) vX1.append(1000*pixToRad(sVar.vX1->at(i)));
        for(int i=0;i<sVar.vX2->size();i++) vX2.append(1000*pixToRad(sVar.vX2->at(i)));
        break;
    }

    switch(unit_y){
    case UNITY_N:
        plot->yAxis->setLabel("Counts, n");
        for(int i=0;i<sVar.vX1->size();i++) vY1.append(sVar.vY1->at(i));
        for(int i=0;i<sVar.vX2->size();i++) vY2.append(sVar.vY2->at(i));
        for(int i=0;i<sVar.vErr1->size();i++) vErr1.append(sVar.vErr1->at(i));
        for(int i=0;i<sVar.vErr2->size();i++) vErr2.append(sVar.vErr2->at(i));
        break;
    case UNITY_Ns:
        plot->yAxis->setLabel("Counts/sec");
        for(int i=0;i<sVar.vX1->size();i++) vY1.append(sVar.vY1->at(i)/sVar.time);
        for(int i=0;i<sVar.vX2->size();i++) vY2.append(sVar.vY2->at(i)/sVar.time);
        for(int i=0;i<sVar.vErr1->size();i++) vErr1.append(sVar.vErr1->at(i)/sVar.time);
        for(int i=0;i<sVar.vErr2->size();i++) vErr2.append(sVar.vErr2->at(i)/sVar.time);
        break;
    }

    minIndex = vX1.size();
    if(minIndex > vX2.size()){
        minIndex = vX2.size();
        for(int i=0;i<minIndex;i++) vXs.append(vX2.at(i));
    }else{
        for(int i=0;i<minIndex;i++) vXs.append(vX1.at(i));
    }
    for(int i=0;i<minIndex;i++) vYs.append(vY1.at(i)+vY2.at(i));
    for(int i=0;i<minIndex;i++) vErr_s.append(vErr1.at(i)+vErr2.at(i));

    plot->clearGraphs();
    errorBar1->data()->clear();
    errorBar2->data()->clear();
    errorBar_s->data()->clear();

    plot->addCurve(&vX1,&vY1,true,"green","Left");
    errorBar1->addData(vErr1);
    //errorBar1->addData(*sVar.vErr1);
    errorBar1->setDataPlottable(plot->graph(0));
    plot->graph(0)->setVisible(actionPlotLeft->isChecked());
    errorBar1->setVisible(actionPlotLeft->isChecked());
    plot->legend->item(0)->setVisible(actionPlotLeft->isChecked());
    errorBar1->selectionDecorator()->setPen(QPen(QColor("green"),
              2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    plot->addCurve(&vX2,&vY2,true,"red","Right");
    errorBar2->addData(vErr2);
    //errorBar2->addData(*sVar.vErr2);
    errorBar2->setDataPlottable(plot->graph(1));
    plot->graph(1)->setVisible(actionPlotRight->isChecked());
    errorBar2->setVisible(actionPlotRight->isChecked());
    plot->legend->item(1)->setVisible(actionPlotRight->isChecked());
    errorBar2->selectionDecorator()->setPen(QPen(QColor("red"),
              2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    plot->addCurve(&vXs,&vYs,true,"black","Amount");
    errorBar_s->addData(vErr_s);
    errorBar_s->setDataPlottable(plot->graph(2));
    plot->graph(2)->setVisible(actionPlotAmount->isChecked());
    errorBar_s->setVisible(actionPlotAmount->isChecked());
    plot->legend->item(2)->setVisible(actionPlotAmount->isChecked());
    errorBar_s->selectionDecorator()->setPen(QPen(QColor("black"),
              2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    plot->xAxis->scaleRange(1.2);
    plot->yAxis->scaleRange(1.2);
    plot->replot();
    return;
}

void windowPlot::slot_actionClose(){
    this->close();
    return;
}

void windowPlot::slot_actionExportQti(){
    QString filename = QFileDialog::getSaveFileName(this,"Export data to QtiPlot format","","*.qti");
    exportToQti(filename);
    return;
}

void windowPlot::slot_actionExportTxt(){
    QString filename = QFileDialog::getSaveFileName(this,"Export data to txt","","*.txt");
    exportToTxt(filename);
    return;
}

void windowPlot::slot_actionExportPNG(){
    QString filename = QFileDialog::getSaveFileName(this,"Export data to txt","","*.png");
    exportToPNG(filename);
    return;
}

void windowPlot::slot_actionExportPDF(){
    QString filename = QFileDialog::getSaveFileName(this,"Export data to pdf","","*.pdf");
    if(filename=="") return;
    if(filename.split('.').at(filename.split('.').size()-1)!="pdf") filename += ".pdf";
    plot->savePdf(filename);
    return;
}

void windowPlot::slot_actionAutoscale(){
    plot->rescaleAxes(true);
    plot->replot();
    return;
}

void windowPlot::slot_actionPlotLeft(bool flag){
    if(plot->graphCount()>0) plot->graph(0)->setVisible(flag);
    errorBar1->setVisible(flag);
    plot->legend->item(0)->setVisible(flag);
    plot->rescaleAxes(false);
    plot->replot();
    return;
}

void windowPlot::slot_actionPlotRigh(bool flag){
    if(plot->graphCount()>1) plot->graph(1)->setVisible(flag);
    errorBar2->setVisible(flag);
    plot->legend->item(1)->setVisible(flag);
    plot->rescaleAxes(false);
    plot->replot();
    return;
}

void windowPlot::slot_actionPlotAmount(bool flag){
    if(plot->graphCount()>2) plot->graph(2)->setVisible(flag);
    errorBar_s->setVisible(flag);
    plot->legend->item(2)->setVisible(flag);
    plot->rescaleAxes(false);
    plot->replot();
    return;
}

void windowPlot::slot_selectXUnitPix(bool flag){
    if(flag){
        actionXunitQA->setChecked(false);
        actionXunitQnm->setChecked(false);
        actionXunitRad->setChecked(false);
        unit_x = UNITX_PIX;
    }
    updatePlot();
    return;
}

void windowPlot::slot_selectXUnitQA(bool flag){
    if(flag){
        actionXunitPix->setChecked(false);
        actionXunitQnm->setChecked(false);
        actionXunitRad->setChecked(false);
        unit_x = UNITX_Q_A;
    }
    updatePlot();
    return;
}

void windowPlot::slot_selectXUnitQnm(bool flag){
    if(flag){
        actionXunitPix->setChecked(false);
        actionXunitQA->setChecked(false);
        actionXunitRad->setChecked(false);
        unit_x = UNITX_Q_nm;
    }
    updatePlot();
    return;
}

void windowPlot::slot_selectXUnitRad(bool flag){
    if(flag){
        actionXunitPix->setChecked(false);
        actionXunitQA->setChecked(false);
        actionXunitQnm->setChecked(false);
        unit_x = UNITX_Rad;
    }
    updatePlot();
    return;
}

void windowPlot::slot_selectYUnitN(bool flag){
    if(flag){
        actionYunitNs->setChecked(false);
        unit_y = UNITY_N;
    }
    updatePlot();
    return;
}

void windowPlot::slot_selectYUnitNs(bool flag){
    if(flag){
        actionYunitN->setChecked(false);
        unit_y = UNITY_Ns;
    }
    updatePlot();
    return;
}

void windowPlot::slot_setXlinScale(){
    plot->xAxis->setScaleType(QCPAxis::stLinear);
    plot->xAxis2->setScaleType(QCPAxis::stLinear);
    plot->replot();
    return;
}

void windowPlot::slot_setXlogScale(){
    plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    plot->replot();
    return;
}

void windowPlot::slot_setYlinScale(){
    plot->yAxis->setScaleType(QCPAxis::stLinear);
    plot->yAxis2->setScaleType(QCPAxis::stLinear);
    plot->replot();
    return;
}

void windowPlot::slot_setYlogScale(){
    plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    plot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    plot->replot();
    return;
}


double windowPlot::pixToQ(double val){
    val /= pow(2,sVar.stepInterpolate);
    return 2*M_PI*atan(val*sVar.size_of_pixel_mm/1000/sVar.dist_source_detector)/sVar.lambdaA;
}

double windowPlot::pixToRad(double val){
    val /= pow(2,sVar.stepInterpolate);
    return atan(val*sVar.size_of_pixel_mm/1000/sVar.dist_source_detector);
}

void windowPlot::slot_deinterpolateThinning(){
    //deinterpolateThinning(&vX1,&vY1,&vErr1,sVar.stepInterpolate);
    return;

}
void windowPlot::slot_deinterpolateAverage(){
    return;
}

void windowPlot::exportToPNG(QString filename){
    if(filename=="") return;
    if(filename.split('.').at(filename.split('.').size()-1)!="png") filename += ".png";
    plot->savePng(filename);
    return;
}

void windowPlot::exportToQti(QString filename){
    if(filename=="") return;
    if(filename.split('.').at(filename.split('.').size()-1)!="qti") filename += ".qti";
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QString QtiVersion = "0.9.9.11";
    int windows=3;
    QString date = "2457958.7432995";
    QString nameWindow;
    QString nameColX;
    QString nameColY;

    switch(unit_x){
    case UNITX_PIX:
        nameColX = "Q(pix)";
        break;
    case UNITX_Q_A:
        nameColX = "Q(1/A)";
        break;
    case UNITX_Q_nm:
        nameColX = "Q(1/nm)";
        break;
    case UNITX_Rad:
        nameColX = "Q(mrad)";
        break;
    }

    switch(unit_y){
    case UNITY_N:
        nameColY = "I(N)";
        break;
    case UNITY_Ns:
        nameColY = "I(N/s)";
        break;
    }

    QTextStream stream(&f);
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream << "<!DOCTYPE qti>\n";
    stream << "<qti app=\"QtiPlot\" "
              "version=\""+QtiVersion+"\" "
              "dpi=\"96\" "
              "arch=\"Gnu64\" "
              "script=\"muParser\" "
              "windows=\""+QString::number(windows)+"\" "
              "expand=\"1\">\n";

    /* LEFT plot table */
    nameWindow = "Left";
    stream << "\t<window type=\"1\" "
              "name=\""+nameWindow+"\" "
              "date=\""+date+"\" "
              "x=\"0\" "
              "y=\"0\" "
              "w=\"356\" "
              "h=\"613\">\n";
    stream << "\t\t<tab rows=\""+QString::number(vX1.size())+"\" cols=\"3\" labels=\"0\">\n";

    stream << "\t\t\t<col name=\""+nameColX+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"1\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vX1.size();i++) stream << QString::number(vX1.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\""+nameColY+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"2\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vY1.size();i++) stream << QString::number(vY1.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\"Err\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"5\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vErr1.size();i++) stream << QString::number(vErr1.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t</tab>\n";
    stream << "\t</window>\n";
    /* end LEFT plot table */

    /* RIGHT plot table */
    nameWindow = "Right";
    stream << "\t<window type=\"1\" "
              "name=\""+nameWindow+"\" "
              "date=\""+date+"\" "
              "x=\"356\" "
              "y=\"0\" "
              "w=\"356\" "
              "h=\"613\">\n";
    stream << "\t\t<tab rows=\""+QString::number(vX1.size())+"\" cols=\"3\" labels=\"0\">\n";

    stream << "\t\t\t<col name=\""+nameColX+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"1\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vX2.size();i++) stream << QString::number(vX2.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\""+nameColY+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"2\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vY2.size();i++) stream << QString::number(vY2.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\"Err\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"5\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vErr2.size();i++) stream << QString::number(vErr2.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t</tab>\n";
    stream << "\t</window>\n";
    /* end RIGHT plot table */

    /* AMOUNT plot table */
    nameWindow = "Amount";
    stream << "\t<window type=\"1\" "
              "name=\""+nameWindow+"\" "
              "date=\""+date+"\" "
              "x=\"712\" "
              "y=\"0\" "
              "w=\"356\" "
              "h=\"613\">\n";
    stream << "\t\t<tab rows=\""+QString::number(vX1.size())+"\" cols=\"3\" labels=\"0\">\n";

    stream << "\t\t\t<col name=\""+nameColX+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"1\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vXs.size();i++) stream << QString::number(vXs.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\""+nameColY+"\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"2\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vYs.size();i++) stream << QString::number(vYs.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t\t<col name=\"Err\" "
              "type=\"6\" "
              "num=\"0\" "
              "prec=\"8\" "
              "plot=\"5\" "
              "width=\"100\" "
              "stat=\"0\"\n";
    stream << "data=\"";
    for(int i=0;i<vErr_s.size();i++) stream << QString::number(vErr_s.at(i))+";";
    stream << "\"/>\n";

    stream << "\t\t</tab>\n";
    stream << "\t</window>\n";
    /* end AMOUNT plot table */


    stream << "</qti>\n";

    f.close();
    return;
}

void windowPlot::exportToTxt(QString filename){
    if(filename=="") return;
    if(filename.split('.').at(filename.split('.').size()-1)!="txt") filename += ".txt";
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream stream(&f);

    stream << "# amount files: [ ";
    for(int i=0;i<(sVar.amountFiles.size()-1);i++){
        stream << sVar.amountFiles.at(i) << ", ";
    }
    if(sVar.amountFiles.size() >0)stream << sVar.amountFiles.at(sVar.amountFiles.size()-1);
    stream << " ]\n";
    stream << "# background files: [ ";
    for(int i=0;i<(sVar.backgroundFiles.size()-1);i++){
        stream << sVar.backgroundFiles.at(i) << ", ";
    }
    if(sVar.backgroundFiles.size() >0) stream << sVar.backgroundFiles.at(sVar.backgroundFiles.size()-1);
    stream << " ]\n";

    stream << "# Ks = " << sVar.Ks << "pix, " << sVar.KsA << " 1/A\n";
    stream << "# \tx\ty\n";
    stream << "# KsL\t" << sVar.KsL_x << "\t" << sVar.KsL_y << "\n";
    stream << "# KsR\t" << sVar.KsR_x << "\t" << sVar.KsR_y << "\n\n";

    switch(unit_x){
    case UNITX_PIX:
        stream << "# units on x - pixels\n";
        break;
    case UNITX_Q_A:
        stream << "# units on x - Q [1/A]\n";
        break;
    case UNITX_Q_nm:
        stream << "# units on x - Q [1/nm]\n";
        break;
    }

    switch(unit_y){
    case UNITY_N:
        stream << "# units on y - counts [N]\n";
        break;
    case UNITY_Ns:
        stream << "# units on y - counts/time exposition [N/s]\n";
        break;
    }

    stream << "\n" " << " << QString::number(vXs.size()) << " points\n";
    stream << "# x\tLeft\tErr\tRight\tErr\tAmount\tErr\n";
    for(int i=0;i<vXs.size();i++){
        stream << vXs.at(i) << "\t" << vY1.at(i) << "\t" << vErr1.at(i) << "\t" << vY2.at(i) << "\t" << vErr2.at(i) << vYs.at(i) << "\t" << vErr_s.at(i) << "\n";
    }

    // Old format
    /*
    stream << "\n# Left (" << QString::number(vX1.size()) << " points)\n";
    stream << "# x\ty\terr\n";

    for(int i=0;i<vX1.size();i++){
        stream << vX1.at(i) << "\t" << vY1.at(i) << "\t" << vErr1.at(i) << "\n";
    }

    stream << "\n# Right (" << QString::number(vX2.size()) << " points)\n";
    stream << "# x\ty\terr\n";
    for(int i=0;i<vX2.size();i++){
        stream << vX2.at(i) << "\t" << vY2.at(i) << "\t" << vErr2.at(i) << "\n";
    }

    stream << "\n# Amount (" << QString::number(vXs.size()) << " points)\n";
    stream << "# x\ty\terr\n";
    for(int i=0;i<vXs.size();i++){
        stream << vXs.at(i) << "\t" << vYs.at(i) << "\t" << vErr_s.at(i) << "\n";
    }
    */

    f.close();
}

void windowPlot::slot_actionExportAll(){
    QString defname = QString::number(sVar.temperature)+"K_"+QString::number(sVar.magnetic_fild)+"T";
    QString filename = QFileDialog::getSaveFileName(this,"Export data",exportDirectory,"*");
    if(filename=="") return;
    exportDirectory="/";
    for(int i=0;i<filename.split("/").size()-1;i++){
        exportDirectory += filename.split("/").at(i)+"/";
    }
    qDebug () << exportDirectory;
    exportToTxt(filename);
    exportToQti(filename);
    exportToPNG(filename);
    return;
}

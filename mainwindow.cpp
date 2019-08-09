#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define DIVCOEFF

using namespace H5;

void sData::init(){
    data = new double * [x];
    for(int i=0;i<x;i++) data[i] = new double [y];
    clear();
    return;
}

void sData::clear(){
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            data[i][j] = 0.0;
        }
    }
}

void MainWindow::toCircle(double *x, double *y, double r, double phi){
    phi = 2*M_PI*phi/360;
    *x = r*cos(phi);
    *y = r*sin(phi);
    return;
}

void MainWindow::paintSector(iQCustomPlot *plot,
                             double x0, double y0,
                             double angle, double oangle, double offset, double len){

    double x,y;

    //QCPItemCurve *arc1 = new QCPItemCurve(plot);
    //arc1->setPen(QPen(QColor("white"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    QCPItemLine *line1 = new QCPItemLine(plot);
    line1->setPen(QPen(QColor("white"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    toCircle(&x,&y,offset,angle+oangle/2);
    line1->start->setCoords(x+x0,y+y0);
  //  arc1->start->setCoords(x+x0,y+y0);
  //  arc1->startDir->setCoords(x0,y0);
    toCircle(&x,&y,len,angle+oangle/2);
    line1->end->setCoords(x+x0,y+y0);

    QCPItemLine *line2 = new QCPItemLine(plot);
    line2->setPen(QPen(QColor("white"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    toCircle(&x,&y,offset,angle-oangle/2);
    line2->start->setCoords(x+x0,y+y0);
   // arc1->end->setCoords(x+x0,y+y0);
   // arc1->endDir->setCoords(x0,y0);
    toCircle(&x,&y,len,angle-oangle/2);
    line2->end->setCoords(x+x0,y+y0);


    return;
}

void MainWindow::paintCross(iQCustomPlot *plot, sData *data, double x, double y, double phi){
    double x1,x2;
    double y1,y2;

    double Mx2 = (double)(data->x-1)/2.0;
    double My2 = (double)(data->y-1)/2.0;

    x1 = My2*tan(2*M_PI*phi/360);
    x2 = Mx2-x1;

    y1 = -Mx2*tan(2*M_PI*phi/360);
    y2 = My2-y1;

    QCPItemLine *line1 = new QCPItemLine(plot);
    line1->setPen(QPen(QColor("black"),1,Qt::DashLine,Qt::SquareCap,Qt::BevelJoin));
    line1->start->setCoords(x1+x,y-My2);
    line1->end->setCoords(x2+x-Mx2,My2+y);

    QCPItemLine *line2 = new QCPItemLine(plot);
    line2->setPen(QPen(QColor("black"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    line2->start->setCoords(x-Mx2,y1+y);
    line2->end->setCoords(Mx2+x,y2+y-My2);

}

void MainWindow::paintSmallCross(iQCustomPlot *plot, sData *data, double x, double y){
     QCPItemLine *line1 = new QCPItemLine(plot);
     line1->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
     line1->start->setCoords(x,y+2);
     line1->end->setCoords(x,y-2);
     QCPItemLine *line2 = new QCPItemLine(plot);
     line2->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
     line2->start->setCoords(x+2,y);
     line2->end->setCoords(x-2,y);
}

int MainWindow::doubleToInt(double val){
    return (int)(val+0.5);
}

void MainWindow::average(sData *data,
                         double x0, double y0,
                         double angle, double oangle,
                         double offset, double len,
                         QVector<double> *vX,QVector<double> *vY,
                         QVector<double> *vErr,bool interp,int step){

    double x,y;
    int oldx=0,oldy=0;
    int ix,iy,n = 0;
    double r = offset,phi;
    double z = 0.0;
    double z2 = 0.0;

    double sigma;

    //double dr = 1;
    double dr = sqrt(2);
    double dphi = 0.1;
    dphi = atan(1.0/len);

    if(interp){
        dr = pow(2,step);
        dphi = atan(dr/len);
        if(step>1) dphi = atan(pow(2,ui->spinBoxStepInterpolatePricessing->value())/len);
    }

    vX->clear();
    vY->clear();
    vErr->clear();


    globalProgressBar->setMaximum(((int)len)-1);

    qDebug() << "dphi = " << dphi;
    qDebug() << "dr = " << dr;

    for(r=offset;r<len;r+=dr){
        for(phi=angle-oangle/2;phi<angle+oangle/2;phi+=dphi){
            toCircle(&x,&y,r,phi);
            //ix = (int) (x+x0);
            //iy = (int) (y+y0);

            ix = doubleToInt(x+x0);
            iy = doubleToInt(y+y0);

            if(!(ix==oldx && iy==oldy)){
                if((ix>0 && iy>0) && (ix<data->x && iy<data->y)){
                    z += data->data[ix][iy];
                    z2 += data->data[ix][iy]*data->data[ix][iy];
                }else{
                    qDebug () << "Out Range!!!";
                }
                /* Test Pixels */
                if(ui->checkBoxT4PaintPixels->isChecked()){
                    QCPItemEllipse *el = new QCPItemEllipse(plot2DProcessing);
                    el->setBrush(QBrush(QColor(0,255,0,150)));
                    el->setPen(QPen(Qt::NoPen));
                    el->topLeft->setCoords(ix+0.3,iy+0.3);
                    el->bottomRight->setCoords(ix-0.3,iy-0.3);
                }
                n++;

            }
            oldx = ix;
            oldy = iy;
        }
        sigma = sqrt((z*z-z2)/(double)n);
        //qDebug() << "CKO : " << sigma << " err: " << sqrt(z/(double)n);
        vY->append((double)z/n);
#ifdef DIVCOEFF
        vErr->append(sqrt(abs(z/(double)n))/sqrt(Parametrs.coeff));
        //vErr->append(sqrt(abs(Parametrs.coeff*z/(double)n)));
#else
        vErr->append(sqrt(abs(z/(double)n)));
        //vErr->append(sigma);
#endif
        //vErr->append(sqrt(sigma));
        vX->append(r);
        n=0;
        z=0.0;
        z2=0.0;
        globalProgressBar->setValue((int)r);
    }

    return;
}

void MainWindow::Interpolate(int type, sData *data_in, sData *data_out){
    data_out->x=data_in->x*2;
    data_out->y=data_in->y*2;
    data_out->init();

    for(int i=0;i<data_in->x;i++){
        for(int j=0;j<data_in->y;j++){
            data_out->data[i*2][j*2] = data_in->data[i][j];
        }
    }

    switch(type){
    case LINEAR_INTERPOLATE:
        for(int i=0;i<data_out->x-2;i+=2){
            for(int j=0;j<data_out->y-2;j+=2){

                data_out->data[i+1][j] = 0.5*(data_out->data[i][j]+data_out->data[i+2][j]);
                data_out->data[i+1][j+2] = 0.5*(data_out->data[i][j+2]+data_out->data[i+2][j+2]);


                data_out->data[i][j+1] = 0.5*(data_out->data[i][j]+data_out->data[i][j+2]);
                data_out->data[i+2][j+1] = 0.5*(data_out->data[i+2][j]+data_out->data[i+2][j+2]);



                data_out->data[i+1][j+1] = 0.25*(data_out->data[i+1][j]+data_out->data[i+1][j+2]+
                                           data_out->data[i][j+1]+data_out->data[i+2][j+1]);

            }
        }

        break;
    }

    return;
}

double MainWindow::getPixelCircle(sData *data, double r, double phi){
    double x,y;
    int ix,iy;
    toCircle(&x,&y,r,phi);

    ix = (int)((data->x/2-1)+x);
    iy = (int)((data->y/2-1)+y);
    return data->data[ix][iy];
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    spinBoxMaxScale = new QDoubleSpinBox;
    spinBoxMinScale = new QDoubleSpinBox;

    spinBoxMaxScale->setRange(-999999.9,999999.9);
    spinBoxMaxScale->setDecimals(6);
    spinBoxMinScale->setRange(-999999.9,999999.9);
    spinBoxMinScale->setDecimals(6);

    ui->mainToolBar->addWidget(spinBoxMinScale);
    ui->mainToolBar->addWidget(spinBoxMaxScale);
    ui->mainToolBar->addSeparator();

    globalProgressBar = new QProgressBar(ui->statusBar);
    globalProgressBar->setMaximumSize(100, 15);
    globalProgressBar->setAlignment(Qt::AlignRight);
    ui->statusBar->addPermanentWidget(globalProgressBar);

    wPlot = new windowPlot;

    connect(this,SIGNAL(signal_sendVariables(sWindowPlotVariables)),
            wPlot,SLOT(slot_loadVariables(sWindowPlotVariables)));

    /* TAB1 Explorer */

    plotExplorer = new iQCustomPlot;
    ui->layoutPlotExplorer->addWidget(plotExplorer);

    plotExplorer->axisRect()->setupFullAxesBox(true);
    //plotExplorer->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag);
    plotExplorer->yAxis->setLabel("y pixels");
    plotExplorer->xAxis->setLabel("x pixels");
    colorMapExplorer = new QCPColorMap(plotExplorer->xAxis,plotExplorer->yAxis);
    colorScale2DExplorer = new QCPColorScale(plotExplorer);
    colorScale2DExplorer->setType(QCPAxis::atRight);
    plotExplorer->plotLayout()->addElement(0,1,colorScale2DExplorer);

    colorMapExplorer->setColorScale(colorScale2DExplorer);
    colorMapExplorer->setGradient(QCPColorGradient::gpJet);

    colorScale2DExplorer->axis()->setLabel("Counts");

    setCentralWidget(ui->mainTabWidget);

    connect(ui->lineEditOpenExporer,SIGNAL(textEdited(QString)),
            this,SLOT(slot_edit_textOpenDirecotry(QString)));
    connect(ui->listWidgetExplprer,SIGNAL(currentTextChanged(QString)),
            this,SLOT(slot_clicked_in_ExplorerList(QString)));

    /* TAB2 Set parameters */

    ui->comboBoxSetParameters->addItem("value from file");
    ui->comboBoxSetParameters->addItem("D11");
    ui->comboBoxSetParameters->addItem("PA20");
    ui->comboBoxSetParameters->addItem("SANS-1 primary");
    ui->comboBoxSetParameters->addItem("SANS-1 secondary");

    connect(ui->comboBoxSetParameters,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_setsizeofpixel()));


    /* TAB3 Find Ks */

    plotKs2D = new iQCustomPlot;
    plotKs = new iQCustomPlot;

    //plotKs2D->yAxis->setLabel("y pixels");
    //plotKs2D->xAxis->setLabel("x pixels");
    colorMapKs2D = new QCPColorMap(plotKs2D->xAxis,plotKs2D->yAxis);
    colorScaleKs2D = new QCPColorScale(plotKs2D);
    colorScaleKs2D->setType(QCPAxis::atRight);
    plotKs2D->plotLayout()->addElement(0,1,colorScaleKs2D);

    colorMapKs2D->setColorScale(colorScaleKs2D);
    colorMapKs2D->setGradient(QCPColorGradient::gpJet);

    //colorScaleKs2D->axis()->setLabel("Counts");

    ui->layoutKsPlot2D->addWidget(plotKs2D);
    ui->layoutKsPlot->addWidget(plotKs);

    ui->comboBoxInterpolate->addItem("Bilinear");


    /* TAB4 Processing */

    ui->comboBoxInterpolateProcessing->addItem("Bilinear");
    plot2DProcessing = new iQCustomPlot;
    ui->layoutProcessing2DPlot->addWidget(plot2DProcessing);

    plot2DProcessing->axisRect()->setupFullAxesBox(true);
    //plotExplorer->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag);
    plot2DProcessing->yAxis->setLabel("y pixels");
    plot2DProcessing->xAxis->setLabel("x pixels");
    colorMapProcessing = new QCPColorMap(plot2DProcessing->xAxis,plot2DProcessing->yAxis);
    colorScale2DProcessing = new QCPColorScale(plot2DProcessing);
    colorScale2DProcessing->setType(QCPAxis::atRight);
    plot2DProcessing->plotLayout()->addElement(0,1,colorScale2DProcessing);

    colorMapProcessing->setColorScale(colorScale2DProcessing);
    colorMapProcessing->setGradient(QCPColorGradient::gpJet);

    colorScale2DProcessing->axis()->setLabel("Counts");

    globalRadius1 = ui->spinBoxT4offset_1->value();
    globalRadius2 = ui->spinBoxT4offset_2->value();

    connect(ui->spinBoxT4angle_1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4angle_2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4Len_1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4Len_2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4offset_1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4offset_2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4OpenAngle_1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4OpenAngle_2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4x1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4x2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4y1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4y2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateProcessingItems(double)));
    connect(ui->spinBoxT4offset_1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateGlobalRadius1(double)));
    connect(ui->spinBoxT4offset_2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_updateGlobalRadius2(double)));

    /* default */
    dataKs = new sData;
    dataKs->x=128;
    dataKs->y=128;
    dataKs->init();

    dataProcessing = new sData;
    dataProcessing->x=128;
    dataProcessing->y=128;
    dataProcessing->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonOpenExplorer_clicked()
{
    QString name;
    openFilesExplorer.clear();
    openFilesExplorer = QFileDialog::getOpenFileNames(this,"Open Files",workDirectory,"ILL (*.32);;"
                                                                      "SANS1 (*.001);;"
                                                                      "SANS1 (*.300);;"
                                                                      "Text files (*.txt);;"
                                                                      "NeXus HDF5 (*.nxs);;"
                                                                      "Theoretical (*.mxm);;"
                                                                      "All files (*)");
    if(openFilesExplorer.size()==0) return;
    workDirectory = "";
    name = openFilesExplorer.at(0);
    for(int i=1;i<name.split('/').size()-1;i++){
        workDirectory += "/"+name.split('/').at(i);
    }

    ui->lineEditOpenExporer->clear();
    ui->lineEditOpenExporer->setText(workDirectory);

    ui->listWidgetExplprer->clear();
    for(int i=0;i<openFilesExplorer.count();i++){
        name = openFilesExplorer.at(i);
        //name = name.split('/').at(name.split('/').size()-1);
        ui->listWidgetExplprer->addItem(name.split('/').at(name.split('/').size()-1));
    }
}


void MainWindow::ReadMyFormat(sData *data, QString filename){

    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) return;

    QString tmp;
    QStringList tmpList;
    int k = 0;

    QXmlStreamReader xml(&f);

    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();
        if(xml.isStartElement()){
            if(xml.name()=="Init"){
                data->x = xml.readElementText().toInt();
                data->y = data->x;
                data->init();
            }
            if(xml.name()=="MagneticFild"){
                Parametrs.magnetic_fild = xml.readElementText().toDouble();
            }
            if(xml.name()=="TimeOfExposition"){
                Parametrs.time_exposition = xml.readElementText().toDouble();
            }
            if(xml.name()=="ResolutionOfDetector"){
                Parametrs.size_of_pixel_mm = xml.readElementText().toDouble();
            }
            if(xml.name()=="DistDetectorSource"){
                Parametrs.long_sample_detector = xml.readElementText().toDouble();
            }
            if(xml.name()=="Monitor1"){
                Parametrs.monitor1 = xml.readElementText().toDouble();
            }
            if(xml.name()=="Lambda"){
                Parametrs.lambdaA = xml.readElementText().toDouble();
            }

            Parametrs.fromfileSumNeutron = 10;
            Parametrs.coeff = Parametrs.fromfileSumNeutron/Parametrs.time_exposition;

            if(xml.name()=="Data"){
                tmp = xml.readElementText();
                tmpList = tmp.split(';');
                k=0;
                for(int i=0;i<data->x;i++){
                    for(int j=0;j<data->y;j++){
                        data->data[i][j] = QString(tmpList.at(k)).toDouble();
                        k++;
                    }
                }

            }
        }
    }

    return;
}

void MainWindow::ReadILL(sData *data, QString filename){
    for(int i=0;i<filename.split('.').size()-1;i++){
        //xmlfile += filename.split('.').at(i);
        //if(i!=filename.split('.').size()-2) xmlfile+=".";
    }
    //xmlfile += ".xml";

    char *buffer = new char [0x10000];
    char *preBuff = new char [0x102];
    uint32_t var;
    int x=0,y=0;

    data->x=128;
    data->y=data->x;
    data->init();

    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    f.read(preBuff,0x100);
    //f.read(preBuff,0x102);
    //f.seek(0x102);
    f.read(buffer,0x10000);

    for(int i=0;i<0x10000;i+=4){
        var = (int) ((0xff&buffer[i+3])<<0x1c)|
                    ((0xff&buffer[i+2])<<0x0f)|
                    ((0xff&buffer[i+1])<<0x08)|
                    ((0xff&buffer[i] << 0x00));

        data->data[x][y] += (double) var;
        x++; if(x>127){x=0;y++;}
     }

    f.close();
    delete [] buffer;

    Parametrs.name = "";
    QString tmp ="";
    for(int i=0;i<0x18;i++) Parametrs.name += QString(preBuff[i]);
    for(int i=0x1e;i<0x26;i++) tmp += QString(preBuff[i]);
    Parametrs.monitor1 = tmp.toDouble();
    qDebug () << "monitor " << Parametrs.monitor1;
    tmp = "";
    for(int i=0x18;i<0x1e;i++) tmp += QString(preBuff[i]);
    Parametrs.time_exposition = tmp.toDouble();

    buffer = new char [0x102];
    QString parse;
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);
    while(!stream.atEnd()){
        parse = stream.readLine();
        QStringList pList = parse.split("=");
        if(pList.at(0)=="Resolution ") Parametrs.size_of_pixel_mm = QString(pList.at(1)).toDouble();
        if(pList.at(0)=="Lambda (lecture) ") Parametrs.lambdaA = QString(pList.at(1)).toDouble();
        if(pList.at(0)=="DetecteurPA_Z ") Parametrs.long_sample_detector = QString(pList.at(1)).toDouble()/1000;
        if(pList.at(0)=="temperature sample ") Parametrs.temperature = QString(pList.at(1)).toDouble();
        if(pList.at(0)=="magnetic field value ") Parametrs.magnetic_fild = QString(pList.at(1)).toDouble();
    }

    f.close();
    delete [] preBuff;

    Parametrs.coeff = Parametrs.monitor1/Parametrs.time_exposition;
    qDebug () << "coeff " << Parametrs.coeff;

    return;
}

void MainWindow::ReadNXS(sData *data, QString filename){
    int k=0;
    data->x = 128;
    data->y = 128;

    double coeff;

    int *tmp_data;

    /* read second txt file */
    QString secondName = filename.split(".nxs").at(0);

    qDebug () << secondName;
    QFile sf(secondName);
    sf.open(QIODevice::ReadOnly);
    QTextStream sstream(&sf);

    QString tmp;
    QVector<QStringList> sinfo;
    QStringList sform;

    int j=0;
    k=0;
    while(!sstream.atEnd()){
        k++;
        tmp = sstream.readLine();
        if(k > 60 && k<87){
            sinfo.append(tmp.split(" "));
            j++;
        }
    }
    sf.close();

    for(int i=0;i<j;i++){
        for(int l=0;l<sinfo[i].size();l++){
            if(sinfo[i].at(l)!="") sform.append(sinfo[i].at(l));
        }
    }

    Parametrs.time_exposition = QString(sform.at(2)).toDouble()/10;
    Parametrs.temperature = QString(sform.at(28)).toDouble();
    Parametrs.magnetic_fild = QString(sform.at(81)).toDouble();
    Parametrs.long_sample_detector = QString(sform.at(18)).toDouble();
    Parametrs.size_of_pixel_mm = QString(sform.at(54)).toDouble();
    Parametrs.lambdaA = QString(sform.at(52)).toDouble();
    Parametrs.monitor1 = QString(sform.at(4)).toDouble();
    Parametrs.fromfileSumNeutron =  QString(sform.at(3)).toDouble();

    //qDebug () << "size Pixel of detector " << Parametrs.size_of_pixel_mm;
    qDebug () << "waveleght " << Parametrs.lambdaA << " A";
    qDebug () << "summ" << Parametrs.fromfileSumNeutron;
    qDebug () << "monitor = " << Parametrs.monitor1;
    //coeff = Parametrs.fromfileSumNeutron/Parametrs.monitor1;
    //Parametrs.coeff = Parametrs.fromfileSumNeutron/Parametrs.time_exposition;
    Parametrs.coeff = Parametrs.monitor1/Parametrs.time_exposition;
    qDebug () << "coeff = " << Parametrs.coeff;

    /* Read NXS */
    k=0;
    const H5std_string FILE_NAME(filename.toStdString());
    const H5std_string DATASET_NAME("/entry0/D11/detector/data");
    //const H5std_string DATASET_NAME("IntArray");
    H5File f(FILE_NAME,H5F_ACC_RDONLY);
    DataSet dataset = f.openDataSet(DATASET_NAME);

    H5F_info2_t info;
    f.getFileInfo(info);

    qDebug () << f.getFileName().data();

    H5T_class_t type_class = dataset.getTypeClass();
    if(type_class == H5T_INTEGER){
        IntType inttype = dataset.getIntType();

        //qDebug () << inttype.getSize();
    }

    DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    hsize_t *dims_out = new hsize_t[rank];

    int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);

    if(rank>=2){
        data->x = (int) dims_out[0];
        data->y = (int) dims_out[1];
    }
    qDebug () << data->x << "x" << data->y;
    data->init();

    tmp_data = new int [data->x*data->y];

    dataset.read(tmp_data,PredType::NATIVE_INT);

    for(int i=0;i<data->x;i++){
        for(int j=0;j<data->y;j++){
#ifndef DIVCOEFF
            data->data[i][j] = tmp_data[k];
#else
            data->data[i][j] = tmp_data[k]/Parametrs.coeff;
#endif
            k++;
        }
    }

    delete [] tmp_data;
    f.close();

    return;
}

void MainWindow::ReadSANS1(sData *data, QString filename){
    enum{
        File,
        Sample,
        Setup,
        Counter,
        History,
        Comment,
        Counts
    };
    int segment;
    int x=0,y=0;
    bool readData = false;

    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);

    QString tmp;
    QString value;
    QStringList stringData;

    while(!stream.atEnd()){
        tmp = stream.readLine();

        if(tmp=="%File") segment=File;
        if(tmp=="%Sample") segment=Sample;
        if(tmp=="%Setup"){
            segment=Setup;
            data->init();
        }
        if(tmp=="%Counter") segment=Counter;
        if(tmp=="%History") segment=History;
        if(tmp=="%Comment") segment=Comment;
        if(tmp=="%Counts")  readData = true;

        switch(segment){
        case File:
            if(tmp.split('=').at(0)=="DataSizeX"){
                value=tmp.split('=').at(1);
                data->x=value.toInt();
            }
            if(tmp.split('=').at(0)=="DataSizeY"){
                value=tmp.split('=').at(1);
                data->y=value.toInt();
            }
            if(tmp.split('=').at(0)=="FileName"){
                value=tmp.split('=').at(1);
                Parametrs.name = value;
            }
            break;

        case Sample:
            if(data->x==0 && data->y==0){
                data->x=128; data->y=128;
            }
            if(tmp.split('=').at(0)=="SampleName"){
                value=tmp.split('=').at(1);
                Parametrs.sampleName = value;
            }
            if(tmp.split('=').at(0)=="Temperature"){
                value=tmp.split('=').at(1);
                Parametrs.temperature = value.toDouble();
            }
            if(tmp.split('=').at(0)=="Magnet"){
                value=tmp.split('=').at(1);
                Parametrs.magnetic_fild = value.toDouble();
            }
            break;

        case Counter:
            if(tmp.split('=').at(0)=="Time"){
                value=tmp.split('=').at(1);
                Parametrs.time_exposition = value.toDouble();

            }
            if(tmp.split('=').at(0)=="Moni1"){
                value=tmp.split('=').at(1);
                Parametrs.monitor1 = value.toDouble();
            }
            if(tmp.split('=').at(0)=="Moni2"){
                value=tmp.split('=').at(1);
                Parametrs.monitor2 = value.toDouble();
            }
            break;

        case Setup:
            if(tmp.split('=').at(0)=="Lambda"){
                value=tmp.split('=').at(1);
                Parametrs.lambdaA = value.toDouble();
                qDebug() << "Lambda: " << Parametrs.lambdaA << "A";
            }
            if(tmp.split('=').at(0)=="SD"){
                value=tmp.split('=').at(1);
                Parametrs.long_sample_detector = value.toDouble();
                qDebug() << "Source Detector: " << Parametrs.long_sample_detector  << "m";
            }
            break;
        }

        Parametrs.coeff = Parametrs.monitor1/Parametrs.time_exposition;

        if(readData){
            y++;
            stringData = tmp.split(',');
            for(x=0;x<stringData.size();x++){
#ifndef DIVCOEFF
                data->data[x][y-1] = QString(stringData.at(x)).toDouble();
#else
                data->data[x][y-1] = QString(stringData.at(x)).toDouble()/Parametrs.coeff;
#endif
            }
        }
    }

    f.close();
}

void MainWindow::ReadSANS1_300(sData *data, QString filename){
    enum{
        File,
        Sample,
        Setup,
        Counter,
        History,
        Comment,
        Counts,
        Errors
    };
    int segment;
    int x=0,y=0;
    int i=0,j=0,k=0;
    bool readData = false;

    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);

    QString tmp;
    QString value;
    QStringList stringData;

    while(!stream.atEnd()){
        tmp = stream.readLine();

        if(tmp=="%File") segment=File;
        if(tmp=="%Sample") segment=Sample;
        if(tmp=="%Setup"){
            segment=Setup;
            data->init();
        }
        if(tmp=="%Counter") segment=Counter;
        if(tmp=="%History") segment=History;
        if(tmp=="%Comment") segment=Comment;
        if(tmp=="%Counts")  readData = true;
        if(tmp=="%Errors"){
            segment=Errors;
            readData = false;
        }

        switch(segment){
        case File:
            if(tmp.split('=').at(0)=="DataSizeX"){
                value=tmp.split('=').at(1);
                data->x=value.toInt();
            }
            if(tmp.split('=').at(0)=="DataSizeY"){
                value=tmp.split('=').at(1);
                data->y=value.toInt();
            }
            if(tmp.split('=').at(0)=="FileName"){
                value=tmp.split('=').at(1);
                Parametrs.name = value;
            }
            break;

        case Sample:
            if(data->x==0 && data->y==0){
                data->x=128; data->y=128;
            }
            if(tmp.split('=').at(0)=="SampleName"){
                value=tmp.split('=').at(1);
                Parametrs.sampleName = value;
            }
            if(tmp.split('=').at(0)=="Temp4"){
                value=tmp.split('=').at(1);
                Parametrs.temperature = value.toDouble();
            }
            if(tmp.split('=').at(0)=="Magnet"){
                value=tmp.split('=').at(1);
                Parametrs.magnetic_fild = value.toDouble();
            }
            break;

        case Counter:
            if(tmp.split('=').at(0)=="Moni1"){
                value=tmp.split('=').at(1);
                Parametrs.monitor1 = value.toDouble();
            }
            if(tmp.split('=').at(0)=="Moni2"){
                value=tmp.split('=').at(1);
                Parametrs.monitor2 = value.toDouble();
            }
            break;
        case History:
            if(tmp.split('=').at(0)=="TotalTime"){
                value=tmp.split('=').at(1);
                Parametrs.time_exposition = value.toDouble();
            }
            if(tmp.split('=').at(0)=="TotalSum"){
                value=tmp.split('=').at(1);
                Parametrs.summ_all_neutrons = value.toDouble();
            }
            if(tmp.split('=').at(0)=="Scaling"){
                value=tmp.split('=').at(1);
                Parametrs.coeff = value.toDouble();
            }
            break;

        case Setup:
            if(tmp.split('=').at(0)=="Lambda"){
                value=tmp.split('=').at(1);
                Parametrs.lambdaA = value.toDouble();
                qDebug() << "Lambda: " << Parametrs.lambdaA << "A";
            }
            if(tmp.split('=').at(0)=="SD"){
                value=tmp.split('=').at(1);
                Parametrs.long_sample_detector = value.toDouble();
                qDebug() << "Source Detector: " << Parametrs.long_sample_detector  << "m";
            }
            break;
        }

        //Parametrs.coeff /= Parametrs.time_exposition;

        QString sTmp;

        if(readData){
            if(tmp=="%Counts") continue;
            k=0;
            for(int i=0;i<8;i++){
                for(int j=0;j<10;j++){
                    sTmp.append(tmp.at(k));
                    k++;
                }
#ifndef DIVCOEFF
                data->data[x][y] = sTmp.toDouble();
#else
                data->data[x][y] = sTmp.toDouble();
                //qDebug () << Parametrs.coeff;
#endif
                sTmp.clear();
                x++;
                if(x>127){
                    x=0;
                    y++;
                    if(y>127) break;
                }
            }
        }

    }


    return;
}

void MainWindow::ReadD11(sData *data, QString filename){

    QStringList tmpSplit;

    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);

    QString tmp;

    data->x = 128;
    data->y = 128;
    data->init();

    while(!stream.atEnd()){
        tmp = stream.readLine();
        tmpSplit = tmp.split('\t');
        qDebug () << tmpSplit.size();
    }

    f.close();
}

void MainWindow::PlotUpdate(QCustomPlot *plot, QCPColorMap *map, sData *data){
    map->data()->clear();
    plot->clearGraphs();
    map->data()->setSize(data->x,data->y);
    map->data()->setRange(QCPRange(0,data->x),QCPRange(0,data->y));

    for(int xIndex=0;xIndex<data->x;xIndex++){
        for(int yIndex=0;yIndex<data->y;yIndex++){
            map->data()->setCell(xIndex,yIndex,data->data[xIndex][yIndex]);
        }
    }

    map->rescaleDataRange();
    map->rescaleAxes(false);
    plot->rescaleAxes(false);
    plot->replot();
}

void MainWindow::updateStatusBar(){

    ui->statusBar->showMessage(Parametrs.name+": "+Parametrs.sampleName+
                                  " T="+QString::number(Parametrs.temperature)+" K,"
                                  " H="+QString::number(Parametrs.magnetic_fild)+" T,"
                                  " t="+QString::number(Parametrs.time_exposition)+" sec,"
                                  " N="+QString::number(Parametrs.summ_all_neutrons)+" count,"
                                  " L="+QString::number(Parametrs.long_sample_detector)+" m ", 0);

}


void MainWindow::LoadFilename(sData *data,QString filename){
    QString name = filename.split("/").at(filename.split("/").size()-1);
    if(name.split('.').at(1)=="32"){
        ReadILL(data,filename);
        return;
    }
    if(name.split('.').at(1)=="001"){
        ReadSANS1(data,filename);
        return;
    }
    if(name.split('.').at(1)=="300"){
        ReadSANS1_300(data,filename);
        return;
    }
    /*
    if(name.split('.').at(1)=="301"){
        ReadSANS1(data,filename);
        return;
    }
    if(name.split('.').at(1)=="302"){
        ReadSANS1(data,filename);
        return;
    }
    if(name.split('.').at(1)=="303"){
        ReadSANS1(data,filename);
        return;
    }
    */
    if(name.split('.').at(1)=="nxs"){
        ReadNXS(data,filename);
        return;
    }
    if(name.split('.').at(1)=="mxm"){
        ReadMyFormat(data,filename);
        return;
    }
    data->x=0;
    data->y=0;
    data->init();
    return;
}

void MainWindow::slot_edit_textOpenDirecotry(QString lineEdit){
    workDirectory = lineEdit;
}

void MainWindow::slot_clicked_in_ExplorerList(QString name){
    if(name=="") return;
    QString filename = workDirectory+"/"+name;

    sData *data = new sData;

    LoadFilename(data,filename);

    Parametrs.count_pix_x = data->x;
    Parametrs.count_pix_x = data->y;
    Parametrs.summ_all_neutrons = 0;

    plotExplorer->clearGraphs();
    colorMapExplorer->data()->clear();
    colorMapExplorer->data()->setSize(data->x,data->y);
    colorMapExplorer->data()->setRange(QCPRange(0,data->x),QCPRange(0,data->y));
    for(int xIndex=0;xIndex<data->x;xIndex++){
        for(int yIndex=0;yIndex<data->y;yIndex++){
            colorMapExplorer->data()->setCell(xIndex,yIndex,data->data[xIndex][yIndex]);
            Parametrs.summ_all_neutrons += (int)data->data[xIndex][yIndex];
        }
    }
    QCPMarginGroup *marginGroup = new QCPMarginGroup(plotExplorer);
    plotExplorer->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale2DExplorer->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    colorMapExplorer->rescaleDataRange();
    colorMapExplorer->rescaleAxes(true);
    plotExplorer->rescaleAxes(true);
    plotExplorer->replot();

    updateStatusBar();

    for(int i=0;i<data->x;i++) delete[] data->data[i];
    delete [] data;

    slot_setsizeofpixel();

    ui->spinBoxDistanceSD->setValue(Parametrs.long_sample_detector);
    ui->spinBoxWavelength->setValue(Parametrs.lambdaA);
}

void MainWindow::on_action_log_scale_triggered()
{
    if(ui->action_log_scale->isChecked()){
        if(ui->mainTabWidget->currentIndex()==0)
            colorMapExplorer->setDataScaleType(QCPAxis::stLogarithmic);
        if(ui->mainTabWidget->currentIndex()==2)
            colorMapKs2D->setDataScaleType(QCPAxis::stLogarithmic);
        if(ui->mainTabWidget->currentIndex()==3)
            colorMapProcessing->setDataScaleType(QCPAxis::stLogarithmic);
    }else{
        if(ui->mainTabWidget->currentIndex()==0)
            colorMapExplorer->setDataScaleType(QCPAxis::stLinear);
        if(ui->mainTabWidget->currentIndex()==2)
            colorMapKs2D->setDataScaleType(QCPAxis::stLinear);
        if(ui->mainTabWidget->currentIndex()==3)
            colorMapProcessing->setDataScaleType(QCPAxis::stLinear);
    }
    if(ui->mainTabWidget->currentIndex()==0)
        plotExplorer->replot();
    if(ui->mainTabWidget->currentIndex()==2)
        plotKs2D->replot();
    if(ui->mainTabWidget->currentIndex()==3)
        plot2DProcessing->replot();
}

void MainWindow::on_action_auto_scale_triggered()
{
    colorMapExplorer->rescaleDataRange();
    colorMapExplorer->rescaleAxes(true);
    plotExplorer->rescaleAxes(true);
    plotExplorer->replot();
    colorMapKs2D->rescaleDataRange();
    colorMapKs2D->rescaleAxes(true);
    plotKs2D->rescaleAxes(true);
    plotKs2D->replot();
    plot2DProcessing->rescaleAxes(true);
    plot2DProcessing->replot();
    colorMapProcessing->rescaleDataRange();
    colorMapProcessing->rescaleAxes(true);
}

void MainWindow::on_actionmanual_scale_triggered()
{
    colorMapExplorer->setDataRange(QCPRange(spinBoxMinScale->value(),
                                            spinBoxMaxScale->value()));
    colorMapKs2D->setDataRange(QCPRange(spinBoxMinScale->value(),
                                            spinBoxMaxScale->value()));
    colorMapProcessing->setDataRange(QCPRange(spinBoxMinScale->value(),
                                            spinBoxMaxScale->value()));

    plotExplorer->replot();
    plotKs2D->replot();
    plot2DProcessing->replot();
}

void MainWindow::on_buttonExplorerKsAdd_clicked()
{
    for(int i=0;i<ui->listWidgetExplprer->selectedItems().size();i++){
        ui->listWidgetKsAdd->addItem(ui->listWidgetExplprer->selectedItems().at(i)->text());
    }
}

void MainWindow::on_buttonExplorerKsRem_clicked()
{
    for(int i=0;i<ui->listWidgetExplprer->selectedItems().size();i++){
        ui->listWidgetKsRem->addItem(ui->listWidgetExplprer->selectedItems().at(i)->text());
    }
}

void MainWindow::on_buttonKsRemListSum_clicked()
{
    for(int i=0;i<ui->listWidgetKsAdd->count();i++){
        if(ui->listWidgetKsAdd->item(i)->isSelected())
            delete ui->listWidgetKsAdd->item(i);
    }
}

void MainWindow::on_buttonKsRemListRem_clicked()
{
    for(int i=0;i<ui->listWidgetKsRem->count();i++){
        if(ui->listWidgetKsRem->item(i)->isSelected())
            delete ui->listWidgetKsRem->item(i);
    }
}

void MainWindow::on_buttonKsSumm_clicked()
{
    dataKs->clear();
    QString filename;
    QString name;

    for(int l=0;l<ui->listWidgetKsAdd->count();l++){
        name = ui->listWidgetKsAdd->item(l)->text();
        if(name=="") continue;
        filename = workDirectory+"/"+name;

        sData *data = new sData;

        LoadFilename(data,filename);

        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataKs->data[i][j] += data->data[i][j];
            }
        }

        dataKs->x = data->x;
        dataKs->y = data->y;

        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    for(int l=0;l<ui->listWidgetKsRem->count();l++){
        name = ui->listWidgetKsRem->item(l)->text();
        if(name=="") continue;
        filename = workDirectory+"/"+name;

        sData *data = new sData;

        LoadFilename(data,filename);

        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataKs->data[i][j] -= data->data[i][j];
            }
        }

        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    /*
    for(int step=0;step<ui->spinBoxStepInterpolate->value();step++){
        sData *data = new sData;
        Interpolate(ui->comboBoxInterpolate->currentIndex(),dataKs,data);
        for(int i=0;i<dataKs->x;i++) delete[] dataKs->data[i];
        delete [] dataKs;

        dataKs = new sData;
        dataKs->x = data->x;
        dataKs->y = data->y;
        dataKs->init();
        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataKs->data[i][j] = data->data[i][j];
            }
        }
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }
    */

    for(int step=0;step<ui->spinBoxStepInterpolate->value();step++){
        sData *data = new sData;
        Interpolate(ui->comboBoxInterpolate->currentIndex(),dataKs,data);
        globalProgressBar->setMaximum(data->x-2);
        for(int i=0;i<dataKs->x;i++) delete[] dataKs->data[i];
        delete [] dataKs;

        dataKs = new sData;
        dataKs->x = data->x;
        dataKs->y = data->y;
        dataKs->init();
        for(int i=1;i<data->x;i++){
            for(int j=1;j<data->y;j++){
                dataKs->data[i][j] = data->data[i-1][j-1];
            }
            globalProgressBar->setValue(i);
        }
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    double uiCx,uiCy;

    if(globalCx==0.0){
        ui->SpinBoxCenterX->setValue((double)(dataKs->x-1)/2.0);
    }else{
        uiCx = globalCx;
        for(int i=0;i<ui->spinBoxStepInterpolate->value();i++){
            uiCx *= 2;
        }
        ui->SpinBoxCenterX->setValue(uiCx);
    }
    if(globalCy==0.0){
        ui->SpinBoxCenterY->setValue((double)(dataKs->y-1)/2.0);
    }else{
        uiCy = globalCx;
        for(int i=0;i<ui->spinBoxStepInterpolate->value();i++){
            uiCy *= 2;
        }
        ui->SpinBoxCenterY->setValue(globalCy*pow(2,ui->spinBoxStepInterpolate->value()));
    }


    plotKs2D->clearItems();
    paintCross(plotKs2D,dataKs,ui->SpinBoxCenterX->value(),
               ui->SpinBoxCenterY->value(),
               ui->SpinBoxAngleCenter->value());
    PlotUpdate(plotKs2D,colorMapKs2D,dataKs);

    ui->spinBoxKsWhdth->setMaximum(dataKs->y);
    //ui->spinBoxKsWhdth->setValue(ui->spinBoxKsWhdth->value()*pow(2,ui->spinBoxStepInterpolate));


}

void MainWindow::on_buttonKsAuto_clicked()
{
    double x,y,z;
    double x1,y1,x2,y2;
    double dy = 1.0;
    //qDebug() << getPixelCircle(dataKs,22,10);
    globalKs = tmpKs/pow(2,ui->spinBoxStepInterpolate->value());
    ui->spinBoxKs->setValue(tmpKs);
    qDebug() << "Ks = " << globalKs << "pix (non interpolate)";

    toCircle(&x,&y,tmpKs,180+ui->SpinBoxAngleCenter->value());
    x1 = x+ui->SpinBoxCenterX->value(); y1 = y+ui->SpinBoxCenterY->value();
    toCircle(&x,&y,tmpKs,ui->SpinBoxAngleCenter->value());
    x2 = x+ui->SpinBoxCenterX->value(); y2 = y+ui->SpinBoxCenterY->value();

    /*
    paintSmallCross(plotKs2D,dataKs,x1,y1);
    paintSmallCross(plotKs2D,dataKs,x2,y2);
    PlotUpdate(plotKs2D,colorMapKs2D,dataKs);
    */

    QVector<double> vTmpX,vTmpY;
    double maxY,minY,MHD_X,MHH_X,MH;

    vTmpX.clear();vTmpY.clear();

    /* Find y for Ks1*/
    for(y = 0;y<dataKs->y;y+=dy){
        vTmpX.append(y);
        z = dataKs->data[(int)x1][(int)y];
        vTmpY.append(z);
    }

    maxY = vTmpY.at(0);
    minY = maxY;
    for(int i=0;i<vTmpY.size();i++){
        if(vTmpY.at(i) > maxY) maxY = vTmpY.at(i);
        if(vTmpY.at(i) < minY) minY = vTmpY.at(i);
    }
    MH = (maxY-minY)/2; // полувысота
    for(int i=1;i<vTmpY.size();i++){
        if(vTmpY.at(i-1) <= MH && vTmpY.at(i) >= MH) MHD_X = vTmpX.at(i); // полувысота снизу
        if(vTmpY.at(i-1) >= MH && vTmpY.at(i) <= MH) MHH_X = vTmpX.at(i); // полувысота сверху
    }

    y1 = MHD_X+(MHH_X-MHD_X)/2;

    /* Find y for Ks2*/
    vTmpX.clear();vTmpY.clear();
    for(y = 0;y<dataKs->y;y+=dy){
        vTmpX.append(y);
        z = dataKs->data[(int)x2][(int)y];
        vTmpY.append(z);
    }

    maxY = vTmpY.at(0);
    minY = maxY;
    for(int i=0;i<vTmpY.size();i++){
        if(vTmpY.at(i) > maxY) maxY = vTmpY.at(i);
        if(vTmpY.at(i) < minY) minY = vTmpY.at(i);
    }
    MH = (maxY-minY)/2; // полувысота
    for(int i=1;i<vTmpY.size();i++){
        if(vTmpY.at(i-1) <= MH && vTmpY.at(i) >= MH) MHD_X = vTmpX.at(i); // полувысота снизу
        if(vTmpY.at(i-1) >= MH && vTmpY.at(i) <= MH) MHH_X = vTmpX.at(i); // полувысота сверху
    }

    y2 = MHD_X+(MHH_X-MHD_X)/2;

    ui->spinBoxKs1_x->setValue(x1);
    ui->spinBoxKs1_y->setValue(y1);
    ui->spinBoxKs2_x->setValue(x2);
    ui->spinBoxKs2_y->setValue(y2);

    ui->SpinBoxCenterY->setValue((y1+y2)/2);
    ui->SpinBoxAngleCenter->setValue(360*atan(((y2-y1)/2)/(x2-ui->SpinBoxCenterX->value()))/2/M_PI);

    /* NEW find Ks */
    /*
    tmpKs = (sqrt(x1*x1+y1*y2)-sqrt(x2*x2+y2*y2))/2;
    globalKs = tmpKs/pow(2,ui->spinBoxStepInterpolate->value());
    ui->spinBoxKs->setValue(tmpKs);
    qDebug() << "Ks = " << globalKs << "pix (non interpolate)";
    */
    qDebug () << " NEW Ks (pix): " << (sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))/2/pow(2,ui->spinBoxStepInterpolate->value());
    qDebug () << " NEW Ks = " << 2*M_PI*atan((sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)))/2/pow(2,ui->spinBoxStepInterpolate->value())*Parametrs.size_of_pixel_mm/1000/
                         Parametrs.long_sample_detector)/Parametrs.lambdaA
                         << " 1/A";

    plotKs2D->clearItems();
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs1_x->value(),ui->spinBoxKs1_y->value());
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs2_x->value(),ui->spinBoxKs2_y->value());
    UPDATE_CROSS

}

void MainWindow::on_buttonKsCut_clicked()
{
    double y=0,x=0,z=0;

    QVector<double> iXR,iYR;
    QVector<double> iXL,iYL;

    double r,phi = ui->SpinBoxAngleCenter->value();
    double dr = 1.0;
    double rMax = sqrt(dataKs->x*dataKs->x+dataKs->y*dataKs->y);
    double dAverage = 0.0;
    double nAverage = 1.0;


    /*
    for(r=0;r<rMax;r+=dr){
        toCircle(&x,&y,r,phi);
        x += ui->SpinBoxCenterX->value();
        y += ui->SpinBoxCenterY->value();
        if(x >= dataKs->x || x<0) break;
        if(y >= dataKs->y || y<0) break;
        z=dataKs->data[(int)x][(int)y];
        x=0;y=0;
        iYR.append(z);
        iXR.append(r);
    }

    for(r=0;r<rMax;r+=dr){
        toCircle(&x,&y,r,180+phi);
        x += ui->SpinBoxCenterX->value();
        y += ui->SpinBoxCenterY->value();
        if(x >= dataKs->x || x<0) break;
        if(y >= dataKs->y || y<0) break;
        z=dataKs->data[(int)x][(int)y];
        x=0;y=0;
        iYL.append(z);
        iXL.append(r);
    }
    */

    dAverage = (ui->spinBoxKsWhdth->value()-1)/2;

    for(r=0;r<rMax;r+=dr){
        nAverage = 1;
        toCircle(&x,&y,r,phi);
        x += ui->SpinBoxCenterX->value();
        y += ui->SpinBoxCenterY->value();
        if(x >= dataKs->x || x<0) break;
        if(y >= dataKs->y || y<0) break;

        z=dataKs->data[(int)x][(int)y];
        //z=dataKs->data[doubleToInt(x)][doubleToInt(y)];
        for(int i=1;i<=dAverage;i++){
            z+=dataKs->data[(int)x][(int)(y+i)];
            z+=dataKs->data[(int)x][(int)(y-i)];
            //z+=dataKs->data[doubleToInt(x)][doubleToInt(y+i)];
            //z+=dataKs->data[doubleToInt(x)][doubleToInt(y-i)];
            nAverage+=2;
        }
        x=0;y=0;
        iYR.append(z/nAverage);
        iXR.append(r);
    }

    for(r=0;r<rMax;r+=dr){
        nAverage = 1;
        toCircle(&x,&y,r,180+phi);
        x += ui->SpinBoxCenterX->value();
        y += ui->SpinBoxCenterY->value();
        if(x >= dataKs->x || x<0) break;
        if(y >= dataKs->y || y<0) break;
        z=dataKs->data[(int)x][(int)y];
        //z=dataKs->data[doubleToInt(x)][doubleToInt(y)];
        for(int i=1;i<=dAverage;i++){
            z+=dataKs->data[(int)x][(int)(y+i)];
            z+=dataKs->data[(int)x][(int)(y-i)];
            //z+=dataKs->data[doubleToInt(x)][doubleToInt(y+i)];
            //z+=dataKs->data[doubleToInt(x)][doubleToInt(y-i)];
            nAverage+=2;
        }
        x=0;y=0;
        iYL.append(z/nAverage);
        iXL.append(r);
    }

    double MaxL=0,MaxR=0;
    double MaxLX,MaxRX;
    double tmpMaxY;
    int index_maximum_R = 0;
    int index_maximum_L = 0;

    double HMRR_Y,HMRL_Y;
    double HMLR_Y,HMLL_Y;
    double HMRR_X,HMRL_X;
    double HMLR_X,HMLL_X;
    double MHMR_X,MHML_X;

    for(int i=0;i<iXR.size();i++){
        if(MaxR<iYR.at(i)){
            MaxR = iYR.at(i);
            MaxRX = iXR.at(i);
            index_maximum_R = i;
        }
    }
    for(int i=0;i<iXL.size();i++){
        if(MaxL<iYL.at(i)){
            MaxL = iYL.at(i);
            MaxLX = iXL.at(i);
            index_maximum_L = i;
        }
    }

    if(MaxL > MaxR){
        tmpMaxY = MaxL;
    }else{
        tmpMaxY = MaxR;
    }

    HMRR_Y = MaxR/2;
    HMRL_Y = HMRR_Y;

    HMLR_Y = MaxL/2;
    HMLL_Y = HMLR_Y;


    /* findning Half maximum Right plot to left */
    for(int i=index_maximum_R;i!=1;i--){
        if(iYR.at(i-1) <= HMRL_Y && iYR.at(i) >= HMRL_Y){
            HMRL_X = iXR.at(i);
            break;
        }
    }


    /* finding Half maximum Right plot to right */
    for(int i=index_maximum_R;i<iXR.size()-1;i++){
        if(iYR.at(i+1) <= HMRR_Y && iYR.at(i+1) <= HMRR_Y){
            HMRR_X = iXR.at(i);
            break;
        }
    }

    /* findning Half maximum Left plot to left */
    for(int i=index_maximum_L;i!=1;i--){
        if(iYL.at(i-1) <= HMLL_Y && iYL.at(i) >= HMLL_Y){
            HMLL_X = iXL.at(i);
            break;
        }
    }

    /* finding Half maximum Left plot to right */
    for(int i=index_maximum_L;i<iXL.size()-1;i++){
        if(iYL.at(i+1) <= HMLR_Y && iYL.at(i+1) <= HMLR_Y){
            HMLR_X = iXL.at(i);
            break;
        }
    }


    plotKs->clearItems();

    QCPItemLine *LineR = new QCPItemLine(plotKs);
    QCPItemLine *LineL = new QCPItemLine(plotKs);
    QCPItemText *textR = new QCPItemText(plotKs);
    QCPItemText *textL = new QCPItemText(plotKs);

    QCPItemLine *LineHMR_R = new QCPItemLine(plotKs);
    QCPItemLine *LineHMR_L = new QCPItemLine(plotKs);

    LineHMR_R->start->setCoords(HMRR_X,0);
    LineHMR_R->end->setCoords(HMRR_X,tmpMaxY);
    LineHMR_L->start->setCoords(HMRL_X,0);
    LineHMR_L->end->setCoords(HMRL_X,tmpMaxY);
    LineHMR_R->setPen(QPen(QColor("red"),1,Qt::DashLine,Qt::SquareCap,Qt::BevelJoin));
    LineHMR_L->setPen(QPen(QColor("red"),1,Qt::DashLine,Qt::SquareCap,Qt::BevelJoin));

    QCPItemLine *LineHML_R = new QCPItemLine(plotKs);
    QCPItemLine *LineHML_L = new QCPItemLine(plotKs);

    LineHML_R->start->setCoords(HMLR_X,0);
    LineHML_R->end->setCoords(HMLR_X,tmpMaxY);
    LineHML_L->start->setCoords(HMLL_X,0);
    LineHML_L->end->setCoords(HMLL_X,tmpMaxY);
    LineHML_R->setPen(QPen(QColor("green"),1,Qt::DashLine,Qt::SquareCap,Qt::BevelJoin));
    LineHML_L->setPen(QPen(QColor("green"),1,Qt::DashLine,Qt::SquareCap,Qt::BevelJoin));


    LineR->setPen(QPen(Qt::red));
    LineL->setPen(QPen(Qt::green));

    LineR->start->setCoords(MaxRX,0);
    LineR->end->setCoords(MaxRX,tmpMaxY);
    LineL->start->setCoords(MaxLX,0);
    LineL->end->setCoords(MaxLX,tmpMaxY);

    textR->position->setCoords(MaxRX,MaxR);
    textR->setText(QString::number(MaxR));
    textR->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    //textR->setPen(QPen(Qt::red));
    textL->position->setCoords(MaxLX,MaxL);
    textL->setText(QString::number(MaxL));
    textL->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    //textL->setPen(QPen(Qt::green));

    plotKs->clearGraphs();
    plotKs->addCurve(&iXR,&iYR,true,"red","Right");
    plotKs->addCurve(&iXL,&iYL,true,"green","Left");
    plotKs->yAxis->scaleRange(1.2);
    plotKs->replot();

    /* on maximum */
    /*
    if(MaxRX > MaxLX){
        tmpKs = (((MaxRX-MaxLX)/2) + MaxLX);
    }else{
        tmpKs = (((MaxLX-MaxRX)/2) + MaxRX);
    }
    */

    /* on HM */
    MHMR_X = (HMRR_X-HMRL_X)/2 + HMRL_X;
    MHML_X = (HMLR_X-HMLL_X)/2 + HMLL_X;
    if(MHMR_X > MHML_X){
        ui->SpinBoxCenterX->setValue(ui->SpinBoxCenterX->value()+((MHMR_X-MHML_X)/2));
        tmpKs = (((MHMR_X-MHML_X)/2) + MHML_X);
    }else{
        ui->SpinBoxCenterX->setValue(ui->SpinBoxCenterX->value()-((MHML_X-MHMR_X)/2));
        tmpKs = (((MHML_X-MHMR_X)/2) + MHMR_X);
    }

    qDebug () << "Tmp Ks: " << tmpKs;

    plotKs2D->clearItems();
    UPDATE_CROSS
}

void MainWindow::on_buttonKsSetCenter_clicked()
{
    plotKs2D->clearItems();
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs1_x->value(),ui->spinBoxKs1_y->value());
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs2_x->value(),ui->spinBoxKs2_y->value());
    UPDATE_CROSS

    globalCx = ui->SpinBoxCenterX->value()/
            pow(2,ui->spinBoxStepInterpolate->value());
    globalCy = ui->SpinBoxCenterY->value()/
            pow(2,ui->spinBoxStepInterpolate->value());
    globalAngleC = ui->SpinBoxAngleCenter->value();
    qDebug() << globalCx << " " << globalCy << " " << globalAngleC;
}

void MainWindow::on_buttonSetKs_clicked()
{
    globalKs1_x = ui->spinBoxKs1_x->value()/pow(2,ui->spinBoxStepInterpolate->value());
    globalKs1_y = ui->spinBoxKs1_y->value()/pow(2,ui->spinBoxStepInterpolate->value());
    globalKs2_x = ui->spinBoxKs2_x->value()/pow(2,ui->spinBoxStepInterpolate->value());
    globalKs2_y = ui->spinBoxKs2_y->value()/pow(2,ui->spinBoxStepInterpolate->value());

    /*
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs1_x->value(),ui->spinBoxKs1_y->value());
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs2_x->value(),ui->spinBoxKs2_y->value());
    PlotUpdate(plotKs2D,colorMapKs2D,dataKs);
    */
    plotKs2D->clearItems();
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs1_x->value(),ui->spinBoxKs1_y->value());
    paintSmallCross(plotKs2D,dataKs,ui->spinBoxKs2_x->value(),ui->spinBoxKs2_y->value());
    UPDATE_CROSS

    qDebug () << "KsR_x: " << globalKs1_x << "KsR_y: " << globalKs1_y;
    qDebug () << "KsL_x: " << globalKs2_x << "KsL_y: " << globalKs2_y;
    pKs1_x = globalKs1_x; pKs2_x = globalKs2_x;
    pKs1_y = globalKs1_y; pKs2_y = globalKs2_y;

    double minKsx=globalKs1_x;
    if(globalKs2_x < minKsx) minKsx = globalKs2_x;

    ui->spinBoxT4x1->setValue(globalKs1_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4x2->setValue(globalKs2_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y1->setValue(globalKs1_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y2->setValue(globalKs2_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4angle_1->setValue(180+ui->SpinBoxAngleCenter->value());
    ui->spinBoxT4angle_2->setValue(ui->SpinBoxAngleCenter->value());

    ui->spinBoxT4Len_1->setValue(minKsx*pow(2,ui->spinBoxStepInterpolatePricessing->value())-1);
    ui->spinBoxT4Len_2->setValue(minKsx*pow(2,ui->spinBoxStepInterpolatePricessing->value())-1);
}

void MainWindow::on_buttonKsOk_clicked()
{
    globalKs = ui->spinBoxKs->value()/pow(2,ui->spinBoxStepInterpolate->value());
    qDebug () << "set Ks = " << globalKs << "pix (non interpolate)";

    double x,y,x1,x2,y1,y2;

    toCircle(&x,&y,ui->spinBoxKs->value(),180+ui->SpinBoxAngleCenter->value());
    x1 = x+ui->SpinBoxCenterX->value(); y1 = y+ui->SpinBoxCenterY->value();
    toCircle(&x,&y,ui->spinBoxKs->value(),ui->SpinBoxAngleCenter->value());
    x2 = x+ui->SpinBoxCenterX->value(); y2 = y+ui->SpinBoxCenterY->value();

    ui->spinBoxKs1_x->setValue(x1);
    ui->spinBoxKs1_y->setValue(y1);
    ui->spinBoxKs2_x->setValue(x2);
    ui->spinBoxKs2_y->setValue(y2);



    ui->labelKs->setText(QString::number(2*M_PI*atan(globalKs*Parametrs.size_of_pixel_mm/1000/
                                         Parametrs.long_sample_detector)/Parametrs.lambdaA)+
                                         " 1/A");

    qDebug () << "Ks = " << 2*M_PI*atan(globalKs*Parametrs.size_of_pixel_mm/1000/
                         Parametrs.long_sample_detector)/Parametrs.lambdaA
                         << " 1/A";


    plotKs2D->clearItems();
    paintSmallCross(plotKs2D,dataKs,x1,y1);
    paintSmallCross(plotKs2D,dataKs,x2,y2);
    UPDATE_CROSS
}

void MainWindow::on_buttonExplorerAdd_clicked()
{
    for(int i=0;i<ui->listWidgetExplprer->selectedItems().size();i++){
        ui->listProcessingAdd->addItem(ui->listWidgetExplprer->selectedItems().at(i)->text());
    }
}

void MainWindow::on_buttonExplorerRem_clicked()
{
    for(int i=0;i<ui->listWidgetExplprer->selectedItems().size();i++){
        ui->listProcessingRem->addItem(ui->listWidgetExplprer->selectedItems().at(i)->text());
    }
}

void MainWindow::on_buttonProcessingLoad_clicked()
{
    dataProcessing->clear();

    QString filename;
    QString name;

    sData *data = new sData;
    name = ui->listProcessingAdd->item(0)->text();
    filename = workDirectory+"/"+name;
    LoadFilename(data,filename);
    dataProcessing->x = data->x;
    dataProcessing->y = data->y;
    dataProcessing->init();
    delete [] data;


    for(int l=0;l<ui->listProcessingAdd->count();l++){
        name = ui->listProcessingAdd->item(l)->text();
        filename = workDirectory+"/"+name;
        data = new sData;
        LoadFilename(data,filename);
        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataProcessing->data[i][j] += data->data[i][j];
            }
        }
        dataProcessing->x = data->x;
        dataProcessing->y = data->y;
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }
    for(int l=0;l<ui->listProcessingRem->count();l++){
        name = ui->listProcessingRem->item(l)->text();
        filename = workDirectory+"/"+name;
        sData *data = new sData;
        LoadFilename(data,filename);
        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataProcessing->data[i][j] -= data->data[i][j];
            }
        }
        dataProcessing->x = data->x;
        dataProcessing->y = data->y;
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    slot_updateProcessingItems(0.0);
    PlotUpdate(plot2DProcessing,colorMapProcessing,dataProcessing);
    processingInterpolate = 0;
}

void MainWindow::on_buttonInterpolateProcessing_clicked()
{

    if(!ui->checkBoxT4_func->isChecked()){
        on_buttonProcessingLoad_clicked();
    }else{
        on_buttonProcessingClearRem_func_clicked();
    }

    pKs1_x = ui->spinBoxT4x1->value()/pow(2,pInterpolate);
    pKs2_x = ui->spinBoxT4x2->value()/pow(2,pInterpolate);
    pKs1_y = ui->spinBoxT4y1->value()/pow(2,pInterpolate);
    pKs2_y = ui->spinBoxT4y2->value()/pow(2,pInterpolate);
    qDebug () << pKs1_x << " pI:" << pInterpolate;

    for(int step=0;step<ui->spinBoxStepInterpolatePricessing->value();step++){

        sData *data = new sData;
        Interpolate(ui->comboBoxInterpolateProcessing->currentIndex(),dataProcessing,data);
        globalProgressBar->setMaximum(data->x-2);
        for(int i=0;i<dataProcessing->x;i++) delete[] dataProcessing->data[i];
        delete [] dataProcessing;

        dataProcessing = new sData;
        dataProcessing->x = data->x;
        dataProcessing->y = data->y;
        dataProcessing->init();
        for(int i=1;i<data->x;i++){
            for(int j=1;j<data->y;j++){
                dataProcessing->data[i][j] = data->data[i-1][j-1];
            }
            globalProgressBar->setValue(i);
        }
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    double minKsx=globalKs1_x;
    if(globalKs2_x < minKsx) minKsx = globalKs2_x;

    pInterpolate = ui->spinBoxStepInterpolatePricessing->value();
    ui->spinBoxT4Len_1->setValue(minKsx*pow(2,ui->spinBoxStepInterpolatePricessing->value())-1);
    ui->spinBoxT4Len_2->setValue(minKsx*pow(2,ui->spinBoxStepInterpolatePricessing->value())-1);
    /*
    ui->spinBoxT4x1->setValue(globalKs1_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4x2->setValue(globalKs2_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y1->setValue(globalKs1_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y2->setValue(globalKs2_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    */
    ui->spinBoxT4x1->setValue(pKs1_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4x2->setValue(pKs2_x*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y1->setValue(pKs1_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4y2->setValue(pKs2_y*pow(2,ui->spinBoxStepInterpolatePricessing->value()));

    ui->spinBoxT4offset_1->setValue(globalRadius1*pow(2,ui->spinBoxStepInterpolatePricessing->value()));
    ui->spinBoxT4offset_2->setValue(globalRadius2*pow(2,ui->spinBoxStepInterpolatePricessing->value()));


    processingInterpolate = ui->spinBoxStepInterpolatePricessing->value();

    slot_updateProcessingItems(0.0);
    PlotUpdate(plot2DProcessing,colorMapProcessing,dataProcessing);
}

void MainWindow::on_buttonProcessingAddListRem_clicked()
{
    for(int i=0;i<ui->listProcessingAdd->count();i++){
        if(ui->listProcessingAdd->item(i)->isSelected())
            delete ui->listProcessingAdd->item(i);
    }
}

void MainWindow::on_buttonProcessingRemListRem_clicked()
{
    for(int i=0;i<ui->listProcessingRem->count();i++){
        if(ui->listProcessingRem->item(i)->isSelected())
            delete ui->listProcessingRem->item(i);
    }
}

void MainWindow::on_buttonProcessingClearRem_clicked(){ui->listProcessingRem->clear();}
void MainWindow::on_buttonProcessingClearAdd_clicked(){ui->listProcessingAdd->clear();}

void MainWindow::on_buttonKsClearAdd_clicked(){ui->listWidgetKsAdd->clear();}
void MainWindow::on_buttonKsClearRem_clicked(){ui->listWidgetKsRem->clear();}

void MainWindow::on_buttonSetParameters_clicked()
{
    Parametrs.size_of_pixel_mm = ui->spinBoxSizePixel->value();
    Parametrs.lambdaA = ui->spinBoxWavelength->value();
    Parametrs.long_sample_detector = ui->spinBoxDistanceSD->value();
}

void MainWindow::on_buttonProcessAverage_clicked()
{
    slot_updateProcessingItems(0.0);

    average(dataProcessing,ui->spinBoxT4x1->value(),ui->spinBoxT4y1->value(),
            ui->spinBoxT4angle_1->value(),ui->spinBoxT4OpenAngle_1->value(),
            ui->spinBoxT4offset_1->value(),ui->spinBoxT4Len_1->value(),&vAveX1,&vAveI1,&vAveErr1,
            ui->checkBoxT4NonInterpolate->isChecked(),ui->spinBoxStepInterpolatePricessing->value());

    average(dataProcessing,ui->spinBoxT4x2->value(),ui->spinBoxT4y2->value(),
            ui->spinBoxT4angle_2->value(),ui->spinBoxT4OpenAngle_2->value(),
            ui->spinBoxT4offset_2->value(),ui->spinBoxT4Len_2->value(),&vAveX2,&vAveI2,&vAveErr2,
            ui->checkBoxT4NonInterpolate->isChecked(),ui->spinBoxStepInterpolatePricessing->value());

    plot2DProcessing->update();
    plot2DProcessing->repaint();
    plot2DProcessing->replot();

    sWindowPlotVariables sWinPlotVar;
    sWinPlotVar.vX1 = &vAveX1;
    sWinPlotVar.vY1 = &vAveI1;
    sWinPlotVar.vX2 = &vAveX2;
    sWinPlotVar.vY2 = &vAveI2;
    sWinPlotVar.vErr1 = &vAveErr1;
    sWinPlotVar.vErr2 = &vAveErr2;
    sWinPlotVar.stepInterpolate = ui->spinBoxStepInterpolatePricessing->value();
    sWinPlotVar.Ks = globalKs;
    sWinPlotVar.KsA = 2*M_PI*atan(globalKs*Parametrs.size_of_pixel_mm/1000/
                                  Parametrs.long_sample_detector)/Parametrs.lambdaA;
    sWinPlotVar.time = Parametrs.time_exposition;
    sWinPlotVar.dist_source_detector = Parametrs.long_sample_detector;
    sWinPlotVar.size_of_pixel_mm = Parametrs.size_of_pixel_mm;
    sWinPlotVar.lambdaA = Parametrs.lambdaA;

    for(int i=0;i<ui->listProcessingAdd->count();i++)
        sWinPlotVar.amountFiles.append(ui->listProcessingAdd->item(i)->text());
    for(int i=0;i<ui->listProcessingRem->count();i++)
        sWinPlotVar.backgroundFiles.append(ui->listProcessingRem->item(i)->text());

    sWinPlotVar.KsL_x = ui->spinBoxT4x1->value()/pow(2,ui->spinBoxStepInterpolatePricessing->value());
    sWinPlotVar.KsL_y = ui->spinBoxT4y1->value()/pow(2,ui->spinBoxStepInterpolatePricessing->value());
    sWinPlotVar.KsR_x = ui->spinBoxT4x2->value()/pow(2,ui->spinBoxStepInterpolatePricessing->value());
    sWinPlotVar.KsR_y = ui->spinBoxT4y2->value()/pow(2,ui->spinBoxStepInterpolatePricessing->value());

    sWinPlotVar.temperature = Parametrs.temperature;
    sWinPlotVar.magnetic_fild = Parametrs.magnetic_fild;

    emit signal_sendVariables(sWinPlotVar);
    wPlot->show();
}

void MainWindow::slot_updateProcessingItems(double val){
    plot2DProcessing->clearItems();
    paintSmallCross(plot2DProcessing,dataProcessing,
                    ui->spinBoxT4x1->value(),ui->spinBoxT4y1->value());
    paintSmallCross(plot2DProcessing,dataProcessing,
                    ui->spinBoxT4x2->value(),ui->spinBoxT4y2->value());
    paintSector(plot2DProcessing,ui->spinBoxT4x1->value(),ui->spinBoxT4y1->value(),
                ui->spinBoxT4angle_1->value(),ui->spinBoxT4OpenAngle_1->value(),
                ui->spinBoxT4offset_1->value(),ui->spinBoxT4Len_1->value());
    paintSector(plot2DProcessing,ui->spinBoxT4x2->value(),ui->spinBoxT4y2->value(),
                ui->spinBoxT4angle_2->value(),ui->spinBoxT4OpenAngle_2->value(),
                ui->spinBoxT4offset_2->value(),ui->spinBoxT4Len_2->value());
    //PlotUpdate(plot2DProcessing,colorMapProcessing,dataProcessing);
    plot2DProcessing->replot();
    plot2DProcessing->repaint();
    plot2DProcessing->update();

    return;
}

void MainWindow::slot_updateGlobalRadius1(double val){
    globalRadius1 = ui->spinBoxT4offset_1->value()/
            pow(2,ui->spinBoxStepInterpolatePricessing->value());
    return;
}

void MainWindow::slot_updateGlobalRadius2(double val){
    globalRadius2 = ui->spinBoxT4offset_2->value()/
            pow(2,ui->spinBoxStepInterpolatePricessing->value());
    return;
}

void MainWindow::on_buttonProcessingAddListAdd_clicked()
{

}

void MainWindow::on_actionE_xit_triggered()
{
    QApplication::quit();
    return;
}

void MainWindow::on_actionSave_triggered()
{
    QString tmp;
    QString filename = QFileDialog::getSaveFileName(this,"Save project file","","*.xml");
    if(filename=="") return;
    if(filename.split('.').at(filename.split('.').size()-1)!="xml") filename += ".xml";
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QXmlStreamWriter xml(&f);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE gelimagnetprocessing>");
    xml.writeStartElement("SaveFile");

    xml.writeStartElement("Options");

    xml.writeTextElement("WorkDirectory",workDirectory);
    tmp = "";
    for(int i=0;i<ui->listWidgetExplprer->count();i++)
        tmp += ui->listWidgetExplprer->item(i)->text()+";";
    xml.writeTextElement("ExplorerList",tmp);

    xml.writeTextElement("SizeOfPixel_mm",QString::number(Parametrs.size_of_pixel_mm));
    xml.writeTextElement("DistSampleDetector",QString::number(Parametrs.long_sample_detector));
    xml.writeTextElement("Wavelegth",QString::number(Parametrs.lambdaA));
    xml.writeTextElement("DataSize_x",QString::number(Parametrs.count_pix_x));
    xml.writeTextElement("DataSize_y",QString::number(Parametrs.count_pix_y));

    tmp = "";
    for(int i=0;i<ui->listWidgetKsAdd->count();i++)
        tmp += ui->listWidgetKsAdd->item(i)->text()+";";
    xml.writeTextElement("ListKsAmount",tmp);
    tmp = "";
    for(int i=0;i<ui->listWidgetKsRem->count();i++)
        tmp += ui->listWidgetKsRem->item(i)->text()+";";
    xml.writeTextElement("ListKsBackground",tmp);

    xml.writeTextElement("globalKs",QString::number(globalKs));
    xml.writeTextElement("globalKs1_x",QString::number(globalKs1_x));
    xml.writeTextElement("globalKs1_y",QString::number(globalKs1_y));
    xml.writeTextElement("globalKs2_x",QString::number(globalKs2_x));
    xml.writeTextElement("globalKs2_y",QString::number(globalKs1_y));
    xml.writeTextElement("globalCx",QString::number(globalCx));
    xml.writeTextElement("globalCy",QString::number(globalCy));
    xml.writeTextElement("globalAngleC",QString::number(globalAngleC));
    xml.writeTextElement("StepInterpolateKs",QString::number(ui->spinBoxStepInterpolate->value()));


    tmp = "";
    for(int i=0;i<ui->listProcessingAdd->count();i++)
        tmp += ui->listProcessingAdd->item(i)->text()+";";
    xml.writeTextElement("ListProgressAmount",tmp);
    tmp = "";
    for(int i=0;i<ui->listProcessingRem->count();i++)
        tmp += ui->listProcessingRem->item(i)->text()+";";
    xml.writeTextElement("ListProgressBackground",tmp);

    xml.writeTextElement("T4OpenAngle1",QString::number(ui->spinBoxT4OpenAngle_1->value()));
    xml.writeTextElement("T4OpenAngle2",QString::number(ui->spinBoxT4OpenAngle_2->value()));
    xml.writeTextElement("T4Radius1",QString::number(ui->spinBoxT4Len_1->value()));
    xml.writeTextElement("T4Radius2",QString::number(ui->spinBoxT4Len_2->value()));
    xml.writeTextElement("T4Offset1",QString::number(ui->spinBoxT4offset_1->value()));
    xml.writeTextElement("T4Offset2",QString::number(ui->spinBoxT4offset_2->value()));
    xml.writeTextElement("T4Angle1",QString::number(ui->spinBoxT4angle_1->value()));
    xml.writeTextElement("T4Angle2",QString::number(ui->spinBoxT4angle_2->value()));
    xml.writeTextElement("T4x1",QString::number(ui->spinBoxT4x1->value()));
    xml.writeTextElement("T4y1",QString::number(ui->spinBoxT4y1->value()));
    xml.writeTextElement("T4x2",QString::number(ui->spinBoxT4x2->value()));
    xml.writeTextElement("T4y2",QString::number(ui->spinBoxT4y2->value()));
    xml.writeTextElement("T4StepInterpolate",QString::number(ui->spinBoxStepInterpolatePricessing->value()));
    xml.writeTextElement("PInterpolate",QString::number(pInterpolate));

    xml.writeEndElement();
    xml.writeEndDocument();

    this->setWindowTitle("helimagnetProcessing - "+filename);

    f.close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString tmp="";
    QString filename = QFileDialog::getOpenFileName(this,"Open project file","","*.xml");
    if(filename=="") return;

    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) return;

    QXmlStreamReader xml(&f);

    ui->listWidgetExplprer->clear();
    ui->listWidgetKsAdd->clear();
    ui->listWidgetKsRem->clear();
    ui->listProcessingAdd->clear();
    ui->listProcessingRem->clear();

    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();
            if(xml.isStartElement()){
                if(xml.name()=="WorkDirectory"){
                    workDirectory = xml.readElementText();
                    ui->lineEditOpenExporer->setText(workDirectory);
                }
                if(xml.name()=="ExplorerList"){
                    tmp = xml.readElementText();
                    for(int i=0;i<tmp.split(';').size();i++){
                        ui->listWidgetExplprer->addItem(tmp.split(';').at(i));
                    }
                }
                if(xml.name()=="SizeOfPixel_mm"){
                    tmp = xml.readElementText();
                    Parametrs.size_of_pixel_mm = tmp.toDouble();
                    ui->spinBoxSizePixel->setValue(Parametrs.size_of_pixel_mm);
                }
                if(xml.name()=="DistSampleDetector"){
                    tmp = xml.readElementText();
                    Parametrs.long_sample_detector = tmp.toDouble();
                    ui->spinBoxDistanceSD->setValue(Parametrs.long_sample_detector);
                }
                if(xml.name()=="Wavelegth"){
                    tmp = xml.readElementText();
                    Parametrs.lambdaA = tmp.toDouble();
                    ui->spinBoxWavelength->setValue(Parametrs.lambdaA);
                }
                if(xml.name()=="ListKsAmount"){
                    tmp = xml.readElementText();
                    for(int i=0;i<tmp.split(';').size();i++){
                        ui->listWidgetKsAdd->addItem(tmp.split(';').at(i));
                    }
                }
                if(xml.name()=="ListKsBackground"){
                    tmp = xml.readElementText();
                    for(int i=0;i<tmp.split(';').size();i++){
                        ui->listWidgetKsRem->addItem(tmp.split(';').at(i));
                    }
                }
                if(xml.name()=="globalKs"){
                    tmp = xml.readElementText();
                    globalKs = tmp.toDouble();
                }
                if(xml.name()=="globalKs1_x"){
                    tmp = xml.readElementText();
                    globalKs1_x = tmp.toDouble();
                }
                if(xml.name()=="globalKs1_y"){
                    tmp = xml.readElementText();
                    globalKs1_y = tmp.toDouble();
                }
                if(xml.name()=="globalKs2_x"){
                    tmp = xml.readElementText();
                    globalKs2_x = tmp.toDouble();
                }
                if(xml.name()=="globalKs2_y"){
                    tmp = xml.readElementText();
                    globalKs2_y = tmp.toDouble();
                }
                if(xml.name()=="globalCx"){
                    tmp = xml.readElementText();
                    globalCx = tmp.toDouble();
                }
                if(xml.name()=="globalCy"){
                    tmp = xml.readElementText();
                    globalCy = tmp.toDouble();
                }
                if(xml.name()=="globalAngleC"){
                    tmp = xml.readElementText();
                    globalAngleC = tmp.toDouble();
                }
                if(xml.name()=="StepInterpolateKs"){
                    tmp = xml.readElementText();
                    ui->spinBoxStepInterpolate->setValue(tmp.toInt());
                }
                if(xml.name()=="ListProgressAmount"){
                    tmp = xml.readElementText();
                    for(int i=0;i<tmp.split(';').size();i++){
                        ui->listProcessingAdd->addItem(tmp.split(';').at(i));
                    }
                }
                if(xml.name()=="ListProgressBackground"){
                    tmp = xml.readElementText();
                    for(int i=0;i<tmp.split(';').size();i++){
                        ui->listProcessingRem->addItem(tmp.split(';').at(i));
                    }
                }
                if(xml.name()=="T4OpenAngle1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4OpenAngle_1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4OpenAngle2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4OpenAngle_2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Radius1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4Len_1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Radius2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4Len_2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Offset1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4offset_1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Offset2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4offset_2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Angle1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4angle_1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4Angle2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4angle_2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4x1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4x1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4y1"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4y1->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4x2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4x2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4y2"){
                    tmp = xml.readElementText();
                    ui->spinBoxT4y2->setValue(tmp.toDouble());
                }
                if(xml.name()=="T4StepInterpolate"){
                    tmp = xml.readElementText();
                    ui->spinBoxStepInterpolatePricessing->setValue(tmp.toInt());
                }
                if(xml.name()=="PInterpolate"){
                    tmp = xml.readElementText();
                    pInterpolate = tmp.toInt();
                }
            }
    }

    for(int i=0;i<ui->listWidgetExplprer->count();i++){
        if(ui->listWidgetExplprer->item(i)->text()=="") delete ui->listWidgetExplprer->item(i);
    }
    for(int i=0;i<ui->listWidgetKsAdd->count();i++){
        if(ui->listWidgetKsAdd->item(i)->text()=="") delete ui->listWidgetKsAdd->item(i);
    }
    for(int i=0;i<ui->listWidgetKsRem->count();i++){
        if(ui->listWidgetKsRem->item(i)->text()=="") delete ui->listWidgetKsRem->item(i);
    }
    for(int i=0;i<ui->listProcessingAdd->count();i++){
        if(ui->listProcessingAdd->item(i)->text()=="") delete ui->listProcessingAdd->item(i);
    }
    for(int i=0;i<ui->listProcessingRem->count();i++){
        if(ui->listProcessingRem->item(i)->text()=="") delete ui->listProcessingRem->item(i);
    }

    plotKs->clearGraphs();

    ui->spinBoxKs1_x->setValue(globalKs1_x*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->spinBoxKs1_y->setValue(globalKs1_y*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->spinBoxKs2_x->setValue(globalKs2_x*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->spinBoxKs2_y->setValue(globalKs2_y*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->spinBoxKs->setValue(globalKs*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->SpinBoxCenterX->setValue(globalCx*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->SpinBoxCenterY->setValue(globalCy*pow(2,ui->spinBoxStepInterpolate->value()));
    ui->SpinBoxAngleCenter->setValue(globalAngleC);

    on_buttonKsSumm_clicked();
    on_buttonKsCut_clicked();
    on_buttonProcessingLoad_clicked();
    on_buttonInterpolateProcessing_clicked();

    pInterpolate = 0;

    this->setWindowTitle("helimagnetProcessing - "+filename);
    f.close();
}


void MainWindow::slot_setsizeofpixel(){
    switch(ui->comboBoxSetParameters->currentIndex()){
    case 0:

        break;
    case 1: //D11
        Parametrs.size_of_pixel_mm = 7.5;
        break;
    case 2: //PA20
        Parametrs.size_of_pixel_mm = 5;
        break;
    case 3: //SANS-1 1
        Parametrs.size_of_pixel_mm = 8;
        break;
    case 4:  //SANS-1 2
        Parametrs.size_of_pixel_mm = 3;
        break;
    }
    ui->spinBoxSizePixel->setValue(Parametrs.size_of_pixel_mm);
}

void MainWindow::on_buttonExplorerClear_clicked()
{
    ui->listWidgetExplprer->clear();
}

void MainWindow::on_buttonProcessingAddListReverse_clicked()
{
    QStringList list;
    for(int i=0;i<ui->listProcessingAdd->count();i++){
        list.append(ui->listProcessingAdd->item(i)->text());
    }
    ui->listProcessingAdd->clear();
    for(int i=0;i<ui->listProcessingRem->count();i++){
        ui->listProcessingAdd->addItem(ui->listProcessingRem->item(i)->text());
    }
    ui->listProcessingRem->clear();
    for(int i=0;i<list.count();i++){
        ui->listProcessingRem->addItem(list.at(i));
    }
}

void MainWindow::on_actionExport_Processing_data_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Export data","","*.mxm");
    filename += ".mxm";

    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&f);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE gelimagnetprocessing>");
    xml.writeStartElement("emulatedFormat");

    xml.writeStartElement("filds");

    xml.writeTextElement("MagneticFild",QString::number(Parametrs.magnetic_fild));
    xml.writeTextElement("TimeOfExposition",QString::number(Parametrs.time_exposition));
    xml.writeTextElement("ResolutionOfDetector",QString::number(Parametrs.size_of_pixel_mm));
    xml.writeTextElement("DistDetectorSource",QString::number(Parametrs.long_sample_detector));
    xml.writeTextElement("Monitor1",QString::number(Parametrs.monitor1));
    xml.writeTextElement("Lambda",QString::number(Parametrs.lambdaA));
    QString tmp="";
    for(int i=0;i<dataProcessing->x;i++)
        for(int j=0;j<dataProcessing->y;j++)
            //tmp += QString::number(data[i][j])+";";
            tmp += QString::number(dataProcessing->data[i][j])+";";
    xml.writeTextElement("Data",tmp);

    xml.writeEndElement();
    xml.writeEndDocument();

    f.close();
}

void MainWindow::on_buttonProcessingClearRem_func_clicked()
{
    dataProcessing->clear();

    QString filename;
    QString name;
    for(int l=0;l<ui->listProcessingAdd->count();l++){
        name = ui->listProcessingAdd->item(l)->text();
        filename = workDirectory+"/"+name;
        sData *data = new sData;
        LoadFilename(data,filename);
        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataProcessing->data[i][j] += data->data[i][j];
            }
        }
        dataProcessing->x = data->x;
        dataProcessing->y = data->y;
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }
    for(int l=0;l<ui->listProcessingRem->count();l++){
        name = ui->listProcessingRem->item(l)->text();
        filename = workDirectory+"/"+name;
        sData *data = new sData;
        LoadFilename(data,filename);
        for(int i=0;i<data->x;i++){
            for(int j=0;j<data->y;j++){
                dataProcessing->data[i][j] -= data->data[i][j]/2;
            }
        }
        dataProcessing->x = data->x;
        dataProcessing->y = data->y;
        for(int i=0;i<data->x;i++) delete[] data->data[i];
        delete [] data;
    }

    slot_updateProcessingItems(0.0);
    PlotUpdate(plot2DProcessing,colorMapProcessing,dataProcessing);
    processingInterpolate = 0;
}

void MainWindow::on_checkBoxT4_func_stateChanged(int arg1)
{

}

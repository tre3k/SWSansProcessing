#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>

#include <H5Cpp.h>
#include <iostream>

#include "iqcustomplot.h"
#include "windowplot.h"

#define DIVCOEFF

#define UPDATE_CROSS QCPRange rangeX,rangeY;\
                     rangeX = plotKs2D->xAxis->range();\
                     rangeY = plotKs2D->yAxis->range();\
                     paintCross(plotKs2D,dataKs,ui->SpinBoxCenterX->value(),\
                                                ui->SpinBoxCenterY->value(),\
                                                ui->SpinBoxAngleCenter->value());\
                     PlotUpdate(plotKs2D,colorMapKs2D,dataKs);\
                     plotKs2D->xAxis->setRange(rangeX);\
                     plotKs2D->yAxis->setRange(rangeY);\
                     plotKs2D->replot();\

/* interpolate types */
enum{
    LINEAR_INTERPOLATE
};

struct sParametrs{
    QString name;
    QString sampleName;
    double temperature;
    double magnetic_fild;
    double long_sample_detector;
    double size_phisical_of_detector;
    double size_of_detector;
    double time_exposition;
    int count_pix_x;
    int count_pix_y;
    int long summ_all_neutrons;
    double lambdaA;
    double size_of_pixel_mm;
    double monitor1;
    double monitor2;
    double coeff;
    double fromfileSumNeutron;
};

struct sData{
    int x,y;
    double **data;
    void init();
    void clear();
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    windowPlot *wPlot;

    iQCustomPlot *plotExplorer;
    QCPColorMap *colorMapExplorer;
    QCPColorScale *colorScale2DExplorer;

    iQCustomPlot *plot2DProcessing;
    QCPColorMap *colorMapProcessing;
    QCPColorScale *colorScale2DProcessing;

    iQCustomPlot *plotKs2D;
    iQCustomPlot *plotKs;
    QCPColorMap *colorMapKs2D;
    QCPColorScale *colorScaleKs2D;

    QString workDirectory;
    QStringList openFilesExplorer;

    QDoubleSpinBox *spinBoxMinScale;
    QDoubleSpinBox *spinBoxMaxScale;
    QProgressBar *globalProgressBar;

    sData *dataKs;
    sData *dataProcessing;

    QVector<double> vAveX1,vAveI1,vAveErr1;
    QVector<double> vAveX2,vAveI2,vAveErr2;

    double globalCx=0.0;
    double globalCy=0.0;
    double globalAngleC=0.0;

    double globalKs1_x=0.0;
    double globalKs1_y=0.0;
    double globalKs2_x=0.0;
    double globalKs2_y=0.0;

    double pKs1_x=0.0;
    double pKs1_y=0.0;
    double pKs2_x=0.0;
    double pKs2_y=0.0;
    int pInterpolate=0;

    double globalKs = 0.0;
    double tmpKs = 0.0;

    double globalRadius1 = 0.0;
    double globalRadius2 = 0.0;

    int processingInterpolate=0;

    void LoadFilename(sData *,QString);
    void ReadILL(sData *,QString);
    void ReadSANS1(sData *,QString);
    void ReadSANS1_300(sData *,QString);
    void ReadD11(sData *,QString);
    void ReadNXS(sData *,QString);
    void ReadMyFormat(sData *,QString);
    void PlotUpdate(QCustomPlot *,QCPColorMap *,sData *);
    void updateStatusBar();
    void toCircle(double *,double *,double,double);
    double getPixelCircle(sData *,double,double);
    void Interpolate(int,sData*,sData*);

    void paintCross(iQCustomPlot *,sData *,double,double,double);
    void paintSmallCross(iQCustomPlot *,sData *,double,double);

    void paintSector(iQCustomPlot *,double,double,double,double,double,double);

    //x,y,angle,open angle,len,offset
    void average(sData *,double,double,double,double,double,double,QVector<double> *,QVector<double> *,QVector<double> *,bool,int);
    int doubleToInt(double);

    sParametrs Parametrs;

private slots:
    void on_buttonOpenExplorer_clicked();

    void slot_edit_textOpenDirecotry(QString);
    void slot_clicked_in_ExplorerList(QString);

    void on_action_log_scale_triggered();

    void on_action_auto_scale_triggered();

    void on_actionmanual_scale_triggered();

    void on_buttonExplorerKsAdd_clicked();

    void on_buttonExplorerKsRem_clicked();

    void on_buttonKsRemListSum_clicked();

    void on_buttonKsRemListRem_clicked();

    void on_buttonKsSumm_clicked();

    void on_buttonKsAuto_clicked();

    void on_buttonKsCut_clicked();

    void on_buttonKsSetCenter_clicked();

    void on_buttonSetKs_clicked();

    void on_buttonKsOk_clicked();

    void on_buttonExplorerAdd_clicked();

    void on_buttonExplorerRem_clicked();

    void on_buttonProcessingLoad_clicked();

    void on_buttonInterpolateProcessing_clicked();

    void on_buttonProcessingAddListRem_clicked();

    void on_buttonProcessingRemListRem_clicked();

    void on_buttonProcessingClearRem_clicked();

    void on_buttonProcessingClearAdd_clicked();

    void on_buttonKsClearAdd_clicked();

    void on_buttonKsClearRem_clicked();

    void on_buttonSetParameters_clicked();

    void on_buttonProcessAverage_clicked();

    void slot_updateProcessingItems(double);
    void slot_updateGlobalRadius1(double);
    void slot_updateGlobalRadius2(double);
    void slot_setsizeofpixel();

    void on_buttonProcessingAddListAdd_clicked();

    void on_actionE_xit_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_buttonExplorerClear_clicked();

    void on_buttonProcessingAddListReverse_clicked();

    void on_actionExport_Processing_data_triggered();

    void on_buttonProcessingClearRem_func_clicked();

    void on_checkBoxT4_func_stateChanged(int arg1);

signals:
    void signal_sendVariables(sWindowPlotVariables);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

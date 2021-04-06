#ifndef WINDOWPLOT_H
#define WINDOWPLOT_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QVector>

#include "iqcustomplot.h"

enum{
    UNITX_PIX,
    UNITX_Q_A,
    UNITX_Q_nm,
    UNITX_Rad
};

enum{
    UNITY_N,
    UNITY_Ns
};

struct sWindowPlotVariables{
    QVector<double> *vX1,*vX2;
    QVector<double> *vY1,*vY2;
    QVector<double> *vErr1,*vErr2;
    int stepInterpolate;
    double time;
    QStringList amountFiles;
    QStringList backgroundFiles;
    double Ks,KsA;
    double KsR_x,KsL_x,KsR_y,KsL_y;
    double lambdaA,size_of_pixel_mm,dist_source_detector;
    double temperature;
    double magnetic_fild;
};

class windowPlot : public QMainWindow
{
    Q_OBJECT
public:
    explicit windowPlot(QWidget *parent = nullptr);

    /* Menu and actions */
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuExport;
    QMenu *menuXaxis;
    QMenu *menuYaxis;
    QMenu *menuPlot;
    QMenu *menuXUnits;
    QMenu *menuYUnits;
    QMenu *menuDeInterpolate;
    QAction *actionDeInterpolateAverage;
    QAction *actionDeInterpolateThinning;
    QAction *actionExportTxt;
    QAction *actionExportQti;
    QAction *actionExportPNG;
    QAction *actionExportJPG;
    QAction *actionExportPDF;
    QAction *actionExportAll;
    QAction *actionClose;
    QAction *actionAutoscale;
    QAction *actionPlotLeft;
    QAction *actionPlotRight;
    QAction *actionPlotAmount;
    QAction *actionXunitPix;
    QAction *actionXunitQA;
    QAction *actionXunitQnm;
    QAction *actionXunitRad;
    QAction *actionYunitN;
    QAction *actionYunitNs;
    QAction *actionXlogScale;
    QAction *actionXlinScale;
    QAction *actionYlogScale;
    QAction *actionYlinScale;

    QString exportDirectory="";

    /* mainPlot */
    QVector<double> vX1,vX2,vXs;
    QVector<double> vY1,vY2,vYs,vYsub;
    QVector<double> vErr1,vErr2,vErr_s,vErr_sub;
    QCPErrorBars *errorBar1, *errorBar2, *errorBar_s;
    iQCustomPlot *plot;

    sWindowPlotVariables sVar;

    int unit_x = 0,unit_y = 0;

    void updatePlot();
    double pixToQ(double);
    double pixToRad(double);
    void exportToQti(QString);
    void exportToTxt(QString);
    void exportToPNG(QString);

signals:

public slots:
    void slot_actionClose();
    void slot_actionExportQti();
    void slot_actionExportTxt();
    void slot_actionExportPNG();
    void slot_actionExportPDF();
    void slot_actionExportAll();
    void slot_actionAutoscale();
    void slot_actionPlotLeft(bool);
    void slot_actionPlotRigh(bool);
    void slot_actionPlotAmount(bool);
    void slot_selectYUnitN(bool);
    void slot_selectYUnitNs(bool);
    void slot_selectXUnitPix(bool);
    void slot_selectXUnitQA(bool);
    void slot_selectXUnitQnm(bool);
    void slot_selectXUnitRad(bool);
    void slot_setXlogScale();
    void slot_setXlinScale();
    void slot_setYlogScale();
    void slot_setYlinScale();
    void slot_deinterpolateAverage();
    void slot_deinterpolateThinning();

    void slot_loadVariables(sWindowPlotVariables);
};

#endif // WINDOWPLOT_H

#ifndef IQCUSTOMPLOT_H
#define IQCUSTOMPLOT_H

#include "qcustomplot.h"

class iQCustomPlot : public QCustomPlot
{
        Q_OBJECT
public:
        explicit iQCustomPlot(QWidget *parent = 0);
        void addCurve(QVector<double> *,QVector<double> *,bool,QColor,QString);

private slots:
        void slot_sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
        void slot_full_drag_zoom(QMouseEvent *);
        void slot_selectionChanged();

};

#endif // IQCUSTOMPLOT_H

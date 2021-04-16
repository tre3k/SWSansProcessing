/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/

#ifndef GENERALPLOT_H
#define GENERALPLOT_H

/* project includes */
#include <generalsetup.h>
#include <qcustomplot.h>

class GeneralPlot : public QCustomPlot
{
        Q_OBJECT
public:
        GeneralPlot();

protected:
        bool x_log = false;
        bool y_log = false;


protected slots:
        void contextMenuReq(QPoint p);
        void sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
        void full_drag_zoom(QMouseEvent *);
        void selectionChanged();




        void exportToPDF();
        void exportToBMP();
        void exportToJPG();
        void exportToPNG();

        void setXLog();
        void setYLog();


};

#endif // GENERALPLOT_H

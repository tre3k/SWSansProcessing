#ifndef GENERALSETUP_H
#define GENERALSETUP_H

#include <QObject>

/* project includes */

class GeneralSetup : public QObject
{
        Q_OBJECT
public:
        explicit GeneralSetup(QObject *parent = nullptr);

        static QString global_locale;
        static QString number_locale;

        static QString work_direcotry;

signals:

};

#endif // GENERALSETUP_H

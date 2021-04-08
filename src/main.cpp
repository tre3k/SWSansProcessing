/*  (c) 2021 Kirill Pshenichnyi
*   GNU GPLv3 License
*/

#include "main.h"

int main(int argc,char **argv){
        /* main init Qt application */
        auto app = new QApplication(argc,argv);

        /* init and show class MainWindow */
        auto mw = new MainWindow(nullptr);
        mw->show();

        /* exec Qt application */
        return app->exec();
}


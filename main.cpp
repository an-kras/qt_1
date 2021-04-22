#include <QApplication>

#include "clcltr.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     Calculator calc;
     calc.show();
     return app.exec();
 }

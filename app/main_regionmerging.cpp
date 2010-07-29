#include <QtGui/QApplication>
#include "regionmerging.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow mwindow;

  if (argc >= 4) {
    printf("%s, %s, %s \n",argv[1], argv[2], argv[3]);
    mwindow.setFileName1(QString(argv[1]));
    mwindow.setFileName2(QString(argv[2]));
    mwindow.setFileName3(QString(argv[3]));
    
    mwindow.loadAllInputImages();
  }
  else {
    QString currentDir = QDir::currentPath();
    if (
        QFile::exists(currentDir + "/../test_data/regionmerging_i1.bmp") &&
        QFile::exists(currentDir + "/../test_data/regionmerging_i2.bmp") &&
        QFile::exists(currentDir + "/../test_data/regionmerging_rg.pan")
      )
    {
      mwindow.setFileName1(currentDir + "/../test_data/regionmerging_i1.bmp");
      mwindow.setFileName2(currentDir + "/../test_data/regionmerging_i2.bmp");
      mwindow.setFileName3(currentDir + "/../test_data/regionmerging_rg.pan");
      mwindow.loadAllInputImages();
    }
  }
  
  mwindow.show();
  return app.exec();
}

#include <QtGui/QApplication>
#include "watershed.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Watershed mwindow;

  if (argc >= 2) {
    printf("%s, %s, %s \n",argv[1], argv[2], argv[3]);
    mwindow.setFileName1(QString(argv[1]));

    mwindow.loadAllInputImages();
  }
  else {
    QString currentDir = QDir::currentPath();
    if (
        QFile::exists(currentDir + "/../test_data/watershed_i1.bmp")
      )
    {
      mwindow.setFileName1(currentDir + "/../test_data/watershed_i1.bmp");
      mwindow.loadAllInputImages();
    }
  }

  mwindow.show();
  return app.exec();
}

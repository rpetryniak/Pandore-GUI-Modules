#ifndef WATERSHED_H
#define WATERSHED_H

#include <QtGui/QMainWindow>

#include "functions.h"
#include "imagecanvas.h"

class ImageCanvas;

namespace Ui
{
    class WatershedClass;
}

class Watershed : public QMainWindow
{
    Q_OBJECT

public:
	Watershed(QWidget *parent = 0);
	~Watershed();

  void loadAllInputImages();
  void setFileName1(QString fileName) { inputFileName1 = fileName; }

private slots:
  void showAboutPandoreMessage();
  void showAboutThisGuiMessage();
  void openOryginalImage();
  void saveResult();
	void showOryginalImage();
	void showImageForComputations();
	void showInitialImage();
  void blurOryginalImage();
  void computeGradient();
  void computeDistanceMap();
  void computeLocalMinimumPoints();
  void computeBinaryImage();
	void runWatershed();
  void refreshHelp();

private:
  void disableAllProperties();

private:
	Ui::WatershedClass *ui;
	
  QImage oryginalImage, bluredImage, gradientImage, distanceMapImage, localMinimaImage, markerImage, binaryImage;
  QImage grayRegionImageTmp, colorRegionImageTmp;
  PRegionImage pRegionImage;
	
  bool isOryginalImage_opened, isBluredImage_computed, isGradientImage_computed, isDistanceMapImage_computed, isLocalMinimaImage_computed, isBinaryImage_computed, isMarkerImage_drawed;
	QColor borderColor;

  QString inputFileName1;
  QString lastPath;
};

#endif // WATERSHED_H

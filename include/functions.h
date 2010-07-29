#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include <QImage>

#include <pandore.h>
#include </usr/include/CImg.h>

using namespace pandore;
using namespace cimg_library;

template<typename T>
void cimg_draw_results(CImg<T> &image, CImg<T> &mask, unsigned char* color, float transp);

typedef struct {
  Reg2d* rg;
  bool loadStatus;
} PRegionImage;


PRegionImage loadRegionImage(QString fileName);
void savePandoreRegionImage(PRegionImage label, QString filename);

void qt2pan (QImage &image, Img2duc &panimg);
Img2duc qt2pan (QImage &image);
QImage pan2qt (Img2duc &image);
CImg<unsigned char> pan2cimg(Img2duc &imin);
CImg<unsigned int>  panreg2cimg(Reg2d &imin);
CImg<unsigned char> qt2cimg (QImage image);

template<typename T> QImage cimg2qt (CImg<T> image);
template<typename T> CImg<unsigned char> colorizeLabelImage(CImg<T> image, bool randomColor=false);

QImage addLabelToImageWithTransparency(QImage image, QImage label, float transparency);
QImage addLabelBoundaryToImage(QImage image, PRegionImage label, QColor color);
PRegionImage mergingAlgorithm(QImage image, PRegionImage label, int number, float threshold, int alg);
QImage blurAlgorithm(QImage image, double sigma);
QImage gradientAlgorithm(QImage imagea);
PRegionImage watershedAlgorithm(QImage image, QImage marker);
QImage distanceMapAlgorithm(QImage image);
QImage localMinimumPointsAlgorithm(QImage image);
QImage binarizationAlgorithm(QImage image, int threshold);

#endif // FUNCTIONS_H

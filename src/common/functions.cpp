#include "functions.h"

#include <QtGui>

#include </usr/include/CImg.h>
#include <pandore.h>

using namespace pandore;
using namespace cimg_library;

namespace pboundary {
  #include "region/pboundary.cpp"
}

/* arithmetic */
namespace pnormalization {
  #include "arithmetic/pnormalization.cpp"
}

/* morphology */
namespace pwatershed {
  #include "morphology/pwatershed.cpp"
}

/* thresholding */
namespace pbinarization {
  #include "thresholding/pbinarization.cpp"
}

/* contour */
namespace pdistance {
  #include "contour/pdistance.cpp"
}

/* filtering */
namespace pgaussianfiltering {
  #include "filtering/pgaussianfiltering.cpp"
}

/* edgedetection */
namespace psobel {
  #include "edgedetection/psobel.cpp"
}

/* imagefeatureextraction */
namespace plocalminima {
  #include "imagefeatureextraction/plocalminima.cpp"
}

/* segmentation */
namespace plabeling {
  #include "segmentation/plabeling.cpp"
}
namespace pcontrastmerging {
  #include "segmentation/pcontrastmerging.cpp"
}
namespace pentropymerging {
  #include "segmentation/pentropymerging.cpp"
}
namespace pmeanmerging {
  #include "segmentation/pmeanmerging.cpp"
}
namespace puniformitymerging {
  #include "segmentation/puniformitymerging.cpp"
}
namespace pvariancemerging {
  #include "segmentation/pvariancemerging.cpp"
}
namespace pboundarymerging {
  #include "segmentation/pboundarymerging.cpp"
}
namespace phistomerging {
  #include "segmentation/phistomerging.cpp"
}

/* casting */
namespace pim2rg{
  #include "casting/pim2rg.cpp"
}
namespace prg2im{
  #include "casting/prg2im.cpp"
}
namespace pim2uc{
  #include "casting/pim2uc.cpp"
}
namespace pim2sl{
  #include "casting/pim2sl.cpp"
}
namespace prg2gr{
  #include "casting/prg2gr.cpp"
}

/* utility */
namespace psetborder{
  #include "utility/psetborder.cpp"
}

typedef struct { uchar r, g, b; } rgb;
//rgb colors[255];
rgb *colors;

// random color
rgb random_rgb(){
  rgb c;

  c.r = (uchar)(rand()%255);
  c.g = (uchar)(rand()%255);
  c.b = (uchar)(rand()%255);

  return c;
}

template<typename T>
void cimg_draw_results(CImg<T> &image, CImg<T> &mask, unsigned char* color, float transp)
{
  CImg<T> layer_c = mask;

  layer_c.draw_rectangle(0, 0, layer_c.width(), layer_c.height(), color, 1.0);

  CImg<T> layer_f1 = mask;
  cimg_forXY(layer_f1,x,y)
  layer_f1(x,y)  = (layer_f1(x,y,0)==255 && layer_f1(x,y,1)==255 && layer_f1(x,y,2)==255)?1.0f:0.0f;
  CImg<T> layer_f2=layer_f1.get_shared_channel(0);

  image.draw_image(layer_c,layer_f2,transp);
}

PRegionImage loadRegionImage(QString fileName){
  PRegionImage pRegionImage;
  pRegionImage.rg = (Reg2d*)LoadFile(fileName.toAscii());;

  pRegionImage.loadStatus = true;
  return pRegionImage;
}

void qt2pan (QImage &image, Img2duc &panimg) {
  const int h=image.height();
  const int w=image.width();

  for (int y=0; y<h;y++) {
    for (int x=0; x<w;x++) {
      panimg[y][x] = qRed(image.pixel(x,y));
    }
  }
}

Img2duc qt2pan (QImage &image) {
  const int h=image.height();
  const int w=image.width();

  Img2duc pim( h,w );

  for (int y=0; y<h;y++) {
    for (int x=0; x<w;x++) {
      pim[y][x] = qRed(image.pixel(x,y));
    }
  }
  return pim;
}

QImage pan2qt (Img2duc &image) {
  const int w=image.Width();
  const int h=image.Height();
  QImage imagePhysic=QImage(w,h,QImage::Format_RGB32);

  for (int y=0; y<h;y++) {
    uint *p = reinterpret_cast<uint *>(imagePhysic.scanLine(y));
    for (int x=0; x<w;x++) {
      int r=0; int g=0; int b=0;
      r = g = b = image[y][x];
      *(p++) =  qRgb(r,g,b);
    }
  }
  return imagePhysic;
}

CImg<unsigned char> pan2cimg(Img2duc &imin) {
  CImg<unsigned char> imout(&(imin[0][0]), imin.Width(), imin.Height(), 1, imin.Bands(), false);
  return imout;
}
CImg<unsigned int> panreg2cimg(Reg2d &imin) {
  CImg<unsigned int> imout(&(imin[0][0]), imin.Width(), imin.Height(), 1, imin.Bands(), false);
  return imout;
}

CImg<unsigned char> qt2cimg (QImage image) {
  const int h=image.height();
  const int w=image.width();

  CImg<unsigned char> imout(w,h,1,3,0);

  for (int y=0; y<h;y++) {
    for (int x=0; x<w;x++) {
      imout(x,y,0) = qRed(image.pixel(x,y));
      imout(x,y,1) = qGreen(image.pixel(x,y));
      imout(x,y,2) = qBlue(image.pixel(x,y));
    }
  }
  return imout;
}

template<typename T>
QImage cimg2qt (CImg<T> image) {
  const int w=image.width();
  const int h=image.height();
  image.normalize(0,255).resize(-100,-100,1,3);

  QImage imagePhysic=QImage(w,h,QImage::Format_RGB32);

  for (int y=0; y<h;y++) {
    uint *p = reinterpret_cast<uint *>(imagePhysic.scanLine(y));
    for (int x=0; x<w;x++) {
      int r=0; int g=0; int b=0;
      r = image(x,y,0);
      g = image(x,y,1);
      b = image(x,y,2);
      *(p++) =  qRgb(r,g,b);
    }
  }
  return imagePhysic;
}
// konkretyzacja funkcji dla wybranych typów:
template QImage cimg2qt (CImg<unsigned char> image);
template QImage cimg2qt (CImg<unsigned int> image);

template<typename T>
CImg<unsigned char> colorizeLabelImage(CImg<T> image, bool randomColor)
{
  int width = image.width();
  int height= image.height();
  int maxvalue = image.max();

  if (randomColor || colors==(rgb *)0 ) { // || maxvalue>sizeof(colors) ) {
    colors = new rgb[maxvalue+1];
    for (int i = 0; i < maxvalue; i++)
      colors[i] = random_rgb();
  }
  CImg<unsigned char> output(width, height, 1, 3, 0);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int comp = image(x,y,0,0);
      if (comp == 0) continue;
      //if (sizeof(colors) < comp) printf("CAUSION!!! maxvalue=%d, comp=%d, sizeof(colors)=%d  \n",maxvalue,comp,sizeof(colors));
      output(x,y,0,0) = colors[comp].r;
      output(x,y,0,1) = colors[comp].g;
      output(x,y,0,2) = colors[comp].b;
    }
  }
  return output;
}
// konkretyzacja funkcji dla wybranych typów:
template CImg<unsigned char> colorizeLabelImage(CImg<unsigned char> image, bool randomColor);
template CImg<unsigned char> colorizeLabelImage(CImg<unsigned int > image, bool randomColor);

QImage addLabelToImageWithTransparency(QImage image, QImage label, float transparency)
{
  CImg<unsigned char> i1 = qt2cimg(image);
  CImg<unsigned char> i2 = qt2cimg(label);

  i1.draw_image(i2,transparency);

  QImage t1 = cimg2qt(i1);
  return t1;
}

QImage addLabelBoundaryToImage(QImage image, PRegionImage label, QColor color)
{
  //Img2duc p1 = qt2pan(label);
  Img2duc p2( label.rg->Height(), label.rg->Width() );

  pboundary::PBoundary(*label.rg,p2,8);
  
  CImg<unsigned char> c1 = pan2cimg(p2);
  CImg<unsigned char> c2 = qt2cimg(image);
  
  CImg<unsigned char> c3(c2.width(),c2.height(),1,3,0);
  cimg_forXYC(c3,x,y,v) c3(x,y,v) = c1(x,y,0);
  //c2.draw_image(c3,0.2);

  unsigned char c[3];
  c[0] = color.red();
  c[1] = color.green();
  c[2] = color.blue();

  cimg_draw_results(c2,c3,c,0.5);

  QImage t1 = cimg2qt(c2);
  return t1;
}

void savePandoreRegionImage(PRegionImage label, QString filename)
{
  label.rg->SaveFile(filename.toAscii());
}

PRegionImage mergingAlgorithm(QImage image, PRegionImage label, int number, float threshold, int alg)
{
  Img2duc i1 = qt2pan(image);
  Reg2d  *r2 = new Reg2d( i1.Height(), i1.Width() );

  Graph2d g1; //( i1.Height(), i1.Width() );
  Graph2d g2; //( i1.Height(), i1.Width() );

  prg2gr::PRg2Gr(*label.rg,g1);

  switch(alg) {
    case 0:
      pcontrastmerging::PContrastMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 1:
      pentropymerging::PEntropyMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 2:
      pmeanmerging::PMeanMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 3:
      puniformitymerging::PUniformityMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 4:
      pvariancemerging::PVarianceMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 5:
      pboundarymerging::PBoundaryMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
    case 6:
      phistomerging::PHistoMerging( *label.rg, g1, i1, *r2, g2, number, threshold );
      break;
  }

  PRegionImage result;
  result.rg = r2;
  return result;
}

QImage blurAlgorithm(QImage image, double sigma)
{
  Img2duc i1 = qt2pan(image);
  Img2duc i2( i1.Height(), i1.Width() );

  pgaussianfiltering::PGaussianFiltering2D(i1,i2, sigma);

  QImage t1 = pan2qt(i2);
  return t1;
}

QImage gradientAlgorithm(QImage image)
{
  Img2duc i1 = qt2pan(image);
  Img2duc i2( i1.Height(), i1.Width() );
  Img2duc i3( i1.Height(), i1.Width() );
  Img2duc i4( i1.Height(), i1.Width() );

  psobel::PSobel(i1,i2);
  psetborder::PSetBorder(i2,i3, 1,1,1,1, 255);
  pnormalization::PNormalization(i3,i4,(Img2duc::ValueType)0,(Img2duc::ValueType)255);

  QImage t1 = pan2qt(i4);
  return t1;
}

QImage binarizationAlgorithm(QImage image, int threshold)
{
  Img2duc i1 = qt2pan(image);
  Img2duc i2( i1.Height(), i1.Width() );

  pbinarization::PBinarization(i1,i2,(float)threshold,(float)1000);

  QImage t1 = pan2qt(i2);
  return t1;
}


QImage distanceMapAlgorithm(QImage image)
{
  Img2duc i1 = qt2pan(image);
  Img2dsf i2( i1.Height(), i1.Width() );
  Img2duc i3( i1.Height(), i1.Width() );
  Img2duc i4( i1.Height(), i1.Width() );

  pdistance::PDistance(i1,i2);
  pim2uc::PIm2Uc(i2,i3);
  pnormalization::PNormalization(i3,i4,(Img2duc::ValueType)0,(Img2duc::ValueType)255);

  QImage t1 = pan2qt(i4);
  return t1;
}

QImage localMinimumPointsAlgorithm(QImage image)
{
  Img2duc i1 = qt2pan(image);
  Img2duc i2( i1.Height(), i1.Width() );

  plocalminima::PLocalMinimum(i1,i2, 8);

  QImage t1 = pan2qt(i2);
  return t1;
}

PRegionImage watershedAlgorithm(QImage image, QImage initial)
{
  Img2duc i1 = qt2pan(image);
  Img2duc i2 = qt2pan(initial);

  Reg2d   r1( i1.Height(), i1.Width() );
  Reg2d  *r2 = new Reg2d( i1.Height(), i1.Width() );

  plabeling::PLabeling(i2,r1,8);
  pwatershed::PWatershed(r1,i1,*r2);

  PRegionImage result;
  result.rg = r2;
  return result;
}

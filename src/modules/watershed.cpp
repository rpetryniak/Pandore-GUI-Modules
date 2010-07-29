#include <QtGui>
#include <QtDebug>

#include "watershed.h"
#include "ui_watershed.h"

#include "functions.h"

Watershed::Watershed(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WatershedClass)
{
  ui->setupUi(this);

  //Menu actions:
  connect(ui->actionOpen_oryginal_image,   SIGNAL(triggered()),   this, SLOT(openOryginalImage()));
  connect(ui->actionSave_result,           SIGNAL(triggered()),   this, SLOT(saveResult()));
  connect(ui->actionAbout_Pandore,         SIGNAL(triggered()),   this, SLOT(showAboutPandoreMessage()));
  connect(ui->actionAbout_this_GUI,        SIGNAL(triggered()),   this, SLOT(showAboutThisGuiMessage()));

  connect(ui->optShowOryginalImage,        SIGNAL(clicked()),     this, SLOT(showOryginalImage()));
  connect(ui->optShowImageForComputations, SIGNAL(clicked()),     this, SLOT(showImageForComputations()));
  connect(ui->optShowInitialImage,         SIGNAL(clicked()),     this, SLOT(showInitialImage()));
  connect(ui->optSetGradientImage,         SIGNAL(clicked()),     this, SLOT(showImageForComputations()));
  connect(ui->optSetDistanceMap,           SIGNAL(clicked()),     this, SLOT(showImageForComputations()));
  connect(ui->optSetLocalMinimum,          SIGNAL(clicked()),     this, SLOT(showInitialImage()));
  connect(ui->optSetMarker,                SIGNAL(clicked()),     this, SLOT(showInitialImage()));
  connect(ui->optSetMarker,                SIGNAL(toggled(bool)), ui->imageLabel, SLOT(showMask(bool)));
  connect(ui->optSetBinaryImage,           SIGNAL(clicked()),     this, SLOT(showInitialImage()));

  connect(ui->bttBlurOryginalImage,        SIGNAL(clicked()),     this, SLOT(blurOryginalImage()));
  connect(ui->bttComputeGradient,          SIGNAL(clicked()),     this, SLOT(computeGradient()));
  connect(ui->bttComputeDistanceMap,       SIGNAL(clicked()),     this, SLOT(computeDistanceMap()));
  connect(ui->bttComputeLocalMinimumPoints,SIGNAL(clicked()),     this, SLOT(computeLocalMinimumPoints()));
  connect(ui->bttComputeBinaryImage,       SIGNAL(clicked()),     this, SLOT(computeBinaryImage()));
  connect(ui->bttRunWatershed,             SIGNAL(clicked()),     this, SLOT(runWatershed()));
  connect(ui->bttDrawMarkers,              SIGNAL(clicked(bool)),ui->imageLabel, SLOT(allowDrawing(bool)));
  connect(ui->bttClearMarkers ,            SIGNAL(clicked()),ui->imageLabel, SLOT(clearMask()));

  lastPath = QDir::currentPath();

  disableAllProperties();

  borderColor = Qt::red;
  refreshHelp();
}

Watershed::~Watershed()
{
    delete ui;
}

void Watershed::showAboutPandoreMessage()
{
  int ret = QMessageBox::information( this, tr("About Pandore"),
    tr("<p><b>Pandore</b> - An open source library of image processing operators developed in GREYC laboratory.</p>" \
      "<p><a href='http://www.greyc.ensicaen.fr/~regis/Pandore/index.html'>http://www.greyc.ensicaen.fr/~regis/Pandore/index.html</a>"),
    QMessageBox::Ok);
}

void Watershed::showAboutThisGuiMessage()
{
  int ret = QMessageBox::information( this, tr("About this GUI"),
    tr("<p><b>Pandore Watershed Interface</b> - A prototype interface for <i>Pandore</i> watershed algorithm " \
      "developed by Rafal Petryniak.</p>" \
      "<p>More info, source code, binaries for Windows and description how to compile for Linux can be found on the author web site:<br>"
      "<a href='http://sites.google.com/site/rafalpetryniakresearch/software/'>http://sites.google.com/site/rafalpetryniakresearch/software/</a></p>"),
    QMessageBox::Ok);
}


void Watershed::disableAllProperties()
{
  isOryginalImage_opened = false;
  isBluredImage_computed = false;
  isGradientImage_computed = false;
  isDistanceMapImage_computed = false;
  isLocalMinimaImage_computed = false;
  isBinaryImage_computed = false;
  isMarkerImage_drawed = false;
}

void Watershed::openOryginalImage()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open oryginal image"), lastPath, "BMP image (*.bmp)");
  if (!fileName.isEmpty()) {
    lastPath = (new QFileInfo(fileName))->dir().absolutePath();

    QImage image(fileName);
    if (image.isNull()) {
      QMessageBox::information(this, tr("Open image"), tr("Cannot load %1.").arg(fileName));
      isOryginalImage_opened=false;
      return;
    }
    oryginalImage = image;
    bluredImage = oryginalImage;
    ui->imageLabel->setImage(image);
    disableAllProperties();
    isBluredImage_computed = false;
    isOryginalImage_opened = true;
  }
  else
    isOryginalImage_opened = false;

  showOryginalImage();
  ui->optShowOryginalImage->setChecked(true);
}

void Watershed::loadAllInputImages()
{
  if ( !QFile::exists(inputFileName1) ) {
    QMessageBox::information(this, tr("Open image"), tr("Cannot find input images."));
    return;
  }

  oryginalImage.load(inputFileName1);
  lastPath = (new QFileInfo(inputFileName1))->dir().absolutePath();

  if (oryginalImage.isNull()) {
    QMessageBox::information(this, tr("Open image"), tr("Cannot load input images."));
    return;
  }

  bluredImage = oryginalImage;
  ui->imageLabel->setImage(bluredImage);
  disableAllProperties();
  isBluredImage_computed = false;
  isOryginalImage_opened = true;
  showOryginalImage();
  ui->optShowOryginalImage->setChecked(true);
}

void Watershed::saveResult()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                              tr("Save result"),
                              lastPath,
                              tr("Pandore region image (*.pan)"));
  if (!fileName.isEmpty()) {
    lastPath = (new QFileInfo(fileName))->dir().absolutePath();
    savePandoreRegionImage(pRegionImage, fileName);
  }
}

void Watershed::showOryginalImage()
{
  ui->imageLabel->setImage(bluredImage);
}

void Watershed::showImageForComputations()
{
  if(ui->optShowImageForComputations->isChecked()) {
    if (ui->optSetGradientImage->isChecked())
      ui->imageLabel->setImage(isGradientImage_computed ? gradientImage:bluredImage);
    else
    if (ui->optSetDistanceMap->isChecked())
      ui->imageLabel->setImage(isDistanceMapImage_computed ? distanceMapImage : bluredImage);
  }
}

void Watershed::showInitialImage()
{
  if(ui->optShowInitialImage->isChecked()) {
    if (ui->optSetLocalMinimum->isChecked())
      ui->imageLabel->setImage(isLocalMinimaImage_computed ? localMinimaImage:bluredImage);
    else
    if (ui->optSetMarker->isChecked())
      ui->imageLabel->setImage(ui->imageLabel->getImage());
    else
    if (ui->optSetBinaryImage->isChecked())
      ui->imageLabel->setImage(isBinaryImage_computed ? binaryImage:bluredImage);
  }
}

void Watershed::blurOryginalImage()
{
  bool ok;
  double sigma = QInputDialog::getDouble(this, tr("Gaussian filtering"),
                                               tr("Sigma:"), 2.0, 0, 100, 2, &ok);

  if (ok) {
    bluredImage = blurAlgorithm( oryginalImage, sigma );
    isBluredImage_computed = true;
    showOryginalImage();
  }
}

void Watershed::computeGradient()
{
  gradientImage = gradientAlgorithm( bluredImage );
  isGradientImage_computed = true;
  showImageForComputations();
}

void Watershed::computeBinaryImage()
{
  bool ok;
  int threshold = QInputDialog::getInt(this, tr("Binarization"),
                                             tr("Threshold value:"), 128, 0, 255, 1, &ok);

  if (ok) {
    binaryImage = binarizationAlgorithm( oryginalImage, threshold );
    isBinaryImage_computed = true;
    showInitialImage();
  }
}

void Watershed::computeDistanceMap()
{
  if (!isBinaryImage_computed) computeBinaryImage();

  distanceMapImage = distanceMapAlgorithm( binaryImage );
  isDistanceMapImage_computed = true;
  showImageForComputations();
}

void Watershed::computeLocalMinimumPoints()
{
  localMinimaImage = localMinimumPointsAlgorithm( bluredImage );
  isLocalMinimaImage_computed = true;
  showInitialImage();
}

void Watershed::runWatershed()
{
  QMessageBox::StandardButton reply;
  QImage imageForComputations, initialImage;

  //Check and set image for computation
  if (ui->optSetGradientImage->isChecked()) {
    if (!isGradientImage_computed) {
      reply = QMessageBox::question(this, tr("Run Watershed algorithm..."),
                                          "Gradient image checked but not computed. \nCompute it now?",
                                          QMessageBox::Yes | QMessageBox::No  );
      if (reply == QMessageBox::Yes) computeGradient(); else return;
    }
    imageForComputations = gradientImage;
  } else
  if (ui->optSetDistanceMap->isChecked()) {
    if (!isDistanceMapImage_computed) {
      reply = QMessageBox::question(this, tr("Run Watershed algorithm..."),
                                          "Distance map checked but not computed. \nCompute it now?",
                                          QMessageBox::Yes | QMessageBox::No  );
      if (reply == QMessageBox::Yes) computeDistanceMap(); else return;
    }
    imageForComputations = distanceMapImage;
  }

  //Check and set initial image
  if (ui->optSetLocalMinimum->isChecked()) {
    if (!isLocalMinimaImage_computed) {
      reply = QMessageBox::question(this, tr("Run Watershed algorithm..."),
                                          "Local minima points checked but not computed. \nCompute it now?",
                                          QMessageBox::Yes | QMessageBox::No  );
      if (reply == QMessageBox::Yes) computeLocalMinimumPoints(); else return;
    }
    initialImage = localMinimaImage;
  } else
  if (ui->optSetBinaryImage->isChecked()) {
    if (!isBinaryImage_computed) {
      reply = QMessageBox::question(this, tr("Run Watershed algorithm..."),
                                          "Binary image checked but not computed. \nCompute it now?",
                                          QMessageBox::Yes | QMessageBox::No  );
      if (reply == QMessageBox::Yes) computeBinaryImage(); else return;
    }
    initialImage = binaryImage;
  } else
  if (ui->optSetMarker->isChecked()) {
    if (!ui->imageLabel->isDrawnOnMask()) {
      reply = QMessageBox::question(this, tr("Run Watershed algorithm..."),
                                          "Marker image checked but not drown. \nDraw it now?",
                                          QMessageBox::Yes | QMessageBox::No );
      if (reply == QMessageBox::Yes) { ui->bttDrawMarkers->setChecked(true); ui->imageLabel->allowDrawing(true); }
      return;
    }
    else
      initialImage = ui->imageLabel->getMask();
  }

  pRegionImage = watershedAlgorithm( imageForComputations, initialImage);

  CImg<unsigned int> cimgRegionImage = panreg2cimg( *pRegionImage.rg );
  grayRegionImageTmp  = cimg2qt( cimgRegionImage );
  colorRegionImageTmp = cimg2qt( colorizeLabelImage(cimgRegionImage) );
  QImage imageWithLabels = addLabelBoundaryToImage(bluredImage, pRegionImage, borderColor);
  ui->imageLabel->setImage(imageWithLabels);
}

void Watershed::refreshHelp()
{
  QString currentDir = QDir::currentPath();
}

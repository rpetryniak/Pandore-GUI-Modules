#include <QtGui>
#include <QtDebug>

#include "regionmerging.h"
#include "ui_regionmerging.h"

#include "functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
  ui->setupUi(this);
  ui->scrollArea->setWidget(&imageLabel);

  //Menu actions:
  connect(ui->actionOpen_oryginal_image,        SIGNAL(triggered()), this, SLOT(openOryginalImage()));
  connect(ui->actionOpen_image_for_computation, SIGNAL(triggered()), this, SLOT(openImageForComputation()));
  connect(ui->actionOpen_prelabeled_image,      SIGNAL(triggered()), this, SLOT(openPrelabeledImage()));
  connect(ui->actionLoad_test_images,           SIGNAL(triggered()), this, SLOT(loadAllInputImages()));
  connect(ui->actionSave_result,                SIGNAL(triggered()), this, SLOT(saveResult()));
  connect(ui->actionAbout_Pandore,              SIGNAL(triggered()), this, SLOT(showAboutPandoreMessage()));
  connect(ui->actionAbout_this_GUI,             SIGNAL(triggered()), this, SLOT(showAboutThisGuiMessage()));

  //Display actions:
  connect(ui->optShowOryginalImage,             SIGNAL(clicked()),   this, SLOT(showOryginalImage()));
  connect(ui->optShowImageForComputations,      SIGNAL(clicked()),   this, SLOT(showImageForComputations()));
  connect(ui->optShowLabeledImage,              SIGNAL(clicked()),   this, SLOT(showLabeledImage()));
  connect(ui->optShowColorLabeledImage,         SIGNAL(clicked()),   this, SLOT(showColorLabeledImage()));
  connect(ui->optShowImageWithTransparentLabels,SIGNAL(clicked()),   this, SLOT(showImageWithTransparentLabels()));
  connect(ui->optShowImageWithLabelBorder,      SIGNAL(clicked()),   this, SLOT(showImageWithLabelBorder()));
  connect(ui->borderColorChooser,               SIGNAL(clicked()),   this, SLOT(changeBorderColor()));

  connect(ui->valueTransparentMask,             SIGNAL(valueChanged(double)), this, SLOT(changeTransparentForLabels(double)));
  connect(ui->bttRandomColorForLabel,           SIGNAL(clicked()),            this, SLOT(randomColorForLabel()));
  connect(ui->bttInvertLabelValue,              SIGNAL(clicked()),            this, SLOT(invertLabelValue()));

  //Algorithm actions:
  connect(ui->valueThreshold,   SIGNAL(valueChanged(int)),        this, SLOT(runAlgorithm()));
  connect(ui->valueNumber,      SIGNAL(valueChanged(int)),        this, SLOT(runAlgorithm()));
  connect(ui->cbAlgorithm,      SIGNAL(currentIndexChanged(int)), this, SLOT(runAlgorithm()));
  connect(ui->cbAlgorithm,      SIGNAL(currentIndexChanged(int)), this, SLOT(refreshHelp()));
  connect(ui->helpForAlgorithm, SIGNAL(clicked()),                this, SLOT(showHelpForAlgorithm()));

  //connect(ui->bttRunAlgorithm,  SIGNAL(clicked()),      this, SLOT(runAlgorithm()));

  isInputImage1_opened=false; isInputImage2_opened=false; isRegionImage_opened=false;

  lastPath = QDir::currentPath();

  borderColor = Qt::red;
  refreshHelp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAboutPandoreMessage()
{
  int ret = QMessageBox::information( this, tr("About Pandore"),
    tr("<p><b>Pandore</b> - An open source library of image processing operators developed in GREYC laboratory.</p>" \
      "<p><a href='http://www.greyc.ensicaen.fr/~regis/Pandore/index.html'>http://www.greyc.ensicaen.fr/~regis/Pandore/index.html</a>"),
    QMessageBox::Ok);
}

void MainWindow::showAboutThisGuiMessage()
{
  int ret = QMessageBox::information( this, tr("About this GUI"),
    tr("<p><b>Pandore Region Merging Interface</b> - A prototype interface for <i>Pandore</i> region merging operators " \
      "developed by Rafal Petryniak.</p>" \
      "<p>More info, source code, binaries for Windows and description how to compile for Linux can be found on the author web site:<br>"
      "<a href='http://sites.google.com/site/rafalpetryniakresearch/software/'>http://sites.google.com/site/rafalpetryniakresearch/software/</a></p>"),
    QMessageBox::Ok);
}

void MainWindow::openOryginalImage()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open oryginal image"), lastPath, "BMP image (*.bmp)");
  if (!fileName.isEmpty()) {
    lastPath = (new QFileInfo(fileName))->dir().absolutePath();

    QImage image(fileName);
    if (image.isNull()) {
      QMessageBox::information(this, tr("Open image"), tr("Cannot load %1.").arg(fileName));
      isInputImage1_opened=false;
      return;
    }
    imageLabel.setImage(image);
    inputImage1 = image;
    isInputImage1_opened=true;
  }
  else
    isInputImage1_opened=false;

  showOryginalImage();
  ui->optShowOryginalImage->setChecked(true);

  if (isInputImage1_opened && isInputImage2_opened && isRegionImage_opened) {
    ui->groupBox->setEnabled(true);
    ui->groupBox->setToolTip("");
  }
}

void MainWindow::openImageForComputation()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open image for computation"), lastPath, "BMP image (*.bmp)");
  if (!fileName.isEmpty()) {
    lastPath = (new QFileInfo(fileName))->dir().absolutePath();
    QImage image(fileName);
    if (image.isNull()) {
      QMessageBox::information(this, tr("Open image"), tr("Cannot load %1.").arg(fileName));
      isInputImage2_opened=false;
      return;
    }
    imageLabel.setImage(image);
    inputImage2 = image;
    isInputImage2_opened=true;
  }
  else
    isInputImage2_opened=false;

  showImageForComputations();
  ui->optShowImageForComputations->setChecked(true);

  if (isInputImage1_opened && isInputImage2_opened && isRegionImage_opened) {
    ui->groupBox->setEnabled(true);
    ui->groupBox->setToolTip("");
  }
}

void MainWindow::openPrelabeledImage()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open prelabeled image"), lastPath, "Pandore region image (*.pan)");
  if (!fileName.isEmpty()) {
    lastPath = (new QFileInfo(fileName))->dir().absolutePath();
    pRegionImage  = loadRegionImage(fileName);
    pRegionImage0 = pRegionImage;
    if (!pRegionImage.loadStatus) {
      QMessageBox::information(this, tr("Open image"), tr("Cannot load %1.").arg(fileName));
      isRegionImage_opened=false;
      return;
    }
    CImg<unsigned int> cimgRegionImage = panreg2cimg( *pRegionImage.rg );
    grayRegionImageTmp  = cimg2qt( cimgRegionImage );
    colorRegionImageTmp = cimg2qt( colorizeLabelImage(cimgRegionImage, true) );
    isRegionImage_opened=true;
  }
  else
    isRegionImage_opened=false;

  showColorLabeledImage();
  ui->optShowColorLabeledImage->setChecked(true);

  if (isInputImage1_opened && isInputImage2_opened && isRegionImage_opened) {
    ui->groupBox->setEnabled(true);
    ui->groupBox->setToolTip("");
  }
}

void MainWindow::loadAllInputImages()
{
  if ( !QFile::exists(inputFileName1) || !QFile::exists(inputFileName2) || !QFile::exists(regionFileName) ) {
    QMessageBox::information(this, tr("Open image"), tr("Cannot find input images."));
    return;
  }

  inputImage1.load(inputFileName1), inputImage2.load(inputFileName2);
  pRegionImage  = loadRegionImage(regionFileName);
  pRegionImage0 = pRegionImage;
  lastPath = (new QFileInfo(inputFileName1))->dir().absolutePath();

  if (inputImage1.isNull() || inputImage2.isNull() || !pRegionImage.loadStatus) {
    QMessageBox::information(this, tr("Open image"), tr("Cannot load input images."));
    return;
  }

  CImg<unsigned int> cimgRegionImage = panreg2cimg( *pRegionImage.rg );
  grayRegionImageTmp  = cimg2qt( cimgRegionImage );
  colorRegionImageTmp = cimg2qt( colorizeLabelImage(cimgRegionImage, true) );

  imageLabel.setImage(inputImage1);

  isInputImage1_opened=true; isInputImage2_opened=true; isRegionImage_opened=true;
  ui->groupBox->setEnabled(true); ui->groupBox->setToolTip("");
}

void MainWindow::saveResult()
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

void MainWindow::showOryginalImage()
{
  imageLabel.setImage(inputImage1);
}

void MainWindow::showImageForComputations()
{
  imageLabel.setImage(inputImage2);
}

void MainWindow::showLabeledImage()
{
  imageLabel.setImage(grayRegionImageTmp);
}

void MainWindow::invertLabelValue()
{
  grayRegionImageTmp.invertPixels();
  imageLabel.setImage(grayRegionImageTmp);
}

void MainWindow::showColorLabeledImage()
{
  imageLabel.setImage(colorRegionImageTmp);
}

void MainWindow::randomColorForLabel()
{
  CImg<unsigned int> cimgRegionImage = panreg2cimg( *pRegionImage.rg );
  colorRegionImageTmp = cimg2qt( colorizeLabelImage(cimgRegionImage, true) );

  showColorLabeledImage();
}

void MainWindow::showImageWithTransparentLabels()
{
  changeTransparentForLabels(ui->valueTransparentMask->value());
}

void MainWindow::changeTransparentForLabels(double transparent)
{
  QImage imageWithLabels = addLabelToImageWithTransparency(inputImage1, colorRegionImageTmp, transparent);
  imageLabel.setImage(imageWithLabels);
}

void MainWindow::showImageWithLabelBorder()
{
  changeBorderColor(borderColor);
}

void MainWindow::changeBorderColor()
{
  QColor color = QColorDialog::getColor(Qt::red, this);
  if (color.isValid()) {
    borderColor = color;
    changeBorderColor(borderColor);
  }
  else
    changeBorderColor(Qt::red);
}

void MainWindow::changeBorderColor(QColor color)
{
  QImage imageWithLabels = addLabelBoundaryToImage(inputImage1, pRegionImage, color);
  imageLabel.setImage(imageWithLabels);
}

void MainWindow::runAlgorithm()
{
  int number      = ui->valueNumber->value();
  float threshold = ui->valueThreshold->value();
  int alg         = ui->cbAlgorithm->currentIndex();
  if (alg==3 || alg==6) threshold=threshold/100;
  ui->lcdThreshold->display(threshold);

  pRegionImage = mergingAlgorithm( inputImage2, pRegionImage0, number, threshold, alg);

  refreshData();
}

void MainWindow::showHelpForAlgorithm()
{
  helpWindow.show();
}

void MainWindow::refreshHelp()
{
  QString currentDir = QDir::currentPath();

  switch(ui->cbAlgorithm->currentIndex() ) {
    case 0: //pcontrastmerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on contrast criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values are from the gray scale of the input image.");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(255);
      //helpWindow.setHelpFile(currentDir + "/doc/pcontrastmerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/pcontrastmerging.html");
      break;
    case 1: //pentropymerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on entropy criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. A typical value is 0.");
      ui->valueThreshold->setMinimum(-10);
      ui->valueThreshold->setMaximum(10);
      //helpWindow.setHelpFile(currentDir + "/doc/pentropymerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/pentropymerging.html");
      break;
    case 2: //pmeanmerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on mean criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values are from the gray scale of the input image.");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(255);
      //helpWindow.setHelpFile(currentDir + "/doc/pmeanmerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/pmeanmerging.html");
      break;
    case 3: //puniformitymerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on uniformity criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values are from  [0..1], where 0 corresponds to non uniform regions, and 1 corresponds to highly uniform regions. A typical value is 0.95.");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(100);
      //helpWindow.setHelpFile(currentDir + "/doc/puniformitymerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/puniformitymerging.html");
      break;
    case 4: //pvariancemerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on variance criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values are from the gray scale of the input image.");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(255);
      //helpWindow.setHelpFile(currentDir + "/doc/pvariancemerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/pvariancemerging.html");
      break;
    case 5: //pboundarymerging
      ui->cbAlgorithm->setToolTip("Performs priority region merging based on uniformity criterion.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values are from the gray scale of the input image.");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(255);
      //helpWindow.setHelpFile(currentDir + "/doc/pboundarymerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/pboundarymerging.html");
      break;
    case 6: //phistomerging
      ui->cbAlgorithm->setToolTip("Perfoms priority region merging based on histogram correlation.");
      ui->valueNumber->setToolTip("<i>number</i> specifies the number of allowed merging. If <i>number</i> = -1 then all possible merging are done.");
      ui->valueThreshold->setToolTip("<i>threshold</i> specifies the maximum difference allowed between two regions to decide to merge them. Values belongs to [0..1], where 1 corresponds to two equal histograms. Typical values are close to 1 (e.g., 0.7).");
      ui->valueThreshold->setMinimum(0);
      ui->valueThreshold->setMaximum(100);
      //helpWindow.setHelpFile(currentDir + "/doc/phistomerging.html");
      helpWindow.setHelpFile("http://www.greyc.ensicaen.fr/~regis/Pandore/programmes/en/operatorsP0/phistomerging.html");
      break;
  }
}

void MainWindow::refreshData()
{
  if (ui->optShowOryginalImage->isChecked())
    showOryginalImage();
  else
    if (ui->optShowImageForComputations->isChecked())
      showImageForComputations();
  else {
    CImg<unsigned int> cimgRegionImage = panreg2cimg( *pRegionImage.rg );
    grayRegionImageTmp  = cimg2qt( cimgRegionImage );
    colorRegionImageTmp = cimg2qt( colorizeLabelImage(cimgRegionImage) );

    if (ui->optShowLabeledImage->isChecked())
      showLabeledImage();
    else
    if (ui->optShowColorLabeledImage->isChecked())
      showColorLabeledImage();
    else
    if (ui->optShowImageWithTransparentLabels->isChecked())
      changeTransparentForLabels(ui->valueTransparentMask->value());
    else
    if (ui->optShowImageWithLabelBorder->isChecked())
      changeBorderColor(borderColor);
  }
}

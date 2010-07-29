#ifndef REGIONMERGING_H
#define REGIONMERGING_H

#include <QtGui/QMainWindow>

#include "helpwindow.h"
#include "functions.h"
#include "imagecanvas.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

  void loadAllInputImages();

  void setFileName1(QString fileName) { inputFileName1 = fileName; }
  void setFileName2(QString fileName) { inputFileName2 = fileName; }
  void setFileName3(QString fileName) { regionFileName = fileName; }

private slots:
  void showAboutPandoreMessage();
  void showAboutThisGuiMessage();

	void openOryginalImage();
	void openImageForComputation();
	void openPrelabeledImage();
  void saveResult();

	void showOryginalImage();
	void showImageForComputations();
	void showLabeledImage();
	void invertLabelValue();
	void showColorLabeledImage();
	void randomColorForLabel();
	void showImageWithTransparentLabels();
	void changeTransparentForLabels(double transparent);
	void showImageWithLabelBorder();
	void changeBorderColor();
	void changeBorderColor(QColor color);
	void runAlgorithm();
  void refreshData();
	void refreshHelp();
	void showHelpForAlgorithm();
private:
	Ui::MainWindowClass *ui;
  ImageCanvas imageLabel;
	HelpWindow helpWindow;

  QImage inputImage1, inputImage2, grayRegionImageTmp, colorRegionImageTmp;
  PRegionImage pRegionImage0, pRegionImage;

  bool isInputImage1_opened, isInputImage2_opened, isRegionImage_opened;
	QColor borderColor;

  QString inputFileName1;
  QString inputFileName2;
  QString regionFileName;

  QString lastPath;
};

#endif // REGIONMERGING_H

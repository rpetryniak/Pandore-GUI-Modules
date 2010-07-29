#include <QtGui>

#include "imagecanvas.h"

ImageCanvas::ImageCanvas(QWidget *parent)
: QLabel(parent)
{
  allow_drawing     = false;
  show_mask         = false;
  is_drawn_on_mask  = false;
  xoffset     = 0;
  yoffset     = 0;
  centerImage = true;
  shapeSize   = 15;
  
  if (allow_drawing) updateCursor(shapeSize);
}

void ImageCanvas::setImage(QImage image)
{
  if (this->image.size() != image.size()) {
    QBitmap bitmap( image.width(),image.height() );
    bitmap.clear();
    this->mask = bitmap;
  }
  
  this->image = image;
  setPixmap(QPixmap::fromImage(image));
  adjustSize();
  setAlignment(Qt::AlignCenter);
}

QImage ImageCanvas::getImage()
{
  return image;
}

void ImageCanvas::setMask(QImage image)
{
  this->mask = QBitmap::fromImage(image);
  update();
}

QImage ImageCanvas::getMask()
{
  QImage t_mask = this->mask.toImage();
  t_mask.convertToFormat(QImage::Format_RGB32);
  t_mask.invertPixels();
  return t_mask;
}

void ImageCanvas::showMask(bool show)
{
  show_mask = show;
}

void ImageCanvas::allowDrawing(bool allow)
{
  allow_drawing = allow;
  if (allow_drawing) show_mask = true;
}

void ImageCanvas::clearMask()
{
  mask.clear();
  is_drawn_on_mask = false;
  update();
}

bool ImageCanvas::isDrawnOnMask()
{
  return is_drawn_on_mask;
}

void ImageCanvas::paintEvent(QPaintEvent * event)
{
  if (allow_drawing || show_mask) {
    QPainter painter(this);
    if (centerImage) painter.translate(xoffset,yoffset);
    painter.drawImage(QPoint(0, 0), image);
    painter.setPen( QColor(0,255,0,70) );
    painter.setBrush( QColor(0,255,0,0) );
    painter.drawPixmap(QPoint(0, 0), mask);
  }
  else
    QLabel::paintEvent(event);
}

void ImageCanvas::mousePressEvent(QMouseEvent *event)
{
  
}

void ImageCanvas::mouseMoveEvent(QMouseEvent *event)
{
  if (!allow_drawing) {
    QLabel::mouseMoveEvent(event);
  }
  else {
    QPainter painter(&mask);
    if (centerImage) painter.translate(-xoffset,-yoffset);
    if ( (event->buttons() & Qt::LeftButton) ) {
      painter.setPen( Qt::black );
      painter.setBrush( Qt::black );
    } else
    if ( (event->buttons() & Qt::RightButton) ) {
      painter.setPen( Qt::white );
      painter.setBrush( Qt::white );
    }
    painter.drawEllipse( event->x()-(shapeSize/2),event->y()-(shapeSize/2),shapeSize,shapeSize);
    is_drawn_on_mask = true;
    update();
  }
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent *event)
{
  QLabel::mouseReleaseEvent(event);
}

void ImageCanvas::wheelEvent ( QWheelEvent * event )
{
  if (!allow_drawing) {
    QLabel::wheelEvent(event);
  }
  else {
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    int size = shapeSize+numSteps;
    
    if (size > 0 && size < image.width() )
      shapeSize = size;
    
    updateCursor(shapeSize);
  }
}

void ImageCanvas::updateCursor(int size)
{
  QPixmap pixm(size+2,size+2);
  
  pixm.fill(Qt::transparent);
  
  QPainter painter(&pixm);
  painter.drawEllipse(1,1,size,size);
  painter.end();
  
  QCursor cursor(pixm);
  this->setCursor( cursor );
}

void ImageCanvas::resizeEvent(QResizeEvent *event)
{
  if (width() > image.width() || height() > image.height()) {
    xoffset = (width() -image.width())/2;
    yoffset = (height()-image.height())/2;
  }
  else {
    xoffset = 0;
    yoffset = 0;
  }
  update();
  QWidget::resizeEvent(event);
}

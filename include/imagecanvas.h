#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QLabel>
#include <QBitmap>

class ImageCanvas : public QLabel
{
  Q_OBJECT
  
  public:
    ImageCanvas(QWidget *parent = 0);
    
    void setImage(QImage image);
    QImage getImage();
    void setMask(QImage image);
    QImage getMask();
    bool isDrawnOnMask();

  public slots:
    void allowDrawing(bool allow);
    void clearMask();
    void showMask(bool show);

  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent * event);
    void resizeEvent(QResizeEvent *event);
    
  private:
    void updateCursor(int size);
    
    QImage image;
    QBitmap mask;

    bool allow_drawing;
    bool show_mask;
    bool is_drawn_on_mask;
    int  shapeSize;
    int  xoffset;
    int  yoffset;
    bool centerImage;
};

#endif

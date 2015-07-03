#ifndef COLOR_DISPLAY_WIDGET_H
#define COLOR_DISPLAY_WIDGET_H

#include <QtGui>
#include "one_digit_spinbox.h"

class CColorDisplayWidget : public QWidget
{
  Q_OBJECT
  public:
    CColorDisplayWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
   ~CColorDisplayWidget(void);

  public slots:
    void setColor(QRgb color);

  protected:
    void paintEvent(QPaintEvent* event);

  private:
    QRgb m_color;
};

class CColorWidget : public QGroupBox
{
  Q_OBJECT
  public:
    CColorWidget(QWidget* parent = nullptr);
   ~CColorWidget(void);

  signals:
    void currentColor(QRgb color);

  public slots:
    void setColor(QRgb color);
    void sendCurrentColor(void);

  private slots:
    void updateColor(void);

  private:
    COneDigitSpinbox* m_color_r;
    COneDigitSpinbox* m_color_g;
    COneDigitSpinbox* m_color_b;
    QRgb m_color;

    CColorDisplayWidget* m_disply_widget;
};

#endif

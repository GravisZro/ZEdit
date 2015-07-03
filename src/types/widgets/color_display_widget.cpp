#include "color_display_widget.h"

#include <QSpinBox>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <common/color_converters.h>

CColorDisplayWidget::CColorDisplayWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_color(0)
{
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setMinimumSize(48, 48);
  setMaximumSize(48, 48);
}

CColorDisplayWidget::~CColorDisplayWidget(void)
{
}

void CColorDisplayWidget::setColor(QRgb color)
{
  m_color = color;
  update();
}

void CColorDisplayWidget::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.fillRect(rect(), m_color);
}


//==================================================================================


CColorWidget::CColorWidget(QWidget* parent)
  : QGroupBox("Current Color", parent)
{
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  m_color_r = new COneDigitSpinbox(0, 7);
  m_color_g = new COneDigitSpinbox(0, 7);
  m_color_b = new COneDigitSpinbox(0, 7);

  QPixmap* r = new QPixmap(QSize(16, 16)); r->fill(Qt::red);
  QPixmap* g = new QPixmap(QSize(16, 16)); g->fill(Qt::green);
  QPixmap* b = new QPixmap(QSize(16, 16)); b->fill(Qt::blue);

  QLabel* label;

  QGridLayout* grid = new QGridLayout();
  setLayout(grid);

  label = new QLabel("R");
  grid->addWidget(label, 0, 1);
  label = new QLabel();
  label->setPixmap(*r);
  grid->addWidget(label, 0, 2);
  grid->addWidget(m_color_r, 0, 3);

  label = new QLabel("G");
  grid->addWidget(label, 1, 1);
  label = new QLabel();
  label->setPixmap(*g);
  grid->addWidget(label, 1, 2);
  grid->addWidget(m_color_g, 1, 3);

  label = new QLabel("B");
  grid->addWidget(label, 2, 1);
  label = new QLabel();
  label->setPixmap(*b);
  grid->addWidget(label, 2, 2);
  grid->addWidget(m_color_b, 2, 3);

  m_disply_widget = new CColorDisplayWidget();
  grid->addWidget(m_disply_widget, 0, 4, 3, 1);
  grid->setColumnStretch(0, 2);
  grid->setColumnStretch(4, 2);

  connect(m_color_r, SIGNAL(valueChanged(int)), this, SLOT(updateColor()), Qt::DirectConnection);
  connect(m_color_g, SIGNAL(valueChanged(int)), this, SLOT(updateColor()), Qt::DirectConnection);
  connect(m_color_b, SIGNAL(valueChanged(int)), this, SLOT(updateColor()), Qt::DirectConnection);
}

CColorWidget::~CColorWidget(void)
{
}

void CColorWidget::setColor(QRgb color)
{
  if(m_color != color)
  {
    // dont emit a signal just because spinboxes changed
    m_color_r->blockSignals(true);
    m_color_g->blockSignals(true);
    m_color_b->blockSignals(true);

    m_color = color;
    QRgb color9bit = convertFrom24To9Bit(color);
    m_color_r->setValue(qRed(color9bit));
    m_color_g->setValue(qGreen(color9bit));
    m_color_b->setValue(qBlue(color9bit));

    m_disply_widget->setColor(m_color);

    m_color_r->blockSignals(false);
    m_color_g->blockSignals(false);
    m_color_b->blockSignals(false);
  }
}


void CColorWidget::updateColor(void)
{
  QRgb color = qRgb(channelFrom9BitTo24(m_color_r->value()),
                    channelFrom9BitTo24(m_color_g->value()),
                    channelFrom9BitTo24(m_color_b->value()));
  if(m_color != color)
  {
    setColor(color);
    emit currentColor(color);
  }
}

void CColorWidget::sendCurrentColor(void)
{
  QRgb color = qRgb(channelFrom9BitTo24(m_color_r->value()),
                    channelFrom9BitTo24(m_color_g->value()),
                    channelFrom9BitTo24(m_color_b->value()));

  emit currentColor(color);
}

#include "palette_buttons.h"
#include <types/widgets/colored_push_button.h>

#include <common/global.h>
#include <types/misc/pce_types.h>
#include <common/color_converters.h>

CPaletteButtons::CPaletteButtons(QSize button_size, QWidget* parent)
  : CPaletteButtons(8, button_size, parent)
{
}

CPaletteButtons::CPaletteButtons(int num_buttons_x, QWidget* parent)
  : CPaletteButtons(num_buttons_x, QSize(20, 20), parent)
{
}

CPaletteButtons::CPaletteButtons(QWidget* parent)
  : CPaletteButtons(8, QSize(20, 20), parent)
{
}

CPaletteButtons::CPaletteButtons(QSize button_size, int num_buttons_x, QWidget* parent)
  : CPaletteButtons(num_buttons_x, button_size, parent)
{
}

CPaletteButtons::CPaletteButtons(int num_buttons_x, QSize button_size, QWidget* parent)
  : QWidget(parent),
    m_block_id(-1),
    m_pal_id(-1),
    m_requester(-1)
{
  CColoredPushButton* button;

  QGridLayout* grid = new QGridLayout(this);

  m_group = new QButtonGroup(this);
  for(int i = 0; i < ZValues::colors_in_palette; ++i)
  {
    button = new CColoredPushButton();
    button->setSize(button_size);
    button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button->setColor(0);
    button->enableMenu(true);
    m_group->addButton(button, i);
    grid->addWidget(button, i / num_buttons_x, i % num_buttons_x);
    connect(button, SIGNAL(currentColor(QRgb)), this, SIGNAL(currentColor(QRgb)), Qt::DirectConnection);
    connect(button, SIGNAL(requestCurrentColor()), this, SLOT(saveRequestInfo()), Qt::DirectConnection);
  }

  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CPaletteButtons::saveRequestInfo(void)
{
  m_requester = m_group->id(qobject_cast<QAbstractButton*>(sender()));
  emit requestCurrentColor();
}

int CPaletteButtons::paletteNumber(void)
{ // m_block_id will be set to -1 if m_pal_id is set
  return (m_block_id > -1) ? GData::GetFileData().get_image_pal_idx(m_block_id) : m_pal_id;
}

void CPaletteButtons::setPaletteID(uint id)
{
  m_pal_id = id;
  m_block_id = -1;
  update();
}

void CPaletteButtons::setBlockID(uint id)
{
  m_block_id = id;
  update();
}

void CPaletteButtons::update(void)
{
  CColoredPushButton* button;
  QVector<QRgb> palette = GData::GetPaletteNum(paletteNumber());

  button = qobject_cast<CColoredPushButton*>(m_group->button(0));

  static const char* const xpm2[] = { "4 4 2 1","0 c #555555","1 c #888888", "0101", "1010", "0101", "1010"};
  button->setIcon(QPixmap(xpm2).scaled(button->sizeHint()));

  for(int i = 1; i < ZValues::colors_in_palette; ++i)
  {
    button = qobject_cast<CColoredPushButton*>(m_group->button(i));
    button->setColor(palette[i]);
  }
  QWidget::update();
}


CCompositePalette::CCompositePalette(int num_buttons_x, QSize button_size, QWidget* parent)
  : QWidget(parent),
    m_num_buttons_x(num_buttons_x),
    m_button_size(button_size)
{
  QGridLayout* grid = new QGridLayout(this);
  m_group = new QButtonGroup(this);

  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void CCompositePalette::update(void)
{
  CColoredPushButton* button;
  QGridLayout* grid = qobject_cast<QGridLayout*>(layout());
  QVector<QRgb> palette;
  QSet<QRgb> raw_palette;

  std::vector<CPaletteData> palettes = GData::GetFileData().tiles.palettes;
  palettes.insert(palettes.end(), GData::GetFileData().sprites.palettes.begin(), GData::GetFileData().sprites.palettes.end());
  for(auto pos = palettes.begin(); pos != palettes.end(); ++pos)
    raw_palette += pos->operator QVector<QRgb>().toList().toSet();

//  foreach(uint16_t raw_color : raw_colors) // C++11 foreach
  for(auto pos = raw_palette.begin(); pos != raw_palette.end(); ++pos)
    palette.push_back(convertFromNativeFormatTo24(*pos));

  auto pal_start = palette.begin();
  pal_start++;
  qSort(pal_start, palette.end(), colorSort);

  for(int i = palette.count(); i < grid->count(); ++i)
  {
    button = qobject_cast<CColoredPushButton*>(m_group->button(i));
    button->hide();
  }

  for(int i = grid->count(); i < palette.count(); ++i)
  {
    button = new CColoredPushButton();
    button->setSize(m_button_size);
    button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    button->setColor(0);
    m_group->addButton(button, i);
    grid->addWidget(button, i / m_num_buttons_x, i % m_num_buttons_x);
    connect(button, SIGNAL(currentColor(QRgb)), this, SIGNAL(currentColor(QRgb)), Qt::DirectConnection);
    connect(button, SIGNAL(requestCurrentColor()), this, SIGNAL(requestCurrentColor()), Qt::DirectConnection);
  }

  for(int i = 0; i < palette.count(); ++i)
  {
    button = qobject_cast<CColoredPushButton*>(m_group->button(i));
    button->setColor(palette[i]);
    button->show();
  }

  QWidget::update();
}

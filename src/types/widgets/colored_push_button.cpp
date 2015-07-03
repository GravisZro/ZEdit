#include "colored_push_button.h"

#include <types/widgets/specialized/palette_buttons.h>
#include <types/misc/pce_types.h>
#include <types/undo_commands/undo_data_alteration.h>
#include <types/misc/linked_image.h>


CColoredPushButton::CColoredPushButton(QWidget *parent)
  : QPushButton(parent),
    m_color(0),
    m_menu_enabled(false),
    m_replace_mode(eDontReplace)
{
  setFlat(false);
  m_menu = new QMenu();
  m_menu->addAction("Replace color globally", this, SLOT(replaceColorGlobally()));
  m_menu->addAction("Replace color in palette", this, SLOT(replaceColorInPalette()));
  m_menu->addAction("Replace color in image", this, SLOT(replaceColorInImage()));

  //m_menu->addAction("Replace Color (Experimental!)", this, SLOT(replaceColorInPalette()));

  connect(this, SIGNAL(clicked()), this, SLOT(sendCurrentColor()), Qt::DirectConnection);
}


QSize CColoredPushButton::sizeHint (void) const
{
  if(!m_size.isValid())
    return QPushButton::sizeHint();
  return m_size;
}

void CColoredPushButton::setColor(QRgb color)
{
  //CPaletteData pal; pal.deallocate();

  if(m_replace_mode == eLocalPaletteReplace ||
     m_replace_mode == eImageColorReplace)
  {
    QObject* obj = this;
    CPaletteButtons* button_palette = nullptr;
    while(!(button_palette = qobject_cast<CPaletteButtons*>(obj = obj->parent())))
      if(obj == nullptr)
        return;

    QLinkedImage* img = GData::GetImage(button_palette->blockID());
    if(m_replace_mode == eLocalPaletteReplace)
      img->setPaletteColor(m_color, color);
    else // replace in only this image
      img->setImageColor(m_color, color);

    //GData::ReinitializeAllImages();
    //img->update();
    //button_palette->update();
  }
  else if(m_replace_mode == eGlobalReplace) // replace in every palette
  {
/*
    CPaletteData pal(GData::GetFileData().get_palette(0, ZValues::max_palettes)); // get all palettes
    for(uint i = 0; i < ZValues::colors_in_palette * ZValues::max_palettes; ++i)
      if(pal.get(i) == m_color)
        pal.set(i, color);
*/
    GData::ReinitializeAllImages();
  }

  m_replace_mode = eDontReplace;

  if(m_color != color)
  {
    static QSize s_margin(4, 4);
    QPixmap pix(sizeHint() + s_margin);
    m_color = color;
    pix.fill(m_color);
    setIcon(QIcon(pix));
  }
}

void CColoredPushButton::sendCurrentColor(void)
{
  emit currentColor(m_color);
}


void CColoredPushButton::replaceColorGlobally(void) // replacing a color in the palette has global effects!!!
{
  m_replace_mode = eGlobalReplace;
  emit requestCurrentColor();
}

void CColoredPushButton::replaceColorInPalette(void)
{
  m_replace_mode = eLocalPaletteReplace;
  emit requestCurrentColor();
}

void CColoredPushButton::replaceColorInImage(void)
{
  m_replace_mode = eImageColorReplace;
  emit requestCurrentColor();
}

void CColoredPushButton::mousePressEvent(QMouseEvent* event)
{
  if(m_menu_enabled && event->button() == Qt::RightButton && event->modifiers() == Qt::NoModifier && group()->id(this) > 0)
    m_menu->popup(adjustedMenuPosition());
  else
    QPushButton::mousePressEvent(event);
}

QPoint CColoredPushButton::adjustedMenuPosition(void)
{

  bool horizontal = true;
#if !defined(QT_NO_TOOLBAR)
  QToolBar *tb = qobject_cast<QToolBar*>(parent());
  if (tb && tb->orientation() == Qt::Vertical)
    horizontal = false;
#endif

  QWidgetItem item(this);
  QRect rect = item.geometry();
  rect.setRect(rect.x() - x(), rect.y() - y(), rect.width(), rect.height());

  QSize menuSize = m_menu->sizeHint();
  QPoint globalPos = mapToGlobal(rect.topLeft());
  int x = globalPos.x();
  int y = globalPos.y();
  if (horizontal) {
    if (globalPos.y() + rect.height() + menuSize.height() <= QApplication::desktop()->availableGeometry().height()) {
      y += rect.height();
    } else {
      y -= menuSize.height();
    }
    if (layoutDirection() == Qt::RightToLeft)
      x += rect.width() - menuSize.width();
  } else {
    if (globalPos.x() + rect.width() + m_menu->sizeHint().width() <= QApplication::desktop()->availableGeometry().width())
      x += rect.width();
    else
      x -= menuSize.width();
  }

  return QPoint(x,y);
}

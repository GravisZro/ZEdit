#ifndef COLORED_PUSH_BUTTON_H
#define COLORED_PUSH_BUTTON_H

#include <QtGui>
#include <common/global.h>

class CColoredPushButton : public QPushButton
{
    Q_OBJECT
  private:
    enum EReplaceType
    {
      eDontReplace,
      eGlobalReplace,
      eLocalPaletteReplace,
      eImageColorReplace
    };
  public:
    CColoredPushButton(QWidget *parent = nullptr);

    QSize sizeHint (void) const;
    void setSize(QSize sz) { m_size = sz; }

    void enableMenu (bool enable  = true) { m_menu_enabled = enable; }
    void disableMenu(bool disable = true) { enableMenu(!disable); }

    QRgb color(void) const { return m_color; }

  public slots:
    void setColor(QRgb color);
    void sendCurrentColor(void);

  signals:
    void currentColor(QRgb color);
    void requestCurrentColor(void);

  protected:
    void mousePressEvent(QMouseEvent* event);

  private slots:
    void replaceColorGlobally(void);
    void replaceColorInPalette(void);
    void replaceColorInImage(void);

  private:
    QPoint adjustedMenuPosition(void);
  private:
    QSize m_size;
    QRgb m_color;
    bool m_menu_enabled;
    QMenu* m_menu;
    EReplaceType m_replace_mode;
};

#endif // COLORED_PUSH_BUTTON_H

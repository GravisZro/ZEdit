#ifndef PALETTE_BUTTONS_H
#define PALETTE_BUTTONS_H

#include <QtWidgets>

class CPaletteButtons : public QWidget
{
    Q_OBJECT
  public:
    CPaletteButtons(int num_buttons_x, QSize button_size, QWidget* parent = nullptr);
    CPaletteButtons(int num_buttons_x,                    QWidget* parent = nullptr);
    CPaletteButtons(QSize button_size, int num_buttons_x, QWidget* parent = nullptr);
    CPaletteButtons(QSize button_size,                    QWidget* parent = nullptr);
    CPaletteButtons(                                      QWidget* parent = nullptr);

    int paletteNumber(void);

    int blockID(void) const { return m_block_id; }

  public slots:
    void update(void);
    void setPaletteID(uint id);
    void setBlockID(uint id);
    //void setColor(QRgb color);

  signals:
    void currentColor(QRgb color);
    void requestCurrentColor(void);

  private slots:
    void saveRequestInfo(void);
  private:
    QButtonGroup* m_group;
    int32_t m_block_id;
    int32_t m_pal_id;
    int m_requester;
};


class CCompositePalette : public QWidget
{
    Q_OBJECT
  public:
    CCompositePalette(int num_buttons_x, QSize button_size, QWidget* parent = nullptr);

  public slots:
    void update(void);
    void setColor(QRgb color) { update(); }

  signals:
    void currentColor(QRgb color);
    void requestCurrentColor(void);

  private:
    QButtonGroup* m_group;
    int m_num_buttons_x;
    const QSize m_button_size;
};

#endif // PALETTE_BUTTONS_H

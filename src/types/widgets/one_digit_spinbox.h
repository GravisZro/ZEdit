#ifndef ONE_DIGIT_SPINBOX_H
#define ONE_DIGIT_SPINBOX_H

#include <QtWidgets>

class COneDigitSpinbox : public QSpinBox
{
    Q_OBJECT
  public:
    explicit COneDigitSpinbox(int min, int max, QWidget* parent = nullptr);
  private slots:
    void forceDeselect(void);
};

#endif // ONE_DIGIT_SPINBOX_H

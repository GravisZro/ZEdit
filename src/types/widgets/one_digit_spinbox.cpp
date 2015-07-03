#include "one_digit_spinbox.h"

COneDigitSpinbox::COneDigitSpinbox(int min, int max, QWidget* parent)
  : QSpinBox(parent)
{
  lineEdit()->setReadOnly(true);
  setRange(min, max);
  setMaximumWidth(36);

  connect(lineEdit(), SIGNAL(selectionChanged()), this, SLOT(forceDeselect()));
}


void COneDigitSpinbox::forceDeselect(void)
{
  lineEdit()->deselect();
}

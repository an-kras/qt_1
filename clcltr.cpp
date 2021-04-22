#include <QtGui>
#include <QLineEdit>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <cmath>
#include <QGridLayout>

#include "btn.h"
#include "clcltr.h"

 Calculator::Calculator(QWidget *parent)
     : QDialog(parent)
 {
     sumInMemory = 0.0;
     sumSoFar = 0.0;
     factorSoFar = 0.0;
     waitingForOperand = true;

     display = new QLineEdit("0");
     display->setReadOnly(true);
     display->setAlignment(Qt::AlignRight);
     display->setMaxLength(15);

     QFont font = display->font();
     font.setPointSize(font.pointSize() + 8);
     display->setFont(font);

     for (int i = 0; i < NumDigitButtons; ++i) {
         digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
     }

     Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
     Button *changeSignButton = createButton(tr("\302\261"), SLOT(changeSignClicked()));

     Button *backspaceButton = createButton(tr("Backspace"), SLOT(backspaceClicked()));
     Button *clearButton = createButton(tr("Clear"), SLOT(clear()));
     Button *clearAllButton = createButton(tr("Clear All"), SLOT(clearAll()));

     Button *clearMemoryButton = createButton(tr("MC"), SLOT(clearMemory()));
     Button *readMemoryButton = createButton(tr("MR"), SLOT(readMemory()));
     Button *setMemoryButton = createButton(tr("MS"), SLOT(setMemory()));
     Button *addToMemoryButton = createButton(tr("M+"), SLOT(addToMemory()));

     Button *divisionButton = createButton(tr("\303\267"), SLOT(multiplicativeOperatorClicked()));
     Button *timesButton = createButton(tr("\303\227"), SLOT(multiplicativeOperatorClicked()));
     Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
     Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));

     Button *squareRootButton = createButton(tr("Sqrt"), SLOT(unaryOperatorClicked()));
     Button *powerButton = createButton(tr("x\302\262"), SLOT(unaryOperatorClicked()));
     Button *reciprocalButton = createButton(tr("1/x"), SLOT(unaryOperatorClicked()));
     Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

     QGridLayout *mainLayout = new QGridLayout;
     mainLayout->setSizeConstraint(QLayout::SetFixedSize);

     mainLayout->addWidget(display, 0, 0, 1, 6);
     mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
     mainLayout->addWidget(clearButton, 1, 2, 1, 2);
     mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);

     mainLayout->addWidget(clearMemoryButton, 2, 0);
     mainLayout->addWidget(readMemoryButton, 3, 0);
     mainLayout->addWidget(setMemoryButton, 4, 0);
     mainLayout->addWidget(addToMemoryButton, 5, 0);

     for (int i = 1; i < NumDigitButtons; ++i) {
         int row = ((9 - i) / 3) + 2;
         int column = ((i - 1) % 3) + 1;
         mainLayout->addWidget(digitButtons[i], row, column);
     }

     mainLayout->addWidget(digitButtons[0], 5, 1);
     mainLayout->addWidget(pointButton, 5, 2);
     mainLayout->addWidget(changeSignButton, 5, 3);

     mainLayout->addWidget(divisionButton, 2, 4);
     mainLayout->addWidget(timesButton, 3, 4);
     mainLayout->addWidget(minusButton, 4, 4);
     mainLayout->addWidget(plusButton, 5, 4);

     mainLayout->addWidget(squareRootButton, 2, 5);
     mainLayout->addWidget(powerButton, 3, 5);
     mainLayout->addWidget(reciprocalButton, 4, 5);
     mainLayout->addWidget(equalButton, 5, 5);
     setLayout(mainLayout);

     setWindowTitle(tr("Calculator"));
 }

 void Calculator::digitClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     int digitValue = clickedButton->text().toInt();
     if (display->text() == "0" && digitValue == 0.0)
         return;

     if (waitingForOperand) {
         display->clear();
         waitingForOperand = false;
     }
     display->setText(display->text() + QString::number(digitValue));
 }

 void Calculator::unaryOperatorClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();
     double operand = display->text().toDouble();
     double result = 0.0;

     if (clickedOperator == tr("Sqrt")) {
         if (operand < 0.0) {
             abortOperation();
             return;
         }
         result = sqrt(operand);
     } else if (clickedOperator == tr("x\302\262")) {
         result = pow(operand, 2.0);
     } else if (clickedOperator == tr("1/x")) {
         if (operand == 0.0) {
             abortOperation();
             return;
         }
         result = 1.0 / operand;
     }
     display->setText(QString::number(result));
     waitingForOperand = true;
 }

 void Calculator::additiveOperatorClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();
     double operand = display->text().toDouble();

     if (!pendingMultiplicativeOperator.isEmpty()) {
         if (!calculate(operand, pendingMultiplicativeOperator)) {
             abortOperation();
             return;
         }
         display->setText(QString::number(factorSoFar));
         operand = factorSoFar;
         factorSoFar = 0.0;
         pendingMultiplicativeOperator.clear();
     }

     if (!pendingAdditiveOperator.isEmpty()) {
         if (!calculate(operand, pendingAdditiveOperator)) {
             abortOperation();
             return;
         }
         display->setText(QString::number(sumSoFar));
     } else {
         sumSoFar = operand;
     }

     pendingAdditiveOperator = clickedOperator;
     waitingForOperand = true;
 }

 void Calculator::multiplicativeOperatorClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();
     double operand = display->text().toDouble();

     if (!pendingMultiplicativeOperator.isEmpty()) {
         if (!calculate(operand, pendingMultiplicativeOperator)) {
             abortOperation();
             return;
         }
         display->setText(QString::number(factorSoFar));
     } else {
         factorSoFar = operand;
     }

     pendingMultiplicativeOperator = clickedOperator;
     waitingForOperand = true;
 }

 void Calculator::equalClicked()
 {
     double operand = display->text().toDouble();

     if (!pendingMultiplicativeOperator.isEmpty()) {
         if (!calculate(operand, pendingMultiplicativeOperator)) {
             abortOperation();
             return;
         }
         operand = factorSoFar; //сохраняет временное значение при выполнении умножений и делений
         factorSoFar = 0.0;
         pendingMultiplicativeOperator.clear();
     }
     if (!pendingAdditiveOperator.isEmpty()) {
         if (!calculate(operand, pendingAdditiveOperator)) {
             abortOperation();
             return;
         }
         pendingAdditiveOperator.clear();
     } else {
         sumSoFar = operand;
     }

     display->setText(QString::number(sumSoFar));
     sumSoFar = 0.0;
     waitingForOperand = true;
 }

 void Calculator::pointClicked()
 {
     if (waitingForOperand)
         display->setText("0");
     if (!display->text().contains("."))
         display->setText(display->text() + tr("."));
     waitingForOperand = false;
 }


 void Calculator::rounding0s()
 {
     QString text = display->text();
     double value = text.toDouble();
     // value = round(value); // что-то пошло не так
     if ((value > 0) && (value < 0.05))
     {
         value = 0;
     }

     /*if (waitingForOperand) {
         QString valueAsString = QString::number(value);
         QString mst1 = "01";
         if (valueAsString == mst1)
         {
             clearAll();
             display->setText(tr("###"));
         }*/
 }


         /*waitingForOperand = false;
         int k = 0, i = 0;
         double n = value;
         while(n > 0)
         {
             k = n; // k - первая цифра числа
             n /= 10;
             i++;
         }
         if(k == 0)
         {
             clearAll();
             display->setText(tr("###"));
         }*/

 void Calculator::clear0s()
 {
     double operand = display->text().toDouble();
     double current_number = (display->text()).toDouble();
     QString s_number = QString::number(current_number);
     display->setText(s_number);
 }

void Calculator::changeSignClicked()
{
     QString text = display->text();
     double value = text.toDouble();
     if (value > 0.0) {
         text.prepend(tr("-"));
     } else if (value < 0.0) {
         text.remove(0, 1);
     } else if (value == 0.0) // исправляем
     {
         value = 0.0;
     }
     display->setText(text);
}

void Calculator::backspaceClicked()
{
     if (waitingForOperand)
         return;

     QString text = display->text();
     text.chop(1);
     if (text.isEmpty() || text == "0") {
         text = "0";
         waitingForOperand = true;
     }

     display->setText(text);

}

 void Calculator::clear()
 {
     if (waitingForOperand)
         return;
     display->setText("0");
     waitingForOperand = true;
 }

 void Calculator::clearAll()
 {
     sumSoFar = 0.0;
     factorSoFar = 0.0;
     pendingAdditiveOperator.clear();
     pendingMultiplicativeOperator.clear();
     display->setText("0");
     waitingForOperand = true;
 }

 void Calculator::clearMemory()
 {
     sumInMemory = 0.0;
 }

 void Calculator::readMemory()
 {
     display->setText(QString::number(sumInMemory));
     waitingForOperand = true;
 }

 void Calculator::setMemory()
 {
     equalClicked();
     sumInMemory = display->text().toDouble();
 }

 void Calculator::addToMemory()
  {
      equalClicked();
      sumInMemory = sumInMemory + display->text().toDouble();
  }

 Button *Calculator::createButton(const QString &text, const char *member)
 {
     Button *button = new Button(text);
     connect(button, SIGNAL(clicked()), this, member);
     return button;
 }

 void Calculator::abortOperation()
 {
     clearAll();
     display->setText(tr("####"));
 }

// штуки

/*
void MainWindow::disable_btn(bool flag){

    ui->btn_plus->setDisabled(flag);
    ui->btn_minus->setDisabled(flag);
    ui->btn_multiply->setDisabled(flag);
    ui->btn_divide->setDisabled(flag);
}
*/

/*
public static string DecimalToString(decimal dec)
{
    string strdec = dec.ToString(CultureInfo.InvariantCulture);
    return strdec.Contains(".") ? strdec.TrimEnd('0').TrimEnd('.') : strdec;
}
*/

// удаляем незначащие 0

/*
 void Calculator::release0s()
 {
     QString text = display->text();
     double value = text.toDouble();
     if(value >= 1)
     {
        double k = 0;
        double t = value;
        do
        {
          k = value;
          value /= 10;
          if (k == 0)
          {
            t = value - *k
          }
         } while(value);

     }
*/
/*
 void Calculator::release0s()
 {
     QString text = display->text();
     double value = text.toDouble();
     if(value >= 1)
     {
        double m = value;
        double n;
        int count = 0;
        m = n;
        while(m % 10.0)
        {
           m /= 10;
           count++;
        }
        m = 1;
        for(int i = 0; i < count - 1; i++)
        {
           m *= 10;
        }
        m *= n / m;
        m -= n;
        cout << n << endl;
      }
 }
void Calculator::release0s()
{
    QString text = display->text();
    double value = text.toDouble();


}
*/



 bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
 {
     if (pendingOperator == tr("+")) {
         sumSoFar += rightOperand;
     } else if (pendingOperator == tr("-")) {
         sumSoFar -= rightOperand;
     } else if (pendingOperator == tr("\303\227")) {
         factorSoFar *= rightOperand;
     } else if (pendingOperator == tr("\303\267")) {
         if (rightOperand == 0.0)
             return false;
         factorSoFar /= rightOperand;
     }
     return true;
 }
 QString rework_restext(QString restext)
 {
      if (restext.contains('.')){
          for (int i = restext.length() - 1; i > 0; --i){
              if (restext[i] == '0')
              {
                  restext.resize(restext.length() - 1);
              } else if ((i == restext.length() - 1) && (restext[i] == '.'))
              {
                  restext.resize(restext.length() - 1);
                  break;
              } else {
                  break;
              }
          }
      }
      return restext;
 }

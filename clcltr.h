#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDialog>

 class QLineEdit;

 class Button;

 class Calculator : public QDialog
 {
     Q_OBJECT

 public:
     Calculator(QWidget *parent = 0);

 private slots:
     void digitClicked();
     void unaryOperatorClicked();
     void additiveOperatorClicked();
     void multiplicativeOperatorClicked();
     void equalClicked();
     void pointClicked();
     void changeSignClicked();
     void backspaceClicked();
     void clear();
     void clearAll();
     void clearMemory();
     void readMemory();
     void setMemory();
     void addToMemory();
     void rounding0s();
     void clear0s();

     //void release0s();

 private:
     Button *createButton(const QString &text, const char *member);
     void abortOperation();
     bool calculate(double rightOperand, const QString &pendingOperator);

     double sumInMemory;
     double sumSoFar;
     double factorSoFar;
     QString pendingAdditiveOperator;
     QString pendingMultiplicativeOperator;
     bool waitingForOperand;
     QString rework_restext(QString restext);

     QLineEdit *display;

     enum { NumDigitButtons = 10 };
     Button *digitButtons[NumDigitButtons];
 };

 #endif

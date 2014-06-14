#ifndef QLABELEDCOMBOBOX_H
#define QLABELEDCOMBOBOX_H

#include <QWidget>
#include <QComboBox>

class QLabel;
class QString;

class QLabeledComboBox : public QComboBox
{
    Q_OBJECT
public:
    QLabeledComboBox(QString text, QWidget *parent = 0);
private:
    QLabel *label;
};

#endif // QLABELEDCOMBOBOX_H

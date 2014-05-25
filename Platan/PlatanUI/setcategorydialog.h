#ifndef SETCATEGORYDIALOG_H
#define SETCATEGORYDIALOG_H

#include <QDialog>

namespace Ui {
class SetCategoryDialog;
}

class SetCategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetCategoryDialog(QWidget *parent = 0);
    ~SetCategoryDialog();
    int category() const;

private:
    Ui::SetCategoryDialog *ui;
};

#endif // SETCATEGORYDIALOG_H

#ifndef CSVIMPORTWIDGET_H
#define CSVIMPORTWIDGET_H

#include <QWidget>
#include <csvreader.h>
#include <memory>
#include <QBoxLayout>
#include <QLabel>

class QComboBox;
class QCheckBox;

class CSVImportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVImportWidget(QWidget *parent = 0);
    void setReader(CSVReader *r);
    void SelectData(QComboBox *comboBox, char c);
signals:
    void readerParametersChanged();
private slots:
    void setSeparator(int index);
    void setQuote(int index);
    void setHeaders(bool b);

private:
    CSVReader* reader;
    template<class T>
    T* AddLabeledWidget(QBoxLayout* mainLayout, QString name);
    QComboBox* quote;
    QComboBox* separator;
    QCheckBox* headers;
};

template<class T>
T* CSVImportWidget::AddLabeledWidget(QBoxLayout* mainLayout, QString name)
{
    QHBoxLayout* subLayout = new QHBoxLayout();
    QLabel *label = new QLabel(name, this);
    subLayout->addWidget(label);
    T* comboBox = new T(this);
    subLayout->addWidget(comboBox);

    mainLayout->addLayout(subLayout);

    return comboBox;
}

#endif // CSVIMPORTWIDGET_H

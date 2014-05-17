#ifndef WIDGETHELPERS_H
#define WIDGETHELPERS_H

#include <QLabel>
#include <QHBoxLayout>

template<class T>
T* AddLabeledWidget(QBoxLayout* mainLayout, QString name)
{
    QHBoxLayout* subLayout = new QHBoxLayout();
    QLabel *label = new QLabel(name, mainLayout->parentWidget());
    subLayout->addWidget(label);
    T* comboBox = new T(mainLayout->parentWidget());
    subLayout->addWidget(comboBox);

    mainLayout->addLayout(subLayout);

    return comboBox;
}

#endif // WIDGETHELPERS_H

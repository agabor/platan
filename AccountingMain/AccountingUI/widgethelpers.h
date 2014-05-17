#ifndef WIDGETHELPERS_H
#define WIDGETHELPERS_H

#include <QLabel>
#include <QHBoxLayout>
#include <utility>

template<class T>
std::pair<T*, QLabel*> AddLabeledWidget(QBoxLayout* mainLayout, QString name)
{
    QHBoxLayout* subLayout = new QHBoxLayout();
    QLabel *label = new QLabel(name, mainLayout->parentWidget());
    subLayout->addWidget(label);
    T* comboBox = new T(mainLayout->parentWidget());
    subLayout->addWidget(comboBox);

    mainLayout->addLayout(subLayout);

    return std::make_pair(comboBox, label);
}


template<class T>
T *AddNamedWidget(QBoxLayout* mainLayout, QString name)
{
    return AddLabeledWidget<T>(mainLayout, name).first;
}

#endif // WIDGETHELPERS_H

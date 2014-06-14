#include "welcomewidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

void addSpacer(QBoxLayout *layout, int w, int y);

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    addSpacer(layout, 40, 120);
    QLabel* label = new QLabel(tr("Welcome to your new Platan project!\n Please import your bankstatements!"), this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    QPushButton *button = new QPushButton(tr("Import"), this);
    button->setMaximumWidth(150);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addSpacer(buttonLayout, 40, 20);
    buttonLayout->addWidget(button);
    addSpacer(buttonLayout, 40, 20);
    layout->addLayout(buttonLayout);
    addSpacer(layout, 40, 120);
    connect(button, SIGNAL(clicked()), this, SIGNAL(clicked()));
}


void addSpacer(QBoxLayout *layout, int w, int y)
{
    layout->addSpacerItem(new QSpacerItem(w, y, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

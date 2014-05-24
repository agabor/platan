#include "rulewidget.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <statements.h>

RuleWidget::RuleWidget(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("If ", this));
    column = new QComboBox(this);
    //column->addItems(Statements::columnList);
    layout->addWidget(column);
    layout->addWidget(new QLabel(", then category is", this));
    category = new QComboBox(this);
    category->addItems(Statements::categoryList);
    layout->addWidget(category);
}

void RuleWidget::setRule(Rule _rule)
{
    rule = _rule;
    column->addItem(Statements::columnList.at(rule.column) + " is " + rule.value);
    column->setEnabled(false);
    category->setCurrentIndex(rule.category);
}

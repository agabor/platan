#include "rulewidget.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <statements.h>


RuleWidget::RuleWidget(Rule _rule, QWidget *parent) :
    QWidget(parent), rule(_rule)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(ifStr() + condition(rule) + thenStr(), this));
    setCategories();
}

RuleWidget::RuleWidget(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel(ifStr(), this));
    column = new QComboBox(this);
    layout->addWidget(column);
    layout->addWidget(new QLabel(thenStr(), this));
    setCategories();
}

void RuleWidget::setCategories()
{
    category = new QComboBox(this);
    category->addItems(Statements::categoryList);
    category->setCurrentIndex(rule.category);
    layout()->addWidget(category);
}

QString RuleWidget::condition(Rule rule)
{
    return Statements::columnList.at(rule.column) + tr(" is ") + rule.value;
}

const QString RuleWidget::ifStr() const
{
    return tr("If ");
}

const QString RuleWidget::thenStr() const
{
    return tr(", then category is");
}


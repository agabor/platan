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
    category->setEnabled(false);
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
    connect(column, SIGNAL(currentIndexChanged(int)), this, SLOT(columnChanged(int)));
}

void RuleWidget::setRow(StatementRow _row)
{
    row = _row;
    column->clear();
    int idx = 0;
    for(QString columnName : Statements::columnList)
    {
        QString value = row.at(idx++).toString();
        if (value.length() > 20)
            value = value.left(17) + "...";
        column->addItem(columnName + tr(" is ") + value);
    }
    columnChanged(column->currentIndex());
}

Rule RuleWidget::getRule() const
{
    return rule;
}

void RuleWidget::categoryChanged(int idx)
{
    rule.category = idx;
}

void RuleWidget::columnChanged(int idx)
{
    rule.column = idx;
    rule.value = row.at(idx).toString();
}

void RuleWidget::setCategories()
{
    category = new QComboBox(this);
    category->addItems(Statements::categoryList);
    category->setCurrentIndex(rule.category);
    layout()->addWidget(category);
    connect(category, SIGNAL(currentIndexChanged(int)), this, SLOT(categoryChanged(int)));
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


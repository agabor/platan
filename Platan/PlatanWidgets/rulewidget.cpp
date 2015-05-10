#include "rulewidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <statements.h>
#include <rules.h>


RuleWidget::RuleWidget(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    auto firstline = new QHBoxLayout();
    auto secondline = new QHBoxLayout();
    layout->addLayout(firstline);
    layout->addLayout(secondline);
    firstline->addWidget(new QLabel(ifStr(), this));
    column = new QComboBox(this);
    auto columnList = Statements::columnList();
    for(auto columnName : columnList)
        column->addItem(columnName);
    firstline->addWidget(column);
    type = new QComboBox(this);
    for(auto typestr : Rules::typeList())
        type->addItem(typestr);

    firstline->addWidget(type);
    value = new QLineEdit(this);
    firstline->addWidget(value);
    firstline->addWidget(new QLabel(",", this));
    secondline->addWidget(new QLabel(thenStr(), this));
    setCategories(secondline);
    connect(column, SIGNAL(currentIndexChanged(int)), this, SLOT(columnChanged(int)));
    connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
    connect(value, SIGNAL(textChanged(QString)), this, SLOT(valueChanged(QString)));
    type->setCurrentIndex(0);
    typeChanged(0);
}

void RuleWidget::setRow(Statement _row)
{
    row = _row;
    columnChanged(column->currentIndex());
}

void RuleWidget::setColumn(int idx)
{
    column->setCurrentIndex(idx);
}

Rule RuleWidget::getRule() const
{
    return rule;
}

void RuleWidget::categoryChanged(int idx)
{
    rule.setCategory(idx);
}

void RuleWidget::setValueFromRow(int idx)
{
    rule.setValue(row.at(idx).toString());
    value->setText(rule.value());
}

void RuleWidget::columnChanged(int idx)
{
    rule.setColumn(static_cast<Statement::Column>(idx));
    setValueFromRow(idx);
}

void RuleWidget::typeChanged(int idx)
{
    value->setEnabled(idx == 1);
    rule.setType(static_cast<Rule::Type>(idx));
    setValueFromRow(static_cast<int>(rule.column()));
}

void RuleWidget::valueChanged(QString text)
{
    rule.setValue(text);
}

void RuleWidget::setCategories(QLayout *layout)
{
    category = new QComboBox(this);
    category->addItems(Statements::categoryList());
    category->setCurrentIndex(rule.category());
    layout->addWidget(category);
    connect(category, SIGNAL(currentIndexChanged(int)), this, SLOT(categoryChanged(int)));
}

QString RuleWidget::condition(Rule rule)
{
    return Statements::columnList().at(static_cast<int>(rule.column())) + tr(" is ") + rule.value();
}

const QString RuleWidget::ifStr() const
{
    return tr("If ");
}

const QString RuleWidget::thenStr() const
{
    return tr("then category is");
}


#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>
#include <rule.h>

class QComboBox;

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(QWidget *parent = 0);
    void setRule(Rule _rule);
private:
    Rule rule;
    QComboBox* column;
    QComboBox* category;
};

#endif // RULEWIDGET_H

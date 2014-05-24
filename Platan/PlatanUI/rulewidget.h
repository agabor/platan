#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>
#include <rule.h>

class QComboBox;

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(Rule _rule, QWidget *parent = 0);
    explicit RuleWidget(QWidget *parent = 0);
private:
    void setCategories();
    QString condition(Rule rule);
    Rule rule;
    QComboBox* column;
    QComboBox* category;
    const QString ifStr() const;
    const QString thenStr() const;
};

#endif // RULEWIDGET_H

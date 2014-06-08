#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>
#include <rule.h>
#include <statement.h>

class QComboBox;

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(Rule _rule, QWidget *parent = 0);
    explicit RuleWidget(QWidget *parent = 0);
    void setRow(Statement _row);
    void setColumn(int idx);
    Rule getRule() const;
private slots:
    void categoryChanged(int idx);
    void columnChanged(int idx);
private:
    void setCategories();
    QString condition(Rule rule);
    Rule rule;
    QComboBox* column;
    QComboBox* category;
    const QString ifStr() const;
    const QString thenStr() const;
    Statement row;
};

#endif // RULEWIDGET_H

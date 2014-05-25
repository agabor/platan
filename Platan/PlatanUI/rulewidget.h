#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>
#include <rule.h>
#include <statementrow.h>

class QComboBox;

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(Rule _rule, QWidget *parent = 0);
    explicit RuleWidget(QWidget *parent = 0);
    void setRow(StatementRow _row);
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
    StatementRow row;
};

#endif // RULEWIDGET_H

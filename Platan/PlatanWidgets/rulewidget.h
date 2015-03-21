#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>

#include <rule.h>
#include <statement.h>
#include <QVector>

class QComboBox;
class QLineEdit;

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(QWidget *parent = nullptr);
    void setRow(Statement _row);
    void setColumn(int idx);
    Rule getRule() const;
    void setValueFromRow(int idx);
private slots:
    void categoryChanged(int idx);
    void columnChanged(int idx);
    void typeChanged(int idx);
    void valueChanged(QString text);
private:
    void setCategories(QLayout *layout);
    QString condition(Rule rule);
    Rule rule;
    QComboBox* column;
    QComboBox* type;
    QComboBox* category;
    QLineEdit* value;
    const QString ifStr() const;
    const QString thenStr() const;
    Statement row;
};

#endif // RULEWIDGET_H

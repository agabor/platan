#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMainWindow>

namespace Ui {
class HelpWindow;
}

class HelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = 0);
    ~HelpWindow();
    void AddFunctionDoc(QString name, QString doc);
    bool isShown() const;
private:
    Ui::HelpWindow *ui;
    void AddHeader(int headerRank, QString header);
    void AddParagraph(QString text);
    bool shown;
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // HELPWINDOW_H

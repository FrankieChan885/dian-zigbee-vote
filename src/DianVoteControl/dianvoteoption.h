#ifndef DIANVOTEOPTION_H
#define DIANVOTEOPTION_H

#include <QDialog>

namespace Ui {
    class DianVoteOption;
}

class DianVoteOption : public QDialog
{
    Q_OBJECT
public:
    explicit DianVoteOption(QDialog *parent = 0);
    ~DianVoteOption();

signals:

public slots:
    void DoWithOkButton();
    void DoWithApplyButton();

private:
    Ui::DianVoteOption *ui;

};

#endif // DIANVOTEOPTION_H

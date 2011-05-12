#include <QByteArray>
#include "hiddevice.h"
#include "ui_testdianvote.h"

class QHidDevice;

class TestDianvote : public QDialog
{
    Q_OBJECT

public:
    TestDianvote(QDialog *parent = 0);
    ~TestDianvote();

public slots:
    void showInData(QByteArray ba);
    void remoteIDChanged(const QString &);
    void usbStartClicked(bool);
    void remoteStateClicked(bool);

private:
    uint count;
    QHidDevice *hidDevice;
    Ui::TestDianVote *ui;
};


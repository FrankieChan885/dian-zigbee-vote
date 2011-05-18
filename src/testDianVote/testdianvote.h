#include <QByteArray>
#include "hidcontrol.h"
#include "ui_testdianvote.h"

class QHidDevice;

class TestDianvote : public QDialog
{
    Q_OBJECT

public:
    TestDianvote(QDialog *parent = 0);
    ~TestDianvote();

public slots:
    void showInData(quint32 id, quint8 option);
    void remoteIDChanged(const QString &);
    void usbStartClicked(bool);
    void remoteStateClicked(bool);
	void stopOnReceiveClicked(bool);
    void startRollCall();
    void rollCallFinished(uint count);

private:
    uint count;
    HidControl *hidControl;
    Ui::TestDianVote *ui;
};


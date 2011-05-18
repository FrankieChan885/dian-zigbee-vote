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
    void showInData(quint16 id, quint8 option);
    void remoteIDChanged(const QString &);
    void usbStartClicked(bool);
    void remoteStateClicked(bool);
    void getIDList();
    void showID(quint16);
    void getIDListLength();
    void showIDAmount(uint);
//	void stopOnReceiveClicked(bool);
//    void startRollCall();
//    void rollCallFinished(uint count);

private:
    uint count;
    uint countID;
    HidControl *hidControl;
    Ui::TestDianVote *ui;
};


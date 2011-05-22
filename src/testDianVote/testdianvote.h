#include <QByteArray>
#include <QList>
#include "hidcontrol.h"
#include "ui_testdianvote.h"

class QTimer;
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
    void idReceiveFinished();
    void showID(quint16);
    void getIDListLength();
    void showIDAmount(uint);
    void showIntenalMessage();
    void clearVoteList();

    static void DianVoteMsgHandler(QtMsgType type, const char *msg);
//	void stopOnReceiveClicked(bool);
//    void startRollCall();
//    void rollCallFinished(uint count);

private:
    uint count;
    uint countID;
    HidControl *hidControl;
    Ui::TestDianVote *ui;
    QTimer *timer;
    static QList< QString > *message;
    QList< QString > *voted;
    QList< QString > *noteVoted;
};


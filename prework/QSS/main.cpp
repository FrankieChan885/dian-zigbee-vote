#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFile qss("test.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    QDialog *dlg = new QDialog();
    QPushButton *btn = new QPushButton("Test Button", dlg);
    dlg->show();

    return a.exec();
}

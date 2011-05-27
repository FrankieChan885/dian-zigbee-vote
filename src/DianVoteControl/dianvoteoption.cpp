#include <QDir>
#include <QIcon>
#include "dianvoteoption.h"
#include "ui_dianvoteoptions.h"

DianVoteOption::DianVoteOption(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::DianVoteOption)
{
    ui->setupUi(this);

    QDir dir;
    dir.setCurrent(QCoreApplication::applicationDirPath());
    QIcon *windowIcon = new QIcon(dir.absoluteFilePath("res/icons/option.png"));
    this->setWindowIcon(*windowIcon);
    this->setWindowTitle(tr("Options"));
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint);

    connect(ui->OkButton, SIGNAL(clicked()),
            this, SLOT(DoWithOkButton()));
    connect(ui->ApplyButton, SIGNAL(clicked()),
            this, SLOT(DoWithApplyButton()));
    connect(ui->CancelButton, SIGNAL(clicked()),
            this, SLOT(hide()));
}

DianVoteOption::~DianVoteOption()
{
}

void DianVoteOption::DoWithApplyButton()
{
    ui->ApplyButton->setDisabled(true);
}

void DianVoteOption::DoWithOkButton()
{
    DoWithApplyButton();
    this->hide();
}

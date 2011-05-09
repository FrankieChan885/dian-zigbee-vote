#include "dianvotedrawer.h"
#include "ui_dianvotedrawer.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteDrawer)
{
    ui->setupUi(this);
    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    histgram = new DrawHistgram();
    ui->DrawConvas->addWidget(histgram);
}

//bool DianVoteDrawer::EnumUserWindowsCB(HWND hwnd, LPARAM lParam)
//{
//    long wflags = GetWindowLong(hwnd, GWL_STYLE);
//    if(!(wflags & WS_VISIBLE)) return TRUE;

//    HWND sndWnd;
//    if( !(sndWnd=FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL)) ) return TRUE;
//    HWND targetWnd;
//    if( !(targetWnd=FindWindowEx(sndWnd, NULL, L"SysListView32", L"FolderView")) ) return TRUE;

//    HWND* resultHwnd = (HWND*)lParam;
//    *resultHwnd = targetWnd;
//    return FALSE;
//}

//HWND DianVoteDrawer::FindDesktopIconWnd()
//{
//    HWND resultHwnd = NULL;
//    EnumWindows((WNDENUMPROC)EnumUserWindowsCB, (LPARAM)&resultHwnd);
//    return resultHwnd;
//}

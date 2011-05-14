#include "drawbase.h"

DrawBase::DrawBase(QWidget *parent) :
    QWidget(parent),
    correctAnswerFlag(false),
    questionName("Question 1"),
    correctAnswer(""),
    voterNums(0),
    optionNums(5)
{
    colorSet = new QStringList();
    drawData = new QList< DisplayData* >();
    setDefaultColorSet();
    setDefaultDrawData();
}

DrawBase::~DrawBase()
{
    for (int i = 0; i < optionNums; i++)
    {
        delete (*drawData)[i];
    }
    delete drawData;
    delete colorSet;
}

void DrawBase::SetCorrectAnswer(QString answer)
{
    correctAnswer = answer;
    update();
}

void DrawBase::SetCorrectAnswerFlag(bool flag)
{
    correctAnswerFlag = flag;
    update();
}

void DrawBase::SetOptionNums(int nums)
{
    ClearData();        // 必先清空数据，因为调用此槽函数的时候必然是需要开始一个新的投票了。
    optionNums = nums;
    setDefaultDrawData();
    update();
}

void DrawBase::SetQuestionName(QString name)
{
    questionName = name;
}

int DrawBase::GetVoterNums()
{
    return voterNums;
}

void DrawBase::SetVoterNums(int nums)
{
    voterNums = nums;
}

void DrawBase::ClearData()
{
    // 必须先要保存数据~~

    fakeVoterNums = INIT_VOTER_NUM;
    drawData->clear();
    // 因为是每一次重新start投票调用这个，所以必须重新设置
    setDefaultColorSet();
    setDefaultDrawData();
    update();
}

void DrawBase::HandleData(int index)
{
    if (index < 0 || index >= drawData->length())
    {
        // 这里应该把错误记录写到错误的log文件中
        return;
    }
    (*drawData)[index]->voterNum++;
    update();

    return;
}

void DrawBase::setDefaultColorSet()
{
    if(colorSet->length())
    {
        return;
    }

    colorSet->append("blue");
    colorSet->append("magenta");
    colorSet->append("cyan");
    colorSet->append("yellow");
    colorSet->append("darkCyan");
    colorSet->append("darkMagenta");
}

void DrawBase::setDefaultDrawData()
{
    if(drawData->length())
    {
        drawData->clear();
    }
    DisplayData *dd = NULL;

    QString tmp[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    int num[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < optionNums; i ++)
    {
        dd = new DisplayData;
        dd->optionName = tmp[i];
        dd->voterNum = num[i];
        dd->color = colorSet->at(i % colorSet->length());

        drawData->append(dd);
    }
}

#include "drawbase.h"

DrawBase::DrawBase(QWidget *parent) :
    QWidget(parent),
    correctAnswerFlag(false),
    questionName("Question 1"),
    correctAnswer(""),
    voterNums(0),
    optionNums(5)
{
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
    optionNums = nums;
}

void DrawBase::SetQuestionName(QString name)
{
    questionName = name;
}

void DrawBase::SetVoterNums(int nums)
{
    voterNums = nums;
}

void DrawBase::HandleData(int index)
{
    (*drawData)[index]->voterNum++;
    update();

    return;
}

void DrawBase::setDefaultColorSet()
{
    colorSet = new QStringList();

    colorSet->append("blue");
    colorSet->append("magenta");
    colorSet->append("cyan");
    colorSet->append("yellow");
    colorSet->append("darkCyan");
    colorSet->append("darkMagenta");
}

void DrawBase::setDefaultDrawData()
{
    DisplayData *dd = NULL;

    QString tmp[] = {"A", "B", "C", "D", "E", "F", "G"};
    int num[] = {0, 2, 2, 0, 0};

    for (int i = 0; i < optionNums; i ++)
    {
        dd = new DisplayData;
        dd->optionName = tmp[i];
        dd->voterNum = num[i];
        dd->color = colorSet->at(i % colorSet->length());

        drawData->append(dd);
    }
}

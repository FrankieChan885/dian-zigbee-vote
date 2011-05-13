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

void DrawBase::ClearData()
{
    // ������Ҫ��������~~

    voterNums = INIT_VOTER_NUM;
    drawData->clear();
    // ��Ϊ��ÿһ������startͶƱ������������Ա�����������
    setDefaultColorSet();
    setDefaultDrawData();
    update();
}

void DrawBase::HandleData(int index)
{
    if (index < 0 || index >= drawData->length())
    {
        // ����Ӧ�ðѴ����¼д�������log�ļ���
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
        return;
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

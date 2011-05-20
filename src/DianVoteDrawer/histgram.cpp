/* ��...��һ����QPainter������д�Ĳ���һ�����...
 * ��ͼ��ʱ�����¶���һ���������һ������0��zeroPoint
 * ���еĻ�ͼ����zeroPointΪ��׼�ģ�����Ҫ�뿴��Ϊʲô��ô������
 * �ȿ���zeroPoint����ô���õ�...OTZ..
 */

#include "histgram.h"
#include <QPoint>
#include <QPen>
#include <QColor>
#include <QFont>

const int WidthSlices = 18;      // �����ڿ�ֳ�20��
const int HeightSlices = 20;     // �����ڸ߷ֳ�20��
const float XaixsLengthRatio = 16 / (float)WidthSlices;      // X����ռ���ڵı���
const float YaxisLengthRatio = 15 / (float)HeightSlices;     // Y����ռ���ڵı���

const int ScaleSize = 5;         // X��Y���Ͽ̶ȵĿ�����߸߶�
static int yFrontSpace = 25;     // ������Y����ľ���
static int xFrontSpace = 25;     // ������X����ľ���
static int FontSize = 15;        // Ĭ�������С
static int YaxisSlices = 10;     // ��10Ϊ������Y���ϵĿ̶�,��ͶƱ�������ڵ���10��ʱ���Ϊ1

const int precision = 4;         // ��ʾ����ʱ�ľ�ȷ��
const float rectRatio = 0.67;    // ������״���Ŀ��
const int ratioRectSize = 10;    // ������ʾ�����ľ��δ�С

Histgram::Histgram(QWidget *parent) :
    QWidget(parent), columnNums(5), voterNums(100),
    yAxisDashLineFlag(true), rectangleStyle(1),
    disCorrectAnswerFlag(false),questionName("Question 1")
{
    setDefaultDrawData();   //Ĭ������
    setDefaultColorSet();   //Ĭ����ɫ
}

// ʵ�ʵ��õ�Histgram��ʼ�����캯��
Histgram::Histgram(QList<OptionData> &data)
{
    // some default values
    voterNums = 100; // default
    yAxisDashLineFlag = true;
    rectangleStyle = 1; // SolidPattern
    disCorrectAnswerFlag = false;
    questionName = "Question 1";

    // ������ɫ
    setDefaultColorSet();

    // ��������
    columnNums = data.length();
    drawData = new QList< OptionData >(data);
}

Histgram::~Histgram()
{
    if (drawData != NULL)
    {
        delete drawData;
    }
    delete colorSet;
}

void Histgram::setDefaultDrawData()
{
    // ��ʼ������
    drawData = new QList< OptionData >();
    // Ĭ��ֵ
    drawData->append({"A", 20});
    drawData->append({"B", 30});
    drawData->append({"C", 19});
    drawData->append({"D", 8});
    //drawData->append({"E", 23});

    columnNums = drawData->length();
}

void Histgram::setDefaultColorSet()
{
    // ��ʼ����ɫ����
    colorSet = new QStringList();
    colorSet->append("blue");
    colorSet->append("magenta");
    colorSet->append("cyan");
    colorSet->append("yellow");
    colorSet->append("darkCyan");
    colorSet->append("darkMagenta");
}

void Histgram::setColumnNums(int nums)
{
    columnNums = nums;
}

int Histgram::getColumnNums()
{
    return columnNums;
}

void Histgram::setData(QList< OptionData > &data)
{
    // ������ݲ�Ϊ�գ��������������
    if (!drawData->empty())
    {
        delete drawData;
    }
    drawData = new QList< OptionData >(data);
}

QString Histgram::getOptionName(int index)
{
    return (*drawData)[index].getOptionName();
}

void Histgram::setColorSet(QList<QString> &set)
{
    if (set.length() < columnNums)
    {
        // ���δ���ʹ��Ĭ����ɫ
        setDefaultColorSet();
    }
    for (int i = 0; i < columnNums; i++)
    {
        (*colorSet)[i] = set[i];
    }
    update();
}

void Histgram::draw(QPainter *painter)
{
    QPen thickPen(palette().foreground(), 4);
    QPen thinPen(palette().foreground(), 0.5);

    // Ĭ��0�����������Ͻ�...
    QPoint zeroPoint(width() / WidthSlices,
                     (HeightSlices - 2) * height() / HeightSlices);
    int xaxisLength = XaixsLengthRatio * width();
    int yaxisLength = YaxisLengthRatio * height();
    painter->setPen(thickPen);
    // ����Y��
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x(), zeroPoint.y() - yaxisLength);
                                                // 0_0 Ĭ��0�����������Ͻ�...
    // ����X��
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x() + xaxisLength, zeroPoint.y());

    // ����Y��̶�
    if (voterNums <= 10)     // ���ͶƱ�������Ƿ�Ϊ0
    {
        if (!voterNums)
        {
            return;
        }
        YaxisSlices = 1;//ͶƱ�������ڵ���10��ʱ���Ϊ1
    }
    int yaxisScaleNums = qRound((float)voterNums / (float)YaxisSlices);    // �̶���Ŀ
    int yaxisScaleSpace = yaxisLength / yaxisScaleNums;     // �̶ȼ��
    for (int i = 1; i <= yaxisScaleNums; i++)
    {
        painter->setPen(thickPen);
        painter->drawLine(zeroPoint.x() - ScaleSize, zeroPoint.y() - i * yaxisScaleSpace,
                          zeroPoint.x(), zeroPoint.y() - i * yaxisScaleSpace);
                                            // �ǡ�-��...ԭ����0�������Ͻ�����...
        QString num = QString("%1").arg(i * YaxisSlices);
        painter->drawText(zeroPoint.x() - yFrontSpace, zeroPoint.y() - i * yaxisScaleSpace, num);
        // ����������߱����ó���ʾ״̬
        if (yAxisDashLineFlag)
        {
            painter->setPen(QPen(Qt::black, 0.1, Qt::DashLine, Qt::SquareCap));
            painter->drawLine(zeroPoint.x(), zeroPoint.y() - i * yaxisScaleSpace,
                              zeroPoint.x() + xaxisLength, zeroPoint.y() - i * yaxisScaleSpace);
        }
    }

    // ������״ͼ��X��
    int xaxisScaleNums = columnNums;    // �̶���Ŀ��������
    int xaxisScaleSpace = xaxisLength / xaxisScaleNums;     // �̶ȼ��
    int rectWidth = rectRatio * xaxisScaleSpace;        // ������״�����
    FontSize = (width() + height()) / 70;              // ���������С
    xFrontSpace = (width() + height()) / 45;            // ��������X��ľ���
    QFont font("Arial",FontSize,QFont::Bold,true);  // ��������
    for (int i = 1; i <= columnNums; i++)
    {
        // ����X��̶�
        painter->setPen(thickPen);
        painter->drawLine(zeroPoint.x() + i * xaxisScaleSpace, zeroPoint.y(),
                          zeroPoint.x() + i * xaxisScaleSpace, zeroPoint.y() + ScaleSize);
        // ����ѡ��
        painter->setFont(font);
        painter->drawText(zeroPoint.x() + i * xaxisScaleSpace -(xaxisScaleSpace + FontSize * drawData->at(i - 1).getOptionName().length()) /2, // ���ֵ�λ����ÿ�����̶ȵ��м�
                          zeroPoint.y() + xFrontSpace,
                          drawData->at(i - 1).getOptionName());

        // ����״ͼ
        painter->setRenderHint(QPainter::Antialiasing, true);
        thinPen.setColor("black");
        painter->setPen(thickPen);
        // style determined by rectangelMetirial
        switch (rectangleStyle)
        {
        case 1:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::SolidPattern));
                break;
            }
        case 2:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::Dense5Pattern));
                break;
            }
        case 3:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::CrossPattern));
                break;
            }
        default:
            {
                // Ĭ��Ϊ��ɫ
                painter->setBrush(QBrush(colorSet->at(i), Qt::SolidPattern));
            }
        }

        // ��������ó���ʾ��ȷ��
        if (disCorrectAnswerFlag)
        {
            // �������ȷ�𰸣����ó���ɫ������Ϊ��ɫ
            if (drawData->at(i - 1).getOptionName() == correctAnswer)
            {
                painter->setBrush(QBrush("green", Qt::SolidPattern));
            }
            else
            {
                painter->setBrush(QBrush("red", Qt::SolidPattern));
            }
        }


        // ���������Ͻ�����
        int rectHeight = qRound(yaxisLength * ((float)drawData->at(i - 1).getVoterNum() / (float)voterNums));
        int rectX = zeroPoint.x() + i * xaxisScaleSpace - (xaxisScaleSpace + rectWidth) / 2;
        int rectY = zeroPoint.y() - rectHeight;

        painter->drawRect(rectX, rectY, rectWidth, rectHeight);
        // ��������ͶƱ����������ռ����
        painter->setFont(font);
        QString nums = QString("%1").arg(drawData->at(i - 1).getVoterNum());    // ��ͶƱ����ת�����ַ���
        QString ratio = QString("%1").arg((float)drawData->at(i - 1).getVoterNum() \
                                          / (float)voterNums * 100);   // ���������ר���ַ���
        if (ratio.size() > precision + 1)
        {
            ratio.resize(precision + 1);        // ֻ��ʾprecisionλС��������С�����+1
        }
        ratio += "%";
        QString text = nums + "(" + ratio + ")";
        painter->drawText(rectX + (rectWidth - FontSize * (text.length() - 3)) / 2, \
                          rectY - FontSize / 2, text);      // 3 ���Գ�����... - -!
    }

    // ������Ŀ����
    QFont titleFont("Arial",FontSize,QFont::Bold,true);
    painter->setFont(titleFont);
    painter->drawText(zeroPoint.x(), FontSize, questionName);
}

void Histgram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // �����ӿ�Ϊ�������ڴ�С
    painter.setViewport(0, 0, width(), height());
    // ���ÿ��Ի�ͼ�Ĵ�С
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

// ������ΪoptionName���еĸ߶ȼ�1
void Histgram::updateData(QString& optionName)
{
    for (int i = 0; i < drawData->length(); i++)
    {
        if ((*drawData)[i].getOptionName() == optionName)
        {
            // Ͷ��ѡ��optionName��ͶƱ����+1
            (*drawData)[i].setVoteNum((drawData->at(i).getVoterNum() + 1));
        }
    }
    update();
}

// �����Ƿ���ʾ��������
void Histgram::setyAxisDashLineFlag()
{
    yAxisDashLineFlag = !yAxisDashLineFlag;
    update();
}

void Histgram::setRectangelStyle(int flag)
{
    rectangleStyle = flag;
    update();
}

void Histgram::setSolidPatternStyle()
{
    setRectangelStyle(1);
}

void Histgram::setDense5PatternStyle()
{
    setRectangelStyle(2);
}

void Histgram::setCrossPatternStyle()
{
    setRectangelStyle(3);
}

void Histgram::setUsetCorrectAnswer()
{
    if (correctAnswer.length())
    {
        disCorrectAnswerFlag = !disCorrectAnswerFlag;
        update();
    }
}

void Histgram::setUsetCorrectAnswer(bool flag)
{
    if (correctAnswer.length())
    {
        disCorrectAnswerFlag = flag;
        update();
    }
}

void Histgram::setCorrectAnswer(QString answer)
{
    disCorrectAnswerFlag = true;
    correctAnswer = answer;
    update();
}

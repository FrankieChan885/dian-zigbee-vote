#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>

// �����洢����һ��ѡ�������
class OptionData {
public:
    OptionData(QString name, int num)
        :optionName(name), voterNum(num)
    {
    }
    QString getOptionName() const
    {
        return optionName;
    }
    int getVoterNum() const
    {
        return voterNum;
    }
    void setVoteNum(int num)
    {
        voterNum = num;
    }

private:
    QString optionName;
    int voterNum;
};

#endif // DATATYPE_H

/**
 * exceptions.h
 *
 *  This file is the exception classes definition file.
 *  All the exception classes for this project will be defined in this file.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-16  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_

#include <exception>
#include <sstream>
#include <string>

class DianVoteException : public std::exception
{
public:
    enum DianVoteExceptionType {
        UI_FILE_NOTFOUND = 0,
        UI_LOAD_FAILED,
        QSS_FILE_NOTFOUND,
        TOPICS_FILE_NOTFOUND
    };
public:
    DianVoteException(enum DianVoteExceptionType type) {
        exceptionType = type;
    }

    virtual const char *what() const throw() {
        std::stringstream ss;
        ss << "Exception \'" << exceptionType << "\' happened..";
        return ss.str().c_str();
    }
private:
    enum DianVoteExceptionType exceptionType;
};

class XmlStreamException : public std::exception
{
public:
    XmlStreamException(const std::string& errorStr, int line, int column)
    : errorString(errorStr)
    , errorLine(line)
    , errorColumn(column)
    {
    }

    virtual ~XmlStreamException() throw() {}

    virtual const char *what() const throw() {
        std::stringstream ss;
        ss << "Exception \'XML stream error\' happened.." << std::endl;
        ss << "line " << errorLine << ", column " << errorColumn <<
            ": " << errorString << std::endl;

        return ss.str().c_str();
    }

private:
    std::string errorString;
    int errorLine;
    int errorColumn;
};

#endif // __EXCEPTIONS_H_

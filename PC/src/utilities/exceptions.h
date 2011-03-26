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
        ss << "UI exception \'" << exceptionType << "\' happened..";
        return ss.str().c_str();
    }
private:
    enum DianVoteExceptionType exceptionType;
};

#endif // __EXCEPTIONS_H_

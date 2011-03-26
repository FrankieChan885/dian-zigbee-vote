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

class UiException : public std::exception
{
public:
    enum UiExceptionType {
        UI_FILE_NOTFOUND = 0,
        UI_LOAD_FAILED,
        QSS_FILE_NOTFOUND
    };
public:
    UiException(enum UiExceptionType type) {
        exceptionType = type;
    }

    virtual const char *what() const throw() {
        std::stringstream ss;
        ss << "UI exception \'" << exceptionType << "\' happened..";
        return ss.str().c_str();
    }
private:
    enum UiExceptionType exceptionType;
};

#endif // __EXCEPTIONS_H_

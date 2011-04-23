/**
 * @file exceptions.h
 * @brief 
 *  This file is the exception classes definition file.
 *  All the exception classes for this project will be defined in this file.
 *
 * @author Tankery Chen @ Dian Group
 *      mailto: tankery.chen@gmail.com
 * @version 1.0.0
 * @date 2011-04-23
 */
#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_

#include <exception>
#include <sstream>
#include <string>

/**
* @brief this class provide an DianVote user level exceptions.
*/
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

    std::string errorstring(enum DianVoteExceptionType type) const {
        switch (type) {
        case UI_FILE_NOTFOUND:
            return "UI_FILE_NOTFOUND";
        case UI_LOAD_FAILED:
            return "UI_LOAD_FAILED";
        case QSS_FILE_NOTFOUND:
            return "QSS_FILE_NOTFOUND";
        case TOPICS_FILE_NOTFOUND:
            return "TOPICS_FILE_NOTFOUND";
        default:
            return "UNKNOW_EXCEPTION";
        }
    }

    virtual const char *what() const throw() {
        std::stringstream ss;
        ss << "Exception \'" << errorstring(exceptionType)
            << "\' happened..";
        return ss.str().c_str();
    }
private:
    enum DianVoteExceptionType exceptionType;
};


/**
* @brief this class provide a xml stream exception.
*/
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

/**
* @brief this class provide an standard exception.
*       you can new one with a error string.
*/
class DianVoteStdException : public std::exception
{
public:
    DianVoteStdException(const std::string& errorStr)
    : errorString(errorStr)
    { }

    virtual ~DianVoteStdException() throw() {}

    virtual const char *what() const throw() {
        return errorString.c_str();
    }

private:
    std::string errorString;
};

#endif // __EXCEPTIONS_H_

/* Copyright (C) 
* 2011 - Tankery Chen @ Dian Group
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/


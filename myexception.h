#pragma once
#include <QException>
#include <QString>
class myexception : public std::exception
{
public:
    myexception(const QString& message, const QString& file, int line ):mes_(message),file_(file),line_(line)
        {};
    QString Message()
    {
        return mes_;
    }
    QString File()
    {
        return file_;
    }
    int line()
    {
        return line_;
    }
private:
    QString mes_;
    QString file_;
    int line_;

};
#define THROW_MTEXCEPTION(message) throw myexception(message,__FILE__,__LINE__)

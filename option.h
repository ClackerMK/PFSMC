#ifndef OPTION_H
#define OPTION_H

#include<string>

#include "statistics.h"

enum OptionType
{
    Combat,
    Magic,
    Social
};


class Option
{
private:
    OptionType type;

public:
    Option();

    std::string getText()
    {

    }

    //Setter
    void setType(OptionType type);

    //Getter
    OptionType getType();
};

#endif // OPTION_H

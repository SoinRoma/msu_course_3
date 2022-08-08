//Калькулятор с базовыми функциями + - * /
//Ссылки на источники
// https://evetro.files.wordpress.com/2012/11/hw7-12.pdf
// http://zonakoda.ru/tekstovyj-kalkulyator-na-c99-1-ya-chast-teoriya.html
// https://ru.wikipedia.org/wiki/Метод_рекурсивного_спуска
// Ваши лекции )

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include "grammar.h"
#include <map>
using namespace std;

Token_value last = SEMICOLON;
double number_value;

string const_value;
map<string,double>table;

double addsub(istream *input, bool get);

double error(const string &type_error){
    cout << "Ошибка: " << type_error << endl;
}

Token_value next_token(istream *input){

    char symbol;
    do{
        if (!input->get(symbol))
            return last = END;
    }
    while (symbol != '\n' && isspace(symbol));
    switch (symbol){
        case SEMICOLON:
        case '\n':
            return last = SEMICOLON;
        case MULTIPLICATION:
        case DIVISION:
        case PLUS:
        case MINUS:
        case LP:
        case RP:
        case EQUALITY:
            return last = Token_value(symbol);
        case NUMBER_0:
        case NUMBER_1:
        case NUMBER_2:
        case NUMBER_3:
        case NUMBER_4:
        case NUMBER_5:
        case NUMBER_6:
        case NUMBER_7:
        case NUMBER_8:
        case NUMBER_9:
        case NUMBER_P:
            input->putback(symbol);
            *input >> number_value;
            return last = NUMBER;
        default:
            if(isalpha(symbol)){
                const_value=symbol;
                while (input->get(symbol)&&isalnum(symbol)){
                    const_value.push_back(symbol);
                }
                    input->putback(symbol);
                    return last=CONST;
            }
            error("Неправильный символ");
            return last = SEMICOLON;
    }
}

//    digit : проверяет что за число, есть ли знак минус и есть ли скобки ;
//    muldiv : digit ( ( '*' | '/' ) digit )*
//    addsub: muldiv ( ( '+' | '-' ) muldiv )*



double digit(istream *input, bool get){
    if (get)
        next_token(input);

    switch (last){
        case NUMBER:{
            double num = number_value;
            //cout<<num;
            next_token(input);

            return num;
        }

        case CONST:{
            double &c=table[const_value];
            if(next_token(input)==EQUALITY){
                c=addsub(input, true);
            }
            return c;
        }


        case MINUS: {
            return -digit(input, true);
        }
        case LP:{
            //cout<<" "<<'('<<" ";
            double sign = addsub(input, true);
            //cout<<" "<<')'<<" ";
            if (last != RP)
                return error("Нет закрывающей ')' ");

            next_token(input);
            return sign;
        }
        default:
            return error("Значений не хватает");
    }
}

double muldiv(istream *input, bool get){
    double factors = digit(input, get);

    for (;;){
        switch (last){
            case MULTIPLICATION:
               // cout<<" "<<'*'<<" ";
                factors *= digit(input, true);
                break;
            case DIVISION:
               // cout<<" "<<'/'<<" ";
                if (double d = digit(input, true))
                {
                    factors /= d;
                    break;
                }
                return error("Делить на 0 нельзя!!!");
            default:
                return factors;
        }
    }
}

double addsub(istream *input, bool get){
    double terms = muldiv(input, get);

    for (;;){
        switch (last){
            case PLUS:
                //cout<<" "<<'+'<<" ";
                terms += muldiv(input, true);
                break;
            case MINUS:
               // cout<<" "<<'-'<<" ";
                terms -= muldiv(input, true);
                break;
            default:
                return terms;
        }
    }
}

int main(int argc, char *argv[]){
    setlocale(LC_CTYPE, "");
    istream *input = nullptr;
    switch (argc){
        case 1:
            input = &cin;
            break;
        case 2:
            input = new istringstream(argv[1]);
            break;
        default:
            error("Много символов");
            return 1;
    }

    table["pi"]=3.14;


    while (*input){

        next_token(input);


        if (last == END)
            break;

        if (last == SEMICOLON)
            continue;

        cout <<" = "<< addsub(input, false) << endl;

    }

    return 0;
}
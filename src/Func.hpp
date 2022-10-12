#ifndef FUNC_HPP
#define FUNC_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <iterator>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <type_traits>

class Func {

    bool correctness = true;

    double read_entity();

    double power();

    double mult();

    double add();

    double factorial(double result);

    int c_pos = 0;

    std::string name;

    std::string expression;

    std::unordered_map<char, double> param;

public:

    Func();

    Func(std::string name);

    Func(std::string name,
         std::string param,
         std::string expression = "");

    void print();

    Func& set_expression(std::string expression);

    double (*calculate)(std::vector<double> &par,
                        Func* obj)
        = [](std::vector<double> &par, Func *obj) -> double {

            int c = 0;

            double result = 0;
            
            if (par.size() == 0) {

                result = obj->add();

                if (obj->correctness)

                    return result;

                else {

                    std::cout << ">: Incorrect expression, token: " << obj->name << ";\n";

                    return 0;
                }

            }

            for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

                (*i).second = par[c];

            result = obj->add();

            for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

                (*i).second = 0;

            obj->c_pos = 0;

            if(obj->correctness)

                return result;

            std::cout << ">: Incorrect expression, token: " << obj->name << ";\n";
            
            return 0;
        };

    friend void set_prototype(std::string name, Func f);

    friend void set_variable(std::string name, double val);

    friend void handle_variable();

    friend void handle_prototype();

    friend bool is_prototype();

    friend bool is_variable();

    template<typename STRM>
    friend void handle_key_word(std::string key_word, STRM &stream);

    template<typename STRM>
    friend void read_buff(STRM &stream);

    template<typename STRM>
    friend void repeat_handle(STRM& stream);

    friend void setup_reserved();

};

extern std::string help;

extern std::unordered_map<std::string, Func> prototypes;

extern std::unordered_map<std::string, double> variables;

extern std::vector<std::string> key_words;

extern std::string BUFF;

#endif
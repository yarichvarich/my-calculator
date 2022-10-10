#ifndef FUNC_HPP
#define FUNC_HPP

#include <iostream>
#include <unordered_map>
#include <string>

class Func {
    std::string name;

    std::string expression;

    std::unordered_map<char, double> param;

    int c_pos = 0;

    bool correctness = true;

    double read_entity();

    double power();

    double mult();

    double add();

    double factorial(double result);

public:

    Func();

    Func(const std::string &name);

    Func(const std::string &name,
         std::string &param,
         const std::string &expression = "");
    
    void set_expression(const std::string& expression);

    void print();

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
};

#endif
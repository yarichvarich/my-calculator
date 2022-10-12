#include <Func.hpp>

std::unordered_map<std::string, Func> prototypes;

std::unordered_map<std::string, double> variables;

const double M_PI = 3.14159265358979323846;

Func::Func() {

    name = "anonym";

}

Func::Func(std::string name) {

    this->name += name;

}

Func::Func(std::string name,
           std::string param,
           std::string expression) {

    this->name += name;
    
    for (std::string::iterator i = param.begin(); i != param.end(); i++) {

        if (*i == ',' || *i == ' ')

            continue;

        else

            this->param[*i] = 0;
    }

    this->expression = expression;

}

void set_prototype(std::string name, Func f) {

    prototypes[name] = f;

}

void set_variable(std::string name, double val) {

variables[name] = val;

}

void Func::print() {

    std::cout << name << "(";

    for (std::unordered_map<char, double>::iterator i = param.begin(); i != param.end(); i++) {

        std::cout << (*i).first << ((i == param.end()) ? ')' : ',');
    }

    std::cout << "=" << expression;

}

double Func::read_entity() {
    if (c_pos >= expression.length()) {

		correctness = false;

		return 0;
	}

	double return_value = 0;

	char c = expression[c_pos];

	int n = 0;

	if (c <= '9' && c >= '0' || c == '.') {

		double number = 0;

		for (; c <= '9' && c >= '0'; c_pos++, c = expression[c_pos])

			number = number * 10 + static_cast<double>(static_cast<int>(expression[c_pos]) - static_cast<int>('0'));

		if (c == '.') {

			c_pos++; c = expression[c_pos];
            
			double frac = 0, p = 0;

			for (; c <= '9' && c >= '0'; c_pos++, c = expression[c_pos], p++)

				frac = frac * 10 + static_cast<double>(static_cast<int>(expression[c_pos]) - static_cast<int>('0'));

			number += frac / pow(10, p);
		}

		return factorial(number);
	}

	else if (c <= 'Z' && c >= 'A' || c >= 'a' && c <= 'z') {

		std::string token;

		for (; c <= 'Z' && c >= 'A' || c >= 'a' && c <= 'z'; c_pos++, c = expression[c_pos])

			token += c;

		if (prototypes.count(token)) {

			c_pos++;

			std::vector<double> par;

			int left_bracket = 1, right_bracket = 0;

			while (expression[c_pos] != ')') {

				if (c_pos >= expression.length()) {

					correctness = false;

					return 0;
				}
				Func anonym_func;

				std::vector<double> external_param;

				for (std::unordered_map<char, double>::iterator e = this->param.begin(); e != this->param.end(); e++)

					external_param.push_back((*e).second);

				while ((expression[c_pos] != ',' || left_bracket - right_bracket != 1) && !(expression[c_pos] == ')' && left_bracket - right_bracket == 0)) {

					if (c_pos >= expression.length()) {

						correctness = false;

						return 0;
					}

					anonym_func.expression += expression[c_pos];

					if (expression[c_pos] == '(')

						left_bracket++;

					c_pos++;

					if (expression[c_pos] == ')')

						right_bracket++;
				}
				if (expression[c_pos] == ',')

					c_pos++;

				anonym_func.expression += ';';

				anonym_func.param = this->param;

				par.push_back(anonym_func.calculate(external_param, &anonym_func));
			}

			c_pos++;

			return factorial(prototypes[token].calculate(par, &prototypes[token]));
		}

		else if (variables.count(token))

			return factorial(variables[token]);

		else if (param.count(token[0]))

			return factorial(param[token[0]]);

		else {

			correctness = false;

			std::cout << ">: No such prototype/variable: " << token << ";\n";

			return 0;
		}
	}

	else {

		if (c == '(') {

			c_pos++;

			double number = add();

			c_pos++;

			if (c_pos < this->expression.length());

				return factorial(number);

		}
		else if (c == ';')

			return 0;

		else if (c == '-') {

			c_pos++;

			return (-1) * read_entity();
		}
			
		correctness = false;
	}
}

double Func::power() {

    if (c_pos >= expression.length()) {

			correctness = false;

			return 0;
	}

	double op1 = read_entity();

	if (c_pos >= expression.length()) {

		correctness = false;

		return 0;
	}

	if (expression[c_pos] == '^') {

			c_pos++;

			op1 = pow(op1, power());
	}

	return op1;
}

double Func::mult() {
    if (c_pos >= expression.length()) {

        correctness = false;

        return 0;
    }

    double op1 = power();

    while (true) {

        if (c_pos >= expression.length()) {

            correctness = false;

            return 0;
        }

        if (expression[c_pos] == '*') {

            c_pos++;

            op1 *= power();
        }

        else if (expression[c_pos] == '/') {

            c_pos++;

            op1 /= power();
        }

        else
            break;
    }

    return op1;
}

double Func::add() {

    if (c_pos >= expression.length()) {

			correctness = false;

			return 0;
		}

    double op1 = mult();

    while (true) {

        if (c_pos >= expression.length()) {

            correctness = false;

            return 0;
        }

        if (expression[c_pos] == '+') {

            c_pos++;

            op1 += mult();

        }
        else if (expression[c_pos] == '-') {

            c_pos++;

            op1 -= mult();
        }

        else
            break;
    }

    if (c_pos >= expression.length()) {

        correctness = false;

        return 0;
    }

    return op1;
}

double Func::factorial(double result) {
    if (c_pos >= expression.length()) {

		correctness = false;

		return 0;
    }

    if (expression[c_pos] == '!') {

        c_pos++; double r = 1;

        for (double m = 1; m <= (int)result; m++)

            r *= m;


        return factorial(r);
    }

    return result;
}

void setup_reserved() {
	Func m_sin("sin", "x"), m_cos("cos", "x"), m_tg("tg", "x", "sin(x)/cos(x);"), m_ctg("ctg", "x", "1/tg(x);"), m_log("log", "x,y"),
		 m_asin("asin", "x"), m_acos("acos", "x"), m_atg("atg", "x"), m_actg("actg", "x", "pi/2 - atg(x);"), m_sh("sh", "x", "(e^x-e^(-x))/2;"),
		 m_ch("ch", "x", "(e^x+e^(-x))/2;");

	m_sin.calculate = [](std::vector<double>& par, Func* obj) -> double {

		int c = 0;

		if (par.size() == 0)

			return sin(0);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

			(*i).second = par[c];

		double result = sin(obj->param['x']);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

			(*i).second = 0;

		obj->c_pos = 0;

		return result;
	};

	m_cos.calculate = [](std::vector<double>& par, Func* obj) -> double {

		int c = 0;

		if (par.size() == 0)

			return 0;

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

			(*i).second = par[c];

		double result = cos(obj->param['x']);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)

			(*i).second = 0;

		obj->c_pos = 0;

		return result;
	};

	m_asin.calculate = [](std::vector<double>& par, Func* obj) -> double {
		int c = 0;

		if (par.size() == 0)
			return sin(0);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = par[c];
		double result = asin(obj->param['x']);
		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = 0;
		obj->c_pos = 0;
		return result;
	};

	m_acos.calculate = [](std::vector<double>& par, Func* obj) -> double {
		int c = 0;

		if (par.size() == 0)
			return sin(0);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = par[c];
		double result = acos(obj->param['x']);
		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = 0;
		obj->c_pos = 0;
		return result;
	};

	m_atg.calculate = [](std::vector<double>& par, Func* obj) -> double {
		int c = 0;

		if (par.size() == 0)
			return sin(0);

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = par[c];
		double result = atan(obj->param['x']);
		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = 0;
		obj->c_pos = 0;
		return result;
	};

	m_log.calculate = [](std::vector<double>& par, Func* obj) -> double {
		int c = 0;

		if (par.size() == 0)
			return 0;

		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = par[c];
		double result = log(obj->param['x']) / log(obj->param['y']);
		for (std::unordered_map<char, double>::iterator i = obj->param.begin(); i != obj->param.end(); i++, c++)
			(*i).second = 0;
		obj->c_pos = 0;
		return result;
	};

	prototypes[m_sin.name] = m_sin;
	prototypes[m_cos.name] = m_cos;
	prototypes[m_log.name] = m_log;
	prototypes[m_tg.name] = m_tg;
	prototypes[m_ctg.name] = m_ctg;
	prototypes[m_asin.name] = m_asin;
	prototypes[m_acos.name] = m_acos;
	prototypes[m_atg.name] = m_atg;
	prototypes[m_actg.name] = m_actg;
	prototypes[m_sh.name] = m_sh;
	prototypes[m_ch.name] = m_ch;
	variables["pi"] = M_PI;
	variables["e"] = exp(1);
	variables["iterator"] = 0;
}
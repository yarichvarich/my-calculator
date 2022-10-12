#include <Func.hpp>

std::vector<std::string> key_words = {"-import", "-help", "-repeat"};

std::string BUFF;

int main(int argc, char *argv[]) {

    std::cout << std::setprecision(10) << std::showpoint << std::fixed;

	setup_reserved();

	std::cout << "<: Welcome! You are using Calculator by Yaroslav." << std::endl;

	std::cout << "   Enter -help to get instructions." << std::endl;

	while (true) {

		std::cout << ">: ";

		read_buff(std::cin);

		if (BUFF == "-quit;")

			return 0;

		if (is_prototype()) {

			handle_prototype();
		}
		else if (is_variable()) {

			handle_variable();
		}
		else {
            
			if (BUFF.length() == 0)

				continue;

			Func BUFF_func{"BUFF_func"};

			BUFF_func.set_expression(BUFF);

			std::vector<double> e;

			std::cout << "<: " << BUFF_func.calculate(e, &BUFF_func) << std::endl;
		}

		BUFF.clear();
	}

    return 0;
}

bool is_prototype() {

	int rslt = BUFF.find('=');

	return ((rslt != std::string::npos && rslt > BUFF.find('(')) ? 1 : 0)
		&& (BUFF.find('(') < BUFF.find(')'))
		&& BUFF.find('(') != std::string::npos
		&& BUFF.find(')') != std::string::npos;
}

bool is_variable() {

	int e = BUFF.find('=');

	int b = BUFF.find('(');
	
	return e != std::string::npos && (e < b || b == std::string::npos);
}

void handle_variable() {

	std::string::iterator i = BUFF.begin();

	Func f{""}; std::vector<double> e;

	for (; *i != '='; i++)

		if (i == BUFF.end() - 1)

			return;

		else

			f.name += *i;

	i++;

	for (; i != BUFF.end(); i++)

		f.expression += *i;

	set_variable(f.name, f.calculate(e, &f));
}

void handle_prototype() {

	Func f{""};

	std::string::iterator i = BUFF.begin();

	for (; *i != '('; i++)

		if (i == BUFF.end() - 1)

			return;

		else

			f.name += *i;

	i++;

	for (; *i != ')'; i++)

		if (*i == ',')

			continue;

		else

			f.param[*i] = 0;

	i += 2;

	for (; i != BUFF.end(); i++)

		f.expression += *i;

	set_prototype(f.name, f);
}

template<typename STRM>
void handle_key_word(std::string key_word, STRM &stream) {

	if (key_word == "-help") {

		std::cout << help;

	}

	if (key_word == "-import") {

		std::string path;

		stream >> path;

		std::ifstream fst(path);

		if (!fst) {

			std::cout << "<: Cannot read from: " << path << std::endl;

			return;
		}

		while (fst.good()) {

			read_buff(fst);

			if (is_prototype()) {

				handle_prototype();

			}

			else if (is_variable()) {

				handle_variable();

			}

			else {

				if (BUFF.length() == 0)

					continue;

				Func BUFF_func{ "BUFF_func" };
				
				
				std::vector<double> e;

				std::cout << "<: " << BUFF_func.calculate(e, &BUFF_func) << std::endl;
			}

			BUFF.clear();

		}

		fst.close();
	}

	if (key_word == "-repeat") {

		repeat_handle(stream);

	}
}

template<typename STRM>
void read_buff(STRM &stream) {

	std::string inp;

	stream >> inp;

	if (std::find(key_words.begin(), key_words.end(), inp) != key_words.end()) {

		handle_key_word(inp, stream);

		return;
	}

	BUFF += inp;

	for (; inp.length() != 0 &&*(inp.end() - 1) != ';' && BUFF != "-end"; ) {

		stream >> inp;

		BUFF += inp;

		if (stream.eof())

			break;
	}

	if (BUFF == "-end")

		BUFF.clear();

	if (BUFF.find(';') == std::string::npos)

		BUFF.clear();
}

template<typename STRM>
void repeat_handle(STRM& stream) {

	int number; stream >> number;

	int r = 1, e = 0;

	std::string line, lines = "";

	while (!stream.eof()) {

		lines += line + "\n";

		if (line.substr(0, 7) == "-repeat") {

			r++;

		}
		if (line == "-end")

			e++;

		if (e >= r)

			break;

		if (std::is_same<STRM, std::istream>::value && stream.rdbuf()->in_avail() == 0)

			std::cout << ">: ";

		getline(stream, line);
	}
	for (int i = 0; i <= number; i++) {

		std::istringstream ss(lines);

		set_variable("iterator", i);

		while (ss.good()) {

			read_buff(ss);

			if (is_prototype()) {

				handle_prototype();

			}

			else if (is_variable()) {

				handle_variable();

			}

			else {

				if (BUFF.length() == 0)

					continue;

				Func BUFF_func{ "BUFF_func" };

				BUFF_func.expression = BUFF;

				std::vector<double> e;

				std::cout << "<: " << BUFF_func.calculate(e, &BUFF_func) << std::endl;

			}

			BUFF.clear();

		}
	}
}
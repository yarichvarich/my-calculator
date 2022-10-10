#include <Handlers.hpp>
#include <Func.hpp>
#include <iostream>

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

			BUFF_func.expression = BUFF;

			std::vector<double> e;

			std::cout << "<: " << BUFF_func.calculate(e, &BUFF_func) << std::endl;
		}

		BUFF.clear();
	}

    return 0;
}
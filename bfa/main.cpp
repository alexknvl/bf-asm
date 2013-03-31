#include <iostream>
#include <fstream>
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

class generator {
	public:
		std::string times(std::string x, std::string count) {
			std::string result = "";
			int c = boost::lexical_cast<int>(count);
			if (c == 0) return result;
			for (int i = 0; i < c; i++) {
				result += x;
			}
			return result;
		}
		std::string pop() { return "<[-]"; }
		std::string push(std::string v) {
			std::string result = "";
			result += "[-]";
			result += times("+", v);
			result += ">";
			return result;
		}
		std::string dup() { return "<[->+>+<<]>>[-<<+>>]"; }
		std::string pull(std::string arg1) {
			std::string result = "";
			result += "<";
			result += times("<", arg1);
			result += "[- >";
			result += times(">", arg1);
			result += "+ >+< <";
			result += times("<", arg1);
			result += "] >";
			result += times(">", arg1);
			result += "> [-<<";
			result += times("<", arg1);
			result += "+>>";
			result += times(">", arg1);
			result += "]";
			return result;
		}
		std::string move_down(std::string arg1) {
			std::string result = "";
			result += "< ";
			result += times("<", arg1);
			result += "[-]";
			result += times(">", arg1);
			result += "[-";
			result += times("<", arg1);
			result += "+";
			result += times(">", arg1);
			result += "]";
			return result;
		}
		std::string swap() {
			return "<[->+<]<[->+<]>>[-<<+>>]";
		}

		std::string addc(std::string arg1) {
			return std::string("<") + times("+", arg1) + ">";
		}
		std::string subc(std::string arg1) {
			return std::string("<") + times("-", arg1) + ">";
		}
		std::string mulc(std::string arg1) {
			return dup() + "<[-<" + times("+", arg1) + "->]";
		}
		std::string add() {
			return "<[-<+>]";
		}
		std::string sub() {
			return "<[-<->]";
		}
		std::string mul() {
			return swap() + dup() + "<<<-[->>> " + pull("1") + add() + " <<<]>>>" + move_down("2") + pop();
		}

		std::string read() {
			return "[-],>";
		}
		std::string write() {
			return "<.>";
		}

		/**
		 * if_not0_pop_do{x}	=	<[[-]x]
		 * if_0_pop_do{x}		=   <[>+<[-]] >[-<+>]< [-x]
		 * pop_if_eq{N,x}		=	<-N[x]
		 * pop2_if_eq{x}		=	<[-<->]<[x]
		 */

		std::string wnz_begin() {
			return "<[>";
		}
		std::string wnz_end() {
			return "<]>";
		}

		std::string ifnz_begin() {
			return dup() + "<[[-]";
		}
		std::string ifnz_end() {
			return "]";
		}
		std::string ifz_begin() {
			return dup() + "+<[>-<[-]]> [-<+>]< [-";
		}
		std::string ifz_end() {
			return "]";
		}

		std::string ifneqc_begin(std::string arg1) {
			return dup() + "<" + times("-", arg1) + "[[-]";
		}
		std::string ifneqc_end() {
			return "]";
		}
};

void parse_doc(std::ofstream& ofs, std::ifstream& ifs);

void parse_line(std::ofstream& ofs, std::string& line) {
	std::vector<std::string> args;
	generator gen;

	boost::split(args, line, boost::is_any_of("\t "));

	if (args.size() > 0) {
		if (args[0] == "pop") {
			ofs << gen.pop();
		} else if (args[0] == "push") {
			ofs << gen.push(args[1]);
		} else if (args[0] == "dup") {
			ofs << gen.dup();
		} else if (args[0] == "pull") {
			ofs << gen.pull(args[1]);
		} else if (args[0] == "move_down") {
			ofs << gen.move_down(args[1]);
		} else if (args[0] == "swap") {
			ofs << gen.swap();
		} else if (args[0] == "addc") {
			ofs << gen.addc(args[1]);
		} else if (args[0] == "subc") {
			ofs << gen.subc(args[1]);
		} else if (args[0] == "mulc") {
			ofs << gen.mulc(args[1]);
		} else if (args[0] == "add") {
			ofs << gen.add();
		} else if (args[0] == "sub") {
			ofs << gen.sub();
		} else if (args[0] == "mul") {
			ofs << gen.mul();
		} else if (args[0] == "read") {
			ofs << gen.read();
		} else if (args[0] == "write") {
			ofs << gen.write();
		} else if (args[0] == "wnz_begin") {
			ofs << gen.wnz_begin();
		} else if (args[0] == "wnz_end") {
			ofs << gen.wnz_end();
		} else if (args[0] == "ifnz_begin") {
			ofs << gen.ifnz_begin();
		} else if (args[0] == "ifnz_end") {
			ofs << gen.ifnz_end();
		} else if (args[0] == "ifz_begin") {
			ofs << gen.ifz_begin();
		} else if (args[0] == "ifz_end") {
			ofs << gen.ifz_end();
		} else if (args[0] == "ifneqc_begin") {
			ofs << gen.ifneqc_begin(args[1]);
		} else if (args[0] == "ifeqc_end") {
			ofs << gen.ifneqc_end();
		} else if (args[0] == "call") {
			std::ifstream ifs(args[1].c_str(), std::ios_base::in);
			parse_doc(ofs, ifs);
		} else if (args[0] == "//") {
			//skip...
		} else {
			boost::erase_all(line, " ");
			ofs << line;
		}
	} else {
		ofs << line;
	}
}

void parse_doc(std::ofstream& ofs, std::ifstream& ifs) {
	std::string line;

	while (!ifs.eof()) {
		std::getline(ifs, line);
		boost::trim(line);
		parse_line(ofs, line);
	}
}

int main(int argc, char** argv) {
	if (argc == 3) {
		std::ifstream ifs(argv[1], std::ios_base::in);
		std::ofstream ofs(argv[2], std::ios_base::out);

		parse_doc(ofs, ifs);
	}
}

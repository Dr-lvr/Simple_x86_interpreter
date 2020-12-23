#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <ctime>

using namespace std;

enum class tokens : int {
	//tokens enumeration
	mov, inc, dec, add,
	sub, mul, div, label,
	jmp, cmp, je, jge, jg,
	jle, jl, call, jne,
	ret, msg, end, comment
};
struct program_state {
	//program actual state
	string actual_op_code;
	string left_destination_const;
	string right_source_const;
	string register_stack;

	string msgToPrint;

	bool end_program = false;
	//-----------------------------------
	int last_blank = 0;
	int first_blank = 0;
	int comma = 0;
	int endline = 0;
	//----------------------------------
	int program_counter = 0;
	bool compare[3] = { false, false, false };

	int end_prg_line = 0;
	int ret_pc = 0;
};
struct program_maps {

	unordered_multimap<tokens, string> tokens_map{  {tokens::mov, "mov"},	{tokens::inc, "inc"},	{tokens::dec, "dec"},	{tokens::add, "add"},	{tokens::jne, "jne"},
													{tokens::sub, "sub"},	{tokens::mul, "mul"},	{tokens::div, "div"},	{tokens::label, "label"},
													{tokens::jmp, "jmp"},	{tokens::cmp, "cmp"},	{tokens::je, "je"},		{tokens::jge, "jge"},
													{tokens::jg, "jg"},		{tokens::jle, "jle"},	{tokens::jl, "jl"},		{tokens::call, "call"},
													{tokens::ret, "ret"},	{tokens::msg, "msg"},	{tokens::end, "end"},	{tokens::comment, ";"} };

	unordered_multimap<string, pair<int, int>> labels_map;
};
inline std::vector<string> rebuildProgram(std::vector<string> parsed_program) {

	vector<string> buildedPrg;
	string temp_string;
	bool EOL = false;
	for (auto n : parsed_program) {
		if (n.size()) {
			temp_string.append(n);
		}
		for (auto m : n) {
			if (m == '\n') {
				EOL = true;
			}
		}
		if (EOL) {
			buildedPrg.push_back(temp_string);
			temp_string.erase(temp_string.begin(), temp_string.end());
		}
		else {
			if (n.size()) {
				temp_string.append(" ");
			}
		}
		EOL = false;
	}
	return buildedPrg;
}
inline std::vector<string> atomizeProgram(std::vector<string> parsed_program) {

	vector<string> atomized_vector;
	vector<string> shrinked_vector;
	int first_BLANK;
	int last_BLANK;
	int end_CMD;

	for (int i = 0; i < (int)parsed_program.size(); i++) {
		while (parsed_program[i].size()) {
			first_BLANK = parsed_program[i].find_first_of(' ', 0);
			last_BLANK = parsed_program[i].find_first_of(' ', first_BLANK + 1);
			end_CMD = (int)parsed_program[i].size() - 1;

			if (first_BLANK < 0 && parsed_program.size()) {
				if (parsed_program[i] != " ") {
					atomized_vector.push_back(parsed_program[i]);
				}
				parsed_program[i].erase(parsed_program[i].begin(), parsed_program[i].end());
				break;
			}
			if (first_BLANK == last_BLANK) {
				atomized_vector.push_back(parsed_program[i].substr(0, last_BLANK + 1));
				parsed_program[i].erase(0, last_BLANK + 1);

				atomized_vector.push_back(parsed_program[i].substr(0, end_CMD + 1));
				parsed_program[i].erase(0, end_CMD + 1);
				break;
			}
			if (parsed_program.size()) {
				if (parsed_program[i].substr(0, first_BLANK + 1) != " ") {
					atomized_vector.push_back(parsed_program[i].substr(0, first_BLANK + 1));
				}
				parsed_program[i].erase(0, first_BLANK + 1);
			}
		}
	}
	for (int i = 0; i < (int)atomized_vector.size(); i++) {

		first_BLANK = atomized_vector[i].find_first_of(' ', 0);
		while (first_BLANK > -1) {

			first_BLANK = atomized_vector[i].find_first_of(' ', 0);
			if (first_BLANK == -1) break;

			atomized_vector[i] = atomized_vector[i].erase(first_BLANK, 1);
		}
		if (!atomized_vector[i].empty() && atomized_vector[i] != " ") {
			shrinked_vector.push_back(atomized_vector[i]);
		}
	}
	return shrinked_vector;
}
inline std::vector<string> appendEOL(std::vector<string> parsed_program) {

	for (int i = 0; i < (int)parsed_program.size(); i++) {

		parsed_program[i].append("\n");
	}
	return parsed_program;
}
inline std::vector<string> deleteComment(std::vector<string> parsed_program) {

	int pos = 0;
	for (int i = 0; i < (int)parsed_program.size(); i++) {
		pos = parsed_program[i].find_first_of(';', 0);
		if (pos != -1) {
			parsed_program[i].erase(pos, parsed_program[i].size() - pos);
		}
	}
	return parsed_program;
}
inline std::vector<string> parseByEOL(std::string program) {

	vector<string> parsedProgram;
	string equals(program);

	while (equals.size()) {

		parsedProgram.push_back(equals.substr(0, equals.find_first_of('\n', 0)));
		equals.erase(0, equals.find_first_of('\n', 0) + 1);
		if ((int)equals.find_first_of('\n', 0) == -1) {
			parsedProgram.push_back(equals);
			equals.erase(equals.begin(), equals.end());
		}
	}
	parsedProgram = deleteComment(parsedProgram);
	parsedProgram = appendEOL(parsedProgram);
	parsedProgram = atomizeProgram(parsedProgram);
	parsedProgram = rebuildProgram(parsedProgram);
	return parsedProgram;
}
inline unordered_multimap<string, pair<int, int>> mapLabels(std::vector<string> parsed_program) {

	unordered_multimap<string, pair<int, int>> mapped_labels;

	int first_blank;
	int EOL;
	string op_code;
	string called_label;

	int colon;
	int label_FBlank;
	string label;

	for (int i = 0; i < (int)parsed_program.size(); i++) {

		if (parsed_program[i].substr(0, parsed_program[i].find_first_of('\n', 0)) == "end") {

			mapped_labels.insert({ "end", { i, 0 } });
		}
		first_blank = -1;
		first_blank = parsed_program[i].find_first_of(' ', 0);

		if (first_blank > -1) {
			op_code = parsed_program[i].substr(0, first_blank);
			if (op_code == "jne" || op_code == "jmp" || op_code == "je" || op_code == "jge" || op_code == "jg" || op_code == "jle" || op_code == "jl" || op_code == "call") {

				EOL = parsed_program[i].find_first_of('\n', 0);
				called_label = parsed_program[i].substr(first_blank + 1, EOL);
				called_label.erase(called_label.find_first_of('\n', 0), 1);

				for (int j = 0; j < (int)parsed_program.size(); j++) {

					label_FBlank = -1;
					colon = -1;
					label_FBlank = parsed_program[j].find_first_of(' ', 0);
					colon = parsed_program[j].find_first_of(':', 0);

					if (label_FBlank > 0) {
						label = parsed_program[j].substr(0, label_FBlank);

						if (label == called_label) {
							mapped_labels.insert({ label, { i,j } });
							break;
						}
					}
					if (colon > 0) {
						label = parsed_program[j].substr(0, colon);
						if (label == called_label) {
							mapped_labels.insert({ label, {i, j} });
							break;
						}
					}
				}
			}
		}
	}
	return mapped_labels;
}
inline string printMsg(string msgLine, unordered_map<string, int> registers) {

	stack<char> strn_pt_stack;
	string temp_strn = msgLine.substr(msgLine.find_first_of(' ', 0) + 1, (msgLine.size() - msgLine.find_first_of(' ', 0)));
	string reg_strn;
	string final_strn;

	for (int i = 0; i < (int)temp_strn.size(); i++) {

		reg_strn.push_back(temp_strn.at(i));

		if (temp_strn.at(i) == '\'') {
			if (strn_pt_stack.empty()) {
				strn_pt_stack.push(temp_strn.at(i));
			}
			else {
				strn_pt_stack.pop();
			}

		}
		else {
			if (strn_pt_stack.empty()) {

				for (auto n : registers) {

					if (n.first == reg_strn) {
						final_strn.append(to_string(n.second));
					}
				}
			}
			else {
				final_strn.append(reg_strn);
			}
		}
		reg_strn.erase(reg_strn.begin(), reg_strn.end());
	}
	return final_strn;
}
std::string assembler_interpreter(std::string program) {

	if (program.empty()) {
		return "-1";
	}
	program_state program_actual_state;
	program_maps actual_program_maps;
	vector<string> parsed_program = parseByEOL(program);

	actual_program_maps.labels_map = mapLabels(parsed_program);

	for (auto n : actual_program_maps.labels_map) {

		if (n.first == "end") {
			program_actual_state.end_prg_line = n.second.first;
			if (n.second.first == -1) {
				return "-1";
			}
		}
	}
	unordered_map<string, int> registers;
	unordered_map<string, int>::iterator it_dest;
	unordered_map<string, int>::iterator it_source;

	for (int pc = 0; pc < (int)parsed_program.size(); pc++) {

		program_actual_state.first_blank = parsed_program[pc].find_first_of(' ', 0);
		program_actual_state.last_blank = parsed_program[pc].find_first_of(' ', program_actual_state.first_blank + 1);
		program_actual_state.endline = parsed_program[pc].find_first_of('\n', 0);
		if (program_actual_state.first_blank != -1) {

			program_actual_state.actual_op_code = parsed_program[pc].substr(0, program_actual_state.first_blank);
			if ((int)program_actual_state.actual_op_code.find_first_of(':', 0) != -1) {

				program_actual_state.actual_op_code = program_actual_state.actual_op_code.substr(0, program_actual_state.actual_op_code.find_first_of(':', 0));
			}
		}
		else {

			program_actual_state.actual_op_code = parsed_program[pc].substr(0, program_actual_state.endline);
			if ((int)program_actual_state.actual_op_code.find_first_of(':', 0) != -1) {

				program_actual_state.actual_op_code = program_actual_state.actual_op_code.substr(0, program_actual_state.actual_op_code.find_first_of(':', 0));
			}
		}

		if (program_actual_state.actual_op_code != "msg") {

			if (program_actual_state.first_blank > -1 && program_actual_state.last_blank > -1) {

				program_actual_state.left_destination_const = parsed_program[pc].substr(program_actual_state.first_blank + 1, (parsed_program[pc].find_first_of(',', 0) - program_actual_state.first_blank) - 1);
				program_actual_state.right_source_const = parsed_program[pc].substr(program_actual_state.last_blank + 1, (program_actual_state.endline - program_actual_state.last_blank) - 1);
				if ((int)program_actual_state.right_source_const.find_first_of(' ', 0) != -1) {
					program_actual_state.right_source_const.erase(program_actual_state.right_source_const.find_first_of(' ', 0), 1);
				}
			}
			if ((int)program_actual_state.last_blank == -1) {

				program_actual_state.left_destination_const = parsed_program[pc].substr(program_actual_state.first_blank + 1, (program_actual_state.endline - program_actual_state.first_blank) - 1);
			}
		}
		if (program_actual_state.left_destination_const.size()) {
			if (islower(program_actual_state.left_destination_const.at(0)) && program_actual_state.left_destination_const.size() == 1) {
				it_dest = registers.find(program_actual_state.left_destination_const);
				if (it_dest == registers.end()) {
					registers.insert({ program_actual_state.left_destination_const, 0 });
				}
			}
		}
		if (program_actual_state.right_source_const.size()) {
			if (islower(program_actual_state.right_source_const.at(0)) && program_actual_state.right_source_const.size() == 1) {
				it_dest = registers.find(program_actual_state.right_source_const);
				if (it_dest == registers.end()) {
					registers.insert({ program_actual_state.right_source_const, 0 });
				}
			}
		}
		for (auto n : actual_program_maps.tokens_map) {
			if (program_actual_state.actual_op_code == n.second) {

				switch (n.first) {
				case tokens::msg:
					if(!program_actual_state.msgToPrint.empty()){
						program_actual_state.msgToPrint.append("\n");
						program_actual_state.msgToPrint = printMsg(parsed_program[pc], registers);
						break;
					} 
					program_actual_state.msgToPrint = printMsg(parsed_program[pc], registers);
					break;
				case tokens::ret:
					pc = program_actual_state.ret_pc;
					break;
				case tokens::end:
					if (program_actual_state.msgToPrint.size()) {

						return program_actual_state.msgToPrint;
					} 
					return "-1";
				case tokens::mov:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								it_dest->second = it_source->second;
							}
						}
						else {
							it_dest->second = stoi(program_actual_state.right_source_const);
						}
					}
					break;
				case tokens::call:
					for (auto m : actual_program_maps.labels_map) {

						if (m.second.first == pc) {
							pc = m.second.second;

							if (m.second.first < program_actual_state.end_prg_line) {

								program_actual_state.ret_pc = m.second.first;
							}
							break;
						}
					}
					break;
				case tokens::cmp:
					for (int i = 0; i < 3; i++) {
						program_actual_state.compare[i] = false;
					}
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								if (it_dest->second == it_source->second) {
									program_actual_state.compare[1] = true;
								}
								if (it_dest->second < it_source->second) {
									program_actual_state.compare[0] = true;
								}
								if (it_dest->second > it_source->second) {
									program_actual_state.compare[2] = true;
								}
							}
						}
						else {
							if (it_dest->second == stoi(program_actual_state.right_source_const)) {
								program_actual_state.compare[1] = true;
							}
							if (it_dest->second < stoi(program_actual_state.right_source_const)) {
								program_actual_state.compare[0] = true;
							}
							if (it_dest->second > stoi(program_actual_state.right_source_const)) {
								program_actual_state.compare[2] = true;
							}
						}
					}
					break;
				case tokens::jne:
					if (!program_actual_state.compare[1]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::je:
					if (program_actual_state.compare[1]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::jge:
					if (program_actual_state.compare[1] || program_actual_state.compare[2]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::jg:
					if (program_actual_state.compare[2]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::jle:
					if (program_actual_state.compare[0] || program_actual_state.compare[1]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::jl:
					if (program_actual_state.compare[0]) {
						for (auto a : actual_program_maps.labels_map) {
							if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
								pc = a.second.second;
							}
						}
					}
					break;
				case tokens::jmp:
					for (auto a : actual_program_maps.labels_map) {
						if (a.first == program_actual_state.left_destination_const && a.second.first == pc) {
							pc = a.second.second;
						}
					}
					break;
				case tokens::inc:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						it_dest->second++;
					}
					break;
				case tokens::dec:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						it_dest->second--;
					}
					break;
				case tokens::add:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								it_dest->second += it_source->second;
							}
						}
						else {
							it_dest->second += stoi(program_actual_state.right_source_const);
						}
					}
					break;
				case tokens::sub:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								it_dest->second -= it_source->second;
							}
						}
						else {
							it_dest->second -= stoi(program_actual_state.right_source_const);
						}
					}
					break;
				case tokens::mul:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								it_dest->second *= it_source->second;
							}
						}
						else {
							it_dest->second *= stoi(program_actual_state.right_source_const);
						}
					}
					break;
				case tokens::div:
					it_dest = registers.find(program_actual_state.left_destination_const);
					if (it_dest != registers.end()) {
						if (islower(program_actual_state.right_source_const.at(0))) {
							it_source = registers.find(program_actual_state.right_source_const);
							if (it_source != registers.end()) {
								it_dest->second /= it_source->second;
							}
						}
						else {
							it_dest->second /= stoi(program_actual_state.right_source_const);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		program_actual_state.actual_op_code.erase(program_actual_state.actual_op_code.begin(), program_actual_state.actual_op_code.end());
		program_actual_state.left_destination_const.erase(program_actual_state.left_destination_const.begin(), program_actual_state.left_destination_const.end());
		program_actual_state.right_source_const.erase(program_actual_state.right_source_const.begin(), program_actual_state.right_source_const.end());

	}
	return "-1";
}

//DRIVER
int main() {

	program_maps first_map;

	time_t timer = 0;
	time_t timer_end = 0;

	time(&timer);

	std::string program = R"(
; My first program
mov  a, 5
inc  a
call function
msg  '(5+1)/2 = ', a    ; output message
end

function:
    div  a, 2
    ret)";

	/*std::string program = R"( //FIBONACCI SERIES
mov   a, 8            ; value
mov   b, 0            ; next
mov   c, 0            ; counter
mov   d, 0            ; first
mov   e, 1            ; second
call  proc_fib
call  print
end

proc_fib:
    cmp   c, 2
    jl    func_0
    mov   b, d
    add   b, e
    mov   d, e
    mov   e, b
    inc   c
    cmp   c, a
    jle   proc_fib
    ret

func_0:
    mov   b, c
    inc   c
    jmp   proc_fib

print:
    msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text
    ret)"; */

	/*std::string program = R"(
mov   a, 11           ; value1
mov   b, 3            ; value2
call  mod_func
msg   'mod(', a, ', ', b, ') = ', d        ; output
end

; Mod function
mod_func:
    mov   c, a        ; temp1
    div   c, b
    mul   c, b
    mov   d, a        ; temp2
    sub   d, c
    ret)";*/

	/*std::string program = R"(
mov   a, 81         ; value1
mov   b, 153        ; value2
call  init
call  proc_gcd
call  print
end

proc_gcd:
    cmp   c, d
    jne   loop
    ret

loop:
    cmp   c, d
    jg    a_bigger
    jmp   b_bigger

a_bigger:
    sub   c, d
    jmp   proc_gcd

b_bigger:
    sub   d, c
    jmp   proc_gcd

init:
    cmp   a, 0
    jl    a_abs
    cmp   b, 0
    jl    b_abs
    mov   c, a            ; temp1
    mov   d, b            ; temp2
    ret

a_abs:
    mul   a, -1
    jmp   init

b_abs:
    mul   b, -1
    jmp   init

print:
    msg   'gcd(', a, ', ', b, ') = ', c
    ret)";*/

/*std::string program = R"(
call  func1
call  print
end

func1:
    call  func2
    ret

func2:
    ret

print:
    msg 'This program should return -1')";*/

/*program = R"(
mov   a, 2            ; value1
mov   b, 10           ; value2
mov   c, a            ; temp1
mov   d, b            ; temp2
call  proc_func
call  print
end

proc_func:
    cmp   d, 1
    je    continue
    mul   c, a
    dec   d
    call  proc_func

continue:
    ret

print:
    msg a, '^', b, ' = ', c
    ret)";*/

	string equals = assembler_interpreter(program);
	

	cout << "Risultato del programma: |" << equals << "|" << endl;

	time(&timer_end);
	cout << timer << " " << timer_end << endl;
}

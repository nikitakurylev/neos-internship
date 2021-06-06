#include <iostream>
#include <fstream>
#include <vector>
#include "packet.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	char* input_file = NULL;
	char* log_file = NULL;

	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "--input") {
			i++;
			input_file = argv[i];
		}
		else if (arg == "--log") {
			i++;
			log_file = argv[i];
		}
		else {
			cout << "Unrecognized command-line option" << endl;
			return 0;
		}
	}

	if (input_file == NULL) {
		cout << "Specify input file" << endl;
		return 0;
	}
	else if (log_file == NULL) {
		cout << "Specify log file" << endl;
		return 0;
	}

	ifstream input(input_file, std::ios::binary);
	if (!input) {
		cout << "Invalid input file" << endl;
		return 0;
	}
	vector<char> bytes(
		(istreambuf_iterator<char>(input)),
		(istreambuf_iterator<char>()));
	input.close();
	
	vector<Packet> packets = ReadPackets(bytes);
	
	ofstream log(log_file);
	if (!log) {
		cout << "Invalid log file" << endl;
		return 0;
	}
	for (Packet packet : packets)
		log << packet << '\n';
	log.close();
}


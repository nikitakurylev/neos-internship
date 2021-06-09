#include <iostream>
#include <fstream>
#include <vector>
#include <pcap.h>
#include "packet.hpp"

using namespace std;

vector<char> g_buffer;

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	int dataLength = pkthdr->len;
	for(int i = 16; i < dataLength; i++)
		g_buffer.push_back(packet[i]);
}

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
			return 1;
		}
	}

	if (input_file == NULL) {
		cout << "Specify input file" << endl;
		return 1;
	}
	else if (log_file == NULL) {
		cout << "Specify log file" << endl;
		return 1;
	}

	pcap_t *descr;
	char errbuf[PCAP_ERRBUF_SIZE];

	descr = pcap_open_offline(input_file, errbuf);
	if (descr == NULL) {
		cout << "Invalid input file: " << errbuf << endl;
		return 1;
	}
	if (pcap_loop(descr, 0, packetHandler, NULL) < 0) {
		cout << "pcap_loop() failed: " << pcap_geterr(descr);
		return 1;
	}

	vector<Packet> packets = ReadPackets(g_buffer);

	ofstream log(log_file);
	if (!log) {
		cout << "Invalid log file" << endl;
		return 0;
	}
	for (Packet packet : packets)
		log << packet << '\n';
	log.close();
}

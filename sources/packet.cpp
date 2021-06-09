#include "packet.hpp"
#include <sstream>
#include <iomanip>
#include <vector>

string ByteToStr(unsigned char byte) {
	stringstream ss;
	ss << "0x" << setw(2) << setfill('0') << hex << (int)byte;
	return ss.str();
}

Packet::Packet(vector<char> buffer) : tid(0), sid(0) {
	flags.buffer = buffer[0];
	int offset = 1 + flags.extended_flags;
	if (flags.has_tid)
		tid = buffer[offset];
	offset += flags.has_tid;
	if (flags.has_sid)
		sid = buffer[offset];
	offset += flags.has_sid;
	did = buffer[offset];
	cid = buffer[offset + 1];
	seq = buffer[offset + 2];
	err = buffer[offset + 3];
}

ostream& operator<<(ostream& os, const Packet& packet)
{
	if (packet.flags.request_response)
		os << "Request Packet";
	else
		os << "Response Packet";
	os << " (flg: " << ByteToStr(packet.flags.buffer);
	if (packet.flags.has_tid)
		os << " tid: " << ByteToStr(packet.tid);
	if (packet.flags.has_sid)
		os << " sid: " << ByteToStr(packet.sid);
	os << " did: " << ByteToStr(packet.did) << " cid: " << ByteToStr(packet.cid) << " seq: " << ByteToStr(packet.seq);
	if (packet.err < 0x0B) {
		string error_type[]{
			"Success",
			"Bad Device ID",
			"Bad Command ID",
			"Not Yet Implemented",
			"Command is Restricted",
			"Bad Data Length",
			"Command Failed",
			"Bad Parameter Value",
			"Busy"
		};
		os << " err: " << error_type[packet.err];
	}
	os << ')';
	return os;
}

vector<Packet> ReadPackets(vector<char> bytes) {
	vector<Packet> result;
	bool reading = false;
	vector<char> buffer;
	Packet packet;
	for (unsigned char byte : bytes) {
		if (byte == 0x8d) {
			buffer = vector<char>();
			reading = true;
		}
		else if (reading) {
			if (byte == 0xd8) {
				reading = false;
				packet = Packet(buffer);
				result.push_back(packet);
			}
			else
				buffer.push_back(byte);
		}
	}
	return result;
}

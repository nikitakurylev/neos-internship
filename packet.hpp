#ifndef ROUTES_H
#define ROUTES_H
#include <vector>
#include <string>

using namespace std;

string ByteToStr(unsigned char byte);

class Packet {
public:
	Packet() :
		tid(),
		sid(),
		did(),
		cid(),
		seq(),
		err()
	{
		flags.buffer = 0;
	}
	Packet(vector<char> buffer);
	friend ostream& operator<<(ostream& os, const Packet& dt);
private:
	union {
		char buffer;
		struct {
			bool          is_response : 1;
			bool          request_response : 1;
			bool          only_error : 1;
			bool          is_activity : 1;
			bool          has_tid : 1;
			bool          has_sid : 1;
			bool : 1;
			bool          extended_flags : 1;
		};
	}flags;
	unsigned char tid;
	unsigned char sid;
	unsigned char did;
	unsigned char cid;
	unsigned char seq;
	unsigned char err;
};

vector<Packet> ReadPackets(vector<char> bytes);

#endif
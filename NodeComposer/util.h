#pragma once
#include "base.h"

namespace ncp
{
	constexpr std::string_view NCP_VERSION = "0.0.1";

	extern uint32_t tempo;
	extern uint32_t tpq;
	extern std::vector<std::thread> threads;
	extern RtMidiOut* midiout;

	namespace debug
	{
		//debug lua stack
		void PrintLuaStack(lua_State*);
	}

	typedef struct NCPVersion
	{
		size_t Major, Minor, Patch;
	};

	namespace exf
	{
		//convertor
		QPoint QSize2QPoint(const QSize&);
		QSize QPoint2QSize(const QPoint&);

		//remove from active thread
		void removeThread(std::thread::id);

		//string to ... with no error
		bool string2bool(const std::string&);
		int string2int(const std::string&);
		float string2float(const std::string&);

		//miditime to millisecond
		int miditime2millisecond(const int&);

		//setup midifile to save
		void setupMidiFile(smf::MidiFile&);

		//get version
		NCPVersion string2version(const std::string&);
	}
}

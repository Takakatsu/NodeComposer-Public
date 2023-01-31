#include "base.h"

namespace ncp
{
	uint32_t tempo;//tempo
	uint32_t tpq;//TicksPerQuarter
	RtMidiOut* midiout;//midi port
	std::vector<std::thread> threads;//message thread

	namespace debug
	{
		void PrintLuaStack(lua_State* L)
		{
			std::cout << "PrintStack" << std::endl;
			std::string str_type = "[";
			std::string str_num = "[";
			int top = lua_gettop(L);
			for (int i = 1; i <= top; i++)
			{
				str_type += " ";
				str_type += luaL_typename(L, i);
				str_num += " ";
				switch (lua_type(L, i))
				{
				case LUA_TNUMBER:
					str_num += std::to_string(lua_tonumber(L, i));
					break;
				case LUA_TSTRING:
					str_num += lua_tostring(L, i);
					break;
				case LUA_TBOOLEAN:
					str_num += (lua_toboolean(L, i) ? "true" : "false");
					break;
				case LUA_TNIL:
					str_num += "nil";
					break;
				default:
					str_num += "pointer";
					break;
				}
			}
			std::cout << "types : " << str_type << " ]" << std::endl;
			std::cout << "nums  : " << str_num << " ]" << std::endl;
		}
	}

	namespace exf
	{
		QPoint QSize2QPoint(const QSize& s) { return QPoint(s.width(), s.height()); }
		QSize QPoint2QSize(const QPoint& s) { return QSize(s.x(), s.y()); }

		void removeThread(std::thread::id id)
		{
			auto iter = find_if(threads.begin(), threads.end(), [=](std::thread& t) { return (t.get_id() == id); });
			if (iter != threads.end())
			{
				iter->detach();
				threads.erase(iter);
			}
		}

		bool string2bool(const std::string& str)
		{
			if (str == "0" || str == "false")return false;
			return true;
		}
		int string2int(const std::string& str)
		{
			try { return std::stoi(str); }
			catch (const std::invalid_argument& e) { return 0; }
			catch (const std::out_of_range& e) { return 0; }
			return 0;
		}
		float string2float(const std::string& str)
		{
			try { return std::stof(str); }
			catch (const std::invalid_argument& e) { return 0; }
			catch (const std::out_of_range& e) { return 0; }
			return 0;
		}

		int miditime2millisecond(const int& t)
		{
			return 60000 * t / (tpq * tempo);
		}

		void setupMidiFile(smf::MidiFile& midi)
		{
			midi.setTPQ(tpq);
			midi.addTimbre(0, 0, 0, 0);
		}
	}
}


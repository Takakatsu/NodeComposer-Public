#include "base.h"

namespace ncp
{
	Sound::Sound() {}
	const void Sound::push2Lua(lua_State* L) {}
	const void Sound::play() {};
	smf::MidiFile Sound::makeMidiFile() { return smf::MidiFile(); };
	const std::string Sound::toString() { return "Sound none"; }

	Tone::Tone() { }
	Tone::Tone(uint8_t c, uint8_t k, uint8_t v, uint32_t d)
	{
		channel = c;
		key = k;
		velocity = v;
		duration = d;
	};
	Tone::Tone(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		lua_next(L, t);
		channel = lua_tointeger(L, -1);
		lua_pop(L, 1);
		lua_next(L, t);
		key = lua_tointeger(L, -1);
		lua_pop(L, 1);
		lua_next(L, t);
		velocity = lua_tointeger(L, -1);
		lua_pop(L, 1);
		lua_next(L, t);
		duration = lua_tointeger(L, -1);
		lua_pop(L, 2);
	}
	const uint8_t& Tone::getChannel() { return channel; };
	void Tone::setChannel(const uint8_t& t) { channel = t; };
	const uint8_t& Tone::getKey() { return key; };
	void Tone::setKey(const uint8_t& t) { key = t; };
	const uint8_t& Tone::getVelocity() { return velocity; };
	void Tone::setVelocity(const uint8_t& t) { velocity = t; };
	const uint32_t& Tone::getDuration() { return duration; };
	void Tone::setDuration(const uint32_t& t) { duration = t; };
	const void Tone::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		lua_pushnumber(L, channel);
		lua_rawseti(L, -2, 1);
		lua_pushnumber(L, key);
		lua_rawseti(L, -2, 2);
		lua_pushnumber(L, velocity);
		lua_rawseti(L, -2, 3);
		lua_pushnumber(L, duration);
		lua_rawseti(L, -2, 4);
	}
	const void Tone::play()
	{
		std::vector<uint8_t> noteon = { 0x90,key,velocity };
		midiout->sendMessage(&noteon);
		threads.push_back(std::thread([this]
			{
				std::this_thread::sleep_for(std::chrono::milliseconds{ exf::miditime2millisecond(duration) });
		std::vector<uint8_t> noteoff = { 0x80,key,0 };
		midiout->sendMessage(&noteoff);
		exf::removeThread(std::this_thread::get_id());
			}));
	}
	smf::MidiFile Tone::makeMidiFile()
	{
		smf::MidiFile midi;
		exf::setupMidiFile(midi);
		midi.addNoteOn(0, 0, channel, key, velocity);
		midi.addNoteOff(0, duration, channel, key);
		return midi;
	}
	const std::string Tone::toString() { return "Tone : pitch " + std::to_string(key) + ", velocity " + std::to_string(velocity) + ", duration " + std::to_string(duration); }
	bool Tone::operator<(const Tone& rt)const { return key > rt.key; }

	Chord::Chord() { }
	Chord::Chord(uint8_t c, std::set<uint8_t> k, uint8_t v, uint32_t d)
	{
		channel = c;
		keys = k;
		velocity = v;
		duration = d;
	};
	Chord::Chord(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		lua_next(L, t);
		channel = lua_tointeger(L, -1);
		lua_pop(L, 1);
		lua_next(L, t);
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			keys.insert(lua_tointeger(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		lua_next(L, t);
		velocity = lua_tointeger(L, -1);
		lua_pop(L, 1);
		lua_next(L, t);
		duration = lua_tointeger(L, -1);
		lua_pop(L, 2);
	}
	const uint8_t& Chord::getChannel() { return channel; };
	void Chord::setChannel(const uint8_t& t) { channel = t; };
	const std::set<uint8_t>& Chord::getKeys() { return keys; };
	void Chord::setKeys(const std::set<uint8_t>& t) { keys = t; };
	void Chord::addKeys(const uint8_t& t) { keys.insert(t); };
	const uint8_t& Chord::getVelocity() { return velocity; };
	void Chord::setVelocity(const uint8_t& t) { velocity = t; };
	const uint32_t& Chord::getDuration() { return duration; };
	void Chord::setDuration(const uint32_t& t) { duration = t; };
	const void Chord::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		lua_pushnumber(L, channel);
		lua_rawseti(L, -2, 1);
		lua_newtable(L);
		int i = 1;
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			lua_pushnumber(L, *it);
			lua_rawseti(L, -2, i);
			i++;
		}
		lua_rawseti(L, -2, 2);
		lua_pushnumber(L, velocity);
		lua_rawseti(L, -2, 3);
		lua_pushnumber(L, duration);
		lua_rawseti(L, -2, 4);
	}
	const void Chord::play()
	{
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			std::vector<uint8_t> noteon = { 0x90,*it,velocity };
			midiout->sendMessage(&noteon);
		}
		threads.push_back(std::thread([this] {
			std::this_thread::sleep_for(std::chrono::milliseconds{ exf::miditime2millisecond(duration) });
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			std::vector<uint8_t> noteoff = { 0x80,*it,0 };
			midiout->sendMessage(&noteoff);
		}
		exf::removeThread(std::this_thread::get_id());
			}));;
	}
	smf::MidiFile Chord::makeMidiFile()
	{
		smf::MidiFile midi;
		exf::setupMidiFile(midi);
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			midi.addNoteOn(0, 0, channel, *it, velocity);
			midi.addNoteOff(0, duration, channel, *it);
		}
		midi.sortTracks();
		return midi;
	};
	const std::string Chord::toString()
	{
		std::string str;
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			str += std::to_string(*it) + ", ";
		}
		return "Chord : pitch " + str + "velocity " + std::to_string(velocity) + ", duration " + std::to_string(duration);
	}

	Melody::Melody() { }
	Melody::Melody(std::vector<std::pair<uint32_t, Tone>> t) { copy(t.begin(), t.end(), back_inserter(tones)); };
	Melody::Melody(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			lua_pushnil(L);
			lua_next(L, -2);
			uint32_t time = lua_tointeger(L, -1);
			lua_pop(L, 1);
			lua_next(L, -2);
			Tone tone = Tone(L, lua_gettop(L));
			lua_pop(L, 3);
			tones.push_back(std::make_pair(time, tone));
		}
	}
	const void Melody::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < tones.size(); i++)
		{
			lua_newtable(L);
			lua_pushnumber(L, tones[i].first);
			lua_rawseti(L, -2, 1);
			tones[i].second.push2Lua(L);
			lua_rawseti(L, -2, 2);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const void Melody::play()
	{
		//make data for play
		std::vector<std::pair<uint32_t, std::pair<Tone, bool>>> data;
		for (int i = 0; i < tones.size(); i++)
		{
			data.push_back(std::make_pair(tones[i].first, std::make_pair(tones[i].second, true)));
			data.push_back(std::make_pair(tones[i].first + tones[i].second.getDuration(), std::make_pair(tones[i].second, false)));
		}
		sort(data.begin(), data.end());
		//play
		threads.push_back(std::thread([data] {
			int t = 0;
		for (int i = 0; i < data.size(); i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds{ exf::miditime2millisecond(data[i].first - t) });
			Tone tone = data[i].second.first;
			std::vector<uint8_t> note = { (uint8_t)(data[i].second.second ? 0x90 : 0x80),tone.getKey(),tone.getVelocity() };
			midiout->sendMessage(&note);
			t = data[i].first;
		}
		exf::removeThread(std::this_thread::get_id());
			}));;
	}
	smf::MidiFile Melody::makeMidiFile()
	{
		smf::MidiFile midi;
		exf::setupMidiFile(midi);
		for (int i = 0; i < tones.size(); i++)
		{
			midi.addNoteOn(0, tones[i].first, tones[i].second.getChannel(), tones[i].second.getKey(), tones[i].second.getVelocity());
			midi.addNoteOff(0, tones[i].first + tones[i].second.getDuration(), tones[i].second.getChannel(), tones[i].second.getKey());
		}
		midi.sortTracks();
		return midi;
	};
	const std::string Melody::toString()
	{
		std::string str = "Melody : ";
		for (int i = 0; i < tones.size(); i++)
		{
			str += "  Time : " + std::to_string(tones[i].first) + "  " + tones[i].second.toString();
		}
		return str;
	}
}
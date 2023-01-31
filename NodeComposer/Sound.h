#pragma once
#include "base.h"

namespace ncp
{
	class Sound
	{
	private:
	public:
		Sound();
		virtual const void push2Lua(lua_State*);
		virtual const void play();
		virtual smf::MidiFile makeMidiFile();
		virtual const std::string toString();
	};

	class Tone : public Sound
	{
	private:
		uint8_t channel = 0;
		uint8_t key = 60;
		uint8_t velocity = 64;
		uint32_t duration = tpq;
	public:
		Tone();
		Tone(uint8_t, uint8_t, uint8_t, uint32_t);
		Tone(lua_State*, size_t);
		const uint8_t& getChannel();
		void setChannel(const uint8_t&);
		const uint8_t& getKey();
		void setKey(const uint8_t&);
		const uint8_t& getVelocity();
		void setVelocity(const uint8_t&);
		const uint32_t& getDuration();
		void setDuration(const uint32_t&);
		const void push2Lua(lua_State*)override;
		const void play()override;
		smf::MidiFile makeMidiFile()override;
		const std::string toString()override;
		bool operator<(const Tone&)const;
	};

	class Chord : public Sound
	{
	private:
		uint8_t channel = 0;
		std::set<uint8_t> keys;
		uint8_t velocity = 64;
		uint32_t duration = tpq;
	public:
		Chord();
		Chord(uint8_t,std::set<uint8_t>, uint8_t, uint32_t);
		Chord(lua_State*, size_t);
		const uint8_t& getChannel();
		void setChannel(const uint8_t&);
		const std::set<uint8_t>& getKeys();
		void setKeys(const std::set<uint8_t>&);
		void addKeys(const uint8_t&);
		const uint8_t& getVelocity();
		void setVelocity(const uint8_t&);
		const uint32_t& getDuration();
		void setDuration(const uint32_t&);
		const void push2Lua(lua_State*)override;
		const void play()override;
		smf::MidiFile makeMidiFile()override;
		const std::string toString()override;
	};

	class Melody : public Sound
	{
	private:
		std::vector<std::pair<uint32_t, Tone>> tones;
	public:
		Melody();
		Melody(std::vector<std::pair<uint32_t, Tone>>);
		Melody(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const void play()override;
		smf::MidiFile makeMidiFile()override;
		const std::string toString()override;
	};
}

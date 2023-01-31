#pragma once
#include "base.h"

namespace ncp
{
	class NodeVariable
	{
	public:
		NodeVariable();
		NodeVariable(lua_State*, size_t);
		virtual const void push2Lua(lua_State*);
		virtual const std::string toString();
		virtual Sound* getSound();
		virtual void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*);
		virtual void readForm(QList<QLineEdit*>*);
		virtual nlohmann::json makeJSON();
		virtual void loadJSON(nlohmann::json);
	};

	class NodeBool : public NodeVariable
	{
	private:
		bool n = false;
	public:
		NodeBool();
		NodeBool(bool);
		NodeBool(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeInt : public NodeVariable
	{
	private:
		int32_t n = 0;
	public:
		NodeInt();
		NodeInt(int32_t);
		NodeInt(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeFloat : public NodeVariable
	{
	private:
		float n = 0;
	public:
		NodeFloat();
		NodeFloat(float);
		NodeFloat(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeTone : public NodeVariable
	{
	private:
		Tone tone = Tone();
	public:
		NodeTone();
		NodeTone(Tone);
		NodeTone(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		Sound* getSound()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeChord : public NodeVariable
	{
	private:
		Chord chord = Chord();
	public:
		NodeChord();
		NodeChord(Chord);
		NodeChord(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		Sound* getSound()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeMelody : public NodeVariable
	{
	private:
		Melody melody = Melody();
	public:
		NodeMelody();
		NodeMelody(Melody);
		NodeMelody(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		Sound* getSound()override;
	};

	class NodeVectorBool : public NodeVariable
	{
	private:
		std::vector<bool> vec;
	public:
		NodeVectorBool();
		NodeVectorBool(std::vector<bool>);
		NodeVectorBool(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeVectorInt : public NodeVariable
	{
	private:
		std::vector<int32_t> vec;
	public:
		NodeVectorInt();
		NodeVectorInt(std::vector<int32_t>);
		NodeVectorInt(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeVectorFloat : public NodeVariable
	{
	private:
		std::vector<float> vec;
	public:
		NodeVectorFloat();
		NodeVectorFloat(std::vector<float>);
		NodeVectorFloat(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		void makeForm(QDialog*, QFormLayout*, QList<QLineEdit*>*)override;
		void readForm(QList<QLineEdit*>*)override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeVectorTone : public NodeVariable
	{
	private:
		std::vector<Tone> vec;
	public:
		NodeVectorTone();
		NodeVectorTone(std::vector<Tone>);
		NodeVectorTone(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeVectorChord : public NodeVariable
	{
	private:
		std::vector<Chord> vec;
	public:
		NodeVectorChord();
		NodeVectorChord(std::vector<Chord>);
		NodeVectorChord(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
		nlohmann::json makeJSON()override;
		void loadJSON(nlohmann::json)override;
	};

	class NodeVectorMelody : public NodeVariable
	{
	private:
		std::vector<Melody> vec;
	public:
		NodeVectorMelody();
		NodeVectorMelody(std::vector<Melody>);
		NodeVectorMelody(lua_State*, size_t);
		const void push2Lua(lua_State*)override;
		const std::string toString()override;
	};
}

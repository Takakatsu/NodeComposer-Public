#include "base.h"

namespace ncp
{
	NodeVariable::NodeVariable() {}
	NodeVariable::NodeVariable(lua_State* L, size_t t) {}
	const void NodeVariable::push2Lua(lua_State* L) {}
	const std::string NodeVariable::toString() { return "null"; }
	Sound* NodeVariable::getSound() { return new Tone(); }
	void NodeVariable::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields) {}
	void NodeVariable::readForm(QList<QLineEdit*>* fields) {}
	nlohmann::json NodeVariable::makeJSON()
	{
		nlohmann::json data;
		return data;
	}
	void NodeVariable::loadJSON(nlohmann::json data) {}

	NodeBool::NodeBool() {}
	NodeBool::NodeBool(bool t) :n(t) {}
	NodeBool::NodeBool(lua_State* L, size_t t) :NodeBool(bool(lua_toboolean(L, t))) {}
	const void NodeBool::push2Lua(lua_State* L) { lua_pushboolean(L, n); }
	const std::string NodeBool::toString() { return n ? "true" : "false"; }
	void NodeBool::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		le->setText(QString::fromStdString(n ? "true" : "false"));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_BOOL)), le);
		fields->append(le);
	}
	void NodeBool::readForm(QList<QLineEdit*>* fields) { n = exf::string2bool((*fields)[0]->text().toStdString()); }
	nlohmann::json NodeBool::makeJSON()
	{
		nlohmann::json data;
		data["value"] = n;
		return data;
	}
	void NodeBool::loadJSON(nlohmann::json data) { n = data["value"]; }

	NodeInt::NodeInt() { }
	NodeInt::NodeInt(int32_t t) :n(t) {}
	NodeInt::NodeInt(lua_State* L, size_t t) :NodeInt(int(lua_tointeger(L, t))) {}
	const void NodeInt::push2Lua(lua_State* L) { lua_pushnumber(L, n); }
	const std::string NodeInt::toString() { return std::to_string(n); }
	void NodeInt::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		le->setText(QString::fromStdString(std::to_string(n)));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_INT)), le);
		fields->append(le);
	}
	void NodeInt::readForm(QList<QLineEdit*>* fields) { n = exf::string2int((*fields)[0]->text().toStdString()); }
	nlohmann::json NodeInt::makeJSON()
	{
		nlohmann::json data;
		data["value"] = n;
		return data;
	}
	void NodeInt::loadJSON(nlohmann::json data)
	{
		n = data["value"];
	}

	NodeFloat::NodeFloat() { }
	NodeFloat::NodeFloat(float t) :n(t) {}
	NodeFloat::NodeFloat(lua_State* L, size_t t) :NodeFloat(float(lua_tonumber(L, t))) {}
	const void NodeFloat::push2Lua(lua_State* L) { lua_pushnumber(L, n); }
	const std::string NodeFloat::toString() { return std::to_string(n); }
	void NodeFloat::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		le->setText(QString::fromStdString(std::to_string(n)));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_FLOAT)), le);
		fields->append(le);
	}
	void NodeFloat::readForm(QList<QLineEdit*>* fields) { n = exf::string2float((*fields)[0]->text().toStdString()); }
	nlohmann::json NodeFloat::makeJSON()
	{
		nlohmann::json data;
		data["value"] = n;
		return data;
	}
	void NodeFloat::loadJSON(nlohmann::json data) { n = data["value"]; }

	NodeTone::NodeTone() { }
	NodeTone::NodeTone(Tone t) :tone(t) {}
	NodeTone::NodeTone(lua_State* L, size_t t) :NodeTone(Tone(L, t)) {}
	const void NodeTone::push2Lua(lua_State* L) { tone.push2Lua(L); }
	const std::string NodeTone::toString() { return tone.toString(); }
	Sound* NodeTone::getSound() { return &tone; }
	void NodeTone::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le0 = new QLineEdit(dialog);
		le0->setText(QString::fromStdString(std::to_string(tone.getChannel())));
		layout->addRow("channel", le0);
		fields->append(le0);
		QLineEdit* le1 = new QLineEdit(dialog);
		le1->setText(QString::fromStdString(std::to_string(tone.getKey())));
		layout->addRow("key", le1);
		fields->append(le1);
		QLineEdit* le2 = new QLineEdit(dialog);
		le2->setText(QString::fromStdString(std::to_string(tone.getVelocity())));
		layout->addRow("velocity", le2);
		fields->append(le2);
		QLineEdit* le3 = new QLineEdit(dialog);
		le3->setText(QString::fromStdString(std::to_string(tone.getDuration())));
		layout->addRow("duration", le3);
		fields->append(le3);
	}
	void NodeTone::readForm(QList<QLineEdit*>* fields)
	{
		tone.setChannel(exf::string2int((*fields)[0]->text().toStdString()));
		tone.setKey(exf::string2int((*fields)[1]->text().toStdString()));
		tone.setVelocity(exf::string2int((*fields)[2]->text().toStdString()));
		tone.setDuration(exf::string2int((*fields)[3]->text().toStdString()));
	}
	nlohmann::json NodeTone::makeJSON()
	{
		nlohmann::json data;
		data["channel"] = tone.getChannel();
		data["key"] = tone.getKey();
		data["velocity"] = tone.getVelocity();
		data["duration"] = tone.getDuration();
		return data;
	}
	void NodeTone::loadJSON(nlohmann::json data)
	{
		tone.setChannel(data["channel"]);
		tone.setKey(data["key"]);
		tone.setVelocity(data["velocity"]);
		tone.setDuration(data["duration"]);
	}

	NodeChord::NodeChord() { }
	NodeChord::NodeChord(Chord c) :chord(c) {}
	NodeChord::NodeChord(lua_State* L, size_t t) :NodeChord(Chord(L, t)) {}
	const void NodeChord::push2Lua(lua_State* L) { chord.push2Lua(L); }
	const std::string NodeChord::toString() { return chord.toString(); }
	Sound* NodeChord::getSound() { return &chord; }
	void NodeChord::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le0 = new QLineEdit(dialog);
		le0->setText(QString::fromStdString(std::to_string(chord.getChannel())));
		layout->addRow("channel", le0);
		fields->append(le0);
		QLineEdit* le1 = new QLineEdit(dialog);
		std::set<uint8_t> keys = chord.getKeys();
		std::string str = "";
		for (auto it = keys.begin(); it != keys.end(); ++it)str += "," + std::to_string(*it);
		if (!str.empty())str.erase(0, 1);
		le1->setText(QString::fromStdString(str));
		layout->addRow("keys", le1);
		fields->append(le1);
		QLineEdit* le2 = new QLineEdit(dialog);
		le2->setText(QString::fromStdString(std::to_string(chord.getVelocity())));
		layout->addRow("velocity", le2);
		fields->append(le2);
		QLineEdit* le3 = new QLineEdit(dialog);
		le3->setText(QString::fromStdString(std::to_string(chord.getDuration())));
		layout->addRow("duration", le3);
		fields->append(le3);
	}
	void NodeChord::readForm(QList<QLineEdit*>* fields)
	{
		chord.setChannel(exf::string2int((*fields)[0]->text().toStdString()));
		std::string str = (*fields)[1]->text().toStdString();
		boost::tokenizer<boost::char_separator<char>> tokens_key(str, boost::char_separator<char>("-, "));
		std::set<uint8_t> keys;
		for (auto it = tokens_key.begin(); it != tokens_key.end(); ++it)
		{
			keys.insert(exf::string2int(it.current_token()));
		}
		chord.setKeys(keys);
		chord.setVelocity(exf::string2int((*fields)[2]->text().toStdString()));
		chord.setDuration(exf::string2int((*fields)[3]->text().toStdString()));
	}
	nlohmann::json NodeChord::makeJSON()
	{
		nlohmann::json data;
		data["channel"] = chord.getChannel();
		data["keys"] = chord.getKeys();
		data["velocity"] = chord.getVelocity();
		data["duration"] = chord.getDuration();
		return data;
	}
	void NodeChord::loadJSON(nlohmann::json data)
	{
		chord.setChannel(data["cnannel"]);
		chord.setKeys(data["keys"]);
		chord.setVelocity(data["velocity"]);
		chord.setDuration(data["duration"]);
	}

	NodeMelody::NodeMelody() { }
	NodeMelody::NodeMelody(Melody m) :melody(m) {}
	NodeMelody::NodeMelody(lua_State* L, size_t t) :NodeMelody(Melody(L, t)) {}
	const void NodeMelody::push2Lua(lua_State* L) { melody.push2Lua(L); }
	const std::string NodeMelody::toString() { return melody.toString(); }
	Sound* NodeMelody::getSound() { return &melody; }

	NodeVectorBool::NodeVectorBool() {}
	NodeVectorBool::NodeVectorBool(std::vector<bool> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorBool::NodeVectorBool(lua_State* L, size_t t) :NodeVectorBool()
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(lua_toboolean(L, -1));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorBool::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			lua_pushboolean(L, vec[i]);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorBool::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += (vec[i] ? "true" : "false") + std::string(" ");
		}
		return str;
	}
	void NodeVectorBool::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		std::string str = "";
		for (int i = 0; i < vec.size(); i++)
		{
			str += ",";
			str += vec[i] ? "true" : "false";
		}
		if (!str.empty())str.erase(0, 1);
		le->setText(QString::fromStdString(str));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_VEC_BOOL)), le);
		fields->append(le);
	}
	void NodeVectorBool::readForm(QList<QLineEdit*>* fields)
	{
		std::string str = (*fields)[0]->text().toStdString();
		boost::tokenizer<boost::char_separator<char>> tokens(str, boost::char_separator<char>(", "));
		vec.clear();
		for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
			vec.push_back(exf::string2bool(it.current_token()));
		}
	}
	nlohmann::json NodeVectorBool::makeJSON()
	{
		nlohmann::json data;
		data["value"] = vec;
		return data;
	}
	void NodeVectorBool::loadJSON(nlohmann::json data)
	{
		vec.clear();
		std::vector<bool> v = data["value"];
		std::copy(v.begin(), v.end(), std::back_inserter(vec));
	}

	NodeVectorInt::NodeVectorInt() { }
	NodeVectorInt::NodeVectorInt(std::vector<int32_t> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorInt::NodeVectorInt(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorInt::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			lua_pushnumber(L, vec[i]);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorInt::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += std::to_string(vec[i]) + std::string(" ");
		}
		return str;
	}
	void NodeVectorInt::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		std::string str = "";
		for (int i = 0; i < vec.size(); i++)str += "," + std::to_string(vec[i]);
		if (!str.empty())str.erase(0, 1);
		le->setText(QString::fromStdString(str));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_VEC_INT)), le);
		fields->append(le);
	}
	void NodeVectorInt::readForm(QList<QLineEdit*>* fields)
	{
		std::string str = (*fields)[0]->text().toStdString();
		boost::tokenizer<boost::char_separator<char>> tokens(str, boost::char_separator<char>(", "));
		vec.clear();
		for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
			vec.push_back(exf::string2int(it.current_token()));
		}
	}
	nlohmann::json NodeVectorInt::makeJSON()
	{
		nlohmann::json data;
		data["value"] = vec;
		return data;
	}
	void NodeVectorInt::loadJSON(nlohmann::json data)
	{
		vec.clear();
		std::vector<int> v = data["value"];
		std::copy(v.begin(), v.end(), std::back_inserter(vec));
	}

	NodeVectorFloat::NodeVectorFloat() { }
	NodeVectorFloat::NodeVectorFloat(std::vector<float> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorFloat::NodeVectorFloat(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorFloat::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			lua_pushnumber(L, vec[i]);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorFloat::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += std::to_string(vec[i]) + std::string(" ");
		}
		return str;
	}
	void NodeVectorFloat::makeForm(QDialog* dialog, QFormLayout* layout, QList<QLineEdit*>* fields)
	{
		QLineEdit* le = new QLineEdit(dialog);
		std::string str = "";
		for (int i = 0; i < vec.size(); i++)str += "," + std::to_string(vec[i]);
		if (!str.empty())str.erase(0, 1);
		le->setText(QString::fromStdString(str));
		layout->addRow(QString::fromStdString(lib::TYPES2string(lib::TN_VEC_FLOAT)), le);
		fields->append(le);
	}
	void NodeVectorFloat::readForm(QList<QLineEdit*>* fields)
	{
		std::string str = (*fields)[0]->text().toStdString();
		boost::tokenizer<boost::char_separator<char>> tokens(str, boost::char_separator<char>(", "));
		vec.clear();
		for (auto it = tokens.begin(); it != tokens.end(); ++it)
		{
			vec.push_back(exf::string2float(it.current_token()));
		}
	}
	nlohmann::json NodeVectorFloat::makeJSON()
	{
		nlohmann::json data;
		data["value"] = vec;
		return data;
	}
	void NodeVectorFloat::loadJSON(nlohmann::json data)
	{
		vec.clear();
		std::vector<bool> v = data["value"];
		std::copy(v.begin(), v.end(), std::back_inserter(vec));
	}

	NodeVectorTone::NodeVectorTone() {}
	NodeVectorTone::NodeVectorTone(std::vector<Tone> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorTone::NodeVectorTone(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(Tone(L, lua_gettop(L)));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorTone::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			vec[i].push2Lua(L);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorTone::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += vec[i].toString() + std::string(" ");
		}
		return str;
	}
	nlohmann::json NodeVectorTone::makeJSON()
	{
		nlohmann::json data;
		data["tones"] = nlohmann::json::array();
		for (int i = 0; i < vec.size(); i++)
		{
			nlohmann::json tonedata;
			tonedata["channel"] = vec[i].getChannel();
			tonedata["key"] = vec[i].getKey();
			tonedata["velocity"] = vec[i].getVelocity();
			tonedata["duration"] = vec[i].getDuration();
			data["tones"].push_back(tonedata);
		}
		return data;
	}
	void NodeVectorTone::loadJSON(nlohmann::json data)
	{
		vec.clear();
		for (int i = 0; i < data["tones"].size(); i++)
		{
			Tone tone;
			tone.setChannel(data["tones"][i]["channel"]);
			tone.setKey(data["tones"][i]["key"]);
			tone.setVelocity(data["tones"][i]["velocity"]);
			tone.setDuration(data["tones"][i]["duration"]);
			vec.push_back(tone);
		}
	}

	NodeVectorChord::NodeVectorChord() {}
	NodeVectorChord::NodeVectorChord(std::vector<Chord> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorChord::NodeVectorChord(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(Chord(L, lua_gettop(L)));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorChord::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			vec[i].push2Lua(L);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorChord::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += vec[i].toString() + std::string(" ");
		}
		return str;
	}
	nlohmann::json NodeVectorChord::makeJSON()
	{
		nlohmann::json data;
		data["chords"] = nlohmann::json::array();
		for (int i = 0; i < vec.size(); i++)
		{
			nlohmann::json tonedata;
			tonedata["channel"] = vec[i].getChannel();
			tonedata["keys"] = vec[i].getKeys();
			tonedata["velocity"] = vec[i].getVelocity();
			tonedata["duration"] = vec[i].getDuration();
			data["chords"].push_back(tonedata);
		}
		return data;
	}
	void NodeVectorChord::loadJSON(nlohmann::json data)
	{
		vec.clear();
		for (int i = 0; i < data["chords"].size(); i++)
		{
			Chord chord;
			chord.setChannel(data["chords"][i]["channel"]);
			chord.setKeys(data["chords"][i]["keys"]);
			chord.setVelocity(data["chords"][i]["velocity"]);
			chord.setDuration(data["chords"][i]["duration"]);
			vec.push_back(chord);
		}
	}

	NodeVectorMelody::NodeVectorMelody() {}
	NodeVectorMelody::NodeVectorMelody(std::vector<Melody> v) { std::copy(v.begin(), v.end(), std::back_inserter(vec)); }
	NodeVectorMelody::NodeVectorMelody(lua_State* L, size_t t)
	{
		lua_pushnil(L);
		while (lua_next(L, t))
		{
			vec.push_back(Melody(L, lua_gettop(L)));
			lua_pop(L, 1);
		}
	}
	const void NodeVectorMelody::push2Lua(lua_State* L)
	{
		lua_newtable(L);
		for (int i = 0; i < vec.size(); i++)
		{
			vec[i].push2Lua(L);
			lua_rawseti(L, -2, i + 1);
		}
	}
	const std::string NodeVectorMelody::toString()
	{
		std::string str;
		for (int i = 0; i < vec.size(); i++)
		{
			str += vec[i].toString() + std::string(" ");
		}
		return str;
	}
}

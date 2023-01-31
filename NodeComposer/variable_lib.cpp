#include "base.h"

namespace ncp
{
	namespace lib
	{
		////data lists////
		boost::bimaps::bimap<TYPES, std::string> types_string;//types name map
		std::map<std::string, std::set<std::string>> nodelist;//list of node name
		std::map<std::string, NodeData> nodelibs;//list of node data
		std::map<TYPES, Qt::GlobalColor> types_color;//list of types color

		////make data////
		void loadAllData()
		{
			setTypesString();
			setTypesColor();
			loadScripts();
		}
		void setTypesString()
		{
			types_string.clear();
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_BOOL, "bool"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_INT, "int"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_FLOAT, "float"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_TONE, "Tone"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_CHORD, "Chord"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_MELODY, "Melody"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_BOOL, "vector<bool>"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_INT, "vector<int>"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_FLOAT, "vector<float>"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_TONE, "vector<Tone>"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_CHORD, "vector<Chord>"));
			types_string.insert(boost::bimaps::bimap<TYPES, std::string>::value_type(TN_VEC_MELODY, "vector<Melody>"));
		}
		void setTypesColor()
		{
			types_color[TN_NULL] = Qt::gray;
			types_color[TN_BOOL] = Qt::blue;
			types_color[TN_INT] = Qt::magenta;
			types_color[TN_FLOAT] = Qt::cyan;
			types_color[TN_TONE] = Qt::green;
			types_color[TN_CHORD] = Qt::yellow;
			types_color[TN_MELODY] = Qt::red;
			types_color[TN_VEC_BOOL] = Qt::darkBlue;
			types_color[TN_VEC_INT] = Qt::darkMagenta;
			types_color[TN_VEC_FLOAT] = Qt::darkCyan;
			types_color[TN_VEC_TONE] = Qt::darkGreen;
			types_color[TN_VEC_CHORD] = Qt::darkYellow;
			types_color[TN_VEC_MELODY] = Qt::darkRed;
		}
		std::string loadScript(std::filesystem::directory_entry file)
		{
			std::ifstream ifs(file.path());
			if (ifs.fail())
			{
				std::cout << "file load error." << std::endl;
				return "";
			}
			//load script data
			std::string name, args, rtns;
			getline(ifs, name);
			name.erase(0, 2);
			getline(ifs, args);
			boost::tokenizer<boost::char_separator<char>> tokens_arg(args, boost::char_separator<char>("-, "));
			getline(ifs, rtns);
			boost::tokenizer<boost::char_separator<char>> tokens_rtn(rtns, boost::char_separator<char>("-, &"));
			//make node data
			NodeData nd;
			nd.path = file.path().string();
			{
				//get num
				size_t num = 0;
				for (auto it = tokens_arg.begin(); it != tokens_arg.end(); ++it)
				{
					std::string str = it.current_token();
					size_t cnt = std::count(str.begin(), str.end(), '|');
					num = qMax(num, cnt + 1);
				}
				for (auto it = tokens_rtn.begin(); it != tokens_rtn.end(); ++it)
				{
					std::string str = it.current_token();
					size_t cnt = std::count(str.begin(), str.end(), '|');
					num = qMax(num, cnt + 1);
				}
				//make data
				for (int i = 0; i < num; i++)
				{
					std::pair<std::vector<std::set<TYPES>>, std::vector<TYPES>> tmp;
					nd.type_sets.push_back(tmp);
				}
				{
					int num_arg = 0;
					for (auto it = tokens_arg.begin(); it != tokens_arg.end(); ++it)
					{
						int num_alter = 0;
						std::string str = it.current_token();
						boost::tokenizer<boost::char_separator<char>> arg_separated(str, boost::char_separator<char>("|"));
						auto itr = arg_separated.begin();
						std::string stri;
						for (int i = 0; i < num; i++)
						{
							std::set<TYPES> tmp;
							if (itr != arg_separated.end())stri = itr.current_token();
							boost::tokenizer<boost::char_separator<char>> arg_list(stri, boost::char_separator<char>("&"));
							for (auto itra = arg_list.begin(); itra != arg_list.end(); ++itra)
							{
								tmp.insert(string2TYPES(itra.current_token()));
							}
							nd.type_sets[num_alter].first.push_back(tmp);
							num_alter++;
							++itr;
						}
						num_arg++;
					}
				}
				{
					int num_rtn = 0;
					for (auto it = tokens_rtn.begin(); it != tokens_rtn.end(); ++it)
					{
						int num_alter = 0;
						std::string str = it.current_token();
						boost::tokenizer<boost::char_separator<char>> rtn_separated(str, boost::char_separator<char>("|"));
						auto itr = rtn_separated.begin();
						std::string stri;
						for (int i = 0; i < num; i++)
						{
							if (itr != rtn_separated.end())stri = itr.current_token();
							nd.type_sets[num_alter].second.push_back(string2TYPES(stri));
							num_alter++;
							itr++;
						}
						num_rtn++;
					}
				}
			}
			std::cout << name << " end" << std::endl;
			nodelibs.insert(make_pair(name, nd));
			return name;
		}
		void loadScripts()
		{
			nodelist.clear();
			for (const auto& file : std::filesystem::directory_iterator("./scripts"))
			{
				if (file.is_directory())
				{
					for (const auto& f : std::filesystem::directory_iterator(file.path()))
					{
						if (f.is_directory())continue;
						std::string name = loadScript(f);
						if (name != "")
						{
							nodelist[file.path().filename().string()].insert(name);
						}
					}
				}
				else
				{
					std::string name = loadScript(file);
					if (name != "")
					{
						nodelist["unlisted"].insert(name);
					}
				}
			}
		}

		////data util////
		std::string TYPES2string(TYPES ty)
		{
			if (types_string.left.find(ty) != types_string.left.end())return types_string.left.at(ty);
			return "undefined";
		}
		TYPES string2TYPES(std::string str)
		{
			if (types_string.right.find(str) != types_string.right.end())return types_string.right.at(str);
			return TN_NULL;
		}
		NodeVariable* fromTypes(const TYPES& ty)
		{
			switch (ty)
			{
			case TN_NULL:return new NodeVariable();
			case TN_BOOL:return new NodeBool();
			case TN_INT:return new NodeInt();
			case TN_FLOAT:return new NodeFloat();
			case TN_TONE:return new NodeTone();
			case TN_CHORD:return new NodeChord();
			case TN_MELODY:return new NodeMelody();
			case TN_VEC_BOOL:return new NodeVectorBool();
			case TN_VEC_INT:return new NodeVectorInt();
			case TN_VEC_FLOAT:return new NodeVectorFloat();
			case TN_VEC_TONE:return new NodeVectorTone();
			case TN_VEC_CHORD:return new NodeVectorChord();
			case TN_VEC_MELODY:return new NodeVectorMelody();
			default:
				break;
			}
			return new NodeVariable();
		}
		NodeVariable* fromTypes(const TYPES& ty, lua_State* L, size_t t)
		{
			switch (ty)
			{
			case TN_NULL:return new NodeVariable(L, t);
			case TN_BOOL:return new NodeBool(L, t);
			case TN_INT:return new NodeInt(L, t);
			case TN_FLOAT:return new NodeFloat(L, t);
			case TN_TONE:return new NodeTone(L, t);
			case TN_CHORD:return new NodeChord(L, t);
			case TN_MELODY:return new NodeMelody(L, t);
			case TN_VEC_BOOL:return new NodeVectorBool(L, t);
			case TN_VEC_INT:return new NodeVectorInt(L, t);
			case TN_VEC_FLOAT:return new NodeVectorFloat(L, t);
			case TN_VEC_TONE:return new NodeVectorTone(L, t);
			case TN_VEC_CHORD:return new NodeVectorChord(L, t);
			case TN_VEC_MELODY:return new NodeVectorMelody(L, t);
			default:
				break;
			}
			return new NodeVariable(L, t);
		}
		NodeVariable* fromString(const std::string& str) { return fromTypes(string2TYPES(str)); }
	}
}
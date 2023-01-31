#pragma once
#include "base.h"

namespace ncp
{
	namespace lib
	{
		//types
		enum TYPES
		{
			TN_NULL,
			TN_BOOL,
			TN_INT,
			TN_FLOAT,
			TN_TONE,
			TN_CHORD,
			TN_MELODY,
			TN_VEC_BOOL,
			TN_VEC_INT,
			TN_VEC_FLOAT,
			TN_VEC_TONE,
			TN_VEC_CHORD,
			TN_VEC_MELODY,
		};

		//info node
		typedef struct NodeData
		{
			std::string path;
			std::vector<std::pair<std::vector<std::set<TYPES>>, std::vector<TYPES>>> type_sets;
		};

		//data lists
		extern boost::bimaps::bimap<TYPES, std::string> types_string;
		extern std::map<std::string, std::set<std::string>> nodelist;
		extern std::map<std::string, NodeData> nodelibs;
		extern std::map<TYPES, Qt::GlobalColor> types_color;

		////////make data////////
		void loadAllData();//load all data(initializer)
		void setTypesString();//load types name
		void setTypesColor();//load color
		std::string loadScript(std::filesystem::directory_entry);//load script data
		void loadScripts();//load scripts list

		////////data util////////
		////convert types string////
		std::string TYPES2string(TYPES);
		TYPES string2TYPES(std::string);
		////make variable from types////
		NodeVariable* fromTypes(const TYPES&);
		NodeVariable* fromTypes(const TYPES&, lua_State*, size_t);
		NodeVariable* fromString(const std::string&);
	}
}

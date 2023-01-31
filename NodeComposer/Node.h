#pragma once
#include "base.h"

namespace ncp
{
	class Node;
	using SingleConnector = std::pair<Node*, size_t>;
	using MutualConnector = std::tuple<Node*, size_t, bool>;

	class Node
	{
	private:
		std::string name;//node name
		size_t types_num = 0;//node argument-return type number
		QRect rect;//body
		Sound* sound = nullptr;//sound
		std::vector<SingleConnector> argument_connection;//arguments connection
		std::vector<NodeVariable*> values_argument;//arguments
		std::vector<NodeVariable*> values_return;//returns

		////////initialization////////
	public:
		Node();
		Node(const QPoint&, const std::string&);
	private:
		void initialize();//initialize by name

		////////get/reference data////////
	public:
		const std::string& getNodeName()const;
		const size_t getTypesNum()const;
		const QRect& getNodeRect()const;
		const QRect& getNodeConnectionRect(const bool&, const size_t&)const;
		const SingleConnector& getArgumentConnection(const size_t&)const;
		NodeVariable* refArgument(const size_t&)const;
		NodeVariable* refResult(const size_t&)const;

		////////utility////////
	public:
		void update();//update data
		void playSound();//play sound
		std::set<Node*> getParentNodes();//get parent nodes
		void saveMidi();//save
	private:
		void doScript();
		void updateSound();

		////////change////////
	public:
		void setTypesNum(size_t);
		void setArgumentConnection(const size_t&, const SingleConnector& connect = SingleConnector{});//set argument connection
		void moveNodeRect(QPoint);//move node to pos

		////////draw////////
	public:
		void draw_body(QPaintDevice*);
		void draw_lines(QPaintDevice*);
	};
}

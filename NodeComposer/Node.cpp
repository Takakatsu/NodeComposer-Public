#include "base.h"

namespace ncp
{
	Node::Node() {}
	Node::Node(const QPoint& pos, const std::string& n)
	{
		this->name = n;
		initialize();
		size_t cnt = 0;
		for (int i = 0; i < lib::nodelibs[name].type_sets.size(); i++)
		{
			cnt = qMax(cnt, lib::nodelibs[name].type_sets[i].first.size());
			cnt = qMax(cnt, lib::nodelibs[name].type_sets[i].second.size());
		}
		this->rect = QRect(pos, QSize(80, (cnt * 2 + 1) * 10));
	}
	void Node::initialize()
	{
		values_argument.clear();
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].first.size(); i++)
		{
			values_argument.push_back(lib::fromTypes(*lib::nodelibs[name].type_sets[types_num].first[i].begin()));
			argument_connection.push_back(SingleConnector{});
		}
		values_return.clear();
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].second.size(); i++)
		{
			values_return.push_back(lib::fromTypes(lib::nodelibs[name].type_sets[types_num].second[i]));
		}
		update();
	}

	const std::string& Node::getNodeName()const { return name; }
	const size_t Node::getTypesNum()const { return types_num; }
	const QRect& Node::getNodeRect()const { return rect; }
	const QRect& Node::getNodeConnectionRect(const bool& is_arg, const size_t& n)const
	{
		QPoint dot_size = QPoint(10, 10);
		if (is_arg)return QRect(rect.topLeft() + dot_size + QPoint(0, dot_size.y() * n * 2), exf::QPoint2QSize(dot_size));
		return QRect(rect.topRight() + QPoint(-dot_size.x() * 2, dot_size.y() * (n * 2 + 1)), exf::QPoint2QSize(dot_size));
	}
	const SingleConnector& Node::getArgumentConnection(const size_t& t)const { return argument_connection[t]; }
	NodeVariable* Node::refArgument(const size_t& t)const { return values_argument[t]; }
	NodeVariable* Node::refResult(const size_t& t)const { return values_return[t]; }

	void Node::update()
	{
		//reflect argument
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].first.size(); i++)
		{
			const SingleConnector& connect = getArgumentConnection(i);
			if (connect.first != nullptr)
			{
				connect.first->update();
				values_argument[i] = connect.first->refResult(connect.second);
			}
		}
		doScript();
		updateSound();
	}
	void Node::playSound() { sound->play(); }
	std::set<Node*> Node::getParentNodes()
	{
		std::set<Node*> parentnodes;
		for (int i = 0; i < argument_connection.size(); i++)
		{
			if (argument_connection[i].first == nullptr)continue;
			if (parentnodes.find(argument_connection[i].first) == parentnodes.end())
			{
				parentnodes.insert(argument_connection[i].first);
				std::set<Node*> t_parentnodes = argument_connection[i].first->getParentNodes();
				parentnodes.insert(t_parentnodes.begin(), t_parentnodes.end());
			}
		}
		return parentnodes;
	}
	void Node::saveMidi() { sound->makeMidiFile().write("./output/saved.mid"); }

	void Node::doScript()
	{
		//setup & error check
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		setLuaFunction(L);
		if (luaL_loadfile(L, lib::nodelibs[name].path.c_str()) || lua_pcall(L, 0, 0, 0))return;
		//set stack
		lua_getglobal(L, "Main");
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].first.size(); i++)
		{
			values_argument[i]->push2Lua(L);
		}
		//run
		if (lua_pcall(L, values_argument.size(), values_return.size(), 0) != 0)return;
		//set return
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].second.size(); i++)
		{
			values_return[i] = lib::fromTypes(lib::nodelibs[name].type_sets[types_num].second[i], L, i + 1);
		}
		//close
		lua_close(L);
	}
	void Node::updateSound()
	{
		sound = new Tone();
		if (lib::nodelibs[name].type_sets[types_num].second.size() > 0)sound = values_return[0]->getSound();
	}

	void Node::setTypesNum(size_t t)
	{
		types_num = t;
		//reset irregular connection
		for (int i = 0; i < argument_connection.size(); i++)
		{
			if (argument_connection[i].first == nullptr)continue;
			std::set<ncp::lib::TYPES> argument_types = ncp::lib::nodelibs[getNodeName()].type_sets[getTypesNum()].first[i];
			ncp::lib::TYPES return_type = ncp::lib::nodelibs[argument_connection[i].first->getNodeName()].type_sets[argument_connection[i].first->getTypesNum()].second[argument_connection[i].second];
			if (argument_types.find(return_type) == argument_types.end())setArgumentConnection(i);
		}
	}
	void Node::setArgumentConnection(const size_t& t, const SingleConnector& connect) { argument_connection[t] = connect; }
	void Node::moveNodeRect(QPoint pq) { rect.moveTopLeft(pq); }

	void Node::draw_body(QPaintDevice* qpd, bool is_active)
	{
		QPainter painter(qpd);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setPen(QPen(Qt::black, 1));
		painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
		painter.drawRect(rect);
		//draw name
		painter.setPen(QPen(Qt::black, 1));
		painter.drawText(rect.bottomLeft(), QString::fromStdString(name));
		//draw values rect
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].first.size(); i++)
		{
			painter.setBrush(QBrush(lib::types_color[*(lib::nodelibs[name].type_sets[types_num].first[i].begin())], Qt::SolidPattern));
			if (!is_active)painter.setBrush(QBrush(lib::types_color[ncp::lib::TYPES::TN_NULL], Qt::SolidPattern));
			painter.drawRect(getNodeConnectionRect(true, i));
		}
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].second.size(); i++)
		{
			painter.setBrush(QBrush(lib::types_color[lib::nodelibs[name].type_sets[types_num].second[i]], Qt::SolidPattern));
			if (!is_active)painter.setBrush(QBrush(lib::types_color[ncp::lib::TYPES::TN_NULL], Qt::SolidPattern));
			painter.drawRect(getNodeConnectionRect(false, i));
		}
		//draw values
		if (!is_active)return;
		painter.setPen(QPen(Qt::black, 1));
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].first.size(); i++)
		{
			painter.drawText(getNodeConnectionRect(true, i).bottomRight() + QPoint(3, -5), QString::fromStdString(values_argument[i]->toString()));
		}
		for (int i = 0; i < lib::nodelibs[name].type_sets[types_num].second.size(); i++)
		{
			painter.drawText(getNodeConnectionRect(false, i).bottomRight() + QPoint(3, 5), QString::fromStdString(values_return[i]->toString()));
		}
	}
	void Node::draw_lines(QPaintDevice* qpd)
	{
		for (int i = 0; i < lib::nodelibs[getNodeName()].type_sets[types_num].first.size(); i++)
		{
			const SingleConnector& connect = getArgumentConnection(i);
			if (connect.first != nullptr)
			{
				QRect qr_argment = getNodeConnectionRect(true, i);
				QRect qr_return = connect.first->getNodeConnectionRect(false, connect.second);
				QPainter painter(qpd);
				painter.setRenderHint(QPainter::Antialiasing, true);
				painter.setPen(QPen(Qt::black, 2));
				painter.drawLine(qr_return.center(), qr_argment.center());
			}
		}
	}
}

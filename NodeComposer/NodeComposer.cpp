#include "NodeComposer.h"

NodeComposer::NodeComposer(QWidget* parent)
	: QMainWindow(parent)
{
	//set up
	ui.setupUi(this);
	setAcceptDrops(true);

	tpqbox = new QSpinBox(this);
	tpqbox->setMinimum(1);
	tpqbox->setMaximum(1000);
	tpqbox->setPrefix(QString("TPQ : "));
	tpqbox->setValue(ncp::tpq);
	connect(tpqbox, &QSpinBox::valueChanged, [&] {ncp::tpq = tpqbox->value(); });
	tempobox = new QSpinBox(this);
	tempobox->setMinimum(1);
	tempobox->setMaximum(1000);
	tempobox->setPrefix(QString("Tempo : "));
	tempobox->setValue(ncp::tempo);
	connect(tempobox, &QSpinBox::valueChanged, [&] {ncp::tempo = tempobox->value(); });
	SetGUIPosition();

	//print all scripts
	for (auto it = ncp::lib::nodelist.begin(); it != ncp::lib::nodelist.end(); ++it)
	{
		std::cout << it->first << std::endl;
		for (auto itr = it->second.begin(); itr != it->second.end(); ++itr)
		{
			std::cout << "  " << *itr << std::endl;
		}
	}
}

NodeComposer::~NodeComposer()
{
	//remove all threads
	for (auto it = ncp::threads.begin(); it != ncp::threads.end();)
	{
		it->join();
	}
}

void NodeComposer::keyPressEvent(QKeyEvent* eve)
{
	//save command
	if (eve->key() == Qt::Key::Key_S)
	{
		if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
		{
			std::cout << "save" << std::endl;
			MakeSave();
		}
	}
}

void NodeComposer::paintEvent(QPaintEvent*)
{
	std::cout << "draw update" << std::endl;

	//draw body
	for (int i = 0; i < nodes.size(); i++)nodes[i]->draw_body(this);
	//draw lines
	for (int i = 0; i < nodes.size(); i++)nodes[i]->draw_lines(this);

	SetGUIPosition();
}

void NodeComposer::clickNodeConnection(ncp::MutualConnector mc)
{
	//first touch
	if (std::get<ncp::Node*>(selected_connection) == nullptr)
	{
		setSelectedConnection(mc);
		return;
	}

	//avoid same node and arg&arg
	if (std::get<ncp::Node*>(selected_connection) == std::get<ncp::Node*>(mc) || std::get<bool>(selected_connection) == std::get<bool>(mc))
	{
		setSelectedConnection(mc);
		return;
	}

	ncp::SingleConnector connector_argument;
	ncp::SingleConnector connector_return;
	if (std::get<bool>(mc))
	{
		connector_argument = ncp::SingleConnector(std::get<ncp::Node*>(mc), std::get<size_t>(mc));
		connector_return = ncp::SingleConnector(std::get<ncp::Node*>(selected_connection), std::get<size_t>(selected_connection));
	}
	else
	{
		connector_argument = ncp::SingleConnector(std::get<ncp::Node*>(selected_connection), std::get<size_t>(selected_connection));
		connector_return = ncp::SingleConnector(std::get<ncp::Node*>(mc), std::get<size_t>(mc));
	}

	//avoid parent node
	std::set<ncp::Node*> p_nodes = connector_return.first->getParentNodes();
	if (p_nodes.find(connector_argument.first) != p_nodes.end())
	{
		setSelectedConnection(mc);
		return;
	}

	//avoid forbiddon types connection
	bool found = false;
	std::set<ncp::lib::TYPES> argument_types;
	ncp::lib::TYPES return_type;
	int i, j;
	for (i = 0; i < ncp::lib::nodelibs[connector_argument.first->getNodeName()].type_sets.size(); i++)
	{
		argument_types = ncp::lib::nodelibs[connector_argument.first->getNodeName()].type_sets[i].first[connector_argument.second];
		for (j = 0; j < ncp::lib::nodelibs[connector_return.first->getNodeName()].type_sets.size(); j++)
		{
			return_type = ncp::lib::nodelibs[connector_return.first->getNodeName()].type_sets[j].second[connector_return.second];
			if (argument_types.find(return_type) != argument_types.end())
			{
				found = true;
				break;
			}
		}
		if (found)break;
	}
	if (!found)
	{
		setSelectedConnection(mc);
		return;
	}

	//type change
	if (connector_argument.first->getTypesNum() != i)connector_argument.first->setTypesNum(i);
	if (connector_return.first->getTypesNum() != j)connector_return.first->setTypesNum(j);

	//connect
	connector_argument.first->setArgumentConnection(connector_argument.second, connector_return);
	connector_argument.first->update();
	setSelectedConnection(ncp::MutualConnector(nullptr, 0, false));
}

void NodeComposer::setSelectedConnection(ncp::MutualConnector mc)
{
	selected_connection = mc;
	update();
}

void NodeComposer::MakeSave()
{
	nlohmann::json maindata = nlohmann::json::array();
	//numbering
	std::map<ncp::Node*, uint32_t> number_list;
	{
		uint32_t i = 0;
		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			number_list.insert(std::make_pair<ncp::Node*, uint32_t>((ncp::Node*)*it, (uint32_t)i));
			i++;
		}
	}
	//make save data
	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		nlohmann::json nd;
		nd["number"] = number_list[*it];
		nd["name"] = (*it)->getNodeName();
		nd["pos_x"] = (*it)->getNodeRect().topLeft().x();
		nd["pos_y"] = (*it)->getNodeRect().topLeft().y();
		nd["type"] = (*it)->getTypesNum();
		nd["connection"] = nlohmann::json::array();
		for (int i = 0; i < ncp::lib::nodelibs[(*it)->getNodeName()].type_sets[(*it)->getTypesNum()].first.size(); i++)
		{
			nlohmann::json cn;
			cn["number"] = i;
			ncp::SingleConnector connect = (*it)->getArgumentConnection(i);
			if (connect.first != nullptr)
			{
				cn["origin_id"] = number_list[connect.first];
				cn["origin_num"] = connect.second;
			}
			else
			{
				cn["data"] = nlohmann::json::array();
				cn["data"] = (*it)->refArgument(i)->makeJSON();
			}
			nd["connection"].push_back(cn);
		}
		maindata.push_back(nd);
	}

	nlohmann::json savedata;
	savedata["version"] = ncp::NCP_VERSION;
	savedata["tpq"] = ncp::tpq;
	savedata["tempo"] = ncp::tempo;
	savedata["maindata"] = maindata;

	std::ofstream savefile;
	std::string filename = "./project.ncp";
	savefile.open(filename, std::ios::out);
	savefile << savedata << std::endl;
	savefile.close();
}
void NodeComposer::LoadSave(nlohmann::json data)
{
	if (data["version"] == "0.0.0")
	{
		ncp::tpq = data["tpq"];
		ncp::tempo = data["tempo"];
		tpqbox->setValue(ncp::tpq);
		tempobox->setValue(ncp::tempo);

		nlohmann::json savedata = data["maindata"];
		std::map<uint32_t, ncp::Node*> number_list;
		for (int j = 0; j < savedata.size(); j++)
		{
			if (ncp::lib::nodelibs.find(savedata[j]["name"]) == ncp::lib::nodelibs.end())continue;
			ncp::Node* nn = new ncp::Node(QPoint(savedata[j]["pos_x"], savedata[j]["pos_y"]), savedata[j]["name"]);
			nn->setTypesNum(savedata[j]["type"]);
			nodes.push_back(nn);
			number_list.insert(std::make_pair<uint32_t, ncp::Node*>((uint32_t)savedata[j]["number"], (ncp::Node*)nn));
		}
		for (int j = 0; j < savedata.size(); j++)
		{
			if (number_list.find(savedata[j]["number"]) == number_list.end())continue;
			for (int k = 0; k < savedata[j]["connection"].size(); k++)
			{
				if (savedata[j]["connection"][k].contains("origin_id"))
				{
					if (number_list.find(savedata[j]["connection"][k]["origin_id"]) == number_list.end())continue;
					number_list[savedata[j]["number"]]->setArgumentConnection(savedata[j]["connection"][k]["number"], std::make_pair<ncp::Node*, size_t>((ncp::Node*)number_list[savedata[j]["connection"][k]["origin_id"]], (size_t)savedata[j]["connection"][k]["origin_num"]));
				}
				else
				{
					number_list[savedata[j]["number"]]->refArgument(savedata[j]["connection"][k]["number"])->loadJSON(savedata[j]["connection"][k]["data"]);
				}
			}
		}
		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			(*it)->update();
		}
	}
	update();
}

void NodeComposer::dragEnterEvent(QDragEnterEvent* eve)
{
	if (eve->mimeData()->hasFormat("text/uri-list"))
	{
		eve->acceptProposedAction();
	}
}
void NodeComposer::dropEvent(QDropEvent* eve)
{
	const QMimeData* m = eve->mimeData();
	if (m->hasUrls())
	{
		nodes.clear();
		QStringList pathList;
		QList<QUrl> urlList = m->urls();
		for (int i = 0; i < urlList.size() && i < 32; i++)
		{
			pathList.append(urlList.at(i).toLocalFile());
		}
		for (int i = 0; i < pathList.size(); i++)
		{
			const std::string path = pathList.at(i).toStdString();
			std::fstream read(path.c_str());
			nlohmann::json savedata;
			read >> savedata;
			LoadSave(savedata);
			break;
		}
	}
}

//R click
void NodeComposer::contextMenuEvent(QContextMenuEvent* event)
{
	QPoint pos = QCursor::pos();
	//node connection click
	{
		ncp::MutualConnector t = getNodeConnectionByPos(this->mapFromGlobal(pos));
		if (std::get<ncp::Node*>(t) != nullptr && std::get<bool>(t))
		{
			QDialog* dialog = new QDialog(this);
			QFormLayout layout = QFormLayout(dialog);
			layout.addRow(new QLabel("Set Argment"));
			QList<QLineEdit*> fields;
			std::get<ncp::Node*>(t)->refArgument(std::get<size_t>(t))->makeForm(dialog, &layout, &fields);
			bool is_assigned = false;//is clicked
			//setting
			QDialogButtonBox btn = QDialogButtonBox();
			QPushButton* qp_apply = btn.addButton(QDialogButtonBox::Apply);
			connect(qp_apply, &QPushButton::clicked, [&]() {
				std::cout << "apply clicked" << std::endl;
			is_assigned = true;
			dialog->reject();
				});
			QPushButton* qp_cancel = btn.addButton(QDialogButtonBox::Cancel);
			connect(qp_cancel, &QPushButton::clicked, [&]() {
				std::cout << "cancel clicked" << std::endl;
			dialog->reject();
				});
			layout.addWidget(&btn);
			dialog->exec();
			if (is_assigned)
			{
				std::get<ncp::Node*>(t)->setArgumentConnection(std::get<size_t>(t));
				std::get<ncp::Node*>(t)->refArgument(std::get<size_t>(t))->readForm(&fields);
				std::get<ncp::Node*>(t)->update();
			}
			return;
		}
	}

	//node click(midi)
	{
		for (auto it = nodes.begin(); it != nodes.end(); )
		{
			if ((*it)->getNodeRect().contains(this->mapFromGlobal(pos)))
			{
				QMenu* m = new QMenu();
				m->addAction("Save Midi", this, [&] {
					(*it)->saveMidi();
				it++;
					});
				m->addAction("Remove Node", this, [&] {
					it = nodes.erase(it);
					});
				m->addAction("Close", this, [&] {
					std::cout << "close" << std::endl;
				it++;
					});
				m->exec(pos);
				update();
				return;
			}
			it++;
		}
	}

	//void click
	{
		QMenu* m = new QMenu();
		QMenu* addnode = m->addMenu("Add Node");
		//add nodelist
		for (auto it = ncp::lib::nodelist.begin(); it != ncp::lib::nodelist.end(); ++it)
		{
			QMenu* folder = addnode->addMenu(QString::fromStdString(it->first));
			for (auto itr = it->second.begin(); itr != it->second.end(); ++itr)
			{
				std::string str = *itr;
				folder->addAction(QString::fromStdString(str), this, [=] {
					QPoint tp = this->mapFromParent(pos);
				nodes.push_back(new ncp::Node(tp, str));
				update();
					});
			}
		}
		m->addAction("Close", this, []
			{
				std::cout << "close" << std::endl;
			});
		m->exec(pos);
		return;
	}
}

//L click
void NodeComposer::mousePressEvent(QMouseEvent* eve)
{
	movenode.first = nullptr;
	if (eve->button() == Qt::LeftButton)
	{
		QPoint pos = this->mapFromGlobal(QCursor::pos());
		//node connection
		{
			ncp::MutualConnector t = getNodeConnectionByPos(pos);
			if (std::get<ncp::Node*>(t) != nullptr)
			{
				clickNodeConnection(t);
				return;
			}
		}
		//node body
		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			if ((*it)->getNodeRect().contains(pos))
			{
				(*it)->update();
				(*it)->playSound();
				movenode = std::make_pair((*it), pos - (*it)->getNodeRect().topLeft());
				return;
			}
		}
	}
}

//drag
void NodeComposer::mouseMoveEvent(QMouseEvent* eve)
{
	if (movenode.first != nullptr)
	{
		movenode.first->moveNodeRect(eve->pos() - movenode.second);
		update();
	}
}

//connections hitbox check
ncp::MutualConnector NodeComposer::getNodeConnectionByPos(QPoint pos)
{
	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		for (int i = 0; i < ncp::lib::nodelibs[(*it)->getNodeName()].type_sets[(*it)->getTypesNum()].first.size(); i++)
		{
			if ((*it)->getNodeConnectionRect(true, i).contains(pos))
			{
				return ncp::MutualConnector((*it), i, true);
			}
		}
		for (int i = 0; i < ncp::lib::nodelibs[(*it)->getNodeName()].type_sets[(*it)->getTypesNum()].second.size(); i++)
		{
			if ((*it)->getNodeConnectionRect(false, i).contains(pos))
			{
				return ncp::MutualConnector((*it), i, false);
			}
		}
	}
	return ncp::MutualConnector(nullptr, 0, false);
}

void NodeComposer::SetGUIPosition()
{
	tpqbox->move(this->width() - tpqbox->width(), this->height() - tpqbox->height());
	tempobox->move(this->width() - tempobox->width(), this->height() - tpqbox->height() - tempobox->height());
}
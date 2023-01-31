#pragma once
#include "base.h"
#include "ui_NodeComposer.h"

class NodeComposer : public QMainWindow
{
	Q_OBJECT

public:
	NodeComposer(QWidget* parent = nullptr);
	~NodeComposer();

protected:
	void MakeSave();
	void LoadSave(nlohmann::json);
	void dragEnterEvent(QDragEnterEvent*);
	void dropEvent(QDropEvent*);
	void keyPressEvent(QKeyEvent*);
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void contextMenuEvent(QContextMenuEvent*);

private:
	Ui::NodeComposerClass ui;

	QSpinBox* tpqbox, * tempobox;

	std::vector<ncp::Node*> nodes;

	std::pair<ncp::Node*, QPoint> movenode;

	//node selector
	ncp::MutualConnector selected_connection;
	void clickNodeConnection(ncp::MutualConnector);
	void setSelectedConnection(ncp::MutualConnector);
	ncp::MutualConnector getNodeConnectionByPos(QPoint);

	//gui
	void SetGUIPosition();
};

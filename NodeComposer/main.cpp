#include "base.h"

int main(int argc, char* argv[])
{
	ncp::lib::loadAllData();

	ncp::tempo = 120;
	ncp::tpq = 480;
	ncp::midiout = new RtMidiOut();
	unsigned int nPorts = ncp::midiout->getPortCount();
	if (nPorts == 0)std::cout << "error" << std::endl;
	int selected_port = 0;
	for (int i = 0; i < nPorts; i++)
	{
		std::cout << ncp::midiout->getPortName(i) << std::endl;
	}
	ncp::midiout->openPort(selected_port);

	QApplication a(argc, argv);
	NodeComposer w;
	w.show();

	return a.exec();
}

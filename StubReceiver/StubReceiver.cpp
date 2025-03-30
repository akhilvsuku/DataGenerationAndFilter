#include "StubReceiver.h"

#define FILTER { 0.00025177 , 0.008666992, 0.078025818, 0.24130249, 0.343757629, \
								0.24130249, 0.078025818, 0.008666992, 0.000125885 }

void StubReceiver::Init() {

	m_pReader->load("config.ini"); 

	m_ReceiverEnable = m_pReader->getint("General", "TCPReceiverEnabled", 1);

	if (m_ReceiverEnable) {

		m_pServerReceiver = new TCPConMan;
		m_pServerReceiver->Init(m_pReader, "TCPReceiver");
		m_pServerReceiver->setQ(&m_oSharedVec);
		// Starting to process the Q data
	}

	Controller::Init();
}

void StubReceiver::ProcessRequest(LineData* pData)
{
	pData->print();
}


#include "FilterAndThreshold.h"

#define FILTER { 0.00025177 , 0.008666992, 0.078025818, 0.24130249, 0.343757629, \
								0.24130249, 0.078025818, 0.008666992, 0.000125885 }

void FilterAndThreshold::Init() {

	m_pReader->load("config.ini");
	m_dThreasholdVal = std::stod(m_pReader->getstring("General", "TV", "1"));

	m_SenderEnable = m_pReader->getint("General", "TCPSenderEnabled", 1);

	if (m_SenderEnable) {

		m_pServerSender = new TCPConMan;
		m_pServerSender->Init(m_pReader, "TCPSender");
	}

	m_ReceiverEnable = m_pReader->getint("General", "TCPReceiverEnabled", 1);
	if (m_ReceiverEnable) {

		m_pServerReceiver = new TCPConMan;
		m_pServerReceiver->Init(m_pReader, "TCPReceiver");
		m_pServerReceiver->setQ(&m_oSharedVec);
		// Starting to process the Q data
	}

	Controller::Init();
}

void FilterAndThreshold::ProcessRequest(LineData* pData)
{
	if (pData->flag == 0xFE)
		m_Exit = 1;
	else {
			
		int ncount = 0;
		while (ncount < pData->sz) {
			m_vec_uData.push_back(pData->data[ncount++]);
		}

		while (m_vec_uData.size() >= 9)
		{
			double nsum = 0;
			uint8_t nout = 0;
			const double m_dFilter[] = FILTER;
			for (size_t i = 0; i < 9; i++)
			{ 
				nsum += (m_vec_uData[i] * m_dFilter[i]);
			}

			if (m_Enable_Log)
				std::cout << "FilterAndThreshold::ProcessRequest Key Sum :" << nsum <<"\n";

			nout = (m_dThreasholdVal >= nsum) ? 1 : 0;


			LineData oLineData(sizeof(nout), &nout);
			*pData = oLineData;

			if (m_SenderEnable)
				SendData(pData);

			m_vec_uData.erase(m_vec_uData.begin()); // shifting the window to next 9 elements
		}

	}
}


#pragma once
#include "TCPConMan.h"
#include "IniReader.h"

using namespace std::chrono_literals; 

class Controller
{
protected:
	DataProvider* m_pServerSender; // Data sender 
	DataProvider*m_pServerReceiver; // Data receiver
	SharedVector<LineData> m_oSharedVec; // shared Q
	int m_process_delay_in_ns; // delay for processing in nano seconds
	std::atomic<int> m_Exit; //exit flag for exiting the application
	std::atomic<bool> m_Enable_Log; // log enable flag
	std::atomic<bool> m_ReceiverEnable; // Receiver flag
	std::atomic<bool> m_SenderEnable; // Receiver flag

	IniReader* m_pReader;
public: 
	//Default constructor
	Controller() : m_pServerSender(0), m_pServerReceiver(0), m_process_delay_in_ns(100),
		m_Enable_Log(1) {
		m_pReader = new IniReader;
	}

	// For initialization
	virtual void Init() = 0 {
    

		m_Enable_Log = m_pReader->getint("General", "EnableLog", 1); 

		m_process_delay_in_ns = m_pReader->getint("General", "ProcessDelayInNS", 500);
		if (m_process_delay_in_ns < 500)
			m_process_delay_in_ns = 500; // minimum delay should be 500ns

		std::thread ReceiverThread([&]() {
			if(m_pServerReceiver)
				m_pServerReceiver->start();
			});

		ProcessData();
	};

	//Function for procesing the Q data
	virtual void ProcessData()
	{

		do{
			LineData oData;
			if (m_oSharedVec.size() > 0) {
				// module is processing data from Q
				if (m_Enable_Log)
					std::cout << "Controller::ProcessData New Data in Q.\n";

				oData = std::move(m_oSharedVec.pop_front());
				ProcessRequest(&oData);

			}

			if (oData.flag == 0x01 && m_SenderEnable)
				SendData(&oData);


			std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
		}while(!m_Exit);
		if (m_Enable_Log)
			std::cout << "Controller::ProcessData Exiting.\n";
	}

	//Function to inherit the derived class for module specific processing on data;
	virtual void ProcessRequest(LineData*) = 0;

	//Sending data to next module 
	virtual void SendData(LineData* pData)
	{
		std::cout << "Controller::SendData Data sending to next module.\n";
		if (m_pServerSender->m_bConnected)
			m_pServerSender->sendMessage(pData->data, pData->sz);
		else
			std::cout << "Controller::SendData Not connected to any socket.\n";
	}


	//destructor
	virtual ~Controller() {
		delete m_pReader;
		if(m_pServerReceiver)
			delete m_pServerReceiver;
		if(m_pServerSender)
			delete m_pServerSender;
	}
};


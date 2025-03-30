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
		m_Enable_Log(1)/*, psharedLogger(nullptr)*/{
		m_pReader = new IniReader;
	}

	//void SetLogger(std::shared_ptr<Logger> pLogger) {
	//	psharedLogger = pLogger;
	//}

	// For initialization
	virtual void Init() = 0 {

		m_process_delay_in_ns = m_pReader->getint("General", "ProcessDelayInNS", 500);
		if (m_process_delay_in_ns < 500)
			m_process_delay_in_ns = 500; // minimum delay should be 500ns

		std::thread ReceiverThread([&]() {
			if(m_pServerReceiver)
				m_pServerReceiver->start();
			});

		Logger::getInstance()->log(Logger::Level::INFO, "Initialization completed");

		ProcessData();
	};

	//Function for procesing the Q data
	virtual void ProcessData()
	{
		Logger::getInstance()->log(Logger::Level::INFO, "Starting ProcessData Thread");

		do{
			LineData oData;
			int vecsz = m_oSharedVec.size();
			if (vecsz  > 0) {
				// module is processing data from Q 

				Logger::getInstance()->log(Logger::Level::INFO, "Vector size : " + std::to_string(vecsz) + ".");

				oData = std::move(m_oSharedVec.pop_front());
				ProcessRequest(&oData);

			}

			if (oData.flag == 0x01 && m_SenderEnable) 
				SendData(&oData);


			std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
		}while(!m_Exit);
		Logger::getInstance()->log(Logger::Level::INFO, "Controller::ProcessData Exiting.");
	}

	//Function to inherit the derived class for module specific processing on data;
	virtual void ProcessRequest(LineData*) = 0;

	//Sending data to next module 
	virtual void SendData(LineData* pData)
	{
		Logger::getInstance()->log(Logger::Level::INFO, "Controller::SendData Data sending to next module.");
		if (m_pServerSender->m_bConnected)
			m_pServerSender->sendMessage(pData->data.get(), pData->sz);
		else
			Logger::getInstance()->log(Logger::Level::WARNING, "Controller::SendData No Sender is connected. Yet tried to send data.");
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


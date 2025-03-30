#pragma once
#include "Common.h" 
#include "DataProvider.h" 

class RandomDataGen : public DataProvider
{
	SharedVector<uint8_t> pvecIn;
	int m_process_delay_in_ns; // delay for processing in nano seconds
protected: 


public:
	RandomDataGen() {
		pSharevec = 0;
		m_process_delay_in_ns = 0; 
	}

	virtual ~RandomDataGen() {

	}

	uint8_t RandomNumber();

	void Init(IniReader* m_pReader, std::string strSectionName) {

		m_process_delay_in_ns = m_pReader->getint("General", "ProcessDelayInNS", 500);
		if (m_process_delay_in_ns < 500)
			m_process_delay_in_ns = 500; // minimum delay should be 500ns
	}; 

	void start() {

		Logger::getInstance()->log(Logger::Level::INFO, "Starting RandomNumGen Thread.");

		while (!m_Exit && pSharevec != nullptr) {
			// different random value generator loops

				uint8_t rnum = RandomNumber();

				pSharevec->push_back(std::move(LineData(1, &rnum)));
				std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
		}
		Logger::getInstance()->log(Logger::Level::INFO, "Exiting RandomNumGen.");
	}; 


void sendMessage(const uint8_t* message, int sz) {
	};

	void GenerateRandomNum(int nType);
};


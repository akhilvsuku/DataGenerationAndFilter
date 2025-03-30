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


		while (!m_Exit && pSharevec != nullptr) {
			// m_atnAlternator used to get alternate number from 
			// different random value generator loops
			/*if (pvecIn.size() > 1) {
				uint8_t rnum[2] = { 0 };
				rnum[0] = RandomNumber();
				rnum[1] = pvecIn.pop_front();*/

				uint8_t rnum = RandomNumber();

				pSharevec->push_back(std::move(LineData(1, &rnum)));
				std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
		}
	}; 


void sendMessage(const uint8_t* message, int sz) {
	};

	void GenerateRandomNum(int nType);
};


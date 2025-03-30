#pragma once
#include "DataProvider.h"
class CSVDataFetcher :
    public DataProvider
{
	int m_process_delay_in_ns; // delay for processing in nano seconds
protected:

	std::string m_strFileName;

public:
	CSVDataFetcher() {
		pSharevec = 0;
		m_process_delay_in_ns = 0;
		m_strFileName = "";
	}

	virtual ~CSVDataFetcher() {

	}

	std::vector<std::string> readCSV(const std::string& filename);

public:
	
	void Init(IniReader* m_pReader, std::string strSectionName) {
		std::string strFileName = "";
		m_strFileName = m_pReader->getstring(strSectionName, "InputFileName", "input.csv");

		m_process_delay_in_ns = m_pReader->getint("General", "ProcessDelayInNS", 500);
		if (m_process_delay_in_ns < 500)
			m_process_delay_in_ns = 500; // minimum delay should be 500ns

	};

	void start() {
		GenerateRandomNum();
	};

	void sendMessage(const uint8_t* message, int sz) {

	};

	void GenerateRandomNum();
};


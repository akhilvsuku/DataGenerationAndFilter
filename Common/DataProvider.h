#pragma once
#include "Common.h"
#include "SharedVector.h"
#include "IniReader.h"

class DataProvider
{
protected:
	std::atomic<int> m_Exit;
	SharedVector<LineData>* pSharevec;

public:

	enum DataProviderMode {
		TCP,
		AUTOLOOP,
		CSV
	};

	std::atomic<int> m_bConnected;
	DataProvider() : m_Exit(0), m_bConnected(0), pSharevec(nullptr){

	}

	void setQ(SharedVector<LineData>* ptVec) {
		pSharevec = ptVec;
	}

	void SetExit(bool val) {
		m_Exit = val;
	}

	virtual void Init(IniReader* m_pReader, std::string strSectionName) = 0;
	virtual void start() = 0;

	virtual ~DataProvider() {

	}

	virtual void sendMessage(const uint8_t* message, int sz) = 0;
};


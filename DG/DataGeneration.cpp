#include "DataGeneration.h"
#include "RandomDataGen.h"
#include "CSVDataFetcher.h"

void DataGeneration::Init() {

    m_pReader->load("config.ini");

    m_SenderEnable = m_pReader->getint("General", "TCPSenderEnabled", 1);

    if (m_SenderEnable) {

        m_pServerSender = new TCPConMan;
        m_pServerSender->Init(m_pReader, "TCPSender"); 
    }

    m_bTestMode = m_pReader->getint("General", "TestMode", 1);

    if (m_bTestMode) {

        m_pServerReceiver = new CSVDataFetcher;
        m_pServerReceiver->Init(m_pReader, "CSVDataFetcher");
        m_pServerReceiver->setQ(&m_oSharedVec);

    }
    else {

        m_pServerReceiver = new RandomDataGen;
        m_pServerReceiver->Init(m_pReader, "RandomDataGen");
        m_pServerReceiver->setQ(&m_oSharedVec);
    }

    Controller::Init();
}

void DataGeneration::ProcessRequest(LineData* pData)
{
    if (m_Enable_Log)
        std::cout << "Controller::ProcessData New Data in Q.\n"; 

    if (!m_bTestMode) {

        uint8_t rnum[2] = { 0 };
        rnum[0] = pData->data[0];
        rnum[1] = ((RandomDataGen*)m_pServerReceiver)->RandomNumber();
        LineData oLineData(2, &rnum[0]);
        *pData = oLineData;
    }
    pData->flag = 0x01; 
}


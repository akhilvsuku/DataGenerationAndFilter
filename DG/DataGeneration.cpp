#include "DataGeneration.h"
#include "RandomDataGen.h"
#include "CSVDataFetcher.h"

void DataGeneration::Init() {

    m_pReader->load("config.ini");

    Logger::getInstance()->InIt(m_pReader);
    Logger::getInstance()->log(Logger::Level::INFO, "Initialization starts");

    m_SenderEnable = m_pReader->getint("General", "TCPSenderEnabled", 1);

    if (m_SenderEnable) {

        m_pServerSender = new TCPConMan;
        m_pServerSender->Init(m_pReader, "TCPSender"); 
    }
    else  
        Logger::getInstance()->log(Logger::Level::INFO, "Sender disabled");

    m_bTestMode = m_pReader->getint("General", "TestMode", 1);

    if (m_bTestMode) {
        Logger::getInstance()->log(Logger::Level::INFO, "Running in Testmode");

        m_pServerReceiver = new CSVDataFetcher;
        m_pServerReceiver->Init(m_pReader, "CSVDataFetcher");
        m_pServerReceiver->setQ(&m_oSharedVec);

    }
    else {
        Logger::getInstance()->log(Logger::Level::INFO, "Running as Random Generate Loop mode");

        m_pServerReceiver = new RandomDataGen;
        m_pServerReceiver->Init(m_pReader, "RandomDataGen");
        m_pServerReceiver->setQ(&m_oSharedVec);
    }

    Controller::Init();
}

void DataGeneration::ProcessRequest(LineData* pData)
{

    if (!m_bTestMode) {

        uint8_t rnum[2] = { 0 };
        rnum[0] = pData->data[0];
        rnum[1] = ((RandomDataGen*)m_pServerReceiver)->RandomNumber();
        LineData oLineData(2, &rnum[0]);
        *pData = oLineData;
    }
    pData->flag = 0x01; 
    pData->print();
}


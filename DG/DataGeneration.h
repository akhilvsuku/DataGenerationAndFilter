#pragma once
#include "Controller.h"
#include "RandomDataGen.h"

class DataGeneration :
    public Controller
{
    std::atomic<bool> m_bTestMode;
public:
    DataGeneration() {
        m_bTestMode = 0;
    }

    void Init();
    void ProcessRequest(LineData*); 

    virtual ~DataGeneration() {

    }
};


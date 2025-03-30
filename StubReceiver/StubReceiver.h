#pragma once
#include "Controller.h"

class StubReceiver :
    public Controller
{
public:
    StubReceiver() {
    }

    void Init();
    void ProcessRequest(LineData*); 

    virtual ~StubReceiver() {

    }
};


#pragma once
#include "Controller.h"

class FilterAndThreshold :
    public Controller
{
    std::vector<uint8_t> m_vec_uData;
    /*const double m_dFilter[9] = {0.00025177 , \
                                    0.008666992,
                                    0.078025818, 
                                    0.24130249,
                                    0.343757629,
                                    0.24130249,
                                    0.078025818,
                                    0.008666992,
                                    0.000125885 } ; */
    uint8_t m_nPos;
    double m_dThreasholdVal;
public:
    FilterAndThreshold() : m_nPos(0), m_dThreasholdVal(0){
    }

    void Init();
    void ProcessRequest(LineData*); 

    virtual ~FilterAndThreshold () {

    }
};


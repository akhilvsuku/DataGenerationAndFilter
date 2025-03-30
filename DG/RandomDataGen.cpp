#include "RandomDataGen.h"
#include <Windows.h>  

uint8_t RandomDataGen::RandomNumber()
{
	int num = rand();
	uint8_t rnum = 0;
	rnum = (num > 0xFF) ? num % 0xFF : num;
	return rnum;
}

void RandomDataGen::GenerateRandomNum(int nType)
{ 
	while (!m_Exit) {
		// m_atnAlternator used to get alternate number from 
		// different random value generator loops 
		if (pvecIn.size() < 10) { 

			pvecIn.push_back(RandomNumber());

		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
	}
}

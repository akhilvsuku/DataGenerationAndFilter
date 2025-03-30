#pragma once 

#include <stdint.h>
#include <memory>
#include <iostream>
#include <memory>

/*
*  Datastructure for Q;
*/

typedef struct _inLine {
    uint8_t flag; // Flag for additional status information
    std::shared_ptr<uint8_t> pData;
	uint8_t* data;  // Pointer to data in Q
	int sz; // Size of the data array

    // Default Constructor (Explicit to prevent implicit conversions)
    explicit _inLine() : sz(0), data(0), flag(0){}

    // Constructor that initializes the array with a given size
	explicit _inLine(int tsz) : sz(tsz), data(new uint8_t[sz]()), flag(0) {}

    // Constructor that initializes with a given size and copies provided data
	_inLine(int tsz, const uint8_t* tdata) : sz(tsz), data(new uint8_t[tsz]), flag(0) {
		memcpy(data, tdata, sz);
	}

    // Copy Constructor (Performs Deep Copy)
    _inLine(const _inLine& other) : sz(other.sz), data(new uint8_t[other.sz]), flag(0) {
		memcpy(data, other.data, sz);
	}

    // Move Constructor (Steals resources from the source)
    _inLine(_inLine&& other) noexcept : sz(0), data(nullptr), flag(0) {
        // Steal data from the source
        sz = other.sz;
        data = other.data;
        flag = other.flag;


        // Nullify the source
        other.sz = 0;
        other.data = nullptr;
        other.flag = 0;
    }

    // Copy Assignment Operator (Deep Copy)
    _inLine& operator=(const _inLine& other) {
        if (this == &other) return *this; // Self-assignment check

        // Free existing memory
        if(data)
            delete[] data;

        // Allocate new memory and copy
        sz = other.sz;
        data = new uint8_t[sz];
        flag = other.flag;
        memcpy(data, other.data, sz);

        return *this;
    }

    // Move Assignment Operator (Steals Resources)
    _inLine& operator=(_inLine&& other) noexcept {
        if (this == &other) return *this; // Self-assignment check

        // Free existing memory
        if (data)
            delete[] data;

        // Steal resources
        sz = other.sz;
        data = other.data;
        flag = other.flag;

        // Nullify source
        other.sz = 0;
        other.data = nullptr;
        other.flag = 0;

        return *this;
    }

    // Converts data to a std::string representation
    std::string to_string() const {
        return std::string(data, data + sz);
    }


    // Print function to display contents of the object
    void print() const {
        std::cout << "Size: " << sz << ", Data: ";
        for (int i = 0; i < sz; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    // Destructor (Frees dynamically allocated memory)
	virtual ~_inLine()
	{
		sz = 0;
		delete [] data;
	}
}LineData;


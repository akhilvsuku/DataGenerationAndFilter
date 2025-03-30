#pragma once 

#include <stdint.h>
#include <memory>
#include <iostream>
#include <memory>

/*
*  Datastructure for Q;
*/

struct LineData {
    uint8_t flag; // Flag for additional status information 
    std::unique_ptr<uint8_t[]> data; // Smart pointer to manage data
    int sz; // Size of the data array

    // Default Constructor
    explicit LineData() : sz(0), data(nullptr), flag(0) {}

    // Constructor that initializes the array with a given size
    explicit LineData(int tsz) : sz(tsz), data(std::make_unique<uint8_t[]>(sz)), flag(0) {}

    // Constructor that initializes with a given size and copies provided data
    LineData(int tsz, const uint8_t* tdata)
        : sz(tsz), data(std::make_unique<uint8_t[]>(tsz)), flag(0) {
        std::memcpy(data.get(), tdata, sz);
    }

    // Copy Constructor (Performs Deep Copy)
    LineData(const LineData& other)
        : sz(other.sz), data(std::make_unique<uint8_t[]>(other.sz)), flag(other.flag) {
        std::memcpy(data.get(), other.data.get(), sz);
    }

    // Move Constructor (Transfers ownership)
    LineData(LineData&& other) noexcept
        : sz(other.sz), data(std::move(other.data)), flag(other.flag) {
        other.sz = 0;
        other.flag = 0;
    }

    // Copy Assignment Operator (Deep Copy)
    LineData& operator=(const LineData& other) {
        if (this == &other) return *this; // Self-assignment check

        // Allocate new memory and copy
        sz = other.sz;
        data = std::make_unique<uint8_t[]>(sz);
        flag = other.flag;
        std::memcpy(data.get(), other.data.get(), sz);

        return *this;
    }

    // Move Assignment Operator (Transfers ownership)
    LineData& operator=(LineData&& other) noexcept {
        if (this == &other) return *this; // Self-assignment check

        // Transfer ownership
        sz = other.sz;
        data = std::move(other.data);
        flag = other.flag;

        // Nullify source
        other.sz = 0;
        other.flag = 0;

        return *this;
    }

    // Converts data to a std::string representation
    std::string to_string() const {
        return std::string(data.get(), data.get() + sz);
    }

    // Print function to display contents of the object
    void print() const {
        std::cout << "Size: " << sz << ", Data: ";
        for (int i = 0; i < sz; ++i) {
            std::cout << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::endl;
    }

    // Destructor is not needed since `std::unique_ptr` manages memory automatically
};


#include "CSVDataFetcher.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void CSVDataFetcher::GenerateRandomNum()
{
    Logger::getInstance()->log(Logger::Level::INFO, "Starting CSVDataFetcher::GenerateRandomNum Thread");
    while (!m_Exit) {
        auto veccsvData = readCSV(m_strFileName);

        for (const auto& row : veccsvData) {
            LineData oData(row.length(), (uint8_t*)row.c_str());

            if (pSharevec)
                pSharevec->push_back(std::move(oData));
            else
                Logger::getInstance()->log(Logger::Level::WARNING, "CSVDataFetcher::GenerateRandomNum No shared Q is provided.");

        }
         
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_process_delay_in_ns));
    }
    Logger::getInstance()->log(Logger::Level::INFO, "Exiting GenerateRandomNum");
}

std::vector<std::string> CSVDataFetcher::readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> data;
    std::string line;

    if (!file.is_open()) {
        Logger::getInstance()->log(Logger::Level::ERRR, "Error opening file: " + filename + ".");
        return {};
    }

    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());
        data.push_back(line);
    }

    file.close();


    // Truncate the file by opening it in output mode with std::ios::trunc
    std::ofstream truncateFile(filename, std::ios::trunc);
    truncateFile.close(); // Just opening with `trunc` mode clears the file

    return data;
}

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <memory>
#include <IniReader.h>

class Logger {
public:
    enum Level {    INFO,
                    WARNING, 
                    ERRR,
                    EXTRAINFO};
     

    // Get the Singleton instance
    static Logger* getInstance(IniReader* pReader = 0) {
        if (m_pInstance == nullptr) 
            m_pInstance = new Logger(pReader);

        return m_pInstance;
    }


    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Log message with level
    void log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string logEntry = formatLogEntry(level, message);

        // Print to console
        if(m_nPrintToConsole)
            std::cout << logEntry << std::endl;

        // Write to file
        logFile << logEntry << std::endl;
        logFile.flush();
    }

    // Convenience methods
    void info(const std::string& message)    { log(Level::INFO, message); }
    void warning(const std::string& message) { log(Level::WARNING, message); }
    void error(const std::string& message)   { log(Level::ERRR, message); }

    void SetPrintToConsole(bool val) { m_nPrintToConsole = val; }

private:
    bool m_nPrintToConsole;
    static Logger* m_pInstance;  // Smart pointer to manage instance
    std::ofstream logFile;
    std::mutex logMutex; 

    // Private constructor for Singleton pattern
    explicit Logger(IniReader* pReader) : m_nPrintToConsole(0){

        std::string filename = "";
        if (pReader) {
            filename = pReader->getstring("General", "LogFile", "logfile.log");

            if (filename[filename.size() - 1] == '//')
                filename += "logfile.log";
        }

        logFile.open(filename, std::ios::app); // Append mode
        if (!logFile) {
            std::cerr << "Logger: Failed to open log file!" << std::endl;
        }
    }

    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Get current timestamp
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        char buffer[20];

         
        struct tm timeInfo;

        // Use localtime_s (safe version of localtime)
        localtime_s(&timeInfo, &time);

        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
        return std::string(buffer);
    }

    // Format log entry
    std::string formatLogEntry(Level level, const std::string& message) {
        std::string levelStr;
        switch (level) {
            case Level::EXTRAINFO:
            case Level::INFO:    levelStr = "[INFO] "; break; 
            case Level::WARNING: levelStr = "[WARNING] "; break;
            case Level::ERRR:   levelStr = "[ERROR] "; break;
        }
        return getCurrentTime() + " " + levelStr + message;
    }
};


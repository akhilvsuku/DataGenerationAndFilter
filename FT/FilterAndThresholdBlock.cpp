// FilterAndThresholdBlock.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FilterAndThreshold.h"
 

int main()
{
    FilterAndThreshold oFilterAndThreshold;
    oFilterAndThreshold.Init();
    /*int nMode = 0;

    std::cout << "\t1 Sender. \n \t2 Receiver\n";
    std::cin >> nMode;

    if (nMode == 2) {
        SharedVector<LineData> oSharedVec;
        TCPConMan server(TCPConMan::Mode::Receiver, PORT, "", &oSharedVec);
        std::thread serverThread([&]() {
            server.start();
            });


        std::thread ProcessThread([&]() {
                while (!server.m_exit) {
                    if (oSharedVec.size() > 0) {
                        LineData oData = oSharedVec.pop_front();
                        oData.print();
                    }
                    Sleep(100);
                }
            });


        serverThread.join();
        ProcessThread.join();

        while (true) {
            std::string str_data = "";
            std::cout << "\tType Exit to exit:\n";
            std::cin >> str_data;

            if (str_data == "Exit" || str_data == "exit")
                break;
        }
    }
    else {

        /* std::thread clientThread([]() {
            TCPConMan sender(TCPConMan::Mode::Sender, "127.0.0.1", PORT);
            std::string msg = "Hello from client!";
            sender.sendMessage((uint8_t*)msg.c_str(), msg.length());
            });

        clientThread.join();* /


        TCPConMan sender(TCPConMan::Mode::Sender, PORT, "127.0.0.1", 0);
        while (true) {
            std::string str_data = "";
            std::cout << "\tEnter data (Type Exit to exit.):\n";
            std::cin >> str_data;

            if (str_data == "Exit" || str_data == "exit")
                break;
             
            sender.sendMessage((uint8_t*)str_data.c_str(), str_data.length());
        }


    }


    */
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

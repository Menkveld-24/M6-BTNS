#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

//communicator class to clean up the main file a bit
#include "Master.h"
#include "Slave.h"
#include <stdint.h>

class Communicator{
    public:
        Communicator();
        void Loop();
        static void OnDataReceive(uint8_t * mac, uint8_t *incomingData, uint8_t len);
        static void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
        bool amISlave();
        inline static bool isSlave = false;

    private:
        static Communicator* _communicator;
        Master *master;
        Slave *slave;
};

#endif
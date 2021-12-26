class Master{
  public:
    Master();
    void Loop();
    void OnDataReceive(uint8_t * mac, uint8_t *incomingData, uint8_t len);
    void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
  
  private:
    //transmission data type
    typedef struct message_structure { 
      int peerId; // -1 = unset
      bool lookingForMaster;
      bool isMaster;
      String message;
    } message_structure;
    message_structure Received_data;

    void registerSlave(uint8_t *mac);
    void SendData(uint8_t *targetMac ,message_structure message);
    void broadCastToAllSlaves(message_structure message);

    String convertMACtoStr(uint8_t *mac);

    unsigned long previousMillis = 0;

    //used for memcmp
    uint8_t placeholderMAC[6] = {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t slaveMACS[10][6] = { //index of slavemac is the id of the slace 
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, //placeholder to reserve memory space for memcpy
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
      {0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    }; 
};

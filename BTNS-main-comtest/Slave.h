class Slave{
  public:
    Slave();
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

    void registerMaster(uint8_t *mac);
    void SendData(uint8_t *targetMac, message_structure message);

    String convertMACtoStr(uint8_t *mac);

    unsigned long previousMillis = 0;
    int slaveId = -1;
    
    uint8_t masterMAC[6]; //{0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

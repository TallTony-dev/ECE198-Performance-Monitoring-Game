void AddDataToBuf(uint8_t* data, int length); //length in bytes, delimits data with comma
void TransmitData(); //sends buffer
bool ConnectToWifi(unsigned long timeoutMs = 10000); //call in initialization, returns true if connected
void DisconnectWifi();
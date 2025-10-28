void addDataToBuf(uint8_t* data, int length); //length in bytes, delimits data with comma
void transmitData(); //sends buffer
bool connectToWifi(unsigned long timeoutMs = 10000); //call in initialization, returns true if connected
void disconnectWifi();
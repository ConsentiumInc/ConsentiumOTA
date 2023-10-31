#ifndef ConsentiumOTA_h
#define ConsentiumOTA_h

class ConsentiumOTA {
public:
    ConsentiumOTA(); 
    void begin();
    void initWiFi(const char*, const char*);
    void checkAndPerformUpdate(const char*); 
};

#endif
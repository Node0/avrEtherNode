#ifndef avrEtherNode_header
#define avrEtherNode_header

typedef struct {
        byte i = 0;
        byte present = 0;
        byte type_s;
        byte data[12];
        byte addr[8];
        byte cfg;
        int16_t raw;
        float celsius = 0;
        float fahrenheit = 0;
        String busLabel = "unlabeled";
    } owbInstanceParam;
    
#endif

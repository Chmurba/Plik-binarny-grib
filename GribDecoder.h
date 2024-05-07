
#ifndef GRIBDECODER_H
#define GRIBDECODER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class GribDecoder {
private:
    std::ifstream file;

    template<typename T>
    T readInt();

    std::string readString(int length);

public:
    GribDecoder();
    ~GribDecoder();

    void setFile(const std::string& filename);
    void decode();
    void decodeSection1();
    void decodeSection2();
    void decodeSection4();
    void decodeGeneralInfo(); // Deklaracja funkcji
};

#endif // GRIBDECODER_H

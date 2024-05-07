#include "gribdecoder.h"

template<typename T>
T GribDecoder::readInt() {
    T value;
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
}

std::string GribDecoder::readString(int length) {
    std::vector<char> buffer(length);
    file.read(buffer.data(), length);
    return std::string(buffer.data(), buffer.size());
}

void GribDecoder::decodeSection1() {
    std::cout << "\n======================\n";
    std::cout << "==  Section 1 Data  ==\n";
    std::cout << "======================\n";

    // Read section length
    int sectionLength = readInt<int16_t>();
    std::cout << "Section 1 length: " << sectionLength << std::endl;

    // Decode other fields from Section 1
    std::cout << "Parameter table Version: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Identification of Centre: " << readString(40) << std::endl;
    std::cout << "Process ID number: " << readString(4) << std::endl;
    std::cout << "Grid Identification: " << readInt<int32_t>() << std::endl;

    // Decode date
    std::string date = readString(14);
    std::string year = date.substr(0, 2);
    std::string month = date.substr(2, 2);
    std::string day = date.substr(4, 2);
    std::string hour = date.substr(7, 2);
    std::string minute = date.substr(10, 2);
    std::cout << "YY/MM/DD|HH:MM  :  " << year << "/" << month << "/" << day << "|" << hour << ":" << minute << std::endl;

    std::cout << "Time unit: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "P1 - Period of time: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "P2 - Period of time: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Time range indicator: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Numbers for last row: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Number missing from averages or accumulation: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Reference century: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Identification of sub Center: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
    std::cout << "Decimal Scale factor: " << static_cast<int>(readInt<uint8_t>()) << std::endl;
}


void GribDecoder::decodeSection2() {
    std::cout << "\n======================\n";
    std::cout << "==  Section 2 Data  ==\n";
    std::cout << "======================\n";

    // Read section length
    int sectionLength = readInt<int16_t>();
    std::cout << "Section 2 length: " << sectionLength << std::endl;

    // Decode and display fields from Section 2
    int numLatitudes = readInt<int16_t>();
    int numLongitudes = readInt<int16_t>();
    std::cout << "Number of Latitudes: " << numLatitudes << std::endl;
    std::cout << "Number of Longitudes: " << numLongitudes << std::endl;
    // Adjust the file pointer and read remaining data if necessary...
}

void GribDecoder::decodeSection4() {
    std::cout << "\n======================\n";
    std::cout << "==  Section 4 Data  ==\n";
    std::cout << "======================\n";

    // Read section length
    int sectionLength = readInt<int16_t>();
    std::cout << "Section 4 length: " << sectionLength << std::endl;

    // Decode and display fields from Section 4
    int flagToDecode = readInt<uint8_t>();
    int binaryScaleFactor = readInt<int16_t>();
    std::cout << "Flag to decode: " << flagToDecode << std::endl;
    std::cout << "Binary Scale Factor: " << binaryScaleFactor << std::endl;
    // Adjust the file pointer and read remaining data if necessary...
}

void GribDecoder::decodeGeneralInfo() {
    std::cout << "\n==========================\n";
    std::cout << "== General Message Info ==\n";
    std::cout << "==========================\n";

    // Get the current position in the file
    std::streampos gribPosition = file.tellg();
    std::cout << "Grib Finda at pos: " << static_cast<int>(gribPosition) << std::endl;

    // Search for '7777' in the file
    file.seekg(0, std::ios::end);
    std::streampos gribEnd = file.tellg();
    file.seekg(gribPosition);
    const int buffer_size = 4;
    char buffer[buffer_size];
    std::string gribEndStr = "7777";
    std::streampos foundPos = -1;
    while (file.tellg() < gribEnd - std::streamoff(buffer_size)) {
        file.read(buffer, buffer_size);
        std::string s(buffer, buffer_size);
        if (s == gribEndStr) {
            foundPos = file.tellg() - std::streamoff(buffer_size);
            break;
        }
        file.seekg(-std::streamoff(buffer_size) + 1, std::ios::cur);
    }
    std::cout << "7777 Find at position: " << static_cast<int>(foundPos) << std::endl;
    std::streamoff distance = foundPos - gribPosition;
    std::cout << "Distance between end of grib and start of 7777: " << static_cast<int>(distance) << std::endl;
    std::cout << "Whole Message length: " << static_cast<int>(gribEnd - gribPosition) << std::endl;
}

GribDecoder::GribDecoder() {}

GribDecoder::~GribDecoder() {
    file.close();
}

void GribDecoder::setFile(const std::string& filename) {
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
}

void GribDecoder::decode() {
    if (!file.is_open()) {
        std::cerr << "No file opened for decoding." << std::endl;
        return;
    }

    // Decode General Message Info
    decodeGeneralInfo();

    // Decode Sections
    decodeSection1();

    // Ensure the file pointer is positioned correctly
    // before decoding other sections
    file.seekg(40); // Assuming the position of Section 2
    decodeSection2();

    file.seekg(4578); // Assuming the position of Section 4
    decodeSection4();
}

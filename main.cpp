#include "gribdecoder.h"

int main() {
    GribDecoder decoder;
    decoder.setFile("all.grib");
    decoder.decode();
    return 0;
}

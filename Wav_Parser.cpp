#include <iostream>
#include "Wav_Parser.h"
Wav_Parser::Wav_Parser(string filename){
    this->filename = filename;
}
Wav_Parser::~Wav_Parser(){
    if(this->header!=nullptr){
        delete(this->header);
    }
}

int Wav_Parser::parse(){
    ifstream file(this->filename, std::ios::binary);
try{
    if (!file.is_open()) {
        throw runtime_error("Unable to open and parse WAV file ");
    }

   // Чтение заголовка WAV
    this->header = new wav_header;
    file.read(reinterpret_cast<char*>(this->header), sizeof(wav_header));
    file.seekg(this->header->subchunk2_size + 4,ios::cur);
    int dataSize;
    file.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));


    file.close();
    
    return dataSize;

}catch(...){
    cout<<"File can't be opened. Make sure that your file is ok.";
}

}

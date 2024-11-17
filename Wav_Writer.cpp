#include "Wav_Writer.h"

Wav_Writer::Wav_Writer(string in_filename,string out_filename){
    this->input_filename = in_filename;
    this->output_filename = out_filename;
}

void Wav_Writer::write(wav_header header) {
    header.subchunk2_id[0] = 'd'; 
    header.subchunk2_id[1] = 'a'; 
    header.subchunk2_id[2] = 't'; 
    header.subchunk2_id[3] = 'a';
    
    ifstream in(this->input_filename, ios::binary);
    ofstream out(this->output_filename, ios::binary);
    out.write(reinterpret_cast<char *>(&header), sizeof(header));
    in.seekg(sizeof(header),ios::beg);
    char byte;
    while(in.read(&byte,sizeof(char))){
        out.write(&byte,sizeof(char));
    }    
    in.close();
    out.close();
}
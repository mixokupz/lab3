#include "Converters.h"
#include "Sound_Proc.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "Wav_Parser.h"

Mute::Mute(vector<string> args){
try{
    if(args.size()!=3){
        throw runtime_error("Incorrect arguments for muting");
    }
    this->start = stoi(args[1]);
    this->end = stoi(args[2]); 
}catch(...){
        cout<<"Incorrect arguments for muting. Use: sound processor -h";
}
}
void Mute::convert(int subchunk2_size,int data_size){
    fstream file(Sound_Proc::output_file_name, std::ios::in | std::ios::out | std::ios::binary);
    
    unsigned long long border = min(static_cast<unsigned long long>(data_size),static_cast<unsigned long long>((this->end)*44100));
    short int zero = 0;
    for(unsigned long long i = (this->start)*44100;i<border;i++){
        file.seekp(2*i + subchunk2_size + 52,ios::beg);
        file.write(reinterpret_cast<char *>(&zero),sizeof(short int));
    }
    file.close();
}
Mix::Mix(vector<string> args) {
try{
    if (args.size() != 2 && args.size() != 3) {
        throw runtime_error("Incorrect arguments for mixing");
    } else if (args.size() == 2) {
        this->time = 0;
    } else {
        this->time = stoi(args[2]);
    }
    
    if (args[1].substr(0, 1) != "$") {
        throw runtime_error("Second argument must start with '$'");
    }

    int position = stoi(args[1].substr(1))-2; // Извлекаем индекс, первый доп файл это input2, его номер на 2 меньше
    
    // Проверка на выход за границы
    if (position < 0 || position >= Sound_Proc::other_input_names.size()) {
        throw std::out_of_range("Index is out of range in other_input_names vector.");
    }
    this->filename = Sound_Proc::other_input_names[position];
}catch(...){
    cout<<"Make sure you input right arguments. Use: sound processor -h";
}
    
}

void Mix::convert(int subchunk2_size,int data_size){
    fstream output(Sound_Proc::output_file_name,std::ios::in| std::ios::out | std::ios::binary);
    ifstream second(this->filename,std::ios::binary);

    Wav_Parser wav_Parser = Wav_Parser(this->filename);
    int dataSize_second = wav_Parser.parse();
    second.seekg(wav_Parser.header->subchunk2_size + 48,ios::beg);
    
    unsigned long long border = min(static_cast<unsigned long long>(data_size),static_cast<unsigned long long>(dataSize_second));
    
    short int smpl1;
    short int smpl2;
    short int smpl3;
    for(unsigned long long i = (this->time)*44100;i<border;i++){
        output.seekg(2*i + subchunk2_size + 52,ios::beg);
        output.read(reinterpret_cast<char *>(&smpl1), sizeof(smpl1));
        second.seekg(2*i + wav_Parser.header->subchunk2_size + 52,ios::beg);
        second.read(reinterpret_cast<char *>(&smpl2), sizeof(smpl2));
        smpl3 = (smpl1 +smpl2)/2;
        output.seekp(2*i + subchunk2_size + 52,ios::beg);
        output.write(reinterpret_cast<char *>(&smpl3),sizeof(short int));

    }
   
    output.close();
    second.close();
}
Volume::Volume(vector<string>args){
try{
    if(args.size()!=4){
        throw runtime_error("Incorrect argumets for voluming");
    }
    this->coef = stof(args[1]);
    this->start = stoi(args[2]);
    this->end = stoi(args[3]);
}catch(...){
    cout<<"Incorrect argumets for voluming. Use: sound processor -h";
}

}
void Volume::convert(int subchunk2_size,int data_size){
    fstream file(Sound_Proc::output_file_name,std::ios::in| std::ios::out | std::ios::binary);
    
    unsigned long long border = min(static_cast<unsigned long long>(data_size),static_cast<unsigned long long>((this->end)*44100));
    short int sampl;
    short int new_sampl;
    for(unsigned long long i = (this->start)*44100;i<border;i++){
        file.seekg(2*i + subchunk2_size + 52,ios::beg);
        file.read(reinterpret_cast<char *>(&sampl), sizeof(sampl));
        file.seekp(2*i+subchunk2_size + 52,ios::beg);
        new_sampl = (short int)(sampl*(this->coef));
        file.write(reinterpret_cast<char *>(&new_sampl),sizeof(short int));
    }
    file.close();
}
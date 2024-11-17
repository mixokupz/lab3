#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "Sound_Proc.h"
#include "Conf_Parser.h"
#include "Converters.h"

#include "Wav_Writer.h"
using namespace std;

Sound_Proc::Sound_Proc(int argc,char**argv){
    this->argc = argc;
    this->argv = argv;
}
Sound_Proc::~Sound_Proc(){
    this->argc = 0;
    this->argv = nullptr;
}

unique_ptr<Converter> create_converter(string&name,vector<string>args){
    if(name=="mute"){
        return make_unique<Mute>(args);
    }else if(name=="mix"){
        return make_unique<Mix>(args);
    }else if(name=="volume"){
        return make_unique<Volume>(args);
    }else{
        throw invalid_argument("Wrong name of converter");
    }
}

void Sound_Proc::run(){
try{
    string type(this->argv[1]);
    if(this->argc<2){
        throw invalid_argument("Wrong inputline! Use for help: sound_processor -h");
    }
    else if(type=="-h"){
        cout<<"It's sound processor. Use this format inputline:\n sound_processor [-h] -c <config.txt> <output.wav> <input1.wav> [<input2.wav> ...]\n";
        cout<<"Use this configure standart:\n#(...) - means comment\nThere're 3 type commands:\nmix $N (time)\nmute (start) (end)\nvolume (coef) (start) (end)\n";
        return;
    }
    else if(type!="-c"){
        throw invalid_argument("Wrong inputline! Use for help: sound_processor -h");
    }
    else if(this->argc<5){
        throw invalid_argument("Wrong inputline! Use for help: sound_processor -h");
    }else{
        string config_file = argv[2];
        this->output_file_name = argv[3];
        this->input_file_name = argv[4];
        
        for(int i = 5; i < argc; i++) {
            this->other_input_names.push_back(argv[i]);
        }
        Conf_Parser config_parser = Conf_Parser(config_file);
        vector<ConfigTuple> parsed = config_parser.Parse();
        
        Wav_Parser wav_Parser = Wav_Parser(this->input_file_name);
        int data_size = wav_Parser.parse();
        Wav_Writer writer = Wav_Writer(this->input_file_name,this->output_file_name);
        writer.write(*wav_Parser.header);
        
        for(const ConfigTuple& tuple : parsed) {
            string conv_name;
            vector<string>args;
            tie(conv_name,args) = tuple;
            try{
                auto converter = create_converter(conv_name,args);
                converter->convert(wav_Parser.header->subchunk2_size,data_size);
            }
            catch(invalid_argument){
                cout<<"Wrong converter name!";
                throw invalid_argument("Wrong converter name!");
            }
            catch(runtime_error){
                cout<<"Wrong command!";
                throw runtime_error("Wrong command!");
            }
        }
    }
}catch(invalid_argument){
    cout<<"Wrong input, or incorrect converter name: use sound processor -h\n";
}catch(runtime_error){
    cout<<"Wrong commands in configure file: use sound processor -h\n";
}        
    

}
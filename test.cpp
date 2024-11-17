#include "Sound_Proc.h"
#include "Conf_Parser.h"
#include "Converters.h"
#include "Wav_Writer.h"
#include "gtest/gtest.h"

TEST(sound_proc_test, sound_proc_create) {
    char* in[] = {"-c", "config.txt", "output.wav", "input.wav"};
    Sound_Proc proc = Sound_Proc(4,in);
    proc.input_file_name = in[3];
    ASSERT_EQ("input.wav", proc.input_file_name);
}
TEST(sound_proc_test, wav_parser) {
    Wav_Parser wavpars = Wav_Parser("Yankee.wav");
    int dataSize = wavpars.parse();
    ASSERT_EQ(dataSize,8675328);
    ASSERT_EQ(wavpars.header->chunk_id[0],'R');
    ASSERT_EQ(wavpars.header->chunk_id[1],'I');
    ASSERT_EQ(wavpars.header->chunk_id[2],'F');
    ASSERT_EQ(wavpars.header->chunk_id[3],'F');

    ASSERT_EQ(wavpars.header->format[0],'W');
    ASSERT_EQ(wavpars.header->format[1],'A');
    ASSERT_EQ(wavpars.header->format[2],'V');
    ASSERT_EQ(wavpars.header->format[3],'E');
    
    ASSERT_EQ(wavpars.header->subchunk2_id[0],'L');
    ASSERT_EQ(wavpars.header->subchunk2_id[1],'I');
    ASSERT_EQ(wavpars.header->subchunk2_id[2],'S');
    ASSERT_EQ(wavpars.header->subchunk2_id[3],'T');
}
TEST(sound_proc_test, wav_writer) {
    Wav_Parser wavpars = Wav_Parser("Yankee.wav");
    int data_size = wavpars.parse();
    Wav_Writer wav_writer  = Wav_Writer("Yankee.wav","output_file.wav");
    wav_writer.write(*wavpars.header);
    
    ifstream file("output_file.wav");
    EXPECT_TRUE(file.is_open());

}
TEST(sound_proc_test, config_parser) {
    Conf_Parser config_parser = Conf_Parser("config.txt");
    vector<ConfigTuple> parsed = config_parser.Parse();
    string conv_name;
    vector<string> args;
    tie(conv_name,args) = parsed[0];
    ASSERT_EQ(conv_name,"mute");
    ASSERT_EQ(args[1],"10");
    ASSERT_EQ(args[2],"15");
    tie(conv_name,args) = parsed[1];
    ASSERT_EQ(conv_name,"mix");
    ASSERT_EQ(args[1],"$2");
    ASSERT_EQ(args[2],"50");
    tie(conv_name,args) = parsed[2];
    ASSERT_EQ(conv_name,"volume");
    ASSERT_EQ(args[1],"5.0");
    ASSERT_EQ(args[2],"1");
    ASSERT_EQ(args[3],"19");
}
TEST(sound_proc_test, converter_mute){
    char* in[] = {"-c", "config.txt", "output.wav", "Yankee.wav"};
    Sound_Proc proc = Sound_Proc(4,in);
    proc.input_file_name = in[3];
    proc.output_file_name = in[2];
    
    vector<string> args2 = {"mute","20","45"};
    Mute mute = Mute(args2);
    Wav_Parser wavpars = Wav_Parser("Yankee.wav");
    int data_size = wavpars.parse();
    Wav_Writer writer = Wav_Writer("Yankee.wav","output.wav");
    writer.write(*wavpars.header);
    mute.convert(wavpars.header->subchunk2_size,data_size);

    fstream file("output.wav", std::ios::in | std::ios::out | std::ios::binary);
    
    short int zero;
    for(unsigned long long i = (20)*44100;i<(45)*44100;i++){
        file.seekg(2*i + wavpars.header->subchunk2_size + 52,ios::beg);
        file.read(reinterpret_cast<char *>(&zero),sizeof(short int));
        ASSERT_EQ(zero,0);
    }
    file.close();
}
TEST(sound_proc_test, converter_volume){
    char* in[] = {"-c", "config.txt", "output.wav", "Yankee.wav"};
    Sound_Proc proc = Sound_Proc(4,in);
    proc.input_file_name = in[3];
    proc.output_file_name = in[2];
    
    vector<string> args2 = {"volume","1.0","20","45"};
    Volume vol = Volume(args2);
    Wav_Parser wavpars = Wav_Parser("Yankee.wav");
    int data_size = wavpars.parse();
    Wav_Writer writer = Wav_Writer("Yankee.wav","output.wav");
    writer.write(*wavpars.header);
    vol.convert(wavpars.header->subchunk2_size,data_size);

    fstream file("output.wav", std::ios::in | std::ios::out | std::ios::binary);
    ifstream file2("Yankee.wav",ios::binary);

    short int first;
    short int second;
    for(unsigned long long i = (20)*44100;i<(45)*44100;i++){
        file.seekg(2*i + wavpars.header->subchunk2_size + 52,ios::beg);
        file2.seekg(2*i + wavpars.header->subchunk2_size + 52,ios::beg);
        file.read(reinterpret_cast<char *>(&first),sizeof(short int));
        file2.read(reinterpret_cast<char *>(&second),sizeof(short int));
        ASSERT_EQ(first,second*1.0);
    }
    file.close();
    file2.close();
}

TEST(sound_proc_test, converter_mix){
    char* in[] = {"-c", "config.txt", "output.wav", "Yankee.wav","funkorama.wav"};
    Sound_Proc proc = Sound_Proc(5,in);
    proc.input_file_name = in[3];
    proc.output_file_name = in[2];
    proc.other_input_names.push_back("funkorama.wav");
    
    vector<string> args2 = {"mix","$2","50"};
    Mix mix = Mix(args2);
    Wav_Parser wavpars = Wav_Parser("Yankee.wav");
    int data_size = wavpars.parse();
    Wav_Writer writer = Wav_Writer("Yankee.wav","output.wav");
    writer.write(*wavpars.header);
    mix.convert(wavpars.header->subchunk2_size,data_size);
    Wav_Parser owavpars = Wav_Parser("funkorama.wav");
    int data2_size = owavpars.parse();
    ifstream file("output.wav",ios::binary);
    ifstream file2("Yankee.wav",ios::binary);
    ifstream file3("funkorama.wav",ios::binary);
    
    short int first;
    short int second;
    short int third;
    for(unsigned long long i = (50)*44100;i<(60)*44100;i++){
        file.seekg(2*i + wavpars.header->subchunk2_size + 52,ios::beg);
        file2.seekg(2*i + wavpars.header->subchunk2_size + 52,ios::beg);
        file3.seekg(2*i + owavpars.header->subchunk2_size + 52,ios::beg);
        file.read(reinterpret_cast<char *>(&first),sizeof(short int));
        file2.read(reinterpret_cast<char *>(&second),sizeof(short int));
        file3.read(reinterpret_cast<char *>(&third),sizeof(short int));
        ASSERT_EQ(first,(second+third)/2);
        
    }
    file.close();
    file2.close();
    file3.close();
}
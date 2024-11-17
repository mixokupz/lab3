#include <fstream>
#include <string>
#include <vector>

#define HERZ 44100
using namespace std;

typedef struct{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;
}wav_header;

class Wav_Parser{
    public:
        Wav_Parser(string filename);
        ~Wav_Parser();
        wav_header *header;
        int parse();
    private:
        string filename;

};
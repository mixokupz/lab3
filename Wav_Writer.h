#include <string>
#include <vector>
#include "Wav_Parser.h"
using namespace std;

class Wav_Writer {
    public:
        Wav_Writer(string in_filename,string out_filename);
        void write(wav_header header);
    private:
        string output_filename;
        string input_filename;
};
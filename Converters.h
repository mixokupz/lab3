#include <string>
#include <vector>

using namespace std;

    class Converter {
        public:
            virtual ~Converter() = default;
            virtual void convert(int subchunk2_size,int data_size) = 0;
    };
    class Mute: public Converter{
        public:
            explicit Mute(vector<string> args);
            void convert(int subchunk2_size,int data_size)override;
        private:
            int start;
            int end;
    };
    class Volume: public Converter{
        public:
            explicit Volume(vector<string> args);
            void convert(int subchunk2_size,int data_size)override;
        private:
            float coef;
            int start;
            int end;
    };
    class Mix: public Converter{
        public:
            explicit Mix(vector<string> args);
            void convert(int subchunk2_size,int data_size)override;
        private:
            string filename;
            int time;
    };
#include <vector>
#include <string>
using namespace std;
class Sound_Proc {
    private:
        int argc;
        char** argv;
    public:
        inline static string input_file_name;
        inline static string output_file_name;
        inline static vector<string> other_input_names;
    
        Sound_Proc(int argc, char** argv);
        ~Sound_Proc();
        void run();
        
};
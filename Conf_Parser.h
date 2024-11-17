#include <string>
#include <vector>
using namespace std;
typedef tuple<string, vector<string>> ConfigTuple;

class Conf_Parser {
    public:
        Conf_Parser(string config_name);
        vector<ConfigTuple> Parse();
    private:
        string config_name;
};
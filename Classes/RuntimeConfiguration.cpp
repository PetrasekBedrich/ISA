#include <string> 
using namespace std;

enum SortByENUM
{
    Packets,
    Bytes,
    None
};

class RuntimeConfiguration
{
    public:
       SortByENUM SortBy;
       char* Interface;
       int NumberOfCommunications = 10;
};

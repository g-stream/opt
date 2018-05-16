#include "pre.h"
using namespace std;
int main(int argc, char* argv[])
{
    for(int i = 0; i < argc; i++) {
        cout << argv[i];
    }
    ParserKFile reader("/home/yinly/opt/lsdyna/file.k", "/home/yinly/opt/lsdyna/output.k");
    reader.parser();
    reader.adjust();
    reader.dump();
    reader.test();
    reader.calcu();
    return 0;
}

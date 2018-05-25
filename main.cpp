#include "pre.h"
#include <cassert>
using namespace std;
int main(int argc, char* argv[])
{
    cout << "starting.... " << endl;
    assert(argc == 2);//输入文件目录与输出文件目录
    for(int i = 0; i < argc; i++) {
        cout << argv[i];
    }
    string in_str(argv[1]);
    system((string("mkdir -p ") + in_str).c_str());
    ParserKFile reader("/home/yinly/opt/lsdyna/file.k", in_str);
    reader.parser();
    reader.adjust();
    reader.dump();
    reader.test();
    reader.calcu();
    return 0;
}

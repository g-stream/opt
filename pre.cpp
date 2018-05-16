#include "pre.h"
double Element_Shell::max_x = 0;
double Element_Shell::min_x = 0;
double Element_Shell::max_y = 0;
double Element_Shell::min_y = 0;
double Element_Shell::max_z = 0;

void ParserKFile::parser()
{
    std::cout << "executing parser" << std::endl;
    m_input.clear();
    m_input.seekg(0,std::ios::beg);
    std::string str;
    while(getline(m_input, str))
    {
        while(str.back()==' ')str.pop_back();
        if(str.size()>0&&str.front()!='$')
        {
            if(str.size()>1&&str.front() == '*')
            {
                str.erase(str.begin());
                m_state = m_stringToBid[str];
                continue;
            }
            switch(m_state)
            {
            case 4://NODE
                handle_node(str);
                break;
            case 6://SECTION_SOLID
                break;
            case 9://PART
                break;
            case 10://ELEMENT_SHELL
                handle_element_shell(str);
                break;
            case 11://ELEMENT_SOLID
                handle_element_solid(str);
                break;
            default:
                break;
            }
        }
    }
}
void ParserKFile::dump()
{
    std::cout << "executing dump" << std::endl;
    m_state = -1;
    m_input.clear();
    m_input.seekg(0,std::ios::beg);
    std::string str;
    while(getline(m_input, str)){
        using namespace std;
        if(str[0] == '*'){
            while(str.back()==' ')str.pop_back();
            m_state = m_stringToBid[str.substr(1,str.size()-1)];
            m_output << str << std::endl;
            char tmp[100];
            switch(m_state){
            case 4:
                for(auto node :m_nodes){
                    sprintf(tmp, "%8d%16.8E%16.8E%16.8E%8d%8d", node.second.nid, node.second.x, node.second.y, node.second.z,0,0);
                    m_output << tmp << std::endl;
                }
                break;
            case 10:
                 for(auto e :m_element_shells){
                    sprintf(tmp, "%8d%8d%8d%8d%8d%8d", e.second.eid, e.second.pid, e.second.n1, e.second.n2, e.second.n3, e.second.n4);
                    m_output << tmp << std::endl;
                }
                break;
            case 11:
                 for(auto e :m_element_solids){
                    sprintf(tmp, "%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d", e.second.eid, e.second.pid, e.second.n1, e.second.n2, e.second.n3, e.second.n4, e.second.n5, e.second.n6, e.second.n7, e.second.n8);
                    m_output << tmp << std::endl;
                }
                break;
            }
            continue;
        }
        switch(m_state){
        case 4:
        case 10:
        case 11:
            break;
        default:
            m_output << str << std::endl;
        }
    }
}
void ParserKFile::adjust()
{
    std::cout << "executing adjust" << std::endl;
    using namespace std;
    std::vector<Opt_Par>& pars = extractOptPar();
    //computer the center of the element_shell
    for(auto p: m_element_shells){
        Element_Shell e = p.second;
        Node n1 = m_nodes[e.n1];
        Node n2 = m_nodes[e.n2];
        Node n3 = m_nodes[e.n3];
        Node n4 = m_nodes[e.n4];
        m_element_shells[e.eid].x = (n1.x + n2.x + n3.x + n4.x)/4;
        m_element_shells[e.eid].y = (n1.y + n2.y + n3.y + n4.y)/4;
        m_element_shells[e.eid].z = (n1.z + n2.z + n3.z + n4.z)/4;

        m_element_shells[e.eid].pid = NORMAL_PART;
        if(n1.x == n2.x && n1.x == n3.x && n1.x>0)
            m_element_shells[e.eid].side = 1;
        if(n1.x == n2.x && n1.x == n3.x && n1.x<0)
            m_element_shells[e.eid].side = 3;
        if(n1.y == n2.y && n1.y == n3.y && n1.y>0)
            m_element_shells[e.eid].side = 2;
        if(n1.y == n2.y && n1.y == n3.y && n1.y<0)
            m_element_shells[e.eid].side = 4;
        double max_x = max4(n1.x,n2.x,n3.x,n4.x);
        double min_x = min4(n1.x,n2.x,n3.x,n4.x);
        double max_y = max4(n1.y,n2.y,n3.y,n4.y);
        double min_y = min4(n1.y,n2.y,n3.y,n4.y);
        double max_z = max4(n1.z,n2.z,n3.z,n4.z);
        Element_Shell::max_x = std::max(Element_Shell::max_x, max_x);
        Element_Shell::min_x = std::min(Element_Shell::min_x, min_x);
        Element_Shell::max_y = std::max(Element_Shell::max_y, max_y);
        Element_Shell::min_y = std::min(Element_Shell::min_y, min_y);
        Element_Shell::max_z = std::max(Element_Shell::max_z, max_z);
    }

    double width = Element_Shell::max_x - Element_Shell::min_x;
    for(auto par:pars){
        for(auto p: m_element_shells){
            Element_Shell e = p.second;
            double lambda,zh1,zh2,zl1,zl2;
            switch(e.side){
            case 1:
                 lambda = (e.y - Element_Shell::min_y)/width;
                 zh1 = par.z[0] + par.l[0];
                 zh2 = par.z[1] + par.l[1];
                 zl1 = par.z[0] - par.l[0];
                 zl2 = par.z[1] - par.l[1];
                if( lambda*zh2+(1-lambda)*zh1 >= e.z && lambda*zl2+(1-lambda)*zl1 <= e.z){
                    m_element_shells[e.eid].pid = ENFORCED_PART;
                }
                break;
            case 2:
                 lambda = (e.x - Element_Shell::min_x)/width;
                 zh1 = par.z[2] + par.l[2];
                 zh2 = par.z[1] + par.l[1];
                 zl1 = par.z[2] - par.l[2];
                 zl2 = par.z[1] - par.l[1];
                if( lambda*zh2+(1-lambda)*zh1 >= e.z && lambda*zl2+(1-lambda)*zl1 <= e.z){
                    m_element_shells[e.eid].pid = ENFORCED_PART;
                }
                break;
            case 3:

                 lambda = (e.y - Element_Shell::min_y)/width;
                 zh1 = par.z[3] + par.l[3];
                 zh2 = par.z[2] + par.l[2];
                 zl1 = par.z[3] - par.l[3];
                 zl2 = par.z[2] - par.l[2];
                if( lambda*zh2+(1-lambda)*zh1 >= e.z && lambda*zl2+(1-lambda)*zl1 <= e.z){
                    m_element_shells[e.eid].pid = ENFORCED_PART;
                }
                break;
            case 4:
                 lambda = (e.x - Element_Shell::min_x)/width;
                 zh1 = par.z[3] + par.l[3];
                 zh2 = par.z[0] + par.l[0];
                 zl1 = par.z[3] - par.l[3];
                 zl2 = par.z[0] - par.l[0];
                if( lambda*zh2+(1-lambda)*zh1 >= e.z && lambda*zl2+(1-lambda)*zl1 <= e.z){
                    m_element_shells[e.eid].pid = ENFORCED_PART;
                }
                break;
            default:
                std::cout << "in switch case and can't match any side!\n"; system("pause");
            }
            //std::cout << e.side << std::endl;
        }
    }

}
void ParserKFile::test()
{
    std::cout << "executing test" << std::endl;
    system("cd /tmp/testlsdyna && pwd && rm /tmp/testlsdyna/* && /usr/ansys_inc/v172/ansys/bin/lsdyna i=/tmp/file.k ncpu=8");
}
void ParserKFile::handle_node(std::string& str)
{
    std::stringstream ss;
    int nid = atoi(str.substr(0,8).c_str());
    double x, y, z;
    std::string tmp = str.substr(8,16);
    ss << tmp;
    ss >> x;
    ss.clear();
    tmp = str.substr(24,16);
    ss << tmp;
    ss >> y;
    ss.clear();
    tmp = str.substr(40,16);
    ss << tmp;
    ss >> z;
    m_nodes[nid] = Node{nid,x,y,z};
}
void ParserKFile::handle_element_shell(std::string& str)
{
    int eid = atoi(str.substr(0,8).c_str());
    int pid = atoi(str.substr(8,8).c_str());
    int n1 = atoi(str.substr(16,8).c_str());
    int n2 = atoi(str.substr(24,8).c_str());
    int n3 = atoi(str.substr(32,8).c_str());
    int n4 = atoi(str.substr(40,8).c_str());
    m_element_shells[eid] = Element_Shell{eid,pid,n1,n2,n3,n4};
}
void ParserKFile::handle_element_solid(std::string& str)
{
    int eid = atoi(str.substr(0,8).c_str());
    int pid = atoi(str.substr(8,8).c_str());
    int n1 = atoi(str.substr(16,8).c_str());
    int n2 = atoi(str.substr(24,8).c_str());
    int n3 = atoi(str.substr(32,8).c_str());
    int n4 = atoi(str.substr(40,8).c_str());
    int n5 = atoi(str.substr(48,8).c_str());
    int n6 = atoi(str.substr(56,8).c_str());
    int n7 = atoi(str.substr(64,8).c_str());
    int n8 = atoi(str.substr(72,8).c_str());
    m_element_solids[eid] = Element_Solid{eid,pid,n1,n2,n3,n4,n5,n6,n7,n8};
}
std::vector<Opt_Par>& ParserKFile::extractOptPar()
{
    std::ifstream file("/home/yinly/opt/lsdyna/optpar.txt");
    std::stringstream ss;
    std::string str;
    std::vector<Opt_Par>* p = new std::vector<Opt_Par>;
    while(getline(file, str)){
        ss << str;
        Opt_Par tmp;
        for(int i = 0; i < 4; i++){
            ss >> tmp.z[i];
        }
        for(int i = 0; i < 4; i++){
            ss >> tmp.l[i];
        }
        ss.clear();
        p->push_back(tmp);
    }
    file.close();
    return *p;
}
void ParserKFile::calcu(){
    using namespace std;
    ifstream file("/tmp/testlsdyna/rcforc");
    string str;
    stringstream ss;
    vector<PointData> result;
    while(getline(file,str)){
        if(str.find("slave") != string::npos) {
            PointData tmp;
            double time, f;
            getline(file, str);//四行中的第二和四行是所需要的数据区域
            unsigned int index_of_time=0;
            for(; index_of_time+3<str.size(); index_of_time++) {
                if(str.substr(index_of_time,4)=="time")
                    break;
            }
            unsigned int index_of_x = index_of_time;
            for(; index_of_x+1<str.size(); index_of_x++) {
                if(str[index_of_x] == 'x')
                    break;
            }
            unsigned int index_of_z = index_of_x;
            for(; index_of_z+1<str.size(); index_of_z++) {
                if(str[index_of_z] == 'z')
                    break;
            }
            unsigned int index_of_m = index_of_x;
            for(; index_of_m+1<str.size(); index_of_m++) {
                if(str[index_of_m] == 'm')
                    break;
            }
            string str_time = str.substr(index_of_time+4,index_of_x-index_of_time-4);
            string str_z = str.substr(index_of_z+1,index_of_m-index_of_z-1);
            ss.clear();
            ss << str_time;
            ss >> time;
            ss.clear();
            ss << str_z;
            ss >> f;
            tmp.t = time;
            tmp.f1 = f;
            getline(file,str);
            getline(file,str);
            str_z = str.substr(index_of_z+1,index_of_m-index_of_z-1);
            ss.clear();
            ss << str_z;
            ss >> f;
            tmp.f2 = f;
            result.push_back(tmp);
        }
    }
    double ans = 0;
    ans += (result[1].f1+result[1].f2)*6*result[1].t/2;
    for(auto i = result.begin()+2; i!=result.end()&&i->t<0.01; ++i){
        auto j = i-1;
        ans += 6*(i->t - j->t)*(i->f1+i->f2+j->f1+j->f2)/2;
    }
    file.close();
    ofstream file2("/home/yinly/opt/lsdyna/obj.txt");
    file2 << ans;
    file2.close();
    std::cout << "Answer is: " << ans <<std::endl;
}

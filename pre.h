#ifndef PRE_H_
#define PRE_H_
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
struct Node
{
    int nid;
    double x,y,z;
    int tc,rc;
};
struct Element_Shell
{
    int eid,pid,n1,n2,n3,n4;
    double x,y,z;
    int side;
    static double max_x ;
    static double min_x ;
    static double max_y ;
    static double min_y ;
    static double max_z ;
};


struct Element_Solid
{
    int eid,pid,n1,n2,n3,n4,n5,n6,n7,n8;
};
struct Opt_Par
{
    double z[4];
    double l[4];
};

struct PointData
{
    double t,f1,f2;
};
class ParserKFile
{
public:
    ParserKFile(std::string in, std::string out):m_input(in),m_output(out){
        if(!m_input)
            std::cout << "Cannot open file: " << in << std::endl;
        if(!m_output)
            std::cout << "Cannot open file: " << out << std::endl;
    }
    ~ParserKFile(){
        m_input.close();
        m_output.close();
        }
    void parser();
    void dump();
    void test();
    void adjust();
    void calcu();



private:
    std::ifstream m_input;
    std::ofstream m_output;
    int m_state = -1;
    std::map<int,Node> m_nodes;
    std::map<int,Element_Shell> m_element_shells;
    std::map<int,Element_Solid> m_element_solids;
    std::map<int, std::string> m_bidToString{{1,"KEYWORD"},{2,"TITLE"},{3,"DATABASE_FORMAT"},{4,"NODE"},{5,"SECTION_SHELL"},\
    {6,"SECTION_SOLID"},{7,"MAT_PLASTIC_KINEMATIC"},{8,"MAT_RIGID"},{9,"PART"},{10,"ELEMENT_SHELL"},{11,"ELEMENT_SOLID"},\
    {12,"DEFINE_CURVE"},{13,"BOUNDARY_PRESCRIBED_MOTION_RIGID"},{14,"SET_NODE_LIST"},{15,"BOUNDARY_SPC_SET"},\
    {16,"CONTACT_AUTOMATIC_SINGLE_SURFACE"},{17,"CONTACT_AUTOMATIC_SURFACE_TO_SURFACE"},{18,"CONTROL_ENERGY"},\
    {19,"CONTROL_SHELL"},{20,"CONTROL_TIMESTEP"},{21,"CONTROL_TERMINATION"},{22,"DATABASE_HISTORY_NODE"},\
    {23,"DATABASE_RWFORC"},{24,"DATABASE_GLSTAT"},{25,"DATABASE_DEFORC"},{26,"DATABASE_MATSUM"},{27,"DATABASE_NCFORC"},\
    {28,"DATABASE_RCFORC"},{29,"DATABASE_DEFGEO"},{30,"DATABASE_SPCFORC"},{31,"DATABASE_SWFORC"},{32,"DATABASE_BNDOUT"},\
    {33,"DATABASE_RBDOUT"},{34,"DATABASE_GCEOUT"},{35,"DATABASE_SLEOUT"},{36,"DATABASE_JNTFORC"},{37,"DATABASE_BINARY_D3PLOT"},\
    {38,"DATABASE_BINARY_D3THDT"},{39,"DATABASE_BINARY_D3DUMP"},{40,"DATABASE_EXTENT_BINARY"},{41,"END"}};
    std::map<const std::string, int> m_stringToBid{{"KEYWORD",1},{"TITLE",2},{"DATABASE_FORMAT",3},{"NODE",4},{"SECTION_SHELL",5},\
    {"SECTION_SOLID",6},{"MAT_PLASTIC_KINEMATIC",7},{"MAT_RIGID",8},{"PART",9},{"ELEMENT_SHELL",10},{"ELEMENT_SOLID",11},\
    {"DEFINE_CURVE",12},{"BOUNDARY_PRESCRIBED_MOTION_RIGID",13},{"SET_NODE_LIST",14},{"BOUNDARY_SPC_SET",15},\
    {"CONTACT_AUTOMATIC_SINGLE_SURFACE",16},{"CONTACT_AUTOMATIC_SURFACE_TO_SURFACE",17},{"CONTROL_ENERGY",18},\
    {"CONTROL_SHELL",19},{"CONTROL_TIMESTEP",20},{"CONTROL_TERMINATION",21},{"DATABASE_HISTORY_NODE",22},{"DATABASE_RWFORC",23},\
    {"DATABASE_GLSTAT",24},{"DATABASE_DEFORC",25},{"DATABASE_MATSUM",26},{"DATABASE_NCFORC",27},{"DATABASE_RCFORC",28},\
    {"DATABASE_DEFGEO",29},{"DATABASE_SPCFORC",30},{"DATABASE_SWFORC",31},{"DATABASE_BNDOUT",32},{"DATABASE_RBDOUT",33},\
    {"DATABASE_GCEOUT",34},{"DATABASE_SLEOUT",35},{"DATABASE_JNTFORC",36},{"DATABASE_BINARY_D3PLOT",37},{"DATABASE_BINARY_D3THDT",38},\
    {"DATABASE_BINARY_D3DUMP",39},{"DATABASE_EXTENT_BINARY",40},{"END",41}};
    void handle_node(std::string& str);
    void handle_element_shell(std::string& str);
    void handle_element_solid(std::string& str);
    std::vector<Opt_Par>& extractOptPar();
    static const int ENFORCED_PART = 1;
    static const int NORMAL_PART = 2;


};
inline double max4(double x, double y, double z, double w){
    return std::max(std::max(x,y),std::max(z,w));
}
inline double min4(double x, double y, double z, double w){
    return std::min(std::min(x,y),std::min(z,w));
}

#endif // PRE_H_

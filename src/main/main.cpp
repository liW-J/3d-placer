#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#include "../utility/paramHdl.h"
#include "../parser/parser.h"
#include "../dataModel/dm.h"

int main(int argc, char** argv){
    /*Preliminary Information*/
    cout << "     #############################################################" << endl;
    cout << "     #                                                           #" << endl;
	cout << "     #                         [EDA LAB]                         #" << endl;
	cout << "     #-----------------------------------------------------------#" << endl;
    cout << "     #                        D2D  Placer                        #" << endl;
    cout << "     #                                                           #" << endl;
	cout << "     #############################################################" << endl;
    cout << endl;
    //variable declaration
    clock_t tStart = clock();

    // parameter handler
    ParamHdl_C paramHdl = ParamHdl_C(argc, argv);
    if(!paramHdl.ok())
        return 0;
    // parser
    Parser_C parser;
    if(!parser.read_file(paramHdl.get_input_fileName()))
        return 0;
    // if(paramHdl.check_flag_exist("debug") || paramHdl.check_flag_exist("d")){ // debug_mode
    //     parser.print_info();
    // }

    //srand( time(NULL) );
    if(paramHdl.check_flag_exist("seed")) {
        srand( stoi(paramHdl.get_para("seed")) );
    } else {
        srand( 5 );
    }

    // control centor
    DmMgr_C* dmMgr = new DmMgr_C(parser, paramHdl, tStart);
    dmMgr->print_info();
    dmMgr->run();
    dmMgr->print_result();
    //dmMgr->dump_info();
    dmMgr->output_result(paramHdl.get_output_fileName());

    return 0;
}
#ifndef PLACER_H
#define PLACER_H
#include <tuple>
#include <vector>
#include <map>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>
#include <numeric>
#include "../utility/color.h"
#include "../utility/aux.h"
#include "../utility/hgr.h"
#include "../dataModel/module.h"
#include "../dataModel/bin.h"
#include "../utility/paramHdl.h"
#include "../utility/drawHtml.h"
#include "../partitioner/partitioner.h"

using namespace std;

class Placer_C{
    ParamHdl_C _paramHdl;
    
    Chip_C* _pChip;
    Design_C* _pDesign;
    vector<Pos> _vCellBestPos;
    vector<Cell_C*>& _vCell;
    vector<Net_C*>& _vNet;
    unordered_map<string,Cell_C*>& _mCell;
    unordered_map<string,Net_C*>& _mNet;
    clock_t _tStart;
    string _RUNDIR = "./run_tmp/"; // become './run_tmp/<casename>' in run();
    string _DRAWDIR = "./draw/"; // become './draw/<casename>' in run();
    vector<BNet_C*> _vBNet;
    string _prPara;
    string _prPara1;
    string _prPara2;
    string _srPara;
    string _srPara1;
    string _srPara2;
    string _rPara;
    string _rPara1;
    string _rPara2;
    bool _multiLevel;
    double _usedArea[2];
    bool _useParallelRePlace;
    bool _useParallelRePlace1;
    bool _useParallelRePlace2;
    
public:
    //Place_C();
    Placer_C(Chip_C*, Design_C*, ParamHdl_C&, clock_t);
    
    void run();
    void run_safe_mode();
    void init_place();
    bool order_place();
    void cell_spreading();
    void cell_spreading(int); // 2d global
    bool shrunked_2d_ntuplace(); // 2d place
    bool shrunked_2d_ntuplace(string); // 2d place
    bool shrunked_2d_replace();
    
    
    bool via_refinement(); //Refinement : Main
    bool check_new_ball_legal(int new_x, int new_y, int cur_net_id); //Refinement : Check new ball place legal   
    bool check_new_ball_legal_sorted(int new_x, int new_y, int cur_net_id, vector<int> &nets_need_compare);
    bool sort_ball_xy_vector(int new_place, int net_id, vector<pair<int, int>> & sorted_ball_vector);
    bool replace_via_spirally();

    void rand_place(int); // die_by_die
    void rand_ball_place();
    void init_ball_place();
    void set_ball();
    void init_run_dir();
    void init_draw_dir();
    void clear();

    /* partition + die-by-die Place */
    bool pseudo3d_ntuplace();
    bool coloquinte_place();
    bool random_d2dplace();
    bool ntu_d2dplace();
    bool pin3d_ntuplace();
    bool shrunk2d_ntuplace();
    bool shrunk2d_replace();
    bool shrunk2d_replace_tryflow();
    void mincut_partition();
    void mincut_k_partition();
    void bin_based_partition_real();
    void bin_based_partition(int bin_num);
    void bin_based_partition_new();
    void partition_refinement();
    void gnn_partition();
    void init_place_ball();
    void run_ntuplace3(string caseName);
    void run_ntuplace3(string caseName, string otherPara);
    void run_replace(string caseName, bool parallel_replace);
    void run_ntuplace4(string caseName);
    void run_hmetis(int k, double ufactor, string caseName); // (k-way part)
    bool read_pl_and_set_pos(string fileName);
    bool read_pl_and_set_pos(string fileName, int dieId);
    bool read_pl_and_set_pos_for_ball(string fileName);


    /* GlobalPlace + Legal + DetailPlace*/
    bool ntu_d2d_legal_detail();
    bool pin3d_ntu_d2d_legal_detail();

    void create_aux_form(AUX&, int dieId, string caseName);
    void create_aux_form_for_ball(AUX&, string caseName);
    void create_aux_form_replace(AUX&, int dieId, string caseName);
    void create_aux_form_for_ball_replace(AUX&, string caseName);
    void add_project_pin(AUX&, int dieID);
    void add_project_ball(AUX&);
    int cal_ball_num();
    int cal_HPWL();
    int cal_HPWL_binbased();

    // visualize
    void draw_layout_result();
    void draw_layout_result(string tag);
    void draw_layout_result_plt(bool show_hpwl);
    void draw_layout_result_plt(bool show_hpwl, string tag);

    // test
    bool placement_testGNN();
    void get_para(bool test);

    void output_result(string);
};

#endif
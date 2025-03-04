#include "module.h"
#include <string>
#include <unordered_set>

//string pos3d2str(Pos pos) {return "("+to_string(get<0>(pos))+","+to_string(get<1>(pos))+","+to_string(get<2>(pos))+")";}
//string pos2d2str(Pos pos) {return "("+to_string(get<1>(pos))+","+to_string(get<2>(pos))+")";}

//-----------------------------------------------------------------------------------------------------//
CellLib_C::CellLib_C(){}
CellLib_C::CellLib_C(string name, int pin_num, int tech_num){
    _name = name;
    _numTech = tech_num;
    _techW.resize(tech_num, 0);
    _techH.resize(tech_num, 0);
    _vTechLibPinOffset.resize(tech_num, vector<Pos>(pin_num));
    _vPinName.clear();
}
void CellLib_C::add_pin(int tech_id, string pinName, Pos offset){
    assert(tech_id < _vTechLibPinOffset.size());
    // find the id for the pin. If not exist than push_back(pinName). Then set pin_offset.
    int pinId;
    auto it = _mPinName2Idx.find(pinName);
    if(it == _mPinName2Idx.end()){
        pinId = _vPinName.size();
        _mPinName2Idx[pinName] = pinId;
        _vPinName.emplace_back(pinName);
    } else{
        pinId = it->second;
    }
    _vTechLibPinOffset[tech_id][pinId] = offset;
}
void CellLib_C::add_pin(int tech_id, string pinName, int offsetX, int offsetY){
    add_pin(tech_id, pinName, Pos(offsetX, offsetY));
}
void CellLib_C::set_size(int tech_id, int sizeX, int sizeY){
    assert(tech_id < _techW.size() && tech_id < _techH.size());
    _techW[tech_id] = sizeX;
    _techH[tech_id] = sizeY;
}
string CellLib_C::get_name(){
    return _name;
}
int CellLib_C::get_tech_num(){
    return _numTech;
}
int CellLib_C::get_pin_num(){
    return _vPinName.size();
}
string CellLib_C::get_pin_name(int pinId){
    return _vPinName[pinId];
}
int CellLib_C::get_pin_id(string pinName){
    return _mPinName2Idx[pinName];
}
int CellLib_C::get_cell_width(int techId){
    return _techW[techId];
}
int CellLib_C::get_cell_height(int techId){
    return _techH[techId];
}
Pos CellLib_C::get_pin_offset(int techId,int pinId){
    return _vTechLibPinOffset[techId][pinId];
}
//-----------------------------------------------------------------------------------------------------//
Row_C::Row_C(){}
Row_C::Row_C(int id,int width,int height){
    _id=id;
    _width = width;
    _height = height;
}
Row_C::Row_C(int id,int width,int height, Die_C* die){
    _id=id;
    _width = width;
    _height = height;
    _die = die;
}
//-----------------------------------------------------------------------------------------------------//
Die_C::Die_C(){}
Die_C::Die_C(int sizeX, int sizeY, int maxUtil, int techId, int rowHeight){
    _id = -1;
    _techId = techId;
    _sizeX = sizeX;
    _sizeY = sizeY;
    _maxUtil = maxUtil / 100.0;
    _rowHeight = rowHeight;
    _vRows.resize(_sizeY/rowHeight);
    for(int i=0;i<_vRows.size();++i){
        _vRows[i] = new Row_C(i, sizeX, sizeY, this);
    }
}
Die_C::Die_C(int id, int sizeX, int sizeY, int maxUtil, int techId, int rowHeight){
    _id = id;
    _techId = techId;
    _sizeX = sizeX;
    _sizeY = sizeY;
    _maxUtil = maxUtil / 100.0;
    _rowHeight = rowHeight;
    _vRows.resize(_sizeY/rowHeight);
    for(int i=0;i<_vRows.size();++i){
        _vRows[i] = new Row_C(i, sizeX, sizeY, this);
    }
}
void Die_C::add_cell(Cell_C* cell){
    _sCells.insert(cell);
}
void Die_C::remove_cell(Cell_C * cell){
    //_vCells.erase(std::remove(_vCells.begin(), _vCells.end(), cell), _vCells.end());
    _sCells.erase(_sCells.find(cell));
}
int Die_C::get_id(){
    return _id;
}
int Die_C::get_techId(){
    return _techId;
}
double Die_C::get_max_util(){
    return _maxUtil;
}
int Die_C::get_row_height(){
    return _rowHeight;
}
int Die_C::get_width(){
    return _sizeX;
}
int Die_C::get_height(){
    return _sizeY;
}
int Die_C::get_row_num(){
    return _vRows.size();
}
unordered_set<Cell_C*>& Die_C::get_cells(){
    return _sCells;
}
//-----------------------------------------------------------------------------------------------------//
Pin_C::Pin_C(int id, Cell_C *cell){
    _id = id;
    _cell = cell;
    _net = nullptr;
}
void Pin_C::set_net(Net_C * net){
    _net = net;
}
int Pin_C::get_id(){
    return _id;
}
string Pin_C::get_name(){
    return _cell->get_master_cell()->get_pin_name(_id);
}
Pos Pin_C::get_pos(){
    Pos cellPos = _cell->get_pos();
    Pos pinOffset = _cell->get_master_cell()->get_pin_offset(_cell->get_die_techId(), _id);
    return Pos(cellPos.z, cellPos.x+pinOffset.x, cellPos.y+pinOffset.y);
}
int Pin_C::get_x(){
    Pos cellPos = _cell->get_pos();
    Pos pinOffset = _cell->get_master_cell()->get_pin_offset(_cell->get_die_techId(), _id);
    return cellPos.x+pinOffset.x;
}
int Pin_C::get_y(){
    Pos cellPos = _cell->get_pos();
    Pos pinOffset = _cell->get_master_cell()->get_pin_offset(_cell->get_die_techId(), _id);
    return cellPos.y+pinOffset.y;
}
Net_C* Pin_C::get_net(){
    return _net;
}
Cell_C* Pin_C::get_cell(){
    return _cell;
}
//-----------------------------------------------------------------------------------------------------//
Net_C::Net_C(){}
Net_C::Net_C(string name){
    _name = name;
    _vll.resize(2,Pos(0,0));
    _vur.resize(2,Pos(0,0));
    _vPinInEachDie.resize(2,unordered_set<Pin_C*>());
    _ballPos = Pos(0,0);
}
void Net_C::update_bbox(){
    _vll.clear();
    _vur.clear();
    _vll.resize(2,Pos(INT_MAX,INT_MAX));
    _vur.resize(2,Pos(0,0));
    if(is_cross_net()){
        // update boundary with ball
        for(int dieId=0;dieId<2;++dieId){
            _vll[dieId].x = min(_vll[dieId].x, _ballPos.x);
            _vll[dieId].y = min(_vll[dieId].y, _ballPos.y);
            _vur[dieId].x = max(_vur[dieId].x, _ballPos.x);
            _vur[dieId].y = max(_vur[dieId].y, _ballPos.y);
        }
    }
    else{
        for(int dieId=0;dieId<2;++dieId){
            if(_vPinInEachDie[dieId].size()==0){
                _vll[dieId] = Pos(0,0);
                _vur[dieId] = Pos(0,0);
            }
        }
    }
    // update boundary with pin
    for(Pin_C* pin : _vPins){
        int dieId = pin->get_cell()->get_dieId();
        _vll[dieId].x = min(_vll[dieId].x, pin->get_x());
        _vll[dieId].y = min(_vll[dieId].y, pin->get_y());
        _vur[dieId].x = max(_vur[dieId].x, pin->get_x());
        _vur[dieId].y = max(_vur[dieId].y, pin->get_y());
    }
}

void Net_C::update_bbox_noball(){
    _vll.clear();
    _vur.clear();
    _vll.resize(2,Pos(INT_MAX,INT_MAX));
    _vur.resize(2,Pos(0,0));
    for(int dieId=0;dieId<2;++dieId){
        if(_vPinInEachDie[dieId].size()==0){
            _vll[dieId] = Pos(0,0);
            _vur[dieId] = Pos(0,0);
        }
    }
    
    // update boundary with pin
    for(Pin_C* pin : _vPins){
        int dieId = pin->get_cell()->get_dieId();
        _vll[dieId].x = min(_vll[dieId].x, pin->get_x());
        _vll[dieId].y = min(_vll[dieId].y, pin->get_y());
        _vur[dieId].x = max(_vur[dieId].x, pin->get_x());
        _vur[dieId].y = max(_vur[dieId].y, pin->get_y());
    }
}

void Net_C::set_id(int id){
    _id = id;
}
void Net_C::set_ball_xy(Pos pos){
    _ballPos = pos;
}
void Net_C::add_pin(Pin_C* pin){
    _vPins.emplace_back(pin);
    pin->set_net(this);
    _vPinInEachDie[pin->get_cell()->get_dieId()].insert(pin);
}
string Net_C::get_name(){
    return _name;
}
int Net_C::get_id(){
    return _id;
}
int Net_C::get_HPWL(int dieId){
    return abs(_vur[dieId].x - _vll[dieId].x) + abs(_vur[dieId].y - _vll[dieId].y);
}
int Net_C::get_total_HPWL(){
    return get_HPWL(0) + get_HPWL(1);
}
Pos Net_C::get_ll(int dieId){
    return _vll[dieId];
}
Pos Net_C::get_ur(int dieId){
    return _vur[dieId];
}
int Net_C::get_pin_num(){
    return _vPins.size();
}
Pin_C* Net_C::get_pin(int i){
    return _vPins[i];
}
vector<Pin_C*>& Net_C::get_pins(){
    return _vPins;
}
Pos Net_C::get_ball_pos(){
    return _ballPos;
}
void Net_C::update_pin_die(Pin_C* pin, int from, int to){
    _vPinInEachDie[from].erase(pin);
    _vPinInEachDie[to].insert(pin);
}
bool Net_C::is_cross_net(){
    return (_vPinInEachDie[0].size() != 0 && _vPinInEachDie[1].size() != 0);
}
//-----------------------------------------------------------------------------------------------------//
Cell_C::Cell_C(){}
Cell_C::Cell_C(string name, CellLib_C* masterCell){
    _name = name;
    _masterCell = masterCell;
    for(int i=0;i<masterCell->get_pin_num();++i){
        Pin_C* pin = new Pin_C(i,this);
        _vPins.emplace_back(pin);
    }
    _pos = Pos(0,0,0);
    _die = nullptr;
    _dieId = 0;
    _degree = 0;
}
void Cell_C::set_id(int id){
    _id = id;
}
void Cell_C::set_pos(Pos pos){
    if(_pos.z != pos.z){ // change die
        for(Pin_C* pin : _vPins){ // change pin_die in net
            Net_C* net = pin->get_net();
            if(net != nullptr){
                net->update_pin_die(pin, _pos.z, pos.z);
            }
        }
    }
    _pos = pos;
    _dieId = pos.z;
}
void Cell_C::set_xy(Pos pos){
    _pos.x = pos.x;
    _pos.y = pos.y;
}
void Cell_C::set_die_quick(Die_C* die){
    die->add_cell(this);
    _dieId = die->get_id();
    set_pos(Pos(_dieId, _pos.x, _pos.y));
    _die = die;
}
void Cell_C::set_die(Die_C* die){
    if(_die != nullptr && _die->get_id() != die->get_id()){// change die
        _die->remove_cell(this);
        die->add_cell(this);
    } else if(_die == nullptr){
        die->add_cell(this);
    }
    _dieId = die->get_id();
    set_pos(Pos(_dieId, _pos.x, _pos.y));
    _die = die;
}
void Cell_C::set_degree(int degree){
    _degree = degree;
}
string Cell_C::get_name(){
    return _name;
}
int Cell_C::get_id(){
    return _id;
}
int Cell_C::get_width(){
    assert(_die != nullptr);
    return _masterCell->get_cell_width(_die->get_techId());
}
int Cell_C::get_height(){
    assert(_die != nullptr);
    return _masterCell->get_cell_height(_die->get_techId()); 
}
int Cell_C::get_width(int techId){
    return _masterCell->get_cell_width(techId);
}
int Cell_C::get_height(int techId){
    return _masterCell->get_cell_height(techId);
}
int Cell_C::get_pin_num(){
    return _vPins.size();
}
Pin_C* Cell_C::get_pin(int pinId){
    return _vPins[pinId];
}
Pin_C* Cell_C::get_pin(string pinName){
    return _vPins[_masterCell->get_pin_id(pinName)];
}
vector<Pin_C*>& Cell_C::get_pins(){
    return _vPins;
}
Pos Cell_C::get_pos(){
    _pos.z = _dieId;
    return _pos;
}
int Cell_C::get_posX(){
    return _pos.x;
}
int Cell_C::get_posY(){
    return _pos.y;
}
int Cell_C::get_posZ(){
    _pos.z = _dieId;
    return _pos.z;
}
CellLib_C* Cell_C::get_master_cell(){
    return _masterCell;
}
int Cell_C::get_die_techId(){
    assert(_die != nullptr);
    return _die->get_techId();
}
int Cell_C::get_dieId(){
    return _dieId;
}
bool Cell_C::check_drc(){
    // check die assignment
    if(_die == nullptr || (_dieId != 1 && _dieId != 0))
        return false;
    // check row overlaping
    if(_pos.y % _die->get_row_height() != 0)
        return false;
    // check if in die
    if(_pos.x < 0 || _pos.x+get_width() > _die->get_width() || _pos.y < 0 || _pos.y+get_height() > _die->get_height())
        return false;
    return true;
}
int Cell_C::get_degree(){
    return _degree;
}
vector<double>& Cell_C::get_GnnFeatures(){
    return _vGnnFeatures;
}
void Cell_C::setGnnFeatures(vector<double> v_feature){
    _vGnnFeatures = v_feature;
}
//-----------------------------------------------------------------------------------------------------//
Chip_C::Chip_C(){}
Chip_C::Chip_C(int sizeX, int sizeY, int dieNum){
    _sizeX = sizeX;
    _sizeY = sizeY;
    _vDie.resize(dieNum);
}
void Chip_C::set_chip_size(int sizeX, int sizeY){
    _sizeX = sizeX;
    _sizeY = sizeY;
}
void Chip_C::set_die(int dieId, int maxUtil, int techId, int rowHeight){
    _vDie[dieId] = new Die_C(dieId, _sizeX, _sizeY, maxUtil, techId, rowHeight);
}
void Chip_C::set_ball(int ballSizeX, int ballSizeY, int ballSpace){
    _ballSizeX = ballSizeX;
    _ballSizeY = ballSizeY;
    _ballSpace = ballSpace;
    _maxBallNum = ((_sizeX-_ballSpace) / (_ballSizeX+_ballSpace)) * ((_sizeY-_ballSpace) / (_ballSizeY+_ballSpace));
}
int Chip_C::get_die_num(){
    return _vDie.size();
}
Die_C* Chip_C::get_die(int dieId){
    return _vDie[dieId];
}
int Chip_C::get_width(){
    return _sizeX;
}
int Chip_C::get_height(){
    return _sizeY;
}
int Chip_C::get_ball_width(){
    return _ballSizeX;
}
int Chip_C::get_ball_height(){
    return _ballSizeY;
}
int Chip_C::get_ball_spacing(){
    return _ballSpace;
}
vector<Net_C*>& Chip_C::get_d2d_nets(){
    return _vD2DNets;
}
int Chip_C::get_max_ball_num(){
    return _maxBallNum;
}
//-----------------------------------------------------------------------------------------------------//
Design_C::Design_C(){
    _maxNetDegree = -1;
    _minNetDegree = INT_MAX;
    _maxCellDegree = -1;
    _minCellDegree = INT_MAX;
}
void Design_C::add_cell(Cell_C* cell){
    cell->set_id(_vCells.size());
    _mCells.emplace(cell->get_name(),cell);
    _vCells.emplace_back(cell);
}
void Design_C::add_net(Net_C* net){
    net->set_id(_vNets.size());
    _mNets.emplace(net->get_name(),net);
    _vNets.emplace_back(net);
    _maxNetDegree = max(_maxNetDegree, net->get_pin_num());
    _minNetDegree = min(_minNetDegree, net->get_pin_num());
}
void Design_C::set_cell_degree(){
    for(Cell_C* cell : _vCells){
        int cellDegree = 0;
        for(int i=0;i<cell->get_pin_num();++i){
            if(cell->get_pin(i)->get_net() != nullptr) cellDegree++;
        }
        cell->set_degree(cellDegree);
        _maxCellDegree = max(_maxCellDegree, cell->get_degree());
        _minCellDegree = min(_minCellDegree, cell->get_degree());
    }
}
int Design_C::get_cell_num(){
    return _vCells.size();
}
int Design_C::get_net_num(){
    return _vNets.size();
}
Cell_C* Design_C::get_cell(string cellName){
    return _mCells[cellName];
}
Cell_C* Design_C::get_cell(int cellId){
    return _vCells[cellId];
}
Net_C* Design_C::get_net(string netName){
    return _mNets[netName];
}
Net_C* Design_C::get_net(int netId){
    return _vNets[netId];
}
vector<Cell_C*>& Design_C::get_cells(){
    return _vCells;
}
vector<Net_C*>& Design_C::get_nets(){
    return _vNets;
}
unordered_map<string,Cell_C*>& Design_C::get_cells_map(){
    return _mCells;
}
unordered_map<string,Net_C*>& Design_C::get_nets_map(){
    return _mNets;
}
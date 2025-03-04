#include "drawHtml.h"
#include <cstdlib>

//string pos2str(drawPos pos) {return "("+to_string(get<0>(pos))+","+to_string(get<1>(pos))+")";}
Drawer_C::Drawer_C(){}
Drawer_C::Drawer_C(string s){
    fileName = s;
    string cmd = "rm -f ";
    cmd += fileName;
    system(cmd.c_str());
}

// svg
void Drawer_C::start_svg(){
    string cmd = "cat " + fileName_head + " > " + fileName;
    system(cmd.c_str());
    fout.open(fileName.c_str(), ofstream::out | ofstream::app);
    fout << "<svg class=\"layout\" height=\"" <<outline_y  << "\" width=\"" << outline_x  << "\">\n";
}
void Drawer_C::end_svg(){
    fout << "</svg>\n";
    fout.close();
    string cmd = "cat " + fileName_tail + " >> " + fileName;
    system(cmd.c_str());
}

void Drawer_C::setting(double p_outline_x,double p_outline_y,double p_scaling,double p_offset_x,double p_offset_y){
    outline_x = p_outline_x * p_scaling + p_offset_x;
    outline_y = p_outline_y * p_scaling + p_offset_y;
    scaling = p_scaling;
    offset_x = p_offset_x;
    offset_y = p_offset_y;
}
void Drawer_C::drawRect(string name, drawBox box, string color){
    drawRect(name,box,color,0.1);
}
void Drawer_C::drawRect(string name, drawBox box, int* rgb){
    drawRect(name,box,rgb,0.1);
}
void Drawer_C::drawRect(string name, drawBox box, int* rgb, double opacity){
    drawRect(name, box, rgb, opacity, map<string,string>());
}
void Drawer_C::drawRect(string name, drawBox box, string color, double opacity){
    drawRect(name, box, color, opacity, map<string,string>());
}
void Drawer_C::drawRect(string name, drawBox box, int* rgb, double opacity, map<string,string> m_para){
    double w_ori = get<0>(get<1>(box)) - get<0>(get<0>(box));
    double h_ori = get<1>(get<1>(box)) - get<1>(get<0>(box));
    
    double w = w_ori*scaling;
    double h = h_ori*scaling;
    double x = get<0>(get<0>(box)) * scaling + offset_x;
    double y = outline_y - (get<1>(get<0>(box)) * scaling + offset_y) - h;
    double stroke_width = (scaling>=1) ? 1.0:scaling;
    // basic para
    fout << "   <rect name=\""<< name << "\" x=\"" << x << "\" y=\"" << y 
        << "\" width=\"" << w << "\" height=\"" << h << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"fill:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke:red;stroke-width:"<< stroke_width << ";fill-opacity:" << opacity << ";stroke-opacity:0.9\" />\n";
}
void Drawer_C::drawRect(string name, drawBox box, string color, double opacity, map<string,string> m_para){
    double w_ori = get<0>(get<1>(box)) - get<0>(get<0>(box));
    double h_ori = get<1>(get<1>(box)) - get<1>(get<0>(box));
    
    double w = w_ori*scaling;
    double h = h_ori*scaling;
    double x = get<0>(get<0>(box)) * scaling + offset_x;
    double y = outline_y - (get<1>(get<0>(box)) * scaling + offset_y) - h;
    double stroke_width = (scaling>=1) ? 1.0:scaling;
    fout << "   <rect name=\""<< name << "\" x=\"" << x << "\" y=\"" << y 
        << "\" width=\"" << w << "\" height=\"" << h << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"fill:"<< color << ";stroke:red;stroke-width:"<< stroke_width << ";fill-opacity:" << opacity << ";stroke-opacity:0.9\" />\n";
}
void Drawer_C::drawBBox(string name, drawBox box, int* rgb, double width, double opacity, map<string,string> m_para){
    double w_ori = get<0>(get<1>(box)) - get<0>(get<0>(box));
    double h_ori = get<1>(get<1>(box)) - get<1>(get<0>(box));
    
    double w = w_ori*scaling;
    double h = h_ori*scaling;
    double x = get<0>(get<0>(box)) * scaling + offset_x;
    double y = outline_y - (get<1>(get<0>(box)) * scaling + offset_y) - h;
    width = width*scaling;
    fout << "   <rect name=\""<< name << "\" x=\"" << x << "\" y=\"" << y 
        << "\" width=\"" << w << "\" height=\"" << h << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"fill:none;stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:"<< width << ";fill-opacity:" << opacity << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawBBox(string name, drawBox box, int* rgb, double width, double opacity, double minW, map<string,string> m_para){
    double w_ori = get<0>(get<1>(box)) - get<0>(get<0>(box));
    double h_ori = get<1>(get<1>(box)) - get<1>(get<0>(box));
    
    double w = w_ori*scaling;
    double h = h_ori*scaling;
    double x = get<0>(get<0>(box)) * scaling + offset_x;
    double y = outline_y - (get<1>(get<0>(box)) * scaling + offset_y) - h;
    width = max(width*scaling, minW);
    fout << "   <rect name=\""<< name << "\" x=\"" << x << "\" y=\"" << y 
        << "\" width=\"" << w << "\" height=\"" << h << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"fill:none;stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:"<< width << ";fill-opacity:" << opacity << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, string color, double width, double opacity, map<string,string> m_para){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line name=\""<< name << "\" x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"stroke:" << color << ";stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, int* rgb, double width, double opacity, map<string,string> m_para){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line name=\""<< name << "\" x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\"";
    // other para
    for(auto para : m_para){
        fout << " " << para.first << "=\"" << para.second << "\"";
    }
    // styles
    fout << " style=\"stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, string color, double width, double opacity){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line name=\""<< name << "\" x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:" << color << ";stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, int* rgb, double width, double opacity){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:" << w << ";stroke-opacity:" << opacity << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, string color, double width){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line name=\""<< name << "\" x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:" << color << ";stroke-width:" << w << "\" />\n";
}
void Drawer_C::drawLine(string name, drawPos p1, drawPos p2, int* rgb, double width){
    double x1 = get<0>(p1) * scaling + offset_x;
    double y1 = outline_y - (get<1>(p1) * scaling + offset_y);
    double x2 = get<0>(p2) * scaling + offset_x;
    double y2 = outline_y - (get<1>(p2) * scaling + offset_y);
    double w = width * scaling;
    fout << "<line name=\""<< name << "\" x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" style=\"stroke:rgb(" << rgb[0] <<","<< rgb[1] <<"," << rgb[2] << ");stroke-width:" << w << "\" />\n";
}
void Drawer_C::drawText(string name, drawPos pos, double size, string str){
    double x = get<0>(pos) * scaling + offset_x;
    double y = outline_y - (get<1>(pos) * scaling + offset_y);
    size *= scaling;
    fout << "   <text name=\""<< name << "\" x=\"" << x << "\" y=\"" << y << "\" font-size=\""<< size <<"px\" fill=\"black\">" << str << "</text>\n";
} 
void Drawer_C::drawText(string name, drawPos pos, double size, string str, double offx, double offy){
    double x = get<0>(pos) * scaling + offx + offset_x;
    double y = outline_y - (get<1>(pos) * scaling + offy + offset_y);
    //size *= scaling;
    fout << "   <text name=\""<< name << "\" x=\"" << x << "\" y=\"" << y << "\" font-size=\""<< size <<"px\" fill=\"black\">" << str << "</text>\n";
} 
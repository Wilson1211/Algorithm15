// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graph.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include "../lib/tm_usage.h"
using namespace std;
using namespace CommonNs;
char G[30];

TestCmd::TestCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("test");
    optMgr_.setDes("test");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

TestCmd::~TestCmd() {}

bool TestCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getParsedOpt("s")) {
        printf("%s\n", optMgr_.getParsedValue("s"));
    }
    else
        printf("hello world !!\n");


    return true;
}



//read_graph
ReadCmd::ReadCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Read the graph in dot format");
    optMgr_.setDes("Read the graph in dot format. Establish nodes and edges.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    //read_graph don't need additional flag
/*
    opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
    opt->addFlag("s");
    optMgr_.regOpt(opt);*/
}

ReadCmd::~ReadCmd() {}

bool ReadCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getNParsedArg() < 1) {
        fprintf(stderr, "**ERROR ReadCmd::exec(): need input file\n");
        return false;
    }
    
    if(graph_ != 0) {
        delete graph_;
        graph_ = 0;
    }
    string name = argv[1];
    int t=0;
    int s = name.size();
    while(name[s-t-1] != '/' && (s-t-1)>=0 ) {t++;};
    if(t>0)
        name = name.substr(size_t(s-t), t); //cout << name <<endl;
    else
        cout << "**ERROR ReadCmd::exec(): open file failed\n";

    graph_ = new Graph(name) ;

    if(!graph_->readFile(argv[1])) {
        cout << "**ERROR ReadCmd::exec(): open file failed\n";
    }
    return true;
}


/********************DFS*********************/
DFSCmd::DFSCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Do DFS from the source node.");
    optMgr_.setDes("Perform depth first search starting from the source node. Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "start dfs from the source node. Default is v0",
                  "node number. EX: v0");
    opt->addFlag("s");
    optMgr_.regOpt(opt);

     opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <intput>_dfs.dot",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);
}

DFSCmd::~DFSCmd() {}

bool DFSCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if(graph_ == 0) {
        fprintf(stderr, "**ERROR DFSCmd::exec(): need read file before doing searching\n");
        return false;
    }

    //parse source node
    string srcnode = "v0";
    if (optMgr_.getParsedOpt("s"))
        srcnode = optMgr_.getParsedValue("s"); //cout << "srcnode: " << srcnode << endl;
    if(srcnode[0] != 'v') {
        fprintf(stderr, "**ERROR DFSCmd::exec(): wrong format of source node. <v0> \n");
        return false;
    }
    string srcnum = &srcnode[1];
    int src_num = atoi(srcnum.c_str()); //cout << "src_num: " << src_num << endl;

    //check source node existence
    if(graph_->shapesMap.find(src_num) == graph_->shapesMap.end()) {
        fprintf(stderr, "**ERROR DFSCmd::exec(): source node doesn't exist \n");
        return false;
    }

    string fname = graph_->name;
    if (optMgr_.getParsedOpt("o"))
        fname = optMgr_.getParsedValue("o");  //cout << "Output file name: " << fname << endl;
    else
        fname += "_dfs.dot" ;

    ofstream outfile(fname.c_str(), ios::out);
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    
    outfile << "// DFS tree produced by graphlab" << endl;
    outfile << "graph " << graph_->name << "_dfs {" << endl;
    
    Shape* src = graph_->shapesMap.find(src_num)->second; //cout << "DFS start" << endl;
    int e_counter=0;
    graph_->sortEdgesOfShape();

    //************************
    tmusg.periodStart();  
    graph_->DFS(src, outfile, e_counter);

    tmusg.getPeriodUsage(stat);
    //*************************
    
    outfile << "}" << endl;
    outfile << "// vertices = " << graph_->shapes.size() << endl;;
    outfile << "// edges = " << e_counter << endl;
    outfile << "// runtime = "<< (stat.uTime + stat.sTime) / 1000000.0 << " sec" << endl;
    outfile << "// memory = " << stat.vmPeak / 1000.0 << " MB" << endl;

    graph_->reset_travel();
    return true;
}


/********************BFS*********************/
BFSCmd::BFSCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Do BFS from the source node.");
    optMgr_.setDes("Perform breadth first search starting from the source node. Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "start bfs from the source node. Default is v0",
                  "node number. EX: v0");
    opt->addFlag("s");
    optMgr_.regOpt(opt);

     opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <intput>_bfs.dot",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);
}

BFSCmd::~BFSCmd() {}

bool BFSCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if(graph_ == 0) {
        fprintf(stderr, "**ERROR BFSCmd::exec(): need read file before doing searching\n");
        return false;
    }

    //parse source node
    string srcnode = "v0";
    if (optMgr_.getParsedOpt("s"))
        srcnode = optMgr_.getParsedValue("s"); //cout << "srcnode: " << srcnode << endl;
    if(srcnode[0] != 'v') {
        fprintf(stderr, "**ERROR BFSCmd::exec(): wrong format of source node. <v0> \n");
        return false;
    }
    string srcnum = &srcnode[1];
    int src_num = atoi(srcnum.c_str()); //cout << "src_num: " << src_num << endl;

    //check source node existence
    if(graph_->shapesMap.find(src_num) == graph_->shapesMap.end()) {
        fprintf(stderr, "**ERROR BFSCmd::exec(): source node doesn't exist \n");
        return false;
    }

    string fname = graph_->name;
    if (optMgr_.getParsedOpt("o"))
        fname = optMgr_.getParsedValue("o");  //cout << "Output file name: " << fname << endl;
    else 
        fname += "_bfs.dot" ;

    ofstream outfile(fname.c_str(), ios::out);
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    
    outfile << "// BFS tree produced by graphlab" << endl;
    outfile << "graph " << graph_->name << "_bfs {" << endl;
    
    Shape* src = graph_->shapesMap.find(src_num)->second; //cout << "BFS start" << endl;
    int e_counter=0;
    queue<Shape*> list;
    graph_->sortEdgesOfShape();
       //************************
    tmusg.periodStart();  
    graph_->BFS(src, outfile, e_counter, list);

    tmusg.getPeriodUsage(stat);
    //*************************
    
    outfile << "}" << endl;
    outfile << "// vertices = " << graph_->shapes.size() << endl;;
    outfile << "// edges = " << e_counter << endl;
    outfile << "// runtime = "<< (stat.uTime + stat.sTime) / 1000000.0 << " sec" << endl;
    outfile << "// memory = " << stat.vmPeak / 1000.0 << " MB" << endl;

    graph_->reset_travel();
    return true;
}


/********************Color*********************/
ColorCmd::ColorCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Color the graph from the source node.");
    optMgr_.setDes("Color the graph from the source node. Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
/*
    opt = new Opt(Opt::STR_REQ,
                  "start coloring from the v0. Default is greedy",
                  "greedy algorithm");
    opt->addFlag("m");
    optMgr_.regOpt(opt);

     opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <intput>_color.dot",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);*/
}

ColorCmd::~ColorCmd() {}

bool ColorCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if(graph_ == 0) {
        fprintf(stderr, "**ERROR ColorCmd::exec(): need read file before doing searching\n");
        return false;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    //************************
    tmusg.periodStart();  
    graph_->Color();

    tmusg.getPeriodUsage(stat);
    //************************
    /*
    for(int i=0; i<graph_->edges.size();i++){
        outfile << "v" << graph_->edges[i]->shape[0]->_id << " -- v" << graph_->edges[i]->shape[1]->_id << ";" << endl;
    }

    graph_->sortShapesByID();
    for(int i=0; i < graph_->shapes.size(); i++) {
        if(graph_->shapes[i]->color  > colornum) colornum = graph_->shapes[i]->color ;
        outfile << "v" << graph_->shapes[i]->_id << "[ label = \"v" << graph_->shapes[i]->_id<< "_color" <<  graph_->shapes[i]->color << "\"];" << endl;
    }*/
    graph_->reset_travel();
    return true;
}


//link
LinkCmd::LinkCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Link shapes");
    optMgr_.setDes("Link shapes");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

}

LinkCmd::~LinkCmd() {}

bool LinkCmd::exec(int argc, char **argv) {
    //optMgr_.parse(argc, argv);
    //cout << "Hi" << endl;
    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

     if(graph_ == 0) {
        fprintf(stderr, "**ERROR ColorCmd::exec(): need to read file before doing linking\n");
        return false;
    }

    graph_->connect();





    return true;
}

//print
PrintCmd::PrintCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("Print graph info");
    optMgr_.setDes("Print graph info. print -[s|c]. default is -s");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "Print all shapes.",
                  "#shape<_id>  edges:");
    opt->addFlag("s");
    optMgr_.regOpt(opt);

}

PrintCmd::~PrintCmd() {}

bool PrintCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

     if(graph_ == 0) {
        fprintf(stderr, "**ERROR ColorCmd::exec(): need to read file before doing linking\n");
        return false;
    }

    //-s -c need fix
    if (optMgr_.getParsedOpt("s")) {
        graph_->printShapes();
        return true;
    }

    if (optMgr_.getParsedOpt("c")) {
        graph_->printColor();
        return true;
    }

    graph_->printShapes();

    return true;
}
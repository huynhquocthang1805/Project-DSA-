/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:28
 */

#ifndef IGRAPH_H
#define IGRAPH_H
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "list/DLinkedList.h"


// class VertexNotFoundException: public std::exception{
// private:
//     string vertex;
// public:
//     VertexNotFoundException(string vertex){
       
//         this->vertex = vertex;
//     }
//     const char * what () const throw (){
//         stringstream os;
//         os << "Vertex (" << this->vertex << "): is not found";
//         return os.str().c_str();
//     }
// };
class VertexNotFoundException: public std::exception {
private:
    string vertex;
    string message;
public:
    VertexNotFoundException(string vertex) {
        this->vertex = vertex;
        stringstream os;
        os << "Vertex (" << vertex << "): is not found";
        this->message = os.str();
    }
    const char* what() const throw() {
        return this->message.c_str();
    }
};
class EdgeNotFoundException: public std::exception {
private:
    string edge;
    string message;
public:
    EdgeNotFoundException(string edge) {
        this->edge = edge;
        stringstream os;
        os << "Edge (" << edge << "): is not found";
        this->message = os.str();
    }
    const char* what() const throw() {
        return this->message.c_str();
    }
};

template<class T>
struct Edge{
    T from, to;
    float weight;
    Edge(T from, T to, float weight=0){
        this->from = from;
        this->to = to;
        this->weight = weight;
    };
    Edge(const Edge& edge){
        this->from = edge.from;
        this->to = edge.to;
        this->weight = edge.weight;
    }
};
/*
 * IGraph: define APIs for a graph data structure
 *  >> T: type of vertices
 */
template<class T>
class IGraph{
public:
    virtual ~IGraph(){};
    virtual void add(T vertex)=0;
    virtual void remove(T vertex)=0;
    virtual bool contains(T vertex)=0;
    
    virtual void connect(T from, T to, float weight=0)=0;
    virtual void disconnect(T from, T to)=0;
    virtual bool connected(T from, T to)=0;
    virtual float weight(T from, T to)=0;
    
    
    virtual DLinkedList<T> getOutwardEdges(T from)=0;
    virtual DLinkedList<T> getInwardEdges(T to)=0;
    
    virtual int size()=0;
    virtual bool empty()=0;
    virtual void clear()=0;
    
    virtual int inDegree(T vertex)=0;
    virtual int outDegree(T vertex)=0;
    
    virtual DLinkedList<T> vertices()=0;
    
    
    virtual string toString()=0;
};

#endif /* IGRAPH_H */


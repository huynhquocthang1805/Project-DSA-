/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
    using VertexNode = typename AbstractGraph<T>::VertexNode; // Định nghĩa kiểu VertexNode
    using EdgeNode = typename AbstractGraph<T>::Edge;

public:
    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);
        if (!fromNode || !toNode)
        {
            typename AbstractGraph<T>::VertexNode rs = typename AbstractGraph<T>::VertexNode(!fromNode ? from : to, this->vertexEQ, this->vertex2str);
            throw VertexNotFoundException(AbstractGraph<T>::vertex2Str(rs));
        }
        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        // auto *fromNode = this->getVertexNode(from);
        // auto *toNode = this->getVertexNode(to);
        // if (!fromNode || !toNode)
        // {
        //     typename AbstractGraph<T>::VertexNode rs = typename AbstractGraph<T>::VertexNode(!fromNode ? from : to, this->vertexEQ, this->vertex2str);
        //     throw VertexNotFoundException(AbstractGraph<T>::vertex2Str(rs));
        // }

        // fromNode->removeTo(toNode);

        if (this->getVertexNode(from) == 0)
            throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to) == 0)
            throw VertexNotFoundException(this->vertex2str(to));
        if (this->connected(from, to) == false)
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");
        typename AbstractGraph<T>::Edge *edge = this->getVertexNode(from)->getEdge(this->getVertexNode(to));
        this->getVertexNode(from)->removeTo(this->getVertexNode(to));
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *vertexNode = this->getVertexNode(vertex);
        if (!vertexNode)
        {
            typename AbstractGraph<T>::VertexNode rs = typename AbstractGraph<T>::VertexNode(vertex, this->vertexEQ, this->vertex2str);
            throw VertexNotFoundException(AbstractGraph<T>::vertex2Str(rs));
        }
        DLinkedList<T> outEdges = this->AbstractGraph<T>::getOutwardEdges(vertex);
        for (auto it = outEdges.begin(); it != outEdges.end(); ++it)
        {
            typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(*it);
            vertexNode->removeTo(toNode);
        }
        DLinkedList<T> inEdges = this->AbstractGraph<T>::getInwardEdges(vertex);
        for (auto it = inEdges.begin(); it != inEdges.end(); ++it)
        {
            typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(*it);
            fromNode->removeTo(vertexNode);
        }
        this->nodeList.removeItem(vertexNode);
        delete vertexNode;
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        auto *graph = new DGraphModel<T>(vertexEQ, vertex2str);

        // Thêm các đỉnh
        for (int i = 0; i < nvertices; ++i)
        {
            graph->add(vertices[i]);
        }

        // Thêm các cạnh
        for (int i = 0; i < nedges; ++i)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */

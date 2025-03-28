/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        // auto fromNode = this->getVertexNode(from);
        // auto toNode = this->getVertexNode(to);

        // if (!fromNode || !toNode)
        // {
        //     typename AbstractGraph<T>::VertexNode rs = typename AbstractGraph<T>::VertexNode(!fromNode ? from : to, this->vertexEQ, this->vertex2str);
        //     throw VertexNotFoundException(AbstractGraph<T>::vertex2Str(rs));
        //     // throw VertexNotFoundException(this->vertex2str(!fromNode ? from : to));
        // }

        // if (this->vertexEQ(from, to) == true)
        // {
        //     // Self-loop
        //     fromNode->connect(fromNode, weight);
        // }
        // else
        // {
        //     fromNode->connect(toNode, weight);
        //     toNode->connect(fromNode, weight);
        // }

        if (this->getVertexNode(from) == 0)
            throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to) == 0)
            throw VertexNotFoundException(this->vertex2str(to));
        this->getVertexNode(from)->connect(this->getVertexNode(to), weight);
        if (this->vertexEQ(from, to) == false)
            this->getVertexNode(to)->connect(this->getVertexNode(from), weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        auto fromNode = this->getVertexNode(from);
        auto toNode = this->getVertexNode(to);

        // if (fromNode == nullptr || toNode == nullptr)
        // {
        //     typename AbstractGraph<T>::VertexNode rs = typename AbstractGraph<T>::VertexNode(!fromNode ? from : to, this->vertexEQ, this->vertex2str);
        //     throw VertexNotFoundException(this->vertex2str(from));
        // }
        if (this->getVertexNode(from) == 0)
            throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to) == 0)
            throw VertexNotFoundException(this->vertex2str(to));
        if (this->connected(from, to) == false)
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");
        if (this->connected(to, from) == false)
            throw EdgeNotFoundException("E(" + this->vertex2str(to) + "," + this->vertex2str(from) + ")");
        if (fromNode == toNode)
        {
            fromNode->removeTo(fromNode);
        }
        else
        {
            fromNode->removeTo(toNode);
            toNode->removeTo(fromNode);
        }
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
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        {
            UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

            // Add vertices
            for (int i = 0; i < nvertices; ++i)
            {
                graph->add(vertices[i]);
            }

            // Add edges
            for (int i = 0; i < nedges; ++i)
            {
                graph->connect(edges[i].from, edges[i].to, edges[i].weight);
            }

            return graph;
        }
    }
};
#endif /* UGRAPHMODEL_H */

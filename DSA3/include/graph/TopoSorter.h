// /*
//  * To change this license header, choose License Headers in Project Properties.
//  * To change this template file, choose Tools | Templates
//  * and open the template in the editor.
//  */

// /*
//  * File:   TopoSorter.h
//  * Author: ltsach
//  *
//  * Created on July 11, 2021, 10:21 PM
//  */

// #ifndef TOPOSORTER_H
// #define TOPOSORTER_H
// #include "graph/DGraphModel.h"
// #include "list/DLinkedList.h"
// #include "sorting/DLinkedListSE.h"
// #include "hash/xMap.h"
// #include "stacknqueue/Stack.h"
// template <class T>
// class TopoSorter
// {
// public:
//     static int DFS;
//     static int BFS;

// protected:
//     DGraphModel<T> *graph;
//     int (*hash_code)(T &, int);

// public:
//     TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
//     {
//         // TODO
//         this->graph = graph;
//         this->hash_code = (hash_code != nullptr) ? hash_code : nullptr;
//     }
//     DLinkedListSE<T> sort(int mode = 0, bool sorted = true)
//     {
//         // TODO
//         if (mode == BFS)
//         {
//             return bfsSort(sorted);
//         }
//         else
//         {
//             return dfsSort(sorted);
//         }  else {
// return DLinkedList<T>();
// }
//     }
//     DLinkedListSE<T> bfsSort(bool sorted = true)
//     {
//         // TODO
//         DLinkedListSE<T> result;
//         xMap<T, int> inDegree = vertex2inDegree(this->hash_code);
//         DLinkedListSE<T> zeroInDegrees = listOfZeroInDegrees();
// Queue<T> queue;
// DLinkedList<T> result;

//         if (sorted)
//         {
//             zeroInDegrees.sort(); // Sắp xếp danh sách các đỉnh có in-degree bằng 0 theo thứ tự tăng dần
//         }

//         while (!zeroInDegrees.empty())
//         {
//             T vertex = zeroInDegrees.removeAt(0); // Lấy đỉnh có in-degree = 0
//             result.add(vertex);

//             auto neighbors = this->graph->getOutwardEdges(vertex);
//             DLinkedListSE<T> neighborList;
//             for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); ++neighbor)
//             {
//                 neighborList.add(*neighbor);
//             }

//             if (sorted)
//             {
//                 neighborList.sort(); // Sắp xếp danh sách các đỉnh kề nếu cần
//             }

//             for (auto neighbor : neighborList)
//             {
//                 inDegree.get(neighbor) -= 1;
//                 if (inDegree.get(neighbor) == 0)
//                 {
//                     zeroInDegrees.add(neighbor);
//                     if (sorted)
//                     {
//                         zeroInDegrees.sort();
//                     }
//                 }
//             }
//             // delete neighbors;
//         }
//         return result;
//     }

//     DLinkedListSE<T> dfsSort(bool sorted = true)
//     {
//         // TODO
//         // DLinkedList<T> result;
//         // xMap<T, bool> visited(this->hash_code, this->graph->vertexCount());

//         // for (auto vertex : *this->graph->vertices())
//         // {
//         //     if (!visited[vertex])
//         //     {
//         //         dfsHelper(vertex, visited, result, sorted);
//         //     }
//         // }

//         // result.reverse(); // Kết quả của DFS cần đảo ngược
//         // return result;
//         DLinkedListSE<T> sortedList;

//         // Tạo danh sách các đỉnh từ đồ thị và sắp xếp nếu cần
//         for (auto it = graph->begin(); it != graph->end(); it++)
//             sortedList.add(*it);
//         if (sorted)
//             sortedList.sort(&SortSimpleOrder<T>::compare4Ascending);

//         DLinkedList<T> result;  // Danh sách kết quả
//         DLinkedList<T> visited; // Danh sách đã thăm
//         Stack<T> tempStack;     // Ngăn xếp tạm để lưu thứ tự topo
//         DLinkedListSE<T> zeroindegrees = listOfZeroInDegrees();

//         // Sắp xếp lại danh sách zeroindegrees nếu cần
//         if (sorted)
//             zeroindegrees.sort(&SortSimpleOrder<T>::compare4Ascending);

//         // Hàm DFS đệ quy
//         auto dfs = [&](T vertex, auto &dfsRef) -> void
//         {
//             visited.add(vertex); // Đánh dấu đỉnh hiện tại đã thăm
//             for (int i = 0; i < sortedList.size(); i++)
//             { // Duyệt sortedList (thứ tự tăng dần)
//                 T neighbor = sortedList.get(i);
//                 if (graph->AbstractGraph<T>::connected(vertex, neighbor) && !visited.contains(neighbor))
//                 {
//                     dfsRef(neighbor, dfsRef); // Gọi đệ quy cho đỉnh kề
//                 }
//             }
//             tempStack.push(vertex); // Đưa đỉnh vào stack sau khi xử lý xong
//         };

//         // Thực hiện DFS trên các đỉnh có bậc vào bằng 0
//         for (int i = 0; i < zeroindegrees.size(); i++)
//         {
//             T vertex = zeroindegrees.get(i);
//             if (!visited.contains(vertex))
//             {
//                 dfs(vertex, dfs); // Bắt đầu DFS từ đỉnh chưa thăm
//             }
//         }

//         // Đưa các đỉnh từ stack vào kết quả
//         while (!tempStack.empty())
//         {
//             result.add(tempStack.pop());
//         }
//         return result;
//     }
//     void dfsHelper(T &vertex, xMap<T, bool> &visited, DLinkedListSE<T> &result, bool sorted)
//     {
//         visited[vertex] = true;

//         auto neighbors = this->graph->getOutwardEdges(vertex);
//         DLinkedListSE<T> neighborList;
//         for (auto neighbor : *neighbors)
//         {
//             neighborList.add(neighbor);
//         }

//         if (sorted)
//         {
//             neighborList.sort();
//         }

//         for (auto neighbor : neighborList)
//         {
//             if (!visited[neighbor])
//             {
//                 dfsHelper(neighbor, visited, result, sorted);
//             }
//         }
//         result.add(vertex);
//         delete neighbors;
//     }

// protected:
//     // Helper functions
//     xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
//     {
//         // Bảng băm để lưu in-degree của mỗi đỉnh
//         xMap<T, int> inDegree(hash, this->graph->size());

//         // Khởi tạo tất cả các đỉnh với in-degree = 0
//         for (auto vertex = this->graph->vertices().begin(); vertex != this->graph->vertices().end(); ++vertex)
//         {
//             inDegree[vertex] = 0;
//         }

//         // Duyệt qua tất cả các cạnh, tăng in-degree của đỉnh đích
//         for (auto vertex = this->graph->vertices().begin(); vertex != this->graph->vertices().end(); ++vertex)
//         {
//             auto neighbors = this->graph->getOutwardEdges(vertex);
//             for (auto neighbor : *neighbors)
//             {
//                 inDegree[neighbor] += 1;
//             }
//             // delete neighbors; // Giải phóng bộ nhớ của danh sách kề
//         }

//         return inDegree;
//     };
//     xMap<T, int> vertex2outDegree(int (*hash)(T &, int));
//     DLinkedList<T> listOfZeroInDegrees()
//     {
//         DLinkedList<T> zeroInDegrees;
//         xMap<T, int> inDegree = vertex2inDegree(this->hash_code);

//         for (auto vertex = this->graph->vertices().begin(); vertex != this->graph->vertices().end(); ++vertex)
//         {
//             if (inDegree[vertex] == 0)
//             {
//                 zeroInDegrees.add(vertex);
//             }
//         }

//         return zeroInDegrees;
//     };

// }; // TopoSorter

// template <class T>
// int TopoSorter<T>::DFS = 0;
// template <class T>
// int TopoSorter<T>::BFS = 1;

// /////////////////////////////End of TopoSorter//////////////////////////////////

// #endif /* TOPOSORTER_H */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "hash/xMap.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"

template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }
    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        // TODO
        if (mode == TopoSorter::DFS)
        {
            return this->dfsSort(sorted);
        }

        else if (mode == TopoSorter::BFS)
        {
            return this->bfsSort(sorted);
        }
        else
        {
            return DLinkedList<T>();
        }
    }
    DLinkedList<T> bfsSort(bool sorted = true)
    {
        // TODO
        xMap<T, int> inDegrees = this->vertex2inDegree(this->hash_code);
        Queue<T> queue;
        DLinkedList<T> result;

        DLinkedListSE<T> zeroInDegrees = this->listOfZeroInDegrees();

        // Sort
        if (sorted)
        {
            zeroInDegrees.sort();
        }
        for (auto vertex : zeroInDegrees)
        {
            queue.push(vertex);
        }
        while (!queue.empty())
        {
            T current = queue.pop();
            result.add(current);
            DLinkedListSE<T> neighbors = this->graph->getOutwardEdges(current);
            // cout << "neighbors: " << neighbors.toString() << endl;
            for (auto neighbor : neighbors)
            {
                inDegrees.put(neighbor, inDegrees.get(neighbor) - 1);
                if (inDegrees.get(neighbor) == 0)
                {
                    queue.push(neighbor);
                }
            }
        }

        return result;
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TODO
        DLinkedListSE<T> VerticesList = this->graph->vertices();
        xMap<T, int> visited = xMap<T, int>(this->hash_code);
        DLinkedList<T> result;
        Stack<T> stack;
        // Sort
        DLinkedListSE<T> zeroInDegrees = this->listOfZeroInDegrees();
        if (sorted)
        {
            zeroInDegrees.sort();
        }

        // initialize visited, all vertices are not visited
        for (auto vertex : VerticesList)
        {
            visited.put(vertex, 0);
        }

        // cout << "visited: " <<visited.toString() << endl;
        for (auto vetex : zeroInDegrees)
        {
            if (visited.get(vetex) == 0)
            {
                stack.push(vetex);
                visited.put(vetex, 1);
                while (!stack.empty())
                {
                    T current = stack.peek();
                    DLinkedListSE<T> neighbors = this->graph->getOutwardEdges(current);
                    bool hasUnvisited = false;
                    for (auto neighbor : neighbors)
                    {
                        if (visited.get(neighbor) == 0)
                        {
                            stack.push(neighbor);
                            visited.put(neighbor, 1);
                            hasUnvisited = true;
                            break;
                        }
                    }
                    if (!hasUnvisited)
                        result.add(0, stack.pop());
                }
            }
        }
        return result;
    }

protected:
    // Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        // TODO
        xMap<T, int> inDegrees(hash);
        DLinkedList<T> vertices = this->graph->vertices();

        for (auto vertex : vertices)
        {
            inDegrees.put(vertex, this->graph->inDegree(vertex));
        }

        return inDegrees;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T &, int));
    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> zeroInDegrees;
        DLinkedList<T> vertices = this->graph->vertices();

        for (auto vertex : vertices)
        {
            if (this->graph->inDegree(vertex) == 0)
            {
                zeroInDegrees.add(vertex);
            }
        }

        return zeroInDegrees;
    }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */

/*DLinkedList<T> dfsSort(bool sorted = true) {
    // Sử dụng hash_code để khởi tạo xMap
    xMap<T, bool> visited(this->hash_code);
    Stack<T> dfsStack; // Ngăn xếp để mô phỏng DFS
    DLinkedList<T> result; // Kết quả sắp xếp tô-pô

    // Khởi tạo trạng thái "chưa thăm" cho tất cả các đỉnh
    for (const T& vertex : graph->vertices()) {
        visited.put(vertex, false);
    }

    // Lặp qua tất cả các đỉnh trong đồ thị
    for (const T& startVertex : graph->vertices()) {
        if (!visited.get(startVertex)) {
            dfsStack.push(startVertex); // Đẩy đỉnh bắt đầu vào ngăn xếp

            while (!dfsStack.empty()) {
                T current = dfsStack.peek(); // Lấy đỉnh đầu ngăn xếp nhưng không xóa
                bool allNeighborsVisited = true;

                // Duyệt các đỉnh kề
                for (const T& neighbor : graph->getOutwardEdges(current)) {
                    if (!visited.get(neighbor)) {
                        dfsStack.push(neighbor); // Đẩy đỉnh kề chưa thăm vào ngăn xếp
                        allNeighborsVisited = false;
break; // Dừng vòng lặp để xử lý đỉnh kề
                    }
                }

                // Nếu tất cả các đỉnh kề đã được thăm
                if (allNeighborsVisited) {
                    dfsStack.pop(); // Xóa đỉnh khỏi ngăn xếp
                    if (!visited.get(current)) {
                        visited.put(current, true); // Đánh dấu đã thăm
                        result.add(current); // Thêm đỉnh vào kết quả
                    }
                }
            }
        }
    }

    // Nếu cần sắp xếp ngược (sorted = true)
    if (sorted) {
        DLinkedList<T> reversedResult; // Danh sách liên kết phụ để lưu kết quả đảo ngược
        typename DLinkedList<T>::BWDIterator it = result.bbegin(); // Bắt đầu từ cuối danh sách
        while (it != result.bend()) {
            reversedResult.add(*it); // Thêm phần tử từ cuối về đầu
            --it;
        }
        return reversedResult; // Trả về danh sách đã đảo ngược
    }

    return result;
}
*/
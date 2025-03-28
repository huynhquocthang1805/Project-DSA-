/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    class BWDIterator
    {
        // TODO implement
    private:
        DLinkedList<T> *pList; // Con trỏ đến danh sách DLinkedList
        Node *pNode;

    public:
        // 1. Constructor
        BWDIterator(DLinkedList<T> *pList, bool begin = true)
        {
            this->pList = pList;
            if (begin)
            {
                if (pList != nullptr)
                {
                    this->pNode = pList->tail->prev;
                }
                else
                {
                    this->pNode = nullptr;
                }
            }
            else
            {
                if (pList != 0)
                {
                    this->pNode = pList->head;
                }
                else
                    pNode = 0;
            }
        }

        // 2. Toán tử gán
        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode; // Sao chép con trỏ nút
            this->pList = iterator.pList; // Sao chép danh sách
            return *this;
        }

        // 3. Phương thức operator*
        T &operator*()
        {
            return pNode->data; // Trả về tham chiếu đến phần tử hiện tại
        }

        // 4. Toán tử so sánh
        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode; // So sánh con trỏ nút
        }

        // ++++++++++++++++++++
        BWDIterator &operator++()
        {
            pNode = pNode->prev;
            return *this;
        }

        //
        BWDIterator operator++(int)
        {
            BWDIterator temp = *this;
            ++(*this);
            return temp;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            if (pNode == nullptr || pNode == pList->head || pNode == pList->tail)
                return;
            Node *nodeToRemove = pNode; // Nút cần xóa
            pNode = pNode->next;

            // Cập nhật liên kết giữa các nút
            nodeToRemove->prev->next = nodeToRemove->next;

            nodeToRemove->next->prev = nodeToRemove->prev;

            if (removeItemData != nullptr)
                removeItemData(nodeToRemove->data); // Gọi hàm để xóa dữ liệu nếu có

            delete nodeToRemove; // Giải phóng bộ nhớ của nút
            pList->count--;      // Giảm số lượng phần tử trong danh sách
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;

    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    Node *current = list.head->next;
    while (current != list.tail)
    {
        this->add(current->data);
        current = current->next;
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    if (this == &list)
        return *this;

    this->clear();

    Node *current = list.head->next;
    while (current != list.tail)
    {
        Node *newnode = new Node(current->data);
        Node *current = this->tail->prev;
        newnode->prev = current;
        newnode->next = tail;
        current->next = newnode;
        tail->prev = newnode;
        this->count++;

        current = current->next;
    }

    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    this->removeInternalData();
}

template <class T>
void DLinkedList<T>::add(T e)
{
    Node *newnode = new Node(e);
    Node *current = this->tail->prev;
    newnode->prev = current;
    newnode->next = tail;
    current->next = newnode;
    tail->prev = newnode;
    this->count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > size())
        throw std::out_of_range("Index is out of range!");
    Node *current = head->next;
    if (index <= count / 2)
    {
        for (int i = 0; i < index; i++)
            current = current->next;
    }
    else
    {
        current = tail;
        for (int i = count; i > index; i--)
            current = current->prev;
    }
    this->count++;
    Node *newnode = new Node(e, current, current->prev);
    current->prev->next = newnode;
    current->prev = newnode;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    Node *current;
    if (index <= this->count / 2)
    {
        current = this->head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
    }

    else
    {
        current = this->tail->prev;
        for (int i = this->count - 1; i > index; i--)
            current = current->prev;
    }
    return current;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is out of range!");
    Node *current;
    if (index <= this->count / 2)
    {
        current = this->head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
    }

    else
    {
        current = this->tail->prev;
        for (int i = this->count - 1; i > index; i--)
            current = current->prev;
    }

    T removeddata = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    this->count--;
    return removeddata;
}

template <class T>
bool DLinkedList<T>::empty()
{
    if (this->count == 0)
        return true;
    return false;
}

template <class T>
int DLinkedList<T>::size()
{
    return this->count;
}

template <class T>
void DLinkedList<T>::clear()
{
    if (this->deleteUserData != nullptr)
        this->deleteUserData(this);
    Node *current = this->head->next;
    while (current != this->tail)
    {
        Node *nextnode = current->next;
        delete current;
        current = nextnode;
    }

    this->count = 0;
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    if (index < 0 || index >= size())
        throw std::out_of_range("Index is out of range!");
    Node *current;
    if (index <= this->count / 2)
    {
        current = this->head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
    }

    else
    {
        current = this->tail->prev;
        for (int i = this->count - 1; i > index; i--)
            current = current->prev;
    }
    return current->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    Node *current = this->head->next;
    int index = 0;
    while (current != this->tail)
    {
        if (equals(current->data, item, itemEqual))
            return index;
        index++;
        current = current->next;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int index = indexOf(item);
    if (index == -1)
        return false;
    else
    {
        T removedItem = removeAt(index);
        if (removeItemData != nullptr)
            removeItemData(removedItem);
        return true;
    }
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    Node *current = this->head->next;
    while (current != this->tail)
    {
        if (equals(current->data, item, itemEqual))
            return true;
        current = current->next;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    string result;
    result += "[";
    int index = 0;
    Node *current = this->head->next;
    while (current != this->tail)
    {
        if (item2str)
            result += item2str(current->data);
        else
        {
            stringstream ss;
            ss << current->data;
            result += ss.str();
        }
        if (index < count - 1)
            result += ", ";
        index++;
        current = current->next;
    }
    result += "]";
    return result;
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    Node *current = list.head->next;
    this->add(current->data);
    current = current->next;

    while (current != list.tail)
    {
        this->head->next = new Node(current->data);
        this->tail->prev = new Node();
        this->count++;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    if (this->deleteUserData != nullptr)
        this->deleteUserData(this);
    Node *current = this->head->next;
    while (current != this->tail)
    {
        Node *nextnode = current->next;

        delete current;
        current = nextnode;
    }

    delete head;
    delete tail;
    this->count = 0;
}

#endif /* DLINKEDLIST_H */

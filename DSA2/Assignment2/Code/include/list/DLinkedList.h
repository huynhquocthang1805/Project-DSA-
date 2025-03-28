/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;
    class Iterator;
    class BWDIterator;

protected:
    Node *head;
    Node *tail;
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);
    void (*deleteUserData)(DLinkedList<T> *);

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

    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }
    int getCount() const
    {
        return count;
    }
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

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
            Node *pNext = pNode->prev;
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
        DLinkedList<T> *pList;
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

        // 5. Toán tử tăng tiền tố (prefix)
        BWDIterator &operator++()
        {
            pNode = pNode->prev;
            return *this;
        }

        // 6. Toán tử tăng hậu tố (postfix)
        BWDIterator operator++(int)
        {
            BWDIterator temp = *this;
            ++(*this);
            return temp;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            if (pNode == nullptr || pNode == pList->head || pNode == pList->tail)
                return; // Không thể xóa nếu không có nút hợp lệ

            Node *nodeToRemove = pNode; // Nút cần xóa
            pNode = pNode->next;

            // Cập nhật liên kết giữa các nút
            nodeToRemove->prev->next = nodeToRemove->next;

            nodeToRemove->next->prev = nodeToRemove->prev;

            if (removeItemData != nullptr)
                removeItemData(nodeToRemove->data);

            delete nodeToRemove; // Giải phóng bộ nhớ của nút
            pList->count--;
        }
    };
};
//////////////////////////////////////////////////////////////////////

template <class T>
using List = DLinkedList<T>;
template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;

    // Tạo các nút sentinel head và tail (đầu và đuôi) để dễ dàng quản lý danh sách
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;

    this->count = 0;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    this->head = new Node(); // Nút sentinel đầu
    this->tail = new Node(); // Nút sentinel đuôi
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;

    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    // Sao chép các phần tử từ danh sách gốc sang danh sách mới
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
    // TODO
    if (this == &list)
    {
        return *this;
    }

    // Xóa dữ liệu hiện tại của danh sách để tránh rò rỉ bộ nhớ
    this->clear();

    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    // Sao chép các phần tử từ danh sách gốc sang đối tượng hiện tại
    Node *current = list.head->next;
    while (current != list.tail)
    {
        this->add(current->data);
        current = current->next;
    }

    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    clear();
    delete this->head;
    delete this->tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO implement
    Node *newNode = new Node(e);

    if (count == 0)
    {
        head->next = newNode;
        newNode->prev = head;
    }
    else
    {
        newNode->prev = tail->prev;
        tail->prev->next = newNode;
    }

    newNode->next = tail;
    tail->prev = newNode;
    count++;
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO implement
    if (index < 0 || index > count)
    {
        throw std::out_of_range("Index is out of range!");
    }

    if (index == count)
    {
        add(e);
        return;
    }

    Node *newNode = new Node(e);

    Node *current = getPreviousNodeOf(index)->next;

    newNode->prev = current->prev;
    newNode->next = current;
    current->prev->next = newNode;
    current->prev = newNode;

    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{

    // TODO
    Node *currentNode;

    if (index < this->count / 2)
    {
        currentNode = this->head;
        for (int i = 0; i <= index; ++i)
        {
            currentNode = currentNode->next;
        }
    }
    else
    {
        currentNode = this->tail;
        for (int i = this->count; i > index; --i)
        {
            currentNode = currentNode->prev;
        }
    }
    return currentNode->prev;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *nodeToRemove = getPreviousNodeOf(index)->next;
    T removedData = nodeToRemove->data;
    nodeToRemove->prev->next = nodeToRemove->next;
    nodeToRemove->next->prev = nodeToRemove->prev;

    delete nodeToRemove;

    count--;

    return removedData;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    if (count == 0)
    {
        return true;
    }
    else
        return false;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    Node *current = head->next;
    if (this->deleteUserData)
    {
        this->deleteUserData(this);
    }
    while (current != tail)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *current = getPreviousNodeOf(index)->next;
    return current->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    int index = 0;
    for (Node *current = head->next; current != tail; current = current->next)
    {
        if (equals(current->data, item, itemEqual))
        {
            return index;
        }
        index++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    if (this->head == nullptr || this->tail == nullptr)
    {
        return false;
    }

    for (Node *current = head->next; current != tail; current = current->next)
    {
        if (equals(current->data, item, itemEqual))
        {

            if (removeItemData != nullptr)
            {
                removeItemData(current->data);
            }

            current->prev->next = current->next;
            current->next->prev = current->prev;

            delete current;
            count--;
            return true;
        }
    }

    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    for (Node *current = head->next; current != tail; current = current->next)
    {

        if (equals(current->data, item, itemEqual))
        {
            return true;
        }
    }

    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{

    // TODO
    stringstream ss;
    ss << "[";

    for (Node *current = head->next; current != tail; current = current->next)
    {

        if (item2str != nullptr)
        {
            ss << item2str(current->data);
        }
        else
        {
            ss << current->data;
        }
        if (current->next != tail)
        {
            ss << ", ";
        }
    }

    ss << "]";
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{

    // TODO
    if (this == &list)
    {
        return;
    }

    this->clear();

    // Khởi tạo các nút sentinels head và tail mới
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;

    // Nếu danh sách nguồn rỗng, không cần sao chép
    if (list.head->next == list.tail)
    {
        return;
    }
    // Duyệt qua 'list' và sao chép từng phần tử
    Node *currentNode = list.head->next;

    while (currentNode != list.tail)
    {
        this->add(currentNode->data);
        currentNode = currentNode->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{

    // TODO
    if (this->deleteUserData != nullptr && this->count > 0)
    {
        this->deleteUserData(this);
    }

    Node *currentNode = this->head->next;
    while (currentNode != this->tail)
    {
        Node *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }

    delete this->head;
    delete this->tail;

    this->head = nullptr;
    this->tail = nullptr;
    this->count = 0;
}

#endif /* DLINKEDLIST_H */

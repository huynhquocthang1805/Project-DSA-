/*
 * File:   XArrayList.h
 */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class XArrayList : public IList<T>
{
public:
  class Iterator; // forward declaration

protected:
  T *data;
  int capacity;
  int count = 0;
  bool (*itemEqual)(T &lhs, T &rhs);
  void (*deleteUserData)(XArrayList<T> *);

public:
  XArrayList(void (*deleteUserData)(XArrayList<T> *) = 0,
             bool (*itemEqual)(T &, T &) = 0, int capacity = 10);
  XArrayList(const XArrayList<T> &list);
  XArrayList<T> &operator=(const XArrayList<T> &list);
  ~XArrayList();

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
  // Inherit from IList: BEGIN

  void println(string (*item2str)(T &) = 0)
  {
    cout << toString(item2str) << endl;
  }
  void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
  {
    this->deleteUserData = deleteUserData;
  }

  Iterator begin() { return Iterator(this, 0); }
  Iterator end() { return Iterator(this, count); }

protected:
  void checkIndex(int index);     // check validity of index for accessing
  void ensureCapacity(int index); // auto-allocate if needed
  void copyFrom(const XArrayList<T> &list);
  void removeInternalData();

  //! FUNTION STATIC
protected:
  static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
  {
    if (itemEqual == 0)
      return lhs == rhs;
    else
      return itemEqual(lhs, rhs);
  }

public:
  static void free(XArrayList<T> *list)
  {
    typename XArrayList<T>::Iterator it = list->begin();
    while (it != list->end())
    {
      delete *it;
      it++;
    }
  }

public:
  class Iterator
  {
  private:
    int cursor;
    XArrayList<T> *pList;

  public:
    Iterator(XArrayList<T> *pList = 0, int index = 0)
    {
      this->pList = pList;
      this->cursor = index;
    }
    Iterator &operator=(const Iterator &iterator)
    {
      cursor = iterator.cursor;
      pList = iterator.pList;
      return *this;
    }
    void remove(void (*removeItemData)(T) = 0)
    {
      T item = pList->removeAt(cursor);
      if (removeItemData != 0)
        removeItemData(item);
      cursor -= 1; // MUST keep index of previous, for ++ later
    }

    T &operator*() { return pList->data[cursor]; }
    bool operator!=(const Iterator &iterator)
    {
      return cursor != iterator.cursor;
    }
    // Prefix ++ overload
    Iterator &operator++()
    {
      this->cursor++;
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
};

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(void (*deleteUserData)(XArrayList<T> *),
                          bool (*itemEqual)(T &, T &), int capacity)
{
  // TODO implement0
  // Gán các tham số cho các thuộc tính của class
  this->capacity = capacity; // Đặt dung lượng
  this->data = new T[this->capacity];
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;
}

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
  // TODO implement
  // Sao chép dung lượng và số lượng phần tử từ danh sách `list`
  this->capacity = list.capacity;
  this->count = list.count;

  // Sao chép con trỏ hàm so sánh và xóa dữ liệu
  this->itemEqual = list.itemEqual;
  this->deleteUserData = list.deleteUserData;

  // Cấp phát bộ nhớ mới cho mảng dữ liệu
  this->data = new T[this->capacity];

  for (int i = 0; i < list.count; ++i)
  {
    this->data[i] = list.data[i];
  }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
  // TODO implement
  if (this == &list)
  {
    return *this;
  }

  delete[] this->data;
  this->capacity = list.capacity;
  this->count = list.count;
  this->itemEqual = list.itemEqual;
  this->deleteUserData = list.deleteUserData;
  this->data = new T[this->capacity];

  for (int i = 0; i < list.count; ++i)
  {
    this->data[i] = list.data[i];
  }
  return *this;
}

template <class T>
XArrayList<T>::~XArrayList()
{
  // TODO implement
  if (this->deleteUserData)
  {
    this->deleteUserData(this); 
  }
  if (this->data != nullptr)
  {
    delete[] this->data;
  }
}

template <class T>
void XArrayList<T>::add(T e)
{
  // TODO implement
  // Đảm bảo rằng có đủ dung lượng cho phần tử mới
  ensureCapacity(this->count + 1);

  // Thêm phần tử mới vào cuối danh sách
  this->data[this->count] = e;

  // Tăng số lượng phần tử hiện tại
  this->count++;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
  // TODO implement
  // Kiểm tra nếu index không hợp lệ
  if (index < 0 || index > this->count)
  {
    throw std::out_of_range("Index is out of range!");
  }

  // Đảm bảo có đủ dung lượng cho phần tử mới
  ensureCapacity(this->count + 1);

  // Di chuyển các phần tử phía sau từ vị trí index để tạo khoảng trống cho phần tử mới
  for (int i = this->count; i > index; --i)
  {
    this->data[i] = this->data[i - 1];
  }

  // Thêm phần tử mới vào vị trí index
  this->data[index] = e;

  // Tăng số lượng phần tử
  this->count++;
}

template <class T>
T XArrayList<T>::removeAt(int index)
{
  // TODO implement
  // // Kiểm tra nếu index không hợp lệ
  if (index < 0 || index >= this->count)
  {
    throw std::out_of_range("Index is out of range!");
  }
  T removedElement = this->data[index];

  for (int i = index; i < (this->count) - 1; i++)
  {
    this->data[i] = this->data[i + 1];
  }

  this->count--;
  return removedElement;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
  // TODO implement

  int index = this->indexOf(item);

  if (index == -1)
    return false;
  T temp = this->removeAt(index);
  if (removeItemData != nullptr)
  {
    removeItemData(temp);
  }
  return true;
}

template <class T>
bool XArrayList<T>::empty()
{
  // TODO implement
  if (this->count == 0)
  {
    return true;
  }
  else
    return false;
}

template <class T>
int XArrayList<T>::size()
{
  // TODO implement
  return this->count;
}

template <class T>
void XArrayList<T>::clear()
{
  // TODO implement
  if (this->deleteUserData)
  {
    this->deleteUserData(this);
  }

  delete[] this->data;
  this->data = nullptr;
  this->count = 0;
  this->capacity = 0;
}

template <class T>
T &XArrayList<T>::get(int index)
{
  // TODO implement
  checkIndex(index);    
  return this->data[index]; // Trả về tham chiếu đến phần tử tại vị trí index
}

template <class T>
int XArrayList<T>::indexOf(T item)
{
  // TODO implement
  for (int i = 0; i < count; i++)
  {
    if (itemEqual != nullptr)
    {
      if (itemEqual(this->get(i), item))
      {
        return i;
      }
    }
    else if (item == this->get(i))
    {
      return i;
    }
  }
  return -1;
}

template <class T>
bool XArrayList<T>::contains(T item)
{
  // TODO implement
  return indexOf(item) != -1; 
}

template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
  // TODO implement
  std::stringstream oss;
  oss << "[";

  for (int i = 0; i < this->count; ++i)
  {
    if (item2str)
    {
      oss << item2str(this->data[i]);
    }
    else
    {
      oss << this->data[i];
    }

    if (i < this->count - 1)
    {
      oss << ", ";
    }
  }

  oss << "]";
  return oss.str();
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index)
{
  // TODO implement
  // Kiểm tra nếu chỉ số nhỏ hơn 0 hoặc lớn hơn hoặc bằng số lượng phần tử hiện tại
  if (index < 0 || index >= this->count)
  {
    throw std::out_of_range("Index is out of range!"); 
  }
}
template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
  // TODO implement
  if (index <= this->capacity)
  {
    return;
  }

  int newCapacity = this->capacity * 2 + 1;

  T *newData = new T[newCapacity];

  for (int i = 0; i < this->count; i++)
  {
    newData[i] = this->data[i];
  }

  delete[] this->data;

  this->data = newData;
  this->capacity = newCapacity;
}

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
  
  // TODO implement
  if (this == &list)
  {
    return;
  }
  delete[] this->data;
  this->capacity = list.capacity;
  this->count = list.count;

  this->itemEqual = list.itemEqual;
  this->deleteUserData = list.deleteUserData;

  this->data = new T[this->capacity];
  for (int i = 0; i < list.count; ++i)
  {
    this->data[i] = list.data[i];
  }
}

template <class T>
void XArrayList<T>::removeInternalData()
{

  // TODO implement
  if (this->deleteUserData != nullptr)
  {
    deleteUserData(this);
  }
  if (this->data != nullptr)
  {
    delete[] data;
    data = nullptr;
  }
  count = 0;
  capacity = 0;
}

#endif /* XARRAYLIST_H */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T> {
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0)
        : DLinkedList<T>(removeData, itemEQ) {}

    DLinkedListSE(const DLinkedList<T> &list) {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0) {
        if (this->count <= 1) return;

        // Node thật đầu tiên (sau head giả) và node thật cuối cùng (trước tail giả)
        typename DLinkedList<T>::Node *firstNode = this->head->next;
        typename DLinkedList<T>::Node *lastNode = this->tail->prev;

        // Nếu danh sách trống hoặc chỉ có một phần tử
        if (firstNode == this->tail || firstNode == lastNode) return;

        // Ngắt liên kết node cuối với tail giả để thực hiện merge sort
        lastNode->next = nullptr;

        // Thực hiện merge sort
        firstNode = performMergeSort(firstNode, comparator);

        // Gắn lại với head giả và tail giả
        this->head->next = firstNode;
        firstNode->prev = this->head;

        // Cập nhật liên kết và xác định node cuối
        typename DLinkedList<T>::Node *current = firstNode;
        typename DLinkedList<T>::Node *previous = this->head;
        while (current != nullptr) {
            current->prev = previous; // Cập nhật prev
            previous = current;
            current = current->next;
        }

        // Kết nối node cuối với tail giả
        previous->next = this->tail;
        this->tail->prev = previous;
    }

protected:
    typename DLinkedList<T>::Node *performMergeSort(
        typename DLinkedList<T>::Node *head,
        int (*comparator)(T &, T &)) {
        if (head == nullptr || head->next == nullptr) return head;

        // Tìm node giữa để chia danh sách
        typename DLinkedList<T>::Node *middleNode = findMiddle(head);
        typename DLinkedList<T>::Node *secondHalf = middleNode->next;
        middleNode->next = nullptr;

        // Đệ quy sắp xếp hai nửa
        typename DLinkedList<T>::Node *left = performMergeSort(head, comparator);
        typename DLinkedList<T>::Node *right = performMergeSort(secondHalf, comparator);

        // Gộp hai nửa đã sắp xếp
        return mergmergeSortedListse(left, right, comparator);
    }

    typename DLinkedList<T>::Node *findMiddle(typename DLinkedList<T>::Node *head) {
        if (head == nullptr) return head;

        typename DLinkedList<T>::Node *slow = head;
        typename DLinkedList<T>::Node *fast = head->next;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    typename DLinkedList<T>::Node *mergeSortedLists(
        typename DLinkedList<T>::Node *left,
        typename DLinkedList<T>::Node *right,
        int (*comparator)(T &, T &)) {
        // Dummy node để bắt đầu danh sách mới
        typename DLinkedList<T>::Node dummyNode;
        typename DLinkedList<T>::Node *tail = &dummyNode;

        // Gộp hai danh sách
        while (left != nullptr && right != nullptr) {
            if (compare(left->data, right->data, comparator) <= 0) {
                tail->next = left;
                left = left->next;
            } else {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }

        // Nối phần còn lại của danh sách
        if (left != nullptr) tail->next = left;
        if (right != nullptr) tail->next = right;

        return dummyNode.next; // Danh sách mới bắt đầu từ dummy.next
    }

    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0) {
        if (comparator != nullptr) {
            return comparator(lhs, rhs);
        } else {
            if (lhs < rhs) return -1;
            if (lhs > rhs) return 1;
            return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */

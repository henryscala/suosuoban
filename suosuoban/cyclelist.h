#ifndef CYCLELIST_H
#define CYCLELIST_H

#include <cstdio>

using namespace std;

template <typename T> class  Node{
public:
    T value;
    Node *next;
    Node *prev;
};

template <typename T> class CycleList
{
public:
    typedef Node<T> NodeType;

    int size;
    Node<T>* head;
public:
    CycleList():
        head(0),
        size(0)
    {

    }
    ~CycleList(){

    }

    Node<T>* peekFront(){
        return head;
    }

    Node<T>* peekBack(){
        return head->prev;
    }

    Node<T>* popFront(){
        Node<T>* node = head;
        remove(node);
        return node;
    }

    Node<T>* popBack(){
        Node<T>* node = head->prev;
        remove(node);
        return node;
    }

    void pushFront(Node<T>* node){
        if (head==0){
            head = node;
            node->prev = head;
            node->next = head;
        } else {
            node->next = head;
            head->prev->next = node;
            node->prev = head->prev;
            head->prev = node;
            head = node;
        }
        size++;
    }

    void pushBack(Node<T>* node){
        if (head==0){
            head = node;
            node->prev = head;
            node->next = head;
        } else {
            node->prev = head->prev;
            head->prev->next = node;
            node->next = head;
            head->prev = node;
        }
        size++;
    }

    void remove(Node<T>* node){
        if (size<=1){
            head = 0;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (head==node){
                head = head->next;
            }
        }
        node->next = NULL;
        node->prev = NULL;
        size --;
    }

    void clear(){
        while (head ){
            remove(head);
        }
    }

//who allocate, who free
//    void removeAndFree(Node<T>* node){
//        remove(node);
//        delete(node);
//    }
//    void removeAndFreeAll(){
//        while (head!=0){
//            removeAndFree(head);
//        }
//    }


};

#endif // CYCLELIST_H

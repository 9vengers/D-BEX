#pragma once
#include <iostream>

class LinkedList {
    struct Node {
        void* data;
        Node* next;
    };
    Node head;
    Node* tail;
    Node* current;

public:
    LinkedList()
    {
        head = { NULL, NULL };
        tail = &head;
        current = NULL;
    }
    ~LinkedList()
    {
        deleteNode();
    }

    int addNode(void* data)
    {
        Node* node = new Node;
        node->data = data;
        node->next = NULL;
        tail->next = node;
        tail = node;

        return 0;
    }

    int deleteNode()
    {
        while (head.next != NULL)
        {
            current = head.next;
            head.next = current->next;
            delete current;
        }
        tail = &head;
        current = NULL;

        return 0;
    }

    void* getNodeData()
    {
        Node* result;
        if (current == NULL)
            current = head.next;
        result = current;
        if (current != NULL)
        {
            current = current->next;
            return result->data;
        }
        else
            return result;
    }

    void resetCurrent()
    {
        current = NULL;
        return;
    }
};
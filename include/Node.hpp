#ifndef Node_HPP
#define Node_HPP
#include "Node.hpp"

class Node
{
private:
    void *_data;
    Node *_prox;
    Node *_anterior;
public:
    Node(void *data, Node *prox, Node *anterior);
    ~Node();

    //getters
    void *getData() const;
    Node *getProx() const;
    Node *getAnterior() const;

    //seters
    void setData(void *data);
    void setProx(Node* prox);
    void setAnterior(Node* anterior);
};

#endif
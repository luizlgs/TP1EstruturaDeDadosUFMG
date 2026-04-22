#include "../include/Node.hpp"

Node::Node(void *data, Node *prox, Node *anterior){
    _data = data;
    _prox = prox;
    _anterior = anterior;
}

Node::~Node() = default; 

//getters
void* Node::getData() const{
    return _data;
}

Node* Node::getProx() const{
    return _prox;
}

Node* Node::getAnterior() const{
    return _anterior;
}

//setters

void Node::setData(void *data){
    _data = data;
}

void Node::setProx(Node *prox){
    _prox = prox;
}

void Node::setAnterior(Node *anterior){
    _anterior = anterior;
}


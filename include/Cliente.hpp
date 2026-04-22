#ifndef CLIENTE_HPP
#define CLIENTE_HPP
#include "Acao.hpp"
#include "Node.hpp"

class Cliente
{
private:
    int _id, _numacoes;
    Node *_primeiraAcao;  
    Node *_ultimaAcao;
    

public:
    Cliente(int id);
    ~Cliente();

    //getters
    int getId() const;
    Node* getPrimeiraAcao() const;

    //setters
    void setId(int id);

    //manipulacao de acoes
    void adicionarAcao(Acao *acao);
    void removerAcao(int id);

};

#endif
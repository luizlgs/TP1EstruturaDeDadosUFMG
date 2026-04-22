#include "../include/Cliente.hpp"

Cliente::Cliente(int id){
    _id = id;
    _primeiraAcao = nullptr;
    _ultimaAcao = nullptr;
    _numacoes = 0;
}

Cliente::~Cliente() = default; 

//getters

int Cliente::getId() const{
    return _id;
}

Node*Cliente::getPrimeiraAcao() const{
    return _primeiraAcao;
}

//setter
void Cliente::setId(int id){
    _id = id;
}

//manipulacao de acoes

void Cliente::adicionarAcao(Acao *acao){
    Node *nova_acao = new Node(acao, nullptr, _ultimaAcao);
    if(_primeiraAcao == nullptr){
        _primeiraAcao = nova_acao;
    }
    if(_numacoes >= 1){
        _ultimaAcao->setProx(nova_acao);
    }
    _ultimaAcao = nova_acao;
    _ultimaAcao->setProx(nullptr);
    
    
    _numacoes++;
}

void Cliente::removerAcao(int id){
    Node *a = _primeiraAcao;
    while(a != nullptr){
        Acao* acao = static_cast<Acao*>(a->getData());
        if(acao->getId() == id){
            if(_numacoes == 1){
                _ultimaAcao = nullptr;
                _primeiraAcao = nullptr;
            }

            else if(a->getAnterior() == nullptr){
                a->getProx()->setAnterior(nullptr);
                _primeiraAcao = a->getProx();
            }
            else if(a->getProx() == nullptr){
                a->getAnterior()->setProx(nullptr);
                _ultimaAcao = a->getAnterior();
            }
            else{
                a->getAnterior()->setProx(a->getProx());
                a->getProx()->setAnterior(a->getAnterior());
            }
            _numacoes--;
            delete a;
            return;
        }
        a = a->getProx();
    }
}




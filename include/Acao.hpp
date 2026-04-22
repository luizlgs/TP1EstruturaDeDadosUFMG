#ifndef ACAO_HPP
#define ACAO_HPP
#include "Node.hpp"
#include <stdexcept>
#include <cmath>
#include <math.h>

class Acao
{
private:
    int _id, _numcotacoes;
    Node *_primeiraCotacao;
    Node *_ultimacotacao;

public:
    Acao(int _id,  Node *primeiraCotacao, Node *ultimacotacao);
    ~Acao();

    //getters
    int getId() const;
    void* getUltimaCotacao() const;

    //manipulacao de cotacoes
    void adicionarCotacao(double valor_cotacao);

    //calculo das metricas
    double calcularRetornoElementar(int qtd_cotacoes_consideradas);
    double calcularRet(int qtd_cotacoes_consideradas);
    double calcularAvgRet(int qtd_cotacoes_consideradas);
    double calcularStab(int qtd_cotacoes_consideradas);
    double calcularCons(int qtd_cotacoes_consideradas);
};

#endif
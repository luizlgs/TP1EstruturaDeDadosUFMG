#include "../include/Acao.hpp"

Acao::Acao(int id, Node *primeiraCotacao, Node *ultimacotacao){
    _id = id;
    _primeiraCotacao = primeiraCotacao;
    _ultimacotacao = ultimacotacao;
    _numcotacoes = 0;
}

Acao::~Acao() {
    Node *atual = _primeiraCotacao;
    while (atual != nullptr) {
        Node *prox = atual->getProx();
        
        if (atual->getData() != nullptr) {
            delete static_cast<double*>(atual->getData());
        }
        
        delete atual;
        atual = prox;
    }
}

//getters
int Acao::getId() const{
    return _id;
}

void *Acao::getUltimaCotacao() const{
    return _ultimacotacao;
}

//manipulacao de cotacoes

double Acao::calcularRetornoElementar(int posicao){
     Node *a = _ultimacotacao;
    if(posicao <= 0 || posicao > _numcotacoes-1){
        throw std::invalid_argument("Posicao inválida");
    }
    else if(_numcotacoes < 2){
        throw std::invalid_argument("Numero de cotacoes inválido");
    }
    double retorno_elementar = 0;
    for(int i = _numcotacoes-1; i > _numcotacoes - posicao && i>0; i--){
        a = a->getAnterior();
    }
    retorno_elementar = ((*(static_cast<double*>(a->getData()))) / (*(static_cast<double*>(a->getAnterior()->getData())))) - 1.0;
    return retorno_elementar;
}

void Acao::adicionarCotacao(double valor_cotacao){
    double *val_cotacao = new double();
    *val_cotacao = std::abs(valor_cotacao);

    Node *cotacao = new Node(val_cotacao, nullptr, _ultimacotacao);
    if(_numcotacoes >= 1){
        _ultimacotacao->setProx(cotacao);
    }
    _ultimacotacao = cotacao;
    if(_primeiraCotacao == nullptr){
        _primeiraCotacao = cotacao;
    }
    

    _numcotacoes++;
}

double Acao::calcularRet(int qtd_cotacoes_consideradas){
    if(qtd_cotacoes_consideradas > _numcotacoes || qtd_cotacoes_consideradas < 2){
        throw std::invalid_argument("Numero de cotacoes invalido");
    }

    Node *a = _ultimacotacao;
    for(int i = 0; i < qtd_cotacoes_consideradas - 1; i++){
        if(a->getAnterior() == nullptr) {
            throw std::runtime_error("A acao nao tem cotacoes suficientes.");
        }
        a = a->getAnterior();
    }

    return (*(static_cast<double*>(_ultimacotacao->getData())) / *(static_cast<double*>(a->getData()))) - 1.0;
}

double Acao::calcularAvgRet(int qtd_cotacoes_consideradas){
    if(qtd_cotacoes_consideradas > _numcotacoes || qtd_cotacoes_consideradas < 2){
        throw std::invalid_argument("Numero de cotacoes inválido");
    }
    double total_avgret = 0;
    Node *a = _ultimacotacao;
    for(int i = 0; i<qtd_cotacoes_consideradas-1; i++){
        if(a->getAnterior() == nullptr){
            throw std::runtime_error("A acao que deseja acessa é invalida ou não existe");
        }
        total_avgret += (*static_cast<double*>(a->getData()) / *static_cast<double*>(a->getAnterior()->getData())) - 1.0;
        a = a->getAnterior();
    }
    total_avgret = total_avgret * (1.0/(qtd_cotacoes_consideradas - 1.0));
    return total_avgret;
}

double Acao::calcularStab(int qtd_cotacoes_consideradas){
    if(qtd_cotacoes_consideradas > _numcotacoes || qtd_cotacoes_consideradas < 2){
        throw std::invalid_argument("Numero de cotacoes inválido");
    }
    Node *a = _ultimacotacao;
    double total_avgret_r=0, avg_ret=calcularAvgRet(qtd_cotacoes_consideradas), vol;
    for(int i=0; i<qtd_cotacoes_consideradas-1; i++){
        if(a->getAnterior() == nullptr){
            throw std::runtime_error("A acao que deseja acessa é invalida ou não existe");
        }
        total_avgret_r += pow((((*static_cast<double*>(a->getData()) / *static_cast<double*>(a->getAnterior()->getData())) - 1.0) - avg_ret), 2);
        a = a->getAnterior();
    }
    vol = sqrt((1.0/(qtd_cotacoes_consideradas-1.0)) * total_avgret_r);
    return 1.0/(1.0 + vol);
    
}

double Acao::calcularCons(int qtd_cotacoes_consideradas){
    if(qtd_cotacoes_consideradas > _numcotacoes || qtd_cotacoes_consideradas < 2){
        throw std::invalid_argument("Numero de cotacoes inválido");
    }
    double cons=0, r_i;
    Node *a = _ultimacotacao;
    for(int i=0 ;i<qtd_cotacoes_consideradas-1; i++){
        if(a->getAnterior() == nullptr){
            throw std::runtime_error("A acao que deseja acessa é invalida ou não existe");
        }
        r_i = (*static_cast<double*>(a->getData()) / *static_cast<double*>(a->getAnterior()->getData())) - 1.0;
        if(r_i > 0){
            cons += 1;
        }
        a = a->getAnterior();
    }
    return (1.0/(qtd_cotacoes_consideradas-1.0)) * cons;
}


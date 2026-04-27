#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include "../include/Acao.hpp"
#include "../include/Cliente.hpp"
#include "../include/Node.hpp"

//struct para relacionar a acao com o seu id e pontuacao para a ordenacao
struct RankItem {
    int index;
    double pontuacao;
    int id;
};

//merge para ordenar duas subpartes do ranking
void merge(RankItem arr[], int const inicio, int const meio, int const fim) {
    int const n1 = meio - inicio + 1;
    int const n2 = fim - meio;

    RankItem* array_esquerdo = new RankItem[n1];
    RankItem* array_direito = new RankItem[n2];

    for (int i = 0; i < n1; i++){
        array_esquerdo[i] = arr[inicio + i];
    }
    for (int j = 0; j < n2; j++){
        array_direito[j] = arr[meio + 1 + j];
    }

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2){
        if (array_esquerdo[i].pontuacao > array_direito[j].pontuacao || 
           (array_esquerdo[i].pontuacao == array_direito[j].pontuacao && array_esquerdo[i].id < array_direito[j].id)) {
            arr[k] = array_esquerdo[i];
            i++;
        } else {
            arr[k] = array_direito[j];
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k] = array_esquerdo[i];
        i++;
        k++;
    }

    while (j < n2){
        arr[k] = array_direito[j];
        j++;
        k++;
    }

    delete[] array_esquerdo;
    delete[] array_direito;
}

//ordenacao do ranking de melhores e piores acoes
void mergeSort(RankItem arr[], int const inicio, int const fim){
    if (inicio >= fim)
        return;

    int meio = inicio + (fim - inicio) / 2;
    mergeSort(arr, inicio, meio);
    mergeSort(arr, meio + 1, fim);
    merge(arr, inicio, meio, fim);
}

int main(){
    int lineCounter = 0;
    char funcao;

    Node acoes = Node(nullptr, nullptr, nullptr); //lista geral de acoes
    Node *ultima_acao_cadastrada = &acoes;

    Node clientes = Node(nullptr, nullptr, nullptr); //lista geral de acoes
    Node *ultimo_cliente_cadastrado = &clientes;

    int numcotacoes; //numero de cotacoes que serao consideradas nas metricas

    std::string line; //variavel usada para guardar a linha analisada


    while (std::getline(std::cin, line)){
        if (line.empty())
            continue;
        lineCounter++;
        std::stringstream ss(line);
        ss >> funcao;

        switch (funcao){

        case 77:{ // M
            ss >> numcotacoes;
            break;
        }
            //----------------------------------------------------------------------------------------------
        case 65:{ // A
            double id_acao;
            ss >> id_acao;
            Acao *acao = new Acao(id_acao, nullptr, nullptr);
            Node *nova_acao = new Node(acao, nullptr, ultima_acao_cadastrada);
            ultima_acao_cadastrada->setProx(nova_acao);
            ultima_acao_cadastrada = nova_acao;

            break;
        }
            //----------------------------------------------------------------------------------------------

        case 85:{ // U
            int id_novo_cliente;
            ss >> id_novo_cliente;
            Cliente *cliente = new Cliente(id_novo_cliente);
            Node *novoCliente = new Node(cliente, nullptr, ultimo_cliente_cadastrado);
            ultimo_cliente_cadastrado->setProx(novoCliente);
            ultimo_cliente_cadastrado = novoCliente;
            break;
        }

            //----------------------------------------------------------------------------------------------

        case 80:{ // P
            Node *a = acoes.getProx();
            int id_procurado;
            double cotacao;
            ss >> id_procurado;
            ss >> cotacao;
            while (a != nullptr){
                Acao *acao = static_cast<Acao *>(a->getData());
                if (acao->getId() == id_procurado){
                    acao->adicionarCotacao(cotacao);
                    break;
                }
                a = a->getProx();
            }
            break;
        }

            //----------------------------------------------------------------------------------------------

        case 66:{ // B
            Node *a = acoes.getProx();
            Node *b = clientes.getProx();
            int id_cliente;
            int id_acao;
            ss >> id_cliente;
            ss >> id_acao;

            while (a != nullptr){
                if (static_cast<Acao *>(a->getData())->getId() == id_acao){
                    break;
                }
                a = a->getProx();
            }
            if (a == nullptr){
                throw std::invalid_argument("Comando inválido na linha " + std::to_string(lineCounter));
            }

            while (b != nullptr){
                if (static_cast<Cliente *>(b->getData())->getId() == id_cliente){
                    static_cast<Cliente *>(b->getData())->adicionarAcao(static_cast<Acao *>(a->getData()));
                    break;
                }
                b = b->getProx();
            }
            break;
        }

            //----------------------------------------------------------------------------------------------

        case 86:{ // V
            Node *a = clientes.getProx();
            int id_cliente, id_acao;
            ss >> id_cliente;
            ss >> id_acao;

            while (a != nullptr){
                if (static_cast<Cliente *>(a->getData())->getId() == id_cliente){
                    static_cast<Cliente *>(a->getData())->removerAcao(id_acao);
                    break;
                }
                a = a->getProx();
            }

            break;
        }

            //----------------------------------------------------------------------------------------------

        case 81:{ //Q
            int id_consulta, id_cliente, num_acoes_retornadas, qtd_metricas;
            ss >> id_consulta >> id_cliente >> num_acoes_retornadas >> qtd_metricas;

            std::string *metricas_nome = new std::string[qtd_metricas];
            double *pesos = new double[qtd_metricas];
            for (int i = 0; i < qtd_metricas; i++){
                ss >> metricas_nome[i] >> pesos[i];
            }

            int qtd_acoes = 0;
            Node *m_node = acoes.getProx();
            while (m_node != nullptr){
                qtd_acoes++;
                m_node = m_node->getProx();
            }

            if (qtd_acoes == 0 || numcotacoes < 2){
                delete[] metricas_nome;
                delete[] pesos;
                break;
            }

            Acao **todas_acoes = new Acao *[qtd_acoes];
            double *pontuacao_global = new double[qtd_acoes];

            m_node = acoes.getProx();
            //a pontuacao global é zerada para calcular uma das metricas especificadas
            for (int i = 0; i < qtd_acoes; i++){
                todas_acoes[i] = static_cast<Acao *>(m_node->getData());
                pontuacao_global[i] = 0.0;
                m_node = m_node->getProx();
            }

            for (int j = 0; j < qtd_metricas; j++){
                RankItem *itens_global = new RankItem[qtd_acoes];

                //laço para atribuir uma pontuacao para uma determinada metrica
                for (int i = 0; i < qtd_acoes; i++){
                    itens_global[i].index = i;
                    itens_global[i].id = todas_acoes[i]->getId();
                    if (metricas_nome[j] == "RET"){
                        itens_global[i].pontuacao = todas_acoes[i]->calcularRet(numcotacoes);
                    }
                    else if (metricas_nome[j] == "AVGRET"){
                        itens_global[i].pontuacao = todas_acoes[i]->calcularAvgRet(numcotacoes);
                    }
                    else if (metricas_nome[j] == "STAB"){
                        itens_global[i].pontuacao = todas_acoes[i]->calcularStab(numcotacoes);
                    }
                    else if (metricas_nome[j] == "CONS"){
                        itens_global[i].pontuacao = todas_acoes[i]->calcularCons(numcotacoes);
                    }
                }

                //ordenacao em relacao as pontuacoes
                mergeSort(itens_global, 0, qtd_acoes - 1);

                //a pontuação da metrica é somada a pontuação anterior da acao
                for (int i = 0; i < qtd_acoes; i++){
                    int original_idx = itens_global[i].index;
                    int soma_pos = qtd_acoes - i;
                    pontuacao_global[original_idx] += (soma_pos * pesos[j]);
                }

                delete[] itens_global;
            }

            Node *c_node = clientes.getProx();
            Cliente *cliente = nullptr;
            while (c_node != nullptr){
                if (static_cast<Cliente *>(c_node->getData())->getId() == id_cliente){
                    cliente = static_cast<Cliente *>(c_node->getData());
                    break;
                }
                c_node = c_node->getProx();
            }

            if (cliente){
                int num_acoes_carteira = 0;
                Node *acao_node = cliente->getPrimeiraAcao();
                while (acao_node != nullptr){
                    num_acoes_carteira++;
                    acao_node = acao_node->getProx();
                }

                if (num_acoes_carteira > 0){
                    RankItem *array_carteira = new RankItem[num_acoes_carteira];
                    acao_node = cliente->getPrimeiraAcao();
                    int idx = 0;
                    while (acao_node != nullptr){
                        Acao *ac = static_cast<Acao *>(acao_node->getData());
                        array_carteira[idx].id = ac->getId();
                        for (int i = 0; i < qtd_acoes; i++){
                            if (todas_acoes[i]->getId() == ac->getId()){
                                array_carteira[idx].pontuacao = pontuacao_global[i];
                                break;
                            }
                        }
                        idx++;
                        acao_node = acao_node->getProx();
                    }

                    int max_imprimir;
                    if(num_acoes_retornadas < num_acoes_carteira) max_imprimir = num_acoes_retornadas;
                    else max_imprimir = num_acoes_carteira;
                    
                    std::cout << std::fixed << std::setprecision(2);

                    mergeSort(array_carteira, 0, num_acoes_carteira - 1);

                    for (int i = 0; i < max_imprimir; i++){
                        std::cout << "R " << id_consulta << " M " << i << " " << array_carteira[i].id << " " << array_carteira[i].pontuacao << std::endl;
                    }

                    for (int i = 0; i < max_imprimir; i++){
                        int idx_pior = num_acoes_carteira - 1 - i;
                        std::cout << "R " << id_consulta << " P " << i << " " << array_carteira[idx_pior].id << " " << array_carteira[idx_pior].pontuacao << std::endl;
                    }
                    delete[] array_carteira;
                }
            }

            delete[] metricas_nome;
            delete[] pesos;
            delete[] todas_acoes;
            delete[] pontuacao_global;

            break;
        }

        default:
            throw std::invalid_argument("Comando inválido na linha " + std::to_string(lineCounter));
            break;
        }
    }

    //limpeza das listas para evitar memory leak

    //limpeza clientes
    Node *atual_cliente = clientes.getProx();
    while (atual_cliente != nullptr) {
        Node *prox = atual_cliente->getProx();
        
        delete static_cast<Cliente*>(atual_cliente->getData());
        
        delete atual_cliente;
        
        atual_cliente = prox;
    }

    //limpeza acoes
    Node *atual_acao = acoes.getProx();
    while (atual_acao != nullptr) {
        Node *prox = atual_acao->getProx();
        
        delete static_cast<Acao*>(atual_acao->getData());
        delete atual_acao;
        
        atual_acao = prox;
    }
}

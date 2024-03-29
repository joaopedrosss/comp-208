#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
A fins didáticos, essa versão da Trie:

-> aceita apenas letras minúsculas
-> palavras sem acento
-> 26 letras do alfabeto, sem Ç
-> em insert node, não verifica se já colocamos a palavra
*/


/*
Quantos caracteres vamos usar?
Isso é definido por TAMANHO_ALFA (tamanho do alfabeto)
*/
#define TAMANHO_ALFABETO 26



/*
    Definindo a estrutura da árvore trie,
    onde cada nó terá um array de ponteiros do
    tamanho do alfabeto adotado (ASCII)
*/
typedef struct trie_node{
    struct trie_node *children[TAMANHO_ALFABETO];
    bool fimDePalavra;
}TRIE_NODE;


/*
Aloca memória para um no da trie, inicializa suas variáveis 
E retorna o esse novo no criado.
*/
TRIE_NODE* criar_trie_node(){
    TRIE_NODE* novo_no = (TRIE_NODE*) malloc(sizeof(TRIE_NODE));

    for(int i = 0; i < TAMANHO_ALFABETO; i++){
        novo_no->children[i] = NULL;
    }
    novo_no->fimDePalavra = false;

    return novo_no;
}


/*
Exemplo: palavra 'gato'
          0 1 2 3
'gato' -> g a t o \0
 char* palavra: aponta para o primeiro caractere (no caso: 'g')

 palavra++ : estou avancando um caractere (agora, palavra aponta para 'a')

 palavra++ : estou avancando um caractere (agora, palavra aponta para 't')
 
 palavra++ : (agora, palavra aponta para 'o')
 
 palavra++ : (agora, palavra aponta para '\0')

 index_char: qual o indice desse caractere no nosso array?

Voltando para o ínicio da parlavra (*palavra é iguala 'g')

int index_char =  *palavra - 'a'; é o mesmo que 'a' - 'a' ; 97 - 97 = 0
palavra++

int index_char =  *palavra - 'a'; é o mesmo que 'b' - 'a' ; 98 - 97 = 1
palavra++

int index_char =  *palavra - 'a'; é o mesmo que 'c' - 'a' ; 99 - 97 = 2
palavra++

NÃO CHEGAREMOS AO PONTO DE '\0' - 'a', pois o loop para assim que *palavra é igual a '\0'
*/

int index_de(char* letra_atual){
    return *letra_atual - 'a';
}

void inserir_node(TRIE_NODE **raiz, char* palavra){

    // Trie vazia
    if(*raiz == NULL){
        *raiz = criar_trie_node();
    }

    TRIE_NODE *node_atual = *raiz;

    char *letra_atual = palavra;

    while (*letra_atual != '\0')// gato = g a t o \0 PERCORRE A NOSSA PALAVRA
    {
        //int index_char =  *palavra - 'a'; // 'a' - 'a' = 99 - 97 = 0
        // [apontando][[NULL][[NULL][[NULL][[NULL][[NULL]
        //   |
        //[NULL][[NULL][[NULL][[NULL][[NULL]
        if(node_atual->children[index_de(letra_atual)] == NULL){
            node_atual->children[index_de(letra_atual)] = criar_trie_node();
        }

        node_atual = node_atual->children[index_de(letra_atual)]; // node_atual->children[1]
        //node_atual = node_atual->children[o];
        letra_atual++; //andamos na palavra 
    }
    node_atual->fimDePalavra = true;

    return;    
}

bool buscar_node(TRIE_NODE *raiz, char* palavra){
    
    if(raiz == NULL){
        return false;
    }

    TRIE_NODE *node_atual = raiz;

    while (*palavra != '\0')// gato = g a t o \0
    {
        int index_char =  *palavra - 'a';

        if(node_atual->children[index_char] == NULL){
            return false;
        }

        node_atual = node_atual->children[index_char];

        palavra++; //andamos na palavra
    }

    return node_atual->fimDePalavra;    
}

//tamanho_palavra tamanho_prefixo
//texto palavra_em_construcao
void auto_complete_rec(TRIE_NODE* node_atual,char *palavra_formada, int tamanho_prefixo){
    char palavra_em_construcao[tamanho_prefixo]; // texto[6] = petiz []
    strcpy(palavra_em_construcao,palavra_formada);
    palavra_em_construcao[tamanho_prefixo] = '\0';

    if(node_atual->fimDePalavra){
        printf("%s\n", palavra_em_construcao);
    }

    for(int i = 0; i < TAMANHO_ALFABETO; i++){
        if(node_atual->children[i] != NULL){
            palavra_em_construcao[tamanho_prefixo - 1] = i + 'a'; // 0 + 97 -> 'a' ;  5 + 97 -> 'o'
            char k = i + 97;
            //printf("%c\n",k);//tamanho_palavra volta a ser 5
            auto_complete_rec(node_atual->children[i],palavra_em_construcao,tamanho_prefixo + 1);
        }
    }



};

void auto_complete(TRIE_NODE *raiz, char *prefixo_procurado){
    // Trie vazia
    if(raiz == NULL){
        return;
    }

    char* prefixo = prefixo_procurado; 

    TRIE_NODE *node_atual = raiz;
    
    int tamanho_prefixo = strlen(prefixo);

    while (*prefixo != '\0')// p e t i z
    {
        int index_char =  *prefixo - 'a';


        if(node_atual->children[index_char] == NULL){
            node_atual->children[index_char] = criar_trie_node();
        }

        node_atual = node_atual->children[index_char];

        prefixo++; //andamos na palavra
    }

    //printf("%d %s\n",node_atual->fimDePalavra,prefixo_procurado);

    auto_complete_rec(node_atual, prefixo_procurado, tamanho_prefixo + 1); //pretiz
}

bool node_has_children(TRIE_NODE *node){
    if (node == NULL) return false;

    for(int i = 0; i < TAMANHO_ALFABETO; i++){
        if (node->children[i] != NULL) return true;
    }

    return false;
}

/*
    recebe apalavra que deseja remover como parametro
    e vai passando para o proximo caractere até encontrar um fim de
    palavra, quando encontra, remove recusivamente aquela palavra,se o
    no nao tiver filho, libera a memoria. Após, confere se a remoção
    foi bem feita, e confere os parametro apra liberar memoria. 
*/

TRIE_NODE *remove_node_rec(TRIE_NODE *node, unsigned char *text, int i, bool *removed){
    //printf("[%c]\n",text[i]);
    if (text[i] == '\0'){
        if (node->fimDePalavra){
            node->fimDePalavra = false;
            *removed = true;
        }

        if (!node_has_children(node)){
            free(node);
            node = NULL;
        }

        return node;
    }

    node->children[text[i] - 'a'] = remove_node_rec(node->children[text[i] - 'a'], text, i + 1, removed);

    if (!node_has_children(node) && *removed && !node->fimDePalavra){
        free(node);
        node = NULL;
    }

    return node;
}
/*
    se o no for nulo, não tem o que remover. Retorna falsepara 
    evidenciar que não houve remoção. Passa pelo type casting e
    inicia result como false para entrar na recursão da função
    anterior
*/
bool remove_node(TRIE_NODE **root, char *palavra){
    if (*root == NULL) return false;

    char *text = palavra;
    bool result = false;

    *root = remove_node_rec(*root, text, 0, &result);

    return result;
}

int main(){
    TRIE_NODE *my_trie = NULL;

    inserir_node(&my_trie, "petiz");
    inserir_node(&my_trie, "petiza");
    inserir_node(&my_trie, "petizada");
    inserir_node(&my_trie, "petizote");

    /*
    printf("Tem? %d\n",buscar_node(my_trie,"petiza"));
    printf("Tem? %d\n",buscar_node(my_trie,"petizote"));
    printf("Tem? %d\n",buscar_node(my_trie,"petiz"));
    printf("Tem? %d\n",buscar_node(my_trie,"petizada"));
    */
    auto_complete(my_trie,"petiz");

    printf("Removi? %d\n",remove_node(&my_trie,"petiz"));
    printf("Tem? %d\n",buscar_node(my_trie,"petiz"));

    auto_complete(my_trie,"p");

}







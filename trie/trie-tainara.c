#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/*
    defnindo o tamanho de NUM_CHAR
    de acordo com a tabela ASCII
*/
#define NUM_CHAR 256

/*
    Definindo a estrutura da árvore trie,
    onde cada nó terá um array de ponteiros do
    tamanho do alfabeto adotado (ASCII)
*/
typedef struct TrieNode{
    struct TrieNode *children[NUM_CHAR];
    bool terminal;
} TrieNode;

/*
    Criando um nó da árvore, alocando memoria, 
    apontando para filhs vazios e botando  o
    terminal (fim de palavra) como falso inicialmente
*/

TrieNode *create_node(){
    TrieNode *new_node = malloc(sizeof(TrieNode));

    for(int i = 0; i < NUM_CHAR; i++){
        new_node->children[i] = NULL;
    }

    new_node->terminal = false;

    return new_node;
}

/*
    Inserindo uma palavra na árvore, garantindo que 
    os prefixos sejam interligados. montando assim, a 
    característica da árvore trie
*/
bool insert_node(TrieNode **root, char *signed_text){
    if (*root == NULL){
        *root = create_node();
    }
    //type casting (sem caracteres negayivos)
    unsigned char *text = (unsigned char *)signed_text;
    TrieNode *check = *root;
    int len = strlen(signed_text);

    for(int i = 0; i < len; i++){
        if (check->children[text[i]] == NULL)check->children[text[i]] = create_node();
        check = check->children[text[i]];
    }

    if (check->terminal) return false;
    check->terminal = true;
    return true;
}

/*
    Buscando na trie
    pegando caractere a caractere e conferindo
    se eles tem filhos correspondentes ao caractere
    da palavra buscada. Conseguimos fazer isso por consequencia
    da dependencia de prefixos
*/
void search_trie(TrieNode *root, char *text){
    unsigned char *unsigned_text = (unsigned char *)text;
    int len = strlen(text);

    TrieNode *check = root;

    for(int i = 0; i < len; i++){
        if (check->children[unsigned_text[i]] == NULL){
            printf("%s NOT FOUND\n", text);
            return;
        }
        check = check->children[unsigned_text[i]];
    }

    printf((check->terminal ? "%s FOUND\n" : "%s NOT FOUND\n"), text);
}

/*
    Conferindo se o nó tem filho
*/
bool node_has_children(TrieNode *node){
    if (node == NULL) return false;

    for(int i = 0; i < NUM_CHAR; i++){
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
TrieNode *remove_node_rec(TrieNode *node, unsigned char *text, int i, bool *removed){
    if (text[i] == '\0'){
        if (node->terminal){
            node->terminal = false;
            *removed = true;
        }

        if (!node_has_children(node)){
            free(node);
            node = NULL;
        }

        return node;
    }

    node->children[text[i]] = remove_node_rec(node->children[text[i]], text, i + 1, removed);

    if (!node_has_children(node) && *removed && !node->terminal){
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
bool remove_node(TrieNode **root, char *signed_text){
    if (*root == NULL) return false;

    unsigned char *text = (unsigned char *)signed_text;
    bool result = false;

    *root = remove_node_rec(*root, text, 0, &result);

    return result;
}


void auto_complete_rec(TrieNode *node, unsigned char *prefix, int len){
    unsigned char text[len + 1];
    strcpy(text, prefix);
    text[len] = '\0';

    if (node->terminal) printf("%s\n", (char *)prefix);

    for(int i = 0; i < NUM_CHAR; i++){
        if (node->children[i] != NULL){
            text[len - 1] = i;
            auto_complete_rec(node->children[i], text, len + 1);
        }
    }
}

void auto_complete(TrieNode *root, char *prefix){
    if (root == NULL){
        printf("NO MATCH!\n");
        return;
    }

    TrieNode *check = root;
    int len = strlen(prefix);
    unsigned char *unsigned_text = (unsigned char *)prefix;

    for(int i = 0; i < len; i++){
        if (check->children[unsigned_text[i]] == NULL){
            printf("NO MATCH!\n");
            return;
        }
        check = check->children[unsigned_text[i]];
    }

    auto_complete_rec(check, unsigned_text, len + 1);
}

int main(){
    TrieNode *root = NULL;

    insert_node(&root, "CARRO");
    insert_node(&root, "CARTA");
    insert_node(&root, "CAVALO");

    auto_complete(root, "CA");

    remove_node(&root, "CARRO");
    auto_complete(root, "CA");

    return 0;
}
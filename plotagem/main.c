#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para um nó de lista encadeada
typedef struct Node {
    int num;
    struct Node *next;
} Node;

// Estrutura para um nó de árvore binária de busca
typedef struct BinaryTree {
    int num;
    struct BinaryTree* left;
    struct BinaryTree* right;
} BinaryTree;

Node *createNode(int num){
    Node *newNode = malloc(sizeof(Node));
    newNode->num = num;
    newNode->next = NULL;

    return newNode;
}

BinaryTree *createBinaryTree(int num){
    BinaryTree *newBT = malloc(sizeof(BinaryTree));
    newBT->num = num;
    newBT->left = NULL;
    newBT->right = NULL;

    return newBT;
}

Node *insertInList(Node *head, int num){
    Node *newNode = createNode(num);
    newNode->next = head;

    return newNode;
}

BinaryTree *insertInBT(BinaryTree *bt, int num){
    if(bt == NULL){
        bt = createBinaryTree(num);
    }else if(num < bt->num){
        bt->left = insertInBT(bt->left, num);
    }else if(num > bt->num){
         bt->right = insertInBT(bt->right, num);
    }

    return bt;
}
/*
insertInBT(NULL, 2)
    bt = createBuinaryTree(2)

insertInBT(arvore binaria, 3) bt -> arvore binaria
    insertInBT(NULL, 3) _> NONO NO

insertInBT(arvore binaria, 9)
     bt->right  = insertInBT(arvore binaria->DIREITO,9); ( ARVOR 9 )
        insertInBT(NULL,9); -> ARVORE 9



*/
int searchInList(Node *node, int num){
    int cont = 0;

    while(node != NULL){
        cont++;

        if(node->num == num){
            return cont;
        }

        node = node->next;
    }

    return cont+1; //CASO O NUMERO NAÕ ESTEJA NA LISTA
}
/* 3
2 6 9 12 16
          *    
CONT = 6
*/

int searchInBST(BinaryTree *node, int num){// 4
    int cont = 0;

    while(node != NULL){
        cont++;

        if(node->num == num) return cont;

        node = num < node->num ? node->left : node->right;
    }

    return cont +1;
}

void printTree(BinaryTree *node){
    if(node == NULL) return;
    printf("%d\n", node->num);
    printTree(node->left);
    printTree(node->right);
}

void printList(Node *node){
    while(node != NULL){
        printf("%d\n", node->num);
        node = node->next;
    }
}

void freeList(Node *node){
    if(node == NULL) return;
    freeList(node->next);
    free(node);
}

void freeTree(BinaryTree *node){
    if(node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}


int main() {
    int indice_random;
    BinaryTree *root = NULL;
    Node *head = NULL;
    FILE *dados;

    //srand(time(NULL));

    
    int range_max = 50000;
    int numeros_inseridos[range_max];

    for(int n = 1; n <= range_max; n++){
        
        int random = rand() % n; //pegar números aleatórios de 0 a n aleatórios para colocar na arvore e lista
        
        root = insertInBT(root,random);
        head = insertInList(head,random);
        numeros_inseridos[n] = random;

        /*
        como n começa com números pequenos, maior é a as chances de virem números pequenos no inicio
        pois menor a é a margem 
            números aleatórios de 0 a 1
            números aleatórios de  0 a 2
            números aleatórios de 0 a 3
            ...
            números aleatórios de 0 a 10000
            ---
            números aleatórios de 0 a 50000
        esses números pequenos no ínício portanto, são aidicionando no aind ano íncio
        fazendo com que eles fiquem no final da lista encadeada 
        */
        
    }
    
    /*
    printTree(root);
    printf("----\n");
    printList(head);
    */

   dados = fopen("dados.txt", "w");
    
    for(int n = 1; n <= range_max; n++){
        int random_indice = rand() % range_max;//pegar números aleatórios de 1 a n aleatórios para colocar na arvore e lista
        int comp_bst = searchInBST(root,numeros_inseridos[random_indice]);
        int comp_list = searchInList(head,numeros_inseridos[random_indice]);

        //printf("[%d] [%d] [%d]\n",numeros_inseridos[random_indice],comp_list, comp_bst);

        fprintf(dados,"%d %d %d\n",numeros_inseridos[random_indice],comp_list,comp_bst);
    }


    fclose(dados);

    printf("Finalizado!\n");

    freeTree(root);
    root = NULL;
    freeList(head);
    head = NULL;

    return 0;
}
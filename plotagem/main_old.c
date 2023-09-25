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
    if(bt == NULL) bt = createBinaryTree(num);
    else if(num < bt->num) bt->left = insertInBT(bt->left, num);
    else if(num > bt->num) bt->right = insertInBT(bt->right, num);

    return bt;
}

int searchInList(Node *node, int num){
    int cont = 0;

    while(node != NULL){
        cont++;

        if(node->num == num) return cont;

        node = node->next;
    }

    return cont + 1;
}

int searchInBST(BinaryTree *node, int num){
    int cont = 0;

    while(node != NULL){
        cont++;

        if(node->num == num) return cont;

        node = num < node->num ? node->left : node->right;
    }

    return cont +1;
}

void createValuesToInsert(int n, Node **head, BinaryTree **root, int numbersInserted[]){
    int random;

    for(int i = 0; i < n; i++){
        random = rand();
        *root = insertInBT(*root,random);
        *head = insertInList(*head, random);

        numbersInserted[i] = random;
    }
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

void biggest_comparison_tree(BinaryTree *node, int *i, int comparisons){
    if(node == NULL){
        if(comparisons > *i) *i = comparisons;
        return;
    }

    biggest_comparison_tree(node->left, i, comparisons + 1);
    biggest_comparison_tree(node->right, i, comparisons + 1);
}

int main() {
    int indice_random;
    BinaryTree *root = NULL;
    Node *head = NULL;
    FILE *dados, *biggest_comparisons;

    srand(time(NULL));

    dados = fopen("dados.txt", "w");
    biggest_comparisons = fopen("biggest_comparisons.txt", "w");

    for(int n = 1000; n <= 44000; n += 50){
        int numbersInserted[n], biggest_possible_comparison_list = n + 1, biggest_possible_comparison_tree = 0;
        createValuesToInsert(n, &head, &root, numbersInserted);

        biggest_comparison_tree(root, &biggest_possible_comparison_tree, 1);

        indice_random = rand() % n;
        fprintf(dados, "%d %d %d\n", n, searchInList(head, numbersInserted[indice_random]), searchInBST(root, numbersInserted[indice_random]));
        fprintf(biggest_comparisons, "%d %d %d\n", n, biggest_possible_comparison_list, biggest_possible_comparison_tree);

        freeTree(root);
        root = NULL;
        freeList(head);
        head = NULL;
    }

    fclose(dados);
    fclose(biggest_comparisons);

    printf("Finalizado!\n");

    return 0;
}
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define size 3
#define uint8_t unsigned char 

typedef struct tree_{
    long frequencia_do_byte;
    uint8_t byte;
    struct tree_ *next, *direita, *esquerda;
}COMPRESS_TREE;

void alocar_novo_no_em(COMPRESS_TREE** novo_node,
                        uint8_t           byte,
                        long           frequencia_do_byte)
                        {

    (*novo_node) = (COMPRESS_TREE*) malloc(sizeof(COMPRESS_TREE));
    if(novo_node == NULL){
        printf("--FALHA NA ALOCACAO DE NO DA ARVORE--\n");
        exit(1);
    }

    (*novo_node)->frequencia_do_byte = frequencia_do_byte;
    (*novo_node)->byte = byte;
    (*novo_node)->esquerda = NULL;
    (*novo_node)->direita = NULL;
    (*novo_node)->next = NULL;

    return;
}

void enqueue(COMPRESS_TREE** cabeca_da_fila,COMPRESS_TREE* novo_node){

    if((*cabeca_da_fila) == NULL || novo_node->frequencia_do_byte < (*cabeca_da_fila)->frequencia_do_byte){
        
        novo_node->next = *cabeca_da_fila;
        *cabeca_da_fila = novo_node;
        
    }else{

        COMPRESS_TREE* node_atual = *cabeca_da_fila;

        while(node_atual->next != NULL){

            if(novo_node->frequencia_do_byte < node_atual->next->frequencia_do_byte){
                break;
            }

            node_atual = node_atual->next;
        }

        novo_node->next = node_atual->next;
        node_atual->next = novo_node;
    }
}

void dequeue(COMPRESS_TREE** cabeca_da_fila,
             COMPRESS_TREE** no_dequeued ){

    if(*cabeca_da_fila == NULL){
        return;
    }
    COMPRESS_TREE* no_removido = (*cabeca_da_fila);
    
    (*cabeca_da_fila) = (*cabeca_da_fila)->next;
    
    no_removido->next = NULL;

    (*no_dequeued) = no_removido;
    return;
    
}

void criar_node_arvore(COMPRESS_TREE** raiz_da_arvore,
                       COMPRESS_TREE* sub_arvore_esquerda,
                       COMPRESS_TREE* sub_arvore_direita,
                       uint8_t no_interno)
{

    (*raiz_da_arvore) = (COMPRESS_TREE*) malloc(sizeof(COMPRESS_TREE));
    if((*raiz_da_arvore) == NULL){
        printf("FALHA NA ALOCACAO DE MEMORIA");
        exit(1);
    }

    long soma_das_frequencias = sub_arvore_esquerda->frequencia_do_byte + sub_arvore_direita->frequencia_do_byte;

    (*raiz_da_arvore)->byte = no_interno;
    (*raiz_da_arvore)->frequencia_do_byte = soma_das_frequencias;
    (*raiz_da_arvore)->esquerda = (sub_arvore_esquerda);
    (*raiz_da_arvore)->direita = (sub_arvore_direita);

    return;
};

void criar_arvore_de_huffman(COMPRESS_TREE** nova_arvore,
                             COMPRESS_TREE** fila_de_prioridade)
{
    uint8_t no_interno = '*';
    while((*fila_de_prioridade)->next != NULL){

        COMPRESS_TREE *sub_arvore_esquerda, *sub_arvore_direita, *raiz_da_arvore;

        dequeue(&(*fila_de_prioridade), &sub_arvore_esquerda);
        dequeue(&(*fila_de_prioridade), &sub_arvore_direita);

        criar_node_arvore(&raiz_da_arvore,sub_arvore_esquerda,sub_arvore_direita, no_interno);

        enqueue(&(*fila_de_prioridade),raiz_da_arvore);

    }
    dequeue(&(*fila_de_prioridade),&(*nova_arvore));

};

void teste_no(void){

    COMPRESS_TREE* novo_no = NULL;
    alocar_novo_no_em(&novo_no, '*', 15);

    CU_ASSERT_PTR_NOT_NULL(novo_no);
    CU_ASSERT_EQUAL(novo_no->frequencia_do_byte, 15);
    CU_ASSERT_EQUAL(novo_no -> byte,  '*');
    CU_ASSERT_PTR_NULL(novo_no->next);
    CU_ASSERT_PTR_NULL(novo_no->esquerda);
    CU_ASSERT_PTR_NULL(novo_no->direita);
    free(novo_no);

    alocar_novo_no_em(&novo_no,  'a' ,0);
    CU_ASSERT_PTR_NOT_NULL(novo_no);
    CU_ASSERT_EQUAL(novo_no -> frequencia_do_byte, 0);
    CU_ASSERT_EQUAL(novo_no -> byte, 'a');
    CU_ASSERT_PTR_NULL(novo_no->next);
    CU_ASSERT_PTR_NULL(novo_no->esquerda);
    CU_ASSERT_PTR_NULL(novo_no->direita);

    free(novo_no);
    
}

void teste_enqueue(void) {

    COMPRESS_TREE* fila = NULL;
    COMPRESS_TREE** exemplo = (COMPRESS_TREE**) malloc(size*sizeof(COMPRESS_TREE));

    for(int i = 0; i < size; i++){
        alocar_novo_no_em(&exemplo[i], 'a' + i , (i+1)*2);
    } 

    enqueue(&fila, exemplo[2]);
    CU_ASSERT_PTR_NOT_NULL(fila);
    CU_ASSERT_EQUAL(fila->frequencia_do_byte, 6);

    enqueue(&fila, exemplo[0]);
    CU_ASSERT_EQUAL(fila->frequencia_do_byte, 2);

    enqueue(&fila, exemplo[1]);

    CU_ASSERT_EQUAL(fila->frequencia_do_byte, 2);
    CU_ASSERT_EQUAL(fila->next->frequencia_do_byte, 4);

    free(fila);
    free(exemplo);

}

void teste_dequeue(void){
    
    COMPRESS_TREE* fila = NULL;
    COMPRESS_TREE* elementoRemovido = NULL;
    
    COMPRESS_TREE** exemplo = (COMPRESS_TREE**) malloc(size*sizeof(COMPRESS_TREE));

    for(int i = 0; i < size; i++){
        alocar_novo_no_em(&exemplo[i], 'a' + i, (i+1)*2);
    } 

    dequeue(&fila, &elementoRemovido);
    CU_ASSERT_PTR_NULL(fila);

    enqueue(&fila, exemplo[1]);
    CU_ASSERT_PTR_NOT_NULL(fila);

    enqueue(&fila, exemplo[0]);
    CU_ASSERT_PTR_NOT_NULL(fila);
    
    dequeue(&fila, &elementoRemovido);
    CU_ASSERT_PTR_NOT_NULL(fila);
    CU_ASSERT_EQUAL(elementoRemovido->frequencia_do_byte, 2);
    
    enqueue(&fila, exemplo[2]);
    dequeue(&fila, &elementoRemovido);
    CU_ASSERT_EQUAL(elementoRemovido->frequencia_do_byte, 4);

    dequeue(&fila, &elementoRemovido);
    CU_ASSERT_EQUAL(elementoRemovido->frequencia_do_byte, 6);
    CU_ASSERT_PTR_NULL(fila);

    dequeue(&fila, &elementoRemovido);
    CU_ASSERT_PTR_NULL(fila);
    
    free(exemplo);

}

void teste_criar_node_arvore(void){

    COMPRESS_TREE* raizDaArvore = NULL;
    COMPRESS_TREE** exemplo = (COMPRESS_TREE**) malloc(size*sizeof(COMPRESS_TREE));

    for(int i = 0; i < size; i++){
        alocar_novo_no_em(&exemplo[i], 'a' + i , (i+1)*2);
    } 

    criar_node_arvore( &raizDaArvore, exemplo[0], exemplo[1], '*');

    CU_ASSERT_PTR_NOT_NULL(raizDaArvore);
    CU_ASSERT_EQUAL(raizDaArvore -> frequencia_do_byte, 6);
    CU_ASSERT_EQUAL(raizDaArvore -> byte, '*');
    CU_ASSERT_EQUAL(raizDaArvore -> esquerda -> frequencia_do_byte, 2);
    CU_ASSERT_EQUAL(raizDaArvore -> direita -> frequencia_do_byte, 4);

    criar_node_arvore(&raizDaArvore, exemplo[2], raizDaArvore, '*');
    CU_ASSERT_EQUAL(raizDaArvore -> frequencia_do_byte, 12);
    CU_ASSERT_EQUAL(raizDaArvore -> byte, '*');
    CU_ASSERT_EQUAL(raizDaArvore -> esquerda -> frequencia_do_byte, 6);
    CU_ASSERT_EQUAL(raizDaArvore -> direita -> frequencia_do_byte, 6);
    
    free(raizDaArvore);
    free(exemplo);

}

void teste_arvore_de_huffman(void){
    
    COMPRESS_TREE*  fila = NULL;
    COMPRESS_TREE*  raizDaArvore = NULL;
    COMPRESS_TREE** exemplo = (COMPRESS_TREE**) malloc(size*sizeof(COMPRESS_TREE));

    for(int i = 0; i < size; i++){

        alocar_novo_no_em(&exemplo[i], 'a' + i , (i+1)*2);
        enqueue(&fila, exemplo[i]);

    } 

    criar_arvore_de_huffman(&raizDaArvore, &fila);

    CU_ASSERT_EQUAL(raizDaArvore -> frequencia_do_byte, 12);
    CU_ASSERT_EQUAL(raizDaArvore -> byte, '*');
    CU_ASSERT_EQUAL(raizDaArvore -> esquerda -> frequencia_do_byte, 6);
    CU_ASSERT_EQUAL(raizDaArvore -> direita -> frequencia_do_byte, 6);
    
    free(raizDaArvore);
    free(exemplo);

}

int main (void){

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite modulo_alocar_no = CU_add_suite("| alocacao do  no |", NULL, NULL);
    CU_ADD_TEST(modulo_alocar_no, teste_no);

    CU_pSuite modulo_fila = CU_add_suite("| fila |", NULL, NULL);
    CU_ADD_TEST(modulo_fila, teste_enqueue);
    CU_ADD_TEST(modulo_fila, teste_dequeue);

    CU_pSuite modulo_arvore = CU_add_suite("| Arvore de Huffman |", NULL,NULL);
    CU_ADD_TEST(modulo_arvore, teste_criar_node_arvore);
    CU_ADD_TEST(modulo_arvore, teste_arvore_de_huffman);
    
    CU_basic_set_mode(CU_BRM_VERBOSE); 

    CU_basic_run_tests(); 
    CU_cleanup_registry();

    return 0;
}

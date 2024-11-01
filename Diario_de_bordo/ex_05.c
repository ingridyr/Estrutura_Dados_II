// Aluna: Ingridy Rodrigues Fagundes
// Matricula: 202201383
// CC6N

#include <stdio.h>
#include <stdlib.h>

// define o grau mínimo (número mínimo de filhos) da árvore B
#define MIN_DEGREE 2

// estrutura para um nó da árvore B
typedef struct BTreeNode {
    int *keys;
    int maxKeys;
    struct BTreeNode **children;
    int currentKeys;
    int isLeaf;
} BTreeNode;

// função aux. para criar um novo nó na árvore B
BTreeNode* createNode(int isLeaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode)); // alocando memória para o novo nó
    newNode->maxKeys = 2 * MIN_DEGREE - 1;                      // definindo o máximo de chaves
    newNode->keys = (int*)malloc(newNode->maxKeys * sizeof(int)); // alocando memória para as chaves
    newNode->children = (BTreeNode**)malloc((newNode->maxKeys + 1) * sizeof(BTreeNode*)); // alocando memória para os filhos
    newNode->currentKeys = 0;                                   // inicializando o número atual de chaves
    newNode->isLeaf = isLeaf;                                   // definindo se o nó é uma folha
    return newNode;                                             // retornando o novo nó
}

// função aux. para dividir um filho
void divideNode(BTreeNode* parentNode, int childIndex) {
    BTreeNode* fullChild = parentNode->children[childIndex]; // filho que está cheio
    BTreeNode* newChild = createNode(fullChild->isLeaf); // criando um novo nó que se tornará o novo filho

    // move as últimas chaves do filho cheio para o novo filho
    for (int i = 0; i < MIN_DEGREE - 1; i++) {
        newChild->keys[i] = fullChild->keys[i + MIN_DEGREE];
    }

    // se não for uma folha, move os filhos do filho cheio para o novo filho
    if (!fullChild->isLeaf) {
        for (int i = 0; i < MIN_DEGREE; i++) {
            newChild->children[i] = fullChild->children[i + MIN_DEGREE];
        }
    }

    // atualiza o número de chaves nos nós
    fullChild->currentKeys = MIN_DEGREE - 1; // reduz as chaves no filho cheio
    newChild->currentKeys = MIN_DEGREE - 1;   // configura as chaves no novo filho

    // move a chave do meio do filho cheio para o nó pai
    for (int i = parentNode->currentKeys; i >= childIndex + 1; i--) {
        parentNode->children[i + 1] = parentNode->children[i];
    }
    parentNode->children[childIndex + 1] = newChild; // adicionando o novo filho ao pai

    // move a chave do meio para o nó pai
    for (int i = parentNode->currentKeys - 1; i >= childIndex; i--) {
        parentNode->keys[i + 1] = parentNode->keys[i];
    }
    parentNode->keys[childIndex] = fullChild->keys[MIN_DEGREE - 1]; // move a chave do meio para o pai

    parentNode->currentKeys++; // incrementa o número de chaves no nó pai
}

// função aux. para inserir uma chave em um nó não cheio
void insertNonFull(BTreeNode* node, int key) {
    int index = node->currentKeys - 1; // índice da nova chave a ser inserida

    // se o nó é uma folha, insere a chave diretamente
    if (node->isLeaf) {
        // encontra a posição certa para a nova chave
        while (index >= 0 && node->keys[index] > key) {
            node->keys[index + 1] = node->keys[index]; // move a chave para a direita
            index--;
        }
        node->keys[index + 1] = key; // insere a nova chave
        node->currentKeys++; // incrementa o número de chaves
    } else { // se o nó não é uma folha
        // encontra a posição do filho a ser usado
        while (index >= 0 && node->keys[index] > key) {
            index--;
        }
        index++;
        // verifica se o filho está cheio
        if (node->children[index]->currentKeys == node->children[index]->maxKeys) {
            // se o filho estiver cheio, divide o filho
            divideNode(node, index); // dividir o nó filho
            // após a divisão, verifica qual dos filhos usar
            if (node->keys[index] < key) {
                index++; // ajusta o índice se a nova chave for maior que a chave do nó
            }
        }
        insertNonFull(node->children[index], key); // insere recursivamente a chave no filho
    }
}

// função para inserir uma nova chave na árvore B
void insert(BTreeNode** root, int key) {
    BTreeNode* currentRoot = *root; // referenciando o nó raiz

    // se a raiz está cheia, cria uma nova raiz
    if (currentRoot->currentKeys == currentRoot->maxKeys) {
        BTreeNode* newRoot = createNode(0); // criando um novo nó que se será a nova raiz
        newRoot->children[0] = currentRoot; // o antigo nó raiz se torna o primeiro filho da nova raiz
        // divide a antiga raiz em dois
        divideNode(newRoot, 0); // dividir o nó antigo
        *root = newRoot; // atualizando a raiz
        // insere a nova chave na nova raiz
        insertNonFull(newRoot, key); // insere na nova raiz não cheia
    } else {
        insertNonFull(currentRoot, key); // se a raiz não está cheia, insere diretamente
    }
}

// função para exibir a árvore B inOrder (para visualizacao)
void inorderTraversal(BTreeNode* root) {
    if (root != NULL) {
        for (int i = 0; i < root->currentKeys; i++) {
            // para cada chave, imprime e segue o filho à esquerda
            if (!root->isLeaf) {
                inorderTraversal(root->children[i]); // exibe o filho antes da chave
            }
            printf("%d ", root->keys[i]); // imprime a chave
        }
        // exibe o último filho (se não for folha)
        if (!root->isLeaf) {
            inorderTraversal(root->children[root->currentKeys]);
        }
    }
}

int main() {
    BTreeNode* root = createNode(1); // criando a raiz como uma folha (1 - True)
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 6);
    insert(&root, 12);

    printf("Arvore:\n");
    inorderTraversal(root);
    printf("\n");

    return 0;
}

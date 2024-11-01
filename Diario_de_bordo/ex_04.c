// Aluna: Ingridy Rodrigues Fagundes
// Matricula: 202201383
// CC6N

#include <stdio.h>
#include <stdlib.h> // para a função rand()

typedef struct TreapNode {
    int key;
    int priority;
    struct TreapNode *leftChild;
    struct TreapNode *rightChild;
} TreapNode;

// função aux. para criar um novo nó na Treap
TreapNode* createNode(int nodeKey) {
    TreapNode* newNode = (TreapNode*)malloc(sizeof(TreapNode));
    newNode->key = nodeKey;
    newNode->priority = rand(); // gera uma prioridade aleatória
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}

// função aux. para rotacionar um nó para a direita
TreapNode* rotateRight(TreapNode* rootNode) {
    TreapNode* newRoot = rootNode->leftChild;
    TreapNode* tempTree = newRoot->rightChild;

    newRoot->rightChild = rootNode;
    rootNode->leftChild = tempTree;

    return newRoot;
}

// função aux. para rotacionar um nó para a esquerda
TreapNode* rotateLeft(TreapNode* rootNode) {
    TreapNode* newRoot = rootNode->rightChild;
    TreapNode* tempTree = newRoot->leftChild;

    newRoot->leftChild = rootNode;
    rootNode->rightChild = tempTree;

    return newRoot;
}

// função de inserção:
TreapNode* insert(TreapNode* rootNode, int nodeKey) {
    // verifica se o nó raiz é NULL (vazio), se sim, cria um novo com o valor da chave
    if (rootNode == NULL)
        return createNode(nodeKey);

    // verifica se o valor da chave do novo nó é menor do que a atual
    if (nodeKey < rootNode->key) {
        rootNode->leftChild = insert(rootNode->leftChild, nodeKey); // se sim, insere recursivamente à esquerda

        // verifica a prioridade do nó inserido e realiza rotação à direita, se necessário
        if (rootNode->leftChild->priority > rootNode->priority)
            rootNode = rotateRight(rootNode);
    }
    // se o valor da chave do novo nó é maior do que a atual
    else if (nodeKey > rootNode->key) {
        rootNode->rightChild = insert(rootNode->rightChild, nodeKey); // entao, insere recursivamente à direita

        // verifica a prioridade do nó inserido e realiza rotação à esquerda, se necessário
        if (rootNode->rightChild->priority > rootNode->priority)
            rootNode = rotateLeft(rootNode);
    }
    
    // e se a chave já existe, não faz nada apenas retorna o nó
    return rootNode;
}


// função para imprimir a Treap em ordem (para visualizacao)
void inorderTraversal(TreapNode* rootNode) {
    if (rootNode != NULL) {
        inorderTraversal(rootNode->leftChild);
        printf("Chave/valor: %d | Prioridade: %d\n", rootNode->key, rootNode->priority);
        inorderTraversal(rootNode->rightChild);
    }
}

int main(void) {
    TreapNode* rootNode = NULL;

    // inserção dos nós na Treap
    rootNode = insert(rootNode, 50);
    rootNode = insert(rootNode, 30);
    rootNode = insert(rootNode, 20);
    rootNode = insert(rootNode, 40);
    rootNode = insert(rootNode, 70);
    rootNode = insert(rootNode, 60);
    rootNode = insert(rootNode, 80);

    printf("Em ordem:\n");
    inorderTraversal(rootNode);

    return 0;
}
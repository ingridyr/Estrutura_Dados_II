// Aluna: Ingridy Rodrigues Fagundes
// Matricula: 202201383
// CC6N

#include <stdio.h>
#include <stdlib.h>

// definição das cores dos nós
#define RED 0
#define BLACK 1

// estrutura de um nó da árvore Red-Black
typedef struct Node {
    int value;              // valor do nó
    int color;              // cor do nó (vermelho ou preto)
    struct Node *left, *right, *parent; // ponteiros para os filhos e o pai
} Node;

// função para criar um novo nó
Node *createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node)); // aloca memória para um novo nó
    if (newNode == NULL) { // verifica se a alocação foi bem-sucedida
        fprintf(stderr, "erro ao alocar memória.\n");
        exit(1); // sai do programa se a alocação falhar
    }
    newNode->value = value; // define o valor do nó
    newNode->color = RED;   // todo nó inserido é inicialmente vermelho
    newNode->left = newNode->right = newNode->parent = NULL; // inicializa os ponteiros
    return newNode; // retorna o novo nó
}

// função aux para verificar se um nó é vermelho
int isRed(Node *node) {
    return node != NULL && node->color == RED; // retorna verdadeiro se o nó é vermelho
}

// função para realizar rotação à esquerda
void rotateLeft(Node **root, Node *pivotNode) {
    Node *rightChild = pivotNode->right; // armazena o filho à direita do nó pivotNode
    pivotNode->right = rightChild->left; // o filho à direita do nó pivotNode agora aponta para o filho à esquerda do rightChild

    if (rightChild->left != NULL) // se o filho à esquerda do rightChild não é nulo
        rightChild->left->parent = pivotNode; // entao ajusta o pai do filho à esquerda

    rightChild->parent = pivotNode->parent; // ajusta o pai do rightChild

    if (pivotNode->parent == NULL) // se o nó pivotNode é a raiz
        *root = rightChild; // entao o rightChild se torna a nova raiz
    else if (pivotNode == pivotNode->parent->left) // senao, se o pivotNode é filho à esquerda do pai
        pivotNode->parent->left = rightChild; // entao ajusta o filho à esquerda do pai
    else
        pivotNode->parent->right = rightChild; // se nao é a raiz nem o filho esquerdo, ajusta o filho à direita do pai

    rightChild->left = pivotNode; // o pivotNode agora é o filho à esquerda do rightChild
    pivotNode->parent = rightChild; // ajusta o pai do pivotNode
}

// função para realizar rotação à direita
void rotateRight(Node **root, Node *pivotNode) {
    Node *leftChild = pivotNode->left; // armazena o filho à esquerda do nó pivotNode (nó atual)
    pivotNode->left = leftChild->right; // o filho à esquerda do nó atual agora aponta para o filho à direita do leftChild

    if (leftChild->right != NULL) // se o filho à direita do leftChild não é nulo
        leftChild->right->parent = pivotNode; // ajusta o pai do filho à direita

    leftChild->parent = pivotNode->parent; // senao, ajusta o pai do leftChild

    if (pivotNode->parent == NULL) // se o nó pivotNode é a raiz
        *root = leftChild; // entao o leftChild se torna a nova raiz
    else if (pivotNode == pivotNode->parent->right) // senao, se o pivotNode é filho à direita do pai
        pivotNode->parent->right = leftChild; // entao ajusta o filho à direita do pai
    else
        pivotNode->parent->left = leftChild; // se nao é a raiz nem o filho direito, ajusta o filho à esquerda do pai

    leftChild->right = pivotNode; // o pivotNode agora é o filho à direita do leftChild
    pivotNode->parent = leftChild; // ajusta o pai do pivotNode
}

// função para recolorir o nó e seus filhos
void recolor(Node *parentNode, Node *uncleNode, Node *grandpaNode) {
    parentNode->color = BLACK; // muda a cor do nó pai para preto
    uncleNode->color = BLACK;  // muda a cor do nó tio para preto
    grandpaNode->color = RED; // muda a cor do nó avô para vermelho
}

// função para corrigir os erros de cor e balanceamento após inserção
void fixInsertion(Node **root, Node *newNode) {
    while (newNode != *root && isRed(newNode->parent)) { // enquanto o novo nó não for a raiz e o pai for vermelho
        Node *parentNode = newNode->parent; // armazena o nó pai e
        Node *grandpaNode = parentNode->parent; // armazena o nó avô

        if (parentNode == grandpaNode->left) { // se o pai for filho à esquerda do avô:
            Node *uncleNode = grandpaNode->right; // armazena o nó tio

            if (isRed(uncleNode)) { // e se o tio for vermelho
                recolor(parentNode, uncleNode, grandpaNode); // recolore os nós e
                newNode = grandpaNode; // sobe o novo nó para o avô
            } else {
                if (newNode == parentNode->right) { // se tio nao for vermelho e o novo nó for filho à direita do pai
                    rotateLeft(root, parentNode); // realiza rotação à esquerda no pai
                    newNode = parentNode; // o novo nó agora é o pai
                }
                rotateRight(root, grandpaNode); // se nao for filho à direita do pai, realiza rotação à direita no avô
                parentNode->color = BLACK; // muda a cor do pai para preto e 
                grandpaNode->color = RED; // muda a cor do avô para vermelho
            }
        } else { // senao (se o pai for filho à direita do avô)
            Node *uncleNode = grandpaNode->left; // armazena o nó tio

            if (isRed(uncleNode)) { // e se o tio for vermelho
                recolor(parentNode, uncleNode, grandpaNode); // recolore os nós e
                newNode = grandpaNode; // sobe o novo nó para o avô
            } else {
                if (newNode == parentNode->left) { // se tio nao for vermelho e se o novo nó for filho à esquerda do pai
                    rotateRight(root, parentNode); // realiza rotação à direita no pai e
                    newNode = parentNode; // o novo nó agora é o pai
                }
                rotateLeft(root, grandpaNode); // se nao for filho à esquerda do pai, realiza rotação à esquerda no avô
                parentNode->color = BLACK; // muda a cor do pai para preto e
                grandpaNode->color = RED; // muda a cor do avô para vermelho
            }
        }
    }

    (*root)->color = BLACK; // a raiz sempre deve ser preta
}

// função para inserir um novo nó na árvore Red-Black
void insertNode(Node **root, int value) {
    Node *newNode = createNode(value); // cria um novo nó
    Node *parentNode = NULL; // inicializa o ponteiro do pai
    Node *currentNode = *root; // inicializa o ponteiro atual como raiz

    // encontrando a posição correta na árvore para inserir o novo nó
    while (currentNode != NULL) {
        parentNode = currentNode; // armazena o nó atual como pai
        if (newNode->value < currentNode->value) // se o valor do novo nó for menor
            currentNode = currentNode->left; // move para a subárvore esquerda
        else
            currentNode = currentNode->right; // senao, move para a subárvore direita
    }

    newNode->parent = parentNode; // define o pai do novo nó
    if (parentNode == NULL) // se não há pai (nova raiz)
        *root = newNode; // define o novo nó como raiz
    else if (newNode->value < parentNode->value) // se o novo nó é menor que o pai
        parentNode->left = newNode; // insere na subárvore esquerda
    else
        parentNode->right = newNode; // senao, insere na subárvore direita

    // chama funcao para corrigir os possíveis erros da árvore após a inserção
    fixInsertion(root, newNode);
}

// função para imprimir a árvore Red-Black em ordem crescente
void inOrderTraversal(Node *root) {
    if (root != NULL) { // se o nó não é nulo
        inOrderTraversal(root->left); // percorre a subárvore esquerda
        printf("%d (%s) ", root->value, (root->color == RED) ? "vermelho" : "preto"); // imprime o valor e a cor do nó
        inOrderTraversal(root->right); // percorre a subárvore direita
    }
}

// função para imprimir a árvore de forma hierárquica
void printTree(Node *root, int space) {
    if (root == NULL) // se o nó é nulo
        return; // não faz nada

    space += 10; // aumenta o espaçamento

    // primeiro imprime o filho direito
    printTree(root->right, space);

    // imprime o nó atual após o espaçamento
    printf("\n");
    for (int i = 10; i < space; i++) // imprime espaços
        printf(" ");
    printf("%d (%s)\n", root->value, (root->color == RED) ? "V" : "P"); // imprime o valor e a cor do nó

    // agora imprime o filho esquerdo
    printTree(root->left, space);
}

int main() {
    Node *root = NULL; // inicializa a raiz como nula

    // inserindo nós na árvore
    insertNode(&root, 10);
    insertNode(&root, 20);
    insertNode(&root, 30);
    insertNode(&root, 15);
    insertNode(&root, 25);
    insertNode(&root, 5);
    insertNode(&root, 1);
    insertNode(&root, 8);
    insertNode(&root, 12);
    insertNode(&root, 18);
    insertNode(&root, 28);
    insertNode(&root, 35);

    // imprime a árvore em ordem
    printf("Impressao em ordem:\n");
    inOrderTraversal(root);
    printf("\n");

    // imprime a árvore de forma hierárquica
    printf("Impressao hierarquica:\n");
    printTree(root, 0);

    return 0;
}

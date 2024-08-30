// Aluna: Ingridy Rodrigues Fagundes
// Matricula: 202201383
// CC6N
//OBS: fiz no replit :D

#include <stdio.h>
#include <stdlib.h>

/*
Exercício 01:
• Crie em C a estrutura de uma árvore binária cuja informação seja um inteiro.
• Escreva funções que recebam um ponteiro para a raiz da árvore e façam:
• Inserção de um elemento na árvore;
• Pesquisa de um elemento na árvore;
• Exclusão de um elemento na árvore
*/

// criando a estrutura do tipo Node (nó)
typedef struct Node {
  int value;
  struct Node *left;
  struct Node *right;
} Node;

// protótipos das funções
Node *createNode(int value);
Node *insertElement(Node *root, int value);
Node *searchElement(Node *root, int value);
Node *deleteElement(Node *root, int value);
Node *nodeMin(Node *root);
void printTree(Node *root, int level);

// função para criar um novo nó
Node *createNode(int value) {
  // alocamos dinamicamente um espaço para o novo nó de acordo com seu tamanho
  Node *newNode = (Node *)malloc(sizeof(Node));

  if (newNode == NULL) {
    printf("Erro: Erro ao alocar memória na criação do novo nó.");
    exit(-1);
  }

  // atribuimos o valor do nó criado à sua variavel (atributo) do struct Node
  newNode->value = value;
  // inicializamos as demais variaveis (atributos) do struct com NULL e
  // retornamos o nó criado
  newNode->left = NULL;
  newNode->right = NULL;
  // retornamos o novo nó
  return newNode;
}

// definindo funçao de inserção
Node *insertElement(Node *root, int value) {

  // se nao existe raiz entao criamos um novo nó e atruibuimos o valor a ser
  // inserido
  if (root == NULL) {
    return createNode(value);
  }

  // se o valor a ser atribuido é maior do que o valor do nó atual
  if (value > root->value) {
    // entao inserimos recursivamente à direita da subarvore
    root->right = insertElement(root->right, value);
  } else {
    // senao inserimos recursivamente à esquerda da subarvore
    root->left = insertElement(root->left, value);
  }

  // por fim retornamos a raiz
  return root;
}

// definindo função de busca
Node *searchElement(Node *root, int value) {
  // primeiramente, verificamos se o valor existe ou se o valor é a própria
  // raiz, se sim retornamos-a
  if (root == NULL || root->value == value) {
    return root;
  }

  // depois, verificamos se o valor buscado é maior do que o do nó atual
  if (value > root->value) {
    // se sim, então fazemos uma busca recursiva ao valor na subárvore da
    // direita
    return searchElement(root->right, value);
  }

  // se não, significa que o valor procurado está na subárvore esquerda, então
  // buscamos recursivamente por lá
  return searchElement(root->left, value);
}

// definindo função de deleção
Node *deleteElement(Node *root, int value) {
  /*
  Caso 1: O nó a ser deletado tem apenas um filho. nó = filho
  Caso 2: O nó a ser deletado é uma folha. apenas remove-lo
  Caso 3: O nó a ser deletado tem dois filhos. Encontrar o menor nó na subárvore
  direita.
  */

  if (root == NULL) {
    return root;
  }

  // caso 1:
  //  se o valor a ser deletado é maior do que o do nó atual exclui na
  //  subarvore da direita
  if (value > root->value) {
    root->right = deleteElement(root->right, value);
    // se não, exclui na esquerda
  } else if (value < root->value) {
    root->left = deleteElement(root->left, value);

    // se nao é menor nem maior entao verificamos:
  } else {
    // caso 1 e 2: nó folha ou nó com um filho
    if (root->left == NULL) {
      Node *replacedNode = root->right;
      return replacedNode;
    } else if (root->right == NULL) {
      Node *replacedNode = root->left;
      return replacedNode;
    }

    // caso 3: Nó com dois filhos
    // nodeMin encontra o menor valor na subárvore direita
    Node *replacedNode = nodeMin(root->right);
    // substituimos o valor
    root->value = replacedNode->value;
    // excluimos o sucessor da subárvore direita
    root->right = deleteElement(root->right, replacedNode->value);
  }

  return root;
}

// função auxiliar para encontrar o menor valor da arvore
Node *nodeMin(Node *root) {
  while (root->left != NULL) {
    root = root->left;
  }
  return root;
}

// função auxiliar para imprimir a árvore (raiz posicionada da esq para a dir)
void printTree(Node *root, int level) {
  if (root == NULL) {
    return;
  }
  // imprime a subárvore direita
  printTree(root->right, level + 1);

  // imprime o valor do nó com espaçamento para representar o nível
  for (int i = 0; i < level; i++) {
    printf("    ");
  }
  printf("%d\n", root->value);

  // imprime a subárvore esquerda
  printTree(root->left, level + 1);
}

// função auxiliar para facilitar visualizacao dos resultados das buscas
void printSearch(Node *root, int value) {
  Node *element = searchElement(root, value);
  if (element != NULL) {
    printf("\nElemento %d encontrado.\n", element->value);
  } else {
    printf("\nElemento %d não encontrado na árvore.\n", value);
  }
}

// Função principal
int main() {
  Node *root = NULL;

  // Inserindo elementos na árvore
  root = insertElement(root, 50);
  root = insertElement(root, 30);
  root = insertElement(root, 70);
  root = insertElement(root, 20);
  root = insertElement(root, 40);
  root = insertElement(root, 60);
  root = insertElement(root, 80);

  // printar a árvore
  printf("----------------arvore íntegra:---------------\n\n");
  printTree(root, 0);
  printf("\n------------arvore após exclusao:-------------\n\n");
  // excluindo alguns elementos
  deleteElement(root, 60);
  deleteElement(root, 40);
  // printar a árvore após exclusao de elementos
  printTree(root, 0);
  printf("\n-----------------------------------------------\n");
  // printar a busca de um numero excluido
  printSearch(root, 60);
  // printar a busca de um numero que ainda existe na arvore
  printSearch(root, 80);

  return 0;
}
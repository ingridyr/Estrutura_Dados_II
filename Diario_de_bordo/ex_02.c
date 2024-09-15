// Aluna: Ingridy Rodrigues Fagundes
// Matricula: 202201383
// CC6N

#include <stdio.h>
#include <stdlib.h>

typedef struct AVLTree
{
  int value;
  struct AVLTree *left;
  struct AVLTree *right;
  int height;
} AVLTree;

// função para criar um novo nó
AVLTree *newNode(int value)
{
  AVLTree *newNode = (AVLTree *)malloc(sizeof(AVLTree));
  if (newNode == NULL)
  {
    printf("Erro: Falha ao alocar memória para o novo nó.\n");
    exit(-1);
  }

  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->height = 1;
  return newNode;
}

// função aux para calcular a altura do nó
int nodeHeight(AVLTree *node)
{
  if (node == NULL)
    return 0;
  return node->height;
}

// função aux para calcular o fator de balanceamento do nó
int balanceFactor(AVLTree *node)
{
  if (node == NULL)
    return 0;
  return nodeHeight(node->left) - nodeHeight(node->right);
}

// função aux para encontrar o máximo entre dois valores
int maxBetween(int a, int b)
{
  return (a > b) ? a : b;
}

// função para rotacionar à direita
AVLTree *rightRotate(AVLTree *node)
{
  AVLTree *newRoot = node->left;
  AVLTree *subTree = newRoot->right;

  // realizando a rotacao
  newRoot->right = node;
  node->left = subTree;

  // atualizando as alturas dos nós apos a rotacao
  node->height = 1 + maxBetween(nodeHeight(node->left), nodeHeight(node->right));
  newRoot->height = 1 + maxBetween(nodeHeight(newRoot->left), nodeHeight(newRoot->right));

  // retornando a nova raiz
  return newRoot;
}

// função para rotacionar à esquerda
AVLTree *leftRotate(AVLTree *node)
{
  AVLTree *newRoot = node->right;
  AVLTree *subTree = newRoot->left;

  // realizando a rotacao
  newRoot->left = node;
  node->right = subTree;

  // atualizando as alturas dos nós apos a rotacao
  node->height = 1 + maxBetween(nodeHeight(node->left), nodeHeight(node->right));
  newRoot->height = 1 + maxBetween(nodeHeight(newRoot->left), nodeHeight(newRoot->right));

  // retornando a nova raiz
  return newRoot;
}

// função para inserir um novo valor na árvore AVL
AVLTree *insertValue(AVLTree *node, int value)
{
  if (node == NULL)
    return newNode(value);

  if (value < node->value)
    node->left = insertValue(node->left, value);
  else if (value > node->value)
    node->right = insertValue(node->right, value);
  else
    return node; // caso o valor esteja duplicado

  // atualizando a altura do nó atual
  node->height = 1 + maxBetween(nodeHeight(node->left), nodeHeight(node->right));

  int balance = balanceFactor(node);

  // se o nó ficar desbalanceado, então:

  // caso 1: subárvore esquerda esquerda - rotacao simples à direita
  if (balance > 1 && value < node->left->value)
  {
    printf("Realizando rotacao simples a direita no no com valor %d\n", node->value);
    return rightRotate(node);
  }

  // caso 2: subárvore esquerda direita - rotacao dupla à direita
  if (balance > 1 && value > node->left->value)
  {
    node->left = leftRotate(node->left);
    printf("Realizando rotacao dupla a direita no no com valor %d\n", node->value);
    return rightRotate(node);
  }

  // caso 3: subárvore direita direita - rotacao simples à esquerda
  if (balance < -1 && value > node->right->value)
  {
    printf("Realizando rotacao simples a esquerda no no com valor %d\n", node->value);
    return leftRotate(node);
  }

  // caso 4: subárvore direita esquerda - rotacao dupla à esquerda
  if (balance < -1 && value < node->right->value)
  {
    node->right = rightRotate(node->right);
    printf("Realizando rotacao dupla a esquerda no no com valor %d\n", node->value);
    return leftRotate(node);
  }

  return node;
}

// função para encontrar o nó com o valor mínimo na subárvore direita
AVLTree *minValueNode(AVLTree *node)
{
  AVLTree *current = node;
  while (current->left != NULL)
    current = current->left;
  return current;
}

// função para deletar um valor da árvore AVL
AVLTree *deleteNode(AVLTree *root, int value)
{
  if (root == NULL)
    return root;

  // encontrar o nó a ser deletado
  if (value < root->value)
    root->left = deleteNode(root->left, value);
  else if (value > root->value)
    root->right = deleteNode(root->right, value);
  else
  {
    if (root->left == NULL)
    {
      AVLTree *temp = root->right;
      free(root);
      return temp;
    }
    else if (root->right == NULL)
    {
      AVLTree *temp = root->left;
      free(root);
      return temp;
    }

    AVLTree *temp = minValueNode(root->right);
    root->value = temp->value;
    root->right = deleteNode(root->right, temp->value);
  }

  // atualizando a altura do nó atual
  root->height = 1 + maxBetween(nodeHeight(root->left), nodeHeight(root->right));

  // calculando o fator de balanceamento
  int balance = balanceFactor(root);

  // novamente, se o nó ficar desbalanceado, então:

  // caso 1: subárvore esquerda esquerda - rotacao simples à direita
  if (balance > 1 && balanceFactor(root->left) >= 0)
  {
    printf("Realizando rotacao simples a direita no no com valor %d\n", root->value);
    return rightRotate(root);
  }

  // caso 2: subárvore esquerda direita - rotacao dupla à direita
  if (balance > 1 && balanceFactor(root->left) < 0)
  {
    root->left = leftRotate(root->left);
    printf("Realizando rotacao dupla a direita no no com valor %d\n", root->value);
    return rightRotate(root);
  }

  // caso 3: subárvore direita direita - rotacao simples à esquerda
  if (balance < -1 && balanceFactor(root->right) <= 0)
  {
    printf("Realizando rotacao simples a esquerda no no com valor %d\n", root->value);
    return leftRotate(root);
  }

  // caso 4: subárvore direita esquerda - rotacao dupla à esquerda
  if (balance < -1 && balanceFactor(root->right) > 0)
  {
    root->right = rightRotate(root->right);
    printf("Realizando rotacao dupla a esquerda no no com valor %d\n", root->value);
    return leftRotate(root);
  }

  return root;
}

// função para buscar um valor na árvore AVL
AVLTree *searchValue(AVLTree *root, int value)
{
  if (root == NULL || root->value == value)
    return root;

  if (value < root->value)
    return searchValue(root->left, value);

  return searchValue(root->right, value);
}

// função aux para imprimir a árvore de forma visual
void printTree(AVLTree *root, int level)
{
  if (root == NULL)
    return;

  printTree(root->right, level + 1);
  printf("\n");
  for (int i = 0; i < level; i++)
    printf("   ");
  printf("%d", root->value);
  printTree(root->left, level + 1);
}

// função aux para mostrar o menu e processar opções
void menu()
{
  printf("Menu:\n");
  printf("1. Inserir valor\n");
  printf("2. Deletar valor\n");
  printf("3. Imprimir arvore\n");
  printf("4. Sair\n");
}

int main()
{
  AVLTree *root = NULL;
  int choice, value;

  while (1)
  {
    menu();
    printf("Escolha uma opcao: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      printf("Digite o valor a ser inserido: ");
      scanf("%d", &value);
      root = insertValue(root, value);
      printf("Valor %d inserido.\n", value);
      break;

    case 2:
      printf("Digite o valor a ser deletado: ");
      scanf("%d", &value);
      root = deleteNode(root, value);
      printf("Valor %d deletado.\n", value);
      break;

    case 3:
      printf("Arvore atual:\n");
      printTree(root, 0);
      printf("\n");
      break;

    case 4:
      printf("Saindo...\n");
      exit(0);

    default:
      printf("Opcao invalida! Tente novamente.\n");
    }
  }

  return 0;
}

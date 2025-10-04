#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Estructura del nodo
typedef struct{
   int sudo[9][9];
}Node;

// Se crea un nodo vacio en la memoria
Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

// Se crea una copia del nodo
Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

// Se lee un sudoku inicial desde el archivo
Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  // Se cargan los datos en una matriz
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

// Se imprime el sudoku 
void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

// Se verifica si un estado es valido:
// No se deben repetir numerosen la filas, columnas ni en las submatrices 3x3
int is_valid(Node* n){
    int i,j,k;

    // Se verifican o revisan las filas
    for(i=0;i<9;i++){
        int seen[10]={0}; // Arreglo para marcar los numeros vistos
        for(j=0;j<9;j++){
            int val = n->sudo[i][j];
            if(val!=0){
                if(seen[val]) return 0;
                seen[val]=1;
            }
        }
    }

    // Se verifican o revisan las columnas
    for(j=0;j<9;j++){
        int seen[10]={0};
        for(i=0;i<9;i++){
            int val = n->sudo[i][j];
            if(val!=0){
                if(seen[val]) return 0;
                seen[val]=1;
            }
        }
    }

    // Se verifican o revisan las submatrices 3x3
    for(k=0;k<9;k++){
        int seen[10]={0};
        for(int p=0;p<9;p++){ // Recorre cada celda de la submatriz
            int i = 3*(k/3) + (p/3); // Fila
            int j = 3*(k%3) + (p%3); // Columna
            int val = n->sudo[i][j];
            if(val!=0){
                if(seen[val]) return 0;
                seen[val]=1;
            }
        }
    }

    return 1; // Si no se encontraron conflictos, el estado es valido
}

// Se generan los nodos adyacentes a un nodo dado
// Se busca la primera casilla vacia y se coloca en ella valores de 1 a 9, retornando solamente los nodos que resulten validos.
List* get_adj_nodes(Node* n){
    List* list = createList();
    int i,j,found=0;

    // Se busca la primera casilla vacia
    for(i=0;i<9 && !found;i++){
        for(j=0;j<9;j++){
            if(n->sudo[i][j] == 0){
                found=1; 
                // Probar valores del 1 al 9
                for(int k=1;k<=9;k++){
                    Node* nuevo = copy(n);
                    nuevo->sudo[i][j] = k;
                    // Validar el nodo antes de añadirlo a la lista
                    if(is_valid(nuevo))
                        pushBack(list, nuevo);
                    else
                        free(nuevo); // Se descarta si no sirve
                }
                break; // Salir del ciclo interno
            }
        }
    }
    return list;
}

// Se verifica si el nodo es un estado final, osea con solucion
int is_final(Node* n){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(n->sudo[i][j]==0) return 0; // Si tiene algun cero, no esta listo
        }
    }
    return 1; // No hay ceros por ende esta listo
}

// Parte desde el nodo inicial y explora el grafo implicito usando una busqueda en profundidad
// luego retorna el nodo final si encuentra una solucion o NULL si no la encuentra
Node* DFS(Node* n, int* cont){ // Cont lleva el conteo de iteraciones
    Stack* S = createStack();
    push(S, n); // Se inserta el nodo inicial en la pila

    while(!is_empty(S)){
        Node* curr = top(S); // Se toma el nodo de encima
        pop(S); // Se saca de la pila
        (*cont)++; // Se aumenta el contador

        if(is_final(curr)){
            return curr; // Solucion encontrada
        }

        // Se generan y agragan nodos adyacentes validos a la pila
        List* adjs = get_adj_nodes(curr);
        Node* adj = first(adjs);
        while(adj!=NULL){
            push(S, adj);
            adj = next(adjs);
        }

        free(curr); // Se libera la memoria
    }
    return NULL; // No se encontro solucion
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/
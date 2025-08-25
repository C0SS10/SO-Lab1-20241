#include <stdio.h>                                                                                
#include <stdlib.h>
#include <string.h> 

int main(int argc, char* argv[]){
  FILE *input = stdin;
  FILE *output = stdout;

  // Si se reciben más de dos argumentos
  if(argc > 3){
      fprintf(stderr, "El uso de la función es: ./reverse <input.txt> <output.txt>");
      exit(1);
  }

  // Si se recibe un archivo que no existe
  if(argc == 2){
      input = fopen(argv[1], "r");
      if(!input){
          fprintf(stderr, "No se pudo abrir el archivo de entrada: %s\n", argv[1]);
          exit(1);
      }
  }

  if(argc == 3){
      output = fopen(argv[2], "w");
      if(!output){
          fprintf(stderr, "No se pudo abrir el archivo de salida: %s\n", argv[2]);
          exit(1);
      }
  }

  return 0;
}

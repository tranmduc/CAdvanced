#include "main.h"

int main(){
  int op;

  do{
    printf("----------------------\n");
    printf("MENU\n");
    printf("1. \n");
    printf("2. \n");
    printf("3. \n");
    printf("4. \n");
    printf("5. \n");
    printf("6. \n");
    do{
      printf("Enter: ");
      scanf(" %d", &op);
    }while(op> 6 || op<1);

    switch(op){
    case 1:

      break;

    case  2:

      break;

    case  3:

      break;

    case  4:

      break;


    }
    
  }while(op != 4);
    
  return 0;
}


int main(){
  int op;

  do{
    printf("----------------------\n");
    printf("MENU\n");
    printf("1. Simulate connection\n");
    printf("2. Show all forwarding table\n");
    printf("3. Show all link state table\n");
    printf("4. Modify network\n");
//    printf("5. \n");
//    printf("6. \n");
    do{
      printf("Enter: ");
      scanf(" %d", &op);
    }while(op> 4 || op<1);

    switch(op){
    case 1:
      printf("Choose 1");
      break;

    case  2:
      printf("Choose 2");
      break;

    case  3:
      printf("Choose 3");
      break;

    case  4:
      printf("Choose 4");
      break;


    }
    
  }while(op != 4);
    
  return 0;
}


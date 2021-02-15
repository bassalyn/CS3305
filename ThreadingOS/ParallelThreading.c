#include <stdio.h>
#include <stdlib.h>       //include libraries
#include <string.h>
#include <pthread.h>

struct bankAccount{        //define a struct for a bank account
  int number;
  int type;               //attributes to store values for bank account
  int balance;
  int depositfee;
  int withdrawfee;
  int transferfee;
  int transactionfee;     //store fees for each transaction
  int transactions;
  int overdraft;
  int overdraftfee;     //counter for overdraft
  int overcounter;
};

struct orderedTuple{
  int account1;         //struct to store two a transation two accounts and amount
  int account2;
  int amount;
};
pthread_mutex_t lock;     // implementation of semaphore using pthread
struct bankAccount accounts[50];    //struct to handle up to 50 accounts

int dthreads =0;
int cthreads =0;                    //counter for deposit and customer threads

void *deposit(void *input){         //deposit method for pthread

    struct orderedTuple *data = input;
    int account = data->account1;       //parse input into account and amount
    int amount = data->amount;

    pthread_mutex_lock(&lock);        //lock for critical section

    if (accounts[account].transactions > 0){    //if free transactions left checks if valid trasaction
      if (((accounts[account].balance+amount-accounts[account].depositfee)<0)&&accounts[account].overdraft==0
    || (accounts[account].balance+amount-accounts[account].depositfee)< -5000){
        pthread_mutex_unlock(&lock);      //if invalid unlocks critical section exits thread
        pthread_exit(NULL);
      }
    }else{                        //else checks if valid transaction with deposit fee
      if (((accounts[account].balance+amount-accounts[account].depositfee-accounts[account].transactionfee)<0)&&accounts[account].overdraft==0
    || (accounts[account].balance+amount-accounts[account].depositfee-accounts[account].transactionfee)< -5000){
        pthread_mutex_unlock(&lock);    //unlocks semophore and exits thread
        pthread_exit(NULL);
      }
    }
    accounts[account].balance += amount;        //adds amount to account
    accounts[account].balance -= accounts[account].depositfee;
    if (accounts[account].transactions < 1){              //remove fee value from account
      accounts[account].balance -= accounts[account].transactionfee;
    }
    accounts[account].transactions -=1;             //increments transaction
    int count1 =0;
    int count2 =0;                    //stores value of overdraft fee and number of times
    while (accounts[account].balance < (accounts[account].overcounter*(-500))){
      accounts[account].balance -= accounts[account].overdraftfee;  //handles the overdraft fee
      count1 += accounts[account].overdraftfee;
      accounts[account].overcounter+=1;
      count2 ++;
    }
    if (accounts[account].balance < -5000){   //if transaction is below minimum value reverses all ops
      accounts[account].balance +=count1;
      accounts[account].overcounter-=count2;          //returns to pre operation values
      accounts[account].transactions++;
      if (accounts[account].transactions < 1){
        accounts[account].balance += accounts[account].transactionfee;
      }
      accounts[account].balance -= amount;
      accounts[account].balance += accounts[account].depositfee;
    }
    pthread_mutex_unlock(&lock);    //unlocks the semaphore for end of critical section exits thread
    pthread_exit(NULL);
}

void *withdraw (void *input){     //withraw method for phthreads

  struct orderedTuple *data = input;
  int account = data->account1;           //converts input to account and amount ints
  int amount = data->amount;

  pthread_mutex_lock(&lock);                //locks for critical section semaphore
    if (accounts[account].transactions > 0){
      if (((accounts[account].balance-amount-accounts[account].withdrawfee)<0)&&accounts[account].overdraft==0
    ||(accounts[account].balance-amount-accounts[account].withdrawfee)< -5000){
        pthread_mutex_unlock(&lock);        //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }else{
      if (((accounts[account].balance-amount-accounts[account].withdrawfee-accounts[account].transactionfee)<0)&&accounts[account].overdraft==0
    || (accounts[account].balance-amount-accounts[account].withdrawfee-accounts[account].transactionfee)< -5000){
        pthread_mutex_unlock(&lock);      //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }
    accounts[account].balance -= amount;    //removes amount from account
    accounts[account].balance -= accounts[account].withdrawfee;
    if (accounts[account].transactions < 1){
      accounts[account].balance -= accounts[account].transactionfee;
    }
    accounts[account].transactions -=1;      //applies fees and decrements transactions
    int count1 =0;
    int count2 =0;
    while (accounts[account].balance < (accounts[account].overcounter*(-500))){
      accounts[account].balance -= accounts[account].overdraftfee;
      count1 += accounts[account].overdraftfee;       //applies overdraft fee and counts value
      accounts[account].overcounter+=1;
      count2 ++;
    }
    if (accounts[account].balance < -5000){
      accounts[account].balance +=count1;         //if invalid transaction returns values to pre op
      accounts[account].overcounter-=count2;
      accounts[account].transactions++;
      if (accounts[account].transactions < 1){
        accounts[account].balance += accounts[account].transactionfee;
      }
      accounts[account].balance += amount;
      accounts[account].balance += accounts[account].depositfee;
    }
    pthread_mutex_unlock(&lock);                //unlocks semaphore and exits thread
    pthread_exit(NULL);
}
void *transfer (void *input){       //fucntion to transfer between accounts

    struct orderedTuple *data = input;
    int account1 = data->account1;
    int account2 = data->account2;        //converts input data to ints
    int amount = data->amount;

    pthread_mutex_lock(&lock);        //locks semaphore for critical section
    if (accounts[account1].transactions > 0){
      if (((accounts[account1].balance-amount-accounts[account1].transferfee)<0)&&accounts[account1].overdraft==0
    || (accounts[account1].balance-amount-accounts[account1].withdrawfee-accounts[account1].transferfee)< -5000){
        pthread_mutex_unlock(&lock);          //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }else{
      if (((accounts[account1].balance-amount-accounts[account1].transferfee-accounts[account1].transactionfee)<0)&&accounts[account1].overdraft==0
    || (accounts[account1].balance-amount-accounts[account1].withdrawfee-accounts[account1].transferfee-accounts[account1].transactionfee)< -5000){
        pthread_mutex_unlock(&lock);          //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }
    if (accounts[account2].transactions > 0){
      if (((accounts[account2].balance+amount-accounts[account2].transferfee)<0)&&accounts[account2].overdraft==0
    || (accounts[account2].balance-amount-accounts[account2].withdrawfee-accounts[account2].transferfee)< -5000){
        pthread_mutex_unlock(&lock);          //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }else{
      if (((accounts[account2].balance+amount-accounts[account2].transferfee-accounts[account2].transactionfee)<0)&&accounts[account2].overdraft==0
    || (accounts[account2].balance-amount-accounts[account2].withdrawfee-accounts[account2].transferfee-accounts[account2].transactionfee)< -5000){
        pthread_mutex_unlock(&lock);          //if invalid transaction unlocks and exits
        pthread_exit(NULL);
      }
    }
    accounts[account1].balance -= amount;
    accounts[account1].balance -= accounts[account1].transferfee;
    accounts[account2].balance += amount;         //transfers ammount and applies fees
    accounts[account2].balance -= accounts[account2].transferfee;
    if (accounts[account1].transactions < 1){
      accounts[account1].balance -= accounts[account1].transactionfee;
    }
    if (accounts[account2].transactions < 1){
      accounts[account2].balance -= accounts[account2].transactionfee;
    }
    accounts[account1].transactions -=1;
    accounts[account2].transactions -=1;      //decrement transactions
    int count1 =0;
    int count2 =0;          //counters for overdraft
    int count3 =0;
    int count4 =0;
    while (accounts[account1].balance < (accounts[account1].overcounter*(-500))){
      accounts[account1].balance -= accounts[account1].overdraftfee;
      count1 += accounts[account1].overdraftfee;    //applies overdraft fees and counters
      accounts[account1].overcounter+=1;
      count2 ++;
    }
    while (accounts[account2].balance < (accounts[account2].overcounter*(-500))){
      accounts[account2].balance -= accounts[account2].overdraftfee;
      count3 += accounts[account2].overdraftfee;
      accounts[account2].overcounter+=1;    //applies overdraft fees and counters
      count4 ++;
    }
    if (accounts[account1].balance < -5000 || accounts[account2].balance < -5000 ){
      accounts[account1].balance +=count1;        //if either account below minimum overdraft value
      accounts[account1].overcounter-=count2;
      accounts[account1].transactions++;
      if (accounts[account1].transactions < 1){         //returns values to pre transaction using counter
        accounts[account1].balance += accounts[account1].transactionfee;
      }
      accounts[account1].balance += amount;
      accounts[account1].balance += accounts[account1].transferfee;
      accounts[account2].balance +=count3;
      accounts[account2].overcounter-=count4;       //returns values to pre transaction using counter
      accounts[account2].transactions++;
      if (accounts[account2].transactions < 1){
        accounts[account2].balance += accounts[account1].transactionfee;
      }
      accounts[account2].balance -= amount;
      accounts[account2].balance += accounts[account2].transferfee;
    }
    pthread_mutex_unlock(&lock);  //unlocks semaphore and exits phthread
    pthread_exit(NULL);
}

void print_output(struct bankAccount account){    //function to print the value of an account
  FILE *fptr;
  fptr = fopen("assignment_3_output_file.txt", "a");
  if (account.type == 0){                       //prints to output file, checks type and prints
    fprintf(fptr,"a%d type personal %d \n", account.number, account.balance);
  }else{
    fprintf(fptr,"a%d type business %d \n", account.number, account.balance);

  }
}


int main(){                           //main function
  pthread_mutex_init(&lock, NULL);
  remove("assignment_3_output_file.txt");
  int size = 0;
  FILE* fptr;
  char delim[] = " ";                  //declares vars, ptr, and delim, char
  fptr = fopen("assignment_3_input_file.txt", "r"); //opens input file
  char line[256];
  pthread_t threads1[200];
  pthread_t threads2[200];                    //handles up to 200 threads for deposit or customer
  struct orderedTuple ddata[200];
  struct orderedTuple cdata[200];         //struct to hold transaction data
  char type[200];
  int priority[200];
  int prioritycounter = 1;
  while (fgets(line, sizeof(line), fptr) && (line[0] == 'a')){
    accounts[size].number = size +1;      //parse for accounts
    accounts[size].balance = 0;
    accounts[size].overcounter = 0;
    char *ptr = strtok(line, delim);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    if (strcmp(ptr,"personal")==0){
      accounts[size].type = 0;          //gets account values and stores to struct
    }else{
      accounts[size].type = 1;
    }
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].depositfee = atoi(ptr);      //converts to int from string
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].withdrawfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);          //gets account values and stores to struct
    accounts[size].transferfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    accounts[size].transactions = atoi(ptr);
    ptr = strtok(NULL, delim);
    accounts[size].transactionfee = atoi(ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    if (strcmp(ptr,"Y")==0){
      accounts[size].overdraft = 1;
      ptr = strtok(NULL, delim);          //gets account values and stores to struct
      accounts[size].overdraftfee = atoi(ptr);
    }else{
      accounts[size].overdraft = 0;
      accounts[size].overdraftfee = 0;
    }
    size++;
  }
  char *ptr = strtok(line, delim);
  int act, amt;
  ptr = strtok(NULL, delim);
  ptr = strtok(NULL, delim);
  while (ptr != NULL){          //parse first deposit
    if (strlen(ptr)==2){
      char num[1] = "";
      num[0] = ptr[1];
      act = atoi(num);
    }else if (strlen(ptr)==3){
      char num[2] = "";
      num[0] = ptr[1];
      num[1] = ptr[2];
      act = atoi(num);
    }                       //saves values and converts to int
    ptr = strtok(NULL, delim);
    amt = atoi(ptr);
    struct orderedTuple newval;
    newval.account1 = (act-1);          //creates stuct for transaction
    newval.amount = amt;
    ddata[dthreads] = newval;
    dthreads++;                     //adds to deposit data and increments
    if (ptr != NULL){
      ptr = strtok(NULL, delim);
    }
    if (ptr != NULL){
      ptr = strtok(NULL, delim);
    }
  }

  while (fgets(line, sizeof(line), fptr)){    //parse for depositors
    char *ptr = strtok(line, delim);
    int act1, act2, amt;
    if (line[0] != 'd'){
      while (ptr != NULL){            //for first line after depositors
        ptr = strtok(NULL, delim);
        if (ptr == NULL){
          break;
        }
        if (strcmp(ptr,"d")==0){    //if deposit transaction
          ptr = strtok(NULL, delim);
          if (strlen(ptr)==2){
            char num[1] = "";
            num[0] = ptr[1];
            act1 = atoi(num);
          }else if (strlen(ptr)==3){    //parses line and stores account and value as int
            char num[2] = "";
            num[0] = ptr[1];
            num[1] = ptr[2];
            act1 = atoi(num);
          }
          ptr = strtok(NULL, delim);
          amt = atoi(ptr);            //makes struct for values
          struct orderedTuple newval;
          newval.account1 = (act1-1);
          newval.amount = amt;
          cdata[cthreads] = newval;
          type[cthreads] = 'd';
          priority[cthreads] = prioritycounter;
          prioritycounter++;
          cthreads++;
        }else if (strcmp(ptr, "w")==0){   //parses line if withraw
          ptr = strtok(NULL, delim);
          if (strlen(ptr)==2){
            char num[1] = "";
            num[0] = ptr[1];
            act1 = atoi(num);
          }else if (strlen(ptr)==3){
            char num[2] = "";
            num[0] = ptr[1];
            num[1] = ptr[2];
            act1 = atoi(num);
          }
          ptr = strtok(NULL, delim);
          amt = atoi(ptr);
          struct orderedTuple newval;
          newval.account1 = (act1-1);
          newval.amount = amt;              //creates struct and stores and increments
          cdata[cthreads] = newval;
          type[cthreads] = 'w';
          priority[cthreads] = prioritycounter;
          prioritycounter++;
          cthreads++;
        }else if (strcmp(ptr, "t")==0){       //if transfer parses stores values
          ptr = strtok(NULL, delim);
          if (strlen(ptr)==2){
            char num[1] = "";
            num[0] = ptr[1];
            act1 = atoi(num);
          }else if (strlen(ptr)==3){
            char num[2] = "";
            num[0] = ptr[1];
            num[1] = ptr[2];
            act1 = atoi(num);
          }
          ptr = strtok(NULL, delim);
          if (strlen(ptr)==2){
            char num[1] = "";             //parses and stores values
            num[0] = ptr[1];
            act2 = atoi(num);
          }else if (strlen(ptr)==3){
            char num[2] = "";
            num[0] = ptr[1];
            num[1] = ptr[2];
            act2 = atoi(num);
          }
          ptr = strtok(NULL, delim);
          amt = atoi(ptr);
          struct orderedTuple newval;       //creates struct and then stores in array
          newval.account1 = (act1-1);
          newval.account2 = (act2-1);
          newval.amount = amt;
          cdata[cthreads] = newval;
          type[cthreads] = 't';             //increments
          priority[cthreads] = prioritycounter;
          prioritycounter++;
          cthreads++;
        }
      }
      break;
    }
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    while (ptr != NULL){          //parses for depositors
      if (strlen(ptr)==2){
        char num[1] = "";
        num[0] = ptr[1];
        act1 = atoi(num);
      }else if (strlen(ptr)==3){
        char num[2] = "";
        num[0] = ptr[1];
        num[1] = ptr[2];
        act1 = atoi(num);
      }
      ptr = strtok(NULL, delim);      //creates object to store values
      amt = atoi(ptr);
      struct orderedTuple newval;
      newval.account1 = (act1-1);
      newval.amount = amt;
      ddata[dthreads] = newval;       //adds to array and increments counter
      dthreads++;
      if (ptr != NULL){
        ptr = strtok(NULL, delim);
      }
      if (ptr != NULL){
        ptr = strtok(NULL, delim);
      }
    }
  }

  while (fgets(line, sizeof(line), fptr)){
    prioritycounter = 1;
    char *ptr = strtok(line, delim);
    int act1,act2, amt;
    while (ptr != NULL){          //parses customer transactions
      ptr = strtok(NULL, delim);
      if (ptr == NULL){
        break;
      }
      if (strcmp(ptr,"d")==0){
        ptr = strtok(NULL, delim);      //if deposit parses
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct orderedTuple newval;     //creates struct and stores vals
        newval.account1 = (act1-1);
        newval.amount = amt;
        cdata[cthreads] = newval;
        type[cthreads] = 'd';
        priority[cthreads] = prioritycounter;
        prioritycounter++;
        cthreads++;
      }else if (strcmp(ptr, "w")==0){     //if withraw trans parses and stores vals
        ptr = strtok(NULL, delim);
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct orderedTuple newval;
        newval.account1 = (act1-1);     //creates struct and adds to array and increments
        newval.amount = amt;
        cdata[cthreads] = newval;
        type[cthreads] = 'w';
        priority[cthreads] = prioritycounter;
        prioritycounter++;
        cthreads++;
      }else if (strcmp(ptr, "t")==0){
        ptr = strtok(NULL, delim);        //if transaction parses and stores
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act1 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act1 = atoi(num);
        }
        ptr = strtok(NULL, delim);    //parses values for both accounts
        if (strlen(ptr)==2){
          char num[1] = "";
          num[0] = ptr[1];
          act2 = atoi(num);
        }else if (strlen(ptr)==3){
          char num[2] = "";
          num[0] = ptr[1];
          num[1] = ptr[2];
          act2 = atoi(num);
        }
        ptr = strtok(NULL, delim);
        amt = atoi(ptr);
        struct orderedTuple newval;       //creates struct and stores in array, increments
        newval.account1 = (act1-1);
        newval.account2 = (act2-1);
        newval.amount = amt;
        cdata[cthreads] = newval;
        type[cthreads] = 't';
        priority[cthreads] = prioritycounter;
        prioritycounter++;
        cthreads++;
      }
    }
  }
  for (int i = 0; i<dthreads;i++){      //first does all the depositer threaads
      pthread_create(&threads1[i], NULL, &deposit, &ddata[i]);
  }
  for (int i = 0; i<dthreads;i++){      //joins the depositer threads
    pthread_join(threads1[i], NULL);
  }
  for (int i = 0; i<cthreads;i++){    //does all the customer threads according to type
    for (int j = 1; j<30; j++){
      if (type[i] == 'd' && priority[i] == j){
        pthread_create(&threads2[i], NULL, &deposit, &cdata[i]);
      }else if (type[i] == 'w' && priority[i] == j){
        pthread_create(&threads2[i], NULL, &withdraw, &cdata[i]);
      }else if (type[i] == 't' && priority[i] == j){
        pthread_create(&threads2[i], NULL, &transfer, &cdata[i]);
      }
    }

  }
  for (int i = 0; i<cthreads;i++){      //ends all the customer threads
    pthread_join(threads2[i], NULL);
  }
  pthread_mutex_destroy(&lock);         //deletes the semaphore

  for (int i =size-1; i>=0; i--){     //prints the values to the output file
    print_output(accounts[i]);
  }
  return 0;
}

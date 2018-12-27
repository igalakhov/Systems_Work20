#include "pipe_networking.h"

static void sighandler(int n){
  if(n == SIGINT){
    printf("[Client] exiting...\n");
    char pid_str[15];
    int pid = getpid();
    snprintf(pid_str, 10,"%d",(int)getpid());
    remove(pid_str);
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_server;
  int from_server;
  char msg_buff[BUFFER_SIZE];

  from_server = client_handshake( &to_server );

  while(1){
    printf("[Client %d] enter message to send: ", getpid());
    fflush(stdout);
    fgets(msg_buff, BUFFER_SIZE, stdin);
    msg_buff[strlen(msg_buff) - 1] = 0; // null terminate
    printf(" sending [%s]\n", msg_buff);
    write(to_server, msg_buff, BUFFER_SIZE);
    read(from_server, msg_buff, BUFFER_SIZE);
    printf(" server says [%s]\n", msg_buff);
  }
}

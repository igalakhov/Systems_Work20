#include "pipe_networking.h"

static void sighandler(int n){
  if(n == SIGINT){
    remove(WKP_NAME);
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;

  while(1){
    printf("[Server] Waiting for connection...\n");
    mkfifo(WKP_NAME, 0666);
    from_client = open(WKP_NAME, O_RDONLY);
    printf("[Server] Client connected! Forking...\n");
    if(!fork()){
      to_client = server_handshake(from_client);
      char buf[BUFFER_SIZE];
      while(read(from_client, buf, BUFFER_SIZE)){
        printf("[Subserver %d] recieved [%s]\n", getpid(), buf);
        char * i;
        for(i = buf; *i; i++){
          *i = ((*i + 13 - 'a') % 26) + 'a';
        }
        printf(" sending back [%s]\n", buf);
        write(to_client, buf, BUFFER_SIZE);
      }
      exit(0);
    } else {
      close(from_client);
      remove(WKP_NAME);
    }
  }
}

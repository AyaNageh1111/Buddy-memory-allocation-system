#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
typedef short bool;
#define true 1
#define false 0
#define null 0

struct processData
{
    int arrivaltime;
    int priority;
    int runningtime;
    int id;
};

////Implementation of the queue using a linked list
////To be used in the round robin algorithm
///From this resource: https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
////**************Shrouk

struct node
{
    char order[100];
    int ID;
    int size;
    struct node *next;
};
typedef struct node node;

struct queue
{
    int count;
    node *front;
    node *rear;
};

void initialize(struct queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

bool isEmpty(struct queue *q)
{
    return !(q->count);
}

void enqueue(struct queue *q, char o[100], int P, int s)
{
    node *tmp;
    tmp = malloc(sizeof(node));
    tmp->ID = P;
    tmp->size=s;
    strcpy(tmp->order,o);
    tmp->next = NULL;

    if (!isEmpty(q))
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {
        q->front = q->rear = tmp;
    }
    q->count++;
}

node dequeue(struct queue *q)
{
    node *tmp;
    node *n = q->front;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    char str[100];
    strcpy(str, n->order);
    free(tmp);  
    strcpy(n->order, str);
    return (*n);
}

void writeFile(char* path, char str[100],int id, int clock, int turn)
{
    switch (turn)
    {
        case 0:  // (0) refer to write in the file for first time so delete its content
        {
            FILE * pFile;
            pFile = fopen(path, "w");
            fprintf(pFile, "at time %d process %d has been %s \t\n", clock, id, str);
            fclose(pFile);
            break;
        }
        case 1:    // (1) refer to write in the file for sveral times so complete over its content
        {
            FILE * pFile;
            pFile = fopen(path, "a");
            fprintf(pFile, "at time %d process %d has been %s \t\n", clock, id, str);
            fclose(pFile);
            break;
        }
    }
}

struct queue* readFile(char* path)
{
    FILE *input;
    char *line=NULL;
    size_t length=0;
    ssize_t line_read;
    input=fopen(path,"r");
    if(input==NULL)
    {
        exit(EXIT_FAILURE);
    }
    struct queue* process_queue;
    initialize(process_queue);
    while((line_read=getline(&line,&length,input))!=-1)
    {
        char str[100];
        int id;
        int SIZE;
        if(line[0]!='#')
        {
            char*token=strtok(line," ");
            int index=0;
            while(token!=NULL)
            {
                switch (index)
                {

                    case 0:
                    {
                        strcpy(str, token);
                        printf("order %s \n", str);   //for check
                        break;
                    }
                    
                    case 1:
                    {
                        id=atoi(strtok(token," "));
                        printf("id %d \n", id);   //for check
                        break;
                    }

                    case 2:
                    {
                        SIZE=atoi(token);
                        printf("size %d \n", SIZE);   //for check
                        break;
                    }
                }
                
                index=index+1;
                token=strtok(NULL,"\t");
            }
            enqueue(process_queue, str, id, SIZE);
            SIZE=0;
            id=0;
            printf("\n");
        }
    }
    fclose(input);
    return process_queue;
}


int main(int argc, char * argv[])
{
    writeFile("/home/os/Downloads/OS/OutPut.txt", "done", 5, 45, 0);
    writeFile("/home/os/Downloads/OS/OutPut.txt", "done", 456, 47, 1);
    struct queue* file;
    printf("file content\n\n");    //for check  //never delete
    file= readFile("/home/os/Downloads/OS/processes.txt");
    node data=dequeue(file);
    printf("success order= %s id= %d and size= %d\n", data.order,data.ID,data.size);
    node data3=dequeue(file);
    printf("success2 order= %s id= %d and size= %d\n", data3.order,data3.ID,data3.size);

}


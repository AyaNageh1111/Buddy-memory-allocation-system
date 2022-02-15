#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



int minSize;
int count;

struct tree_str
{
        int used;                       /* size taken in allocated size */
        int size;                       /* size of size */
        int count;
        char alloc;                     /* name of allocated size */
        struct tree_str * right;        /* pointer to right child */
        struct tree_str * left;         /* pointer to left child */
        struct tree_str * parent;
};

typedef struct tree_str node;

int isLeaf(node * tree)
{
        return (!(tree->right) && !(tree->left));
}

void print(node * tree)
{
    if(!isLeaf(tree))
        print(tree->left);
    if(isLeaf(tree)){
            if(tree->used == 0)
                printf("free:%d\n", tree->size);
            else
                printf("%c:%d\n",tree->alloc,tree->used);
    }
    if(!isLeaf(tree))
        print(tree->right);

}

int isFree(node * tree)
{
        return (tree && isLeaf(tree) && !tree->used);
}

void Compining(node* tree)
{
        if(!isLeaf(tree) && isFree(tree->left) && isFree(tree->right))
        {
                free(tree->left);
                free(tree->right);
                tree->left = NULL;
                tree->right = NULL;
                tree->count = ++count;
        }
        if(tree->parent)
                Compining(tree->parent);
}

node* dallocRec(node *tree,char P)
{
        //
    node *temp;
    if(tree->alloc == P)
    {
        //
        tree->used = 0;
        tree->alloc = '\0';
        tree->count = ++count;
        //
        return tree;
    }
        //
    if(isLeaf(tree))
        return NULL;
    else
    {
        //
        if((temp = dallocRec(tree->left, P)))
                return temp;
        //
        return dallocRec(tree->right, P);
    }
}

void deallocate(node * tree, char P)
{
    printf("Process %d Deallocated size of %d at count %d \n", P, tree->size, tree->count);
        Compining(dallocRec(tree, P));
        
}

node * allocRec(node *tree, int used)
{
    node * left;
    node * right;
    if(tree->used != 0 || tree->size < used)
        return NULL;
    if( (!tree->parent || tree->size >= used) && isLeaf(tree))
        return tree;
    if(!isLeaf(tree) && tree->size >= used)
    {
        if(!(left = allocRec(tree->left, used)))
                return allocRec(tree->right, used);
        if(!(right = allocRec(tree->right, used)))
                return left;
    }
    if(left->size > right->size)
        return right;
    if(left->size < right->size)
        return left;
    if(left->count < right->count)
        return right;
    return left;
}

node * split(node * small, int used)
{
        while(used * 2 <= small->size && small->size > minSize)
        {
                small->left = (node *)calloc(1,sizeof(node));
                small->left->size = (small->size / 2);
                small->left->parent = small;
                small->left->count = ++count;
                small->right = (node *)calloc(1,sizeof(node));
                small->right->size = (small->size / 2);
                small->right->parent = small;
                small->right->count = ++count;
                small = small->left;
        }
        return small;
}
int allocate(node * tree, char P, int used)
{
        node * small = allocRec(tree, used);
        small = split(small, used);
        small->used = used;
        small->alloc = P;
       
        printf("Process %d Allocated size of %d at count %d \n", P, small->size, small->count);
       /// printf("in size %d  to %d allo %c\n",n,e, allo);
       return small->count;
       

     
      //  printf("allocated from %d", )
}
int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}
int main()
{
        int U, L, S;
        char in[20], *pEnd;
        node * root = (node *)calloc(1,sizeof(node));
        fgets(in, 20, stdin);
        U = strtol (in,&pEnd,10);
        L = strtol (pEnd,NULL,10);
        minSize = 1<<L;
        root->size = 1<<U;
 
int count;
// Allocate 1 200
count= allocate(root, '1', 200);
// Allocate 2 30
count= allocate(root, '2', 30);
// Allocate 3 55
count=  allocate(root, '3', 55);
// Deallocate 2
deallocate(root, '2');


// Deallocate 3

deallocate(root, '3');
// Allocate 4 14
count=  allocate(root, '4', 14);



// Deallocate 1
deallocate(root, '1');

// Deallocate 4
deallocate(root, '4');
// Allocate 5 255
count=  allocate(root, '5', 255);
// Allocate 6 150
count=  allocate(root, '6', 150);
// Deallocate 5
deallocate(root, '5');
// Deallocate 6
deallocate(root, '6');

//   deallocate(root, 'B');
   
//   deallocate(root, 'C');
//   deallocate(root, 'A');
  printf("successfuly Deallocate\n");


        return 0;
}
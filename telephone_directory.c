#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 50

struct node {
    char name[SIZE];
    char phone[SIZE];
    struct node *left;
    struct node *right;
};

struct node* newnode(char* name, char* phone) {
    struct node* new;
    new = malloc(sizeof(struct node)); if(!new) return NULL;
    strcpy(new->name, name); strcpy(new->phone, phone);
    new->left = NULL; new->right = NULL;
    return new;
}

struct node* add(struct node* root, char* name, char* phone, int* status) {    
    if(root==NULL) { 
        *status=1; return newnode(name, phone); 
    }
    int diff = strcmp(name, root->name);
    if(diff<0) root->left = add(root->left, name, phone, status);
    else if(diff>0) root->right = add(root->right, name, phone, status);
    else *status=0;
    return root;
}

struct node* find(struct node* root, char* name) {
    if(root==NULL) return NULL;
    int c = strcmp(name, root->name);
    if(c==0) return root;
    if(c<0) return find(root->left, name);
    return find(root->right, name);
}

struct node* findMin(struct node* root) {
    if(root==NULL) return NULL;
    while(root->left) root=root->left;
    return root;
}

struct node* delete(struct node* root, char* name, int* deleted) {
    if(root==NULL) {
           *deleted=0; return NULL; 
        }
    int diff = strcmp(name, root->name);
    if(diff<0) root->left = delete(root->left, name, deleted);
    else if(diff>0) root->right = delete(root->right, name, deleted);
    else {
        *deleted=1;
        if(root->left==NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right==NULL) {
            struct node* temp2 = root->left;
            free(root);
            return temp2;
        }
        else {
            struct node* minNode = findMin(root->right);
            strcpy(root->name, minNode->name);
            strcpy(root->phone, minNode->phone);
            int dummy=0;
            root->right = delete(root->right, minNode->name, &dummy);
        }
    }
    return root;
}

void printAll(struct node* root) {
    if(root==NULL) return;
    printAll(root->left);
    printf("%s: %s\n", root->name, root->phone);
    printAll(root->right);
}

struct node* update(struct node* root, char* oldName, char* newName, char* newPhone, int* result) {
    if(strcmp(oldName, newName)!=0 && find(root, newName)) { 
        *result=-1; return root; 
    }
    struct node* foundNode = find(root, oldName);
    if(foundNode) {
        int s=0, e=0;
        root = delete(root, oldName, &s);
        root = add(root, newName, newPhone, &e);
        *result=1;
    } else *result=0;
    return root;
}

void clearTree(struct node* root) {
    if(root==NULL) return;
    clearTree(root->left);
    clearTree(root->right);
    free(root);
}

int main() {
    struct node* root = NULL;
    char command[20], name[SIZE], phone[SIZE], newName[SIZE], newPhone[SIZE];
    int loop = 1;
    printf("phonebook!\ncommands: add delete search list update exit\n");
    while(loop) {
        printf("\nenter command: ");
        scanf("%s", command);
        if(strcmp(command,"add")==0) {
            printf("name: "); scanf("%s", name);
            printf("phone: "); scanf("%s", phone);
            int status=0;
            root = add(root, name, phone, &status);
            if(status) printf("%s added\n", name);
            else printf("%s already exists\n", name);
        }
        else if(strcmp(command,"delete")==0) {
            printf("name: "); scanf("%s", name);
            int deleted=0;
            root = delete(root, name, &deleted);
            if(deleted) printf("%s deleted from phonebook\n", name);
            else printf("%s not found\n", name);
        }
        else if(strcmp(command,"search")==0) {
            printf("name: "); scanf("%s", name);
            struct node* foundNode = find(root, name);
            if(foundNode) printf("found: %s\n", foundNode->phone);
            else printf("%s not found\n", name);
        }
        else if(strcmp(command,"list")==0) {
            printf("phonebook:\n");
            printAll(root);
        }
        else if(strcmp(command,"update")==0) {
            printf("old name: "); scanf("%s", name);
            printf("new name: "); scanf("%s", newName);
            printf("new phone: "); scanf("%s", newPhone);
            int result=0;
            root = update(root, name, newName, newPhone, &result);
            if(result==1) printf("updated\n");
            else if(result==-1) printf("name already exists\n");
            else printf("%s not found\n", name);
        }
        else if(strcmp(command,"exit")==0) {
            printf("exiting program...\n");
            loop=0;
        }
        else {
            printf("Invalid command\n");
        }
    }
    clearTree(root);
    system("pause");
    return 0;
}
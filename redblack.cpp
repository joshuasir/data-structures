#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define RED 0
#define BLACK 1

struct node {
	int key,color;
	node *left,*right,*parent;
}*root;

node *createNode(int key) {
	node *toAdd = (node *)malloc(sizeof(node));
	toAdd->key = key;
	toAdd->color = RED;
	toAdd->left = toAdd->right = toAdd->parent = NULL;
	return toAdd;
}
void swapColor(node *a,node *b) {
	int temp = a->color;
	a->color = b->color;
	b->color = temp;
}

void rotate(node *curr, const char *flag) {
	node *y,*z;
	if(!strcmp(flag,"left")) {
		
		y = curr->right;
    	z = y->left;
    	curr->right = z;
    	y->left = curr;

	} else if(!strcmp(flag,"right")) {
		
		y = curr->left;
    	z = y->right;
    	y->right = curr;
    	curr->left = z;
    	
	}
	if(!curr->parent){
		root = y;
	}else if(curr->parent->left==curr){
		curr->parent->left = y;
	}else{
		curr->parent->right = y;
	}
	if(z){
		z->parent = curr;
	}
	y->parent = curr->parent;
	curr->parent = y;
	}
void fixViolation(node *curr) {

		node *grandparent = NULL;
		node *parent = NULL;
		while ((curr->parent) && (curr->color != BLACK) && (curr->parent->color == RED)){ 
		
			parent = curr->parent;
			grandparent = parent->parent;
			node *uncle = (parent == grandparent->right) ? grandparent->left : grandparent->right;
			
			if(uncle && uncle->color == RED) {
				uncle->color = parent->color = BLACK;
				grandparent->color = RED;
				curr = grandparent;
			} else {
				if(grandparent->right == parent ) {
					if(parent->left == curr){
						rotate(parent,"right");
						curr = parent;
						parent = curr->parent;
					}
					rotate(grandparent,"left");
				} else if(grandparent->left == parent) {
					if(parent->right == curr){
						rotate(parent,"left");
						curr = parent;
						parent = curr->parent;	
					}
					rotate(grandparent,"right");
				}
				swapColor(parent,grandparent);
				curr = parent;
			}
	}
	root->color = BLACK;
}
node *BSTinsert(node *toAdd,node *curr) {
	if(!curr) {
		return toAdd;
	}

	if(curr->key > toAdd->key) {
		curr->left = BSTinsert(toAdd,curr->left);
		curr->left->parent = curr;
	} else if(curr->key < toAdd->key) {
		curr->right = BSTinsert(toAdd,curr->right);
		curr->right->parent = curr;
	}

	return curr;
}

void insertion(int key) {
	node *toAdd = createNode(key);
	root = BSTinsert(toAdd,root);
	fixViolation(toAdd);
}
void inorder(node *curr,int depth){
	if(curr){
	inorder(curr->left,depth+1);
	printf("%d %d\n",curr->key,depth);
	inorder(curr->right,depth+1);}
}
void doubleBlack(node *curr){
	if(curr == root){
		return;
	}
	
	node *parent = curr->parent,*sibling = (curr->parent->left == curr) ? curr->parent->right : curr->parent->left;
	if(sibling == NULL){
		doubleBlack(parent);
	}else{
		if(sibling->color == RED){
			swapColor(parent,sibling);
			if(parent->left == curr){
				rotate(parent,"right");
			}else{
				rotate(parent,"left");
			}
			doubleBlack(curr);
		}else{
			if((sibling->left && sibling->left->color==RED)){
				if(parent->left == sibling){
					sibling->left->color = sibling->color;
              		sibling->color = parent->color;
					rotate(parent,"right");
				}else{
					sibling->left->color = parent->color;
					rotate(sibling,"right");
					rotate(parent,"left");
				}
			}else if((sibling->right && sibling->right->color==RED)){
				if(parent->right == sibling){
					sibling->right->color = sibling->color;
             		sibling->color = parent->color;
					rotate(parent,"left");
				}else{
					sibling->right->color = parent->color;
					rotate(sibling,"left");
					rotate(parent,"right");
				}
			}else{
				
				sibling->color = RED;
				if(parent->color == BLACK){
					doubleBlack(parent);
				}
				parent->color = BLACK;
			}
		}
	}
}
void deletion(int key,node *curr){
	if(!curr){
		return;
	}
	
	if(curr->key > key){
		deletion(key,curr->left);
	}else if(curr->key < key){
		deletion(key,curr->right);
	}else{
		if(!curr->right && !curr->left){
			if(curr->color == BLACK){
				doubleBlack(curr);	
			}
			if(curr == root){
				root=NULL;
			}else if(curr->parent->left == curr){
				curr->parent->left = NULL;
			}else{
				curr->parent->right = NULL;
			}
			free(curr);
			curr = NULL;
		}else{
			node *toDel;
			if(curr->right){
				toDel = curr->right;
				while(toDel && toDel->left){
					toDel = toDel->left;
				}
				curr->key = toDel->key;
				deletion(toDel->key,curr->right);
			}else{
				toDel = curr->left;
				while(toDel && toDel->right){
					toDel = toDel->right;
				}
				curr->key = toDel->key;
				deletion(curr->key,curr->left);
			}
		}
	}
}
int main() {

	insertion(200);
	insertion(300);
	insertion(400);
	insertion(700);
	insertion(500);
	insertion(600);
	insertion(800);
	insertion(900);
	insertion(1000);
	insertion(1100);
	insertion(1200);
	insertion(1300);
	//inorder(root,0);
	deletion(200,root);
	deletion(300,root);
	deletion(400,root);
	inorder(root,0);
}
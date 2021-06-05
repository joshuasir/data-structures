#include<stdio.h>
#include<stdlib.h>

#define RED 0
#define BLACK 1
#define LEFT 2
#define RIGHT 3


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

void rotate(node *curr, int flag) {
	node *y,*z;
	
	if(flag == LEFT) {
		
		y = curr->right;
		z = y->left;
    	curr->right = y->left;
    	y->left = curr;

	} else {
		
		y = curr->left;
		z = y->right;
    	curr->left = y->right;
    	y->right = curr;
    	
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
	node *parent,*uncle ,*grandparent;
	
	while ((curr->parent) && (curr->color == RED) && (curr->parent->color == RED)){ 
	
		parent = curr->parent;
		grandparent = parent->parent;
		uncle = (parent == grandparent->right) ? grandparent->left : grandparent->right;
		
		if(uncle && uncle->color == RED) {
			uncle->color = parent->color = BLACK;
			grandparent->color = RED;
			curr = grandparent;
		} else {
			if( grandparent->right == parent ) {
				if(parent->left == curr){
					rotate(parent,RIGHT);
					curr = parent;
					parent = curr->parent;
				}
				rotate(grandparent,LEFT);
			} else if( grandparent->left == parent ) {
				if(parent->right == curr){
					rotate(parent,LEFT);
					curr = parent;
					parent = curr->parent;	
				}
				rotate(grandparent, RIGHT);
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

void doubleBlack(node *curr){
	if(curr == root){ 
		return;
	}
	
	node *parent = curr->parent,*sibling = (curr->parent->left == curr) ? curr->parent->right : curr->parent->left; 
	if(!sibling){ 
		doubleBlack(parent);
	}else{
		if(sibling->color == RED){ 
			swapColor(parent,sibling);
			if(parent->left == sibling){
				rotate(parent,RIGHT);
			}else{
				rotate(parent,LEFT);
			}
			doubleBlack(curr);
		}else{
			if(sibling->left && sibling->left->color == RED){
				if(parent->left == sibling){
					sibling->left->color = sibling->color;
              		sibling->color = parent->color;
					rotate(parent,RIGHT);
				}else{
					sibling->left->color = parent->color;
					rotate(sibling,RIGHT);
					rotate(parent,LEFT);
				}
			}else if(sibling->right && sibling->right->color == RED){
				if(parent->right == sibling){
					sibling->right->color = sibling->color;
             		sibling->color = parent->color;
					rotate(parent,LEFT);
				}else{
					sibling->right->color = parent->color;
					rotate(sibling,LEFT);
					rotate(parent,RIGHT);
				}
			}else{
				
				sibling->color = RED;
				if(parent->color == BLACK){
					doubleBlack(parent);
					return;
				}
			}
			parent->color = BLACK;
		}
	}
}

node *getSucc(node *curr){
	while(curr->left){
		curr = curr->left;
	}
	return curr;
}

node *getPred(node *curr){
	while(curr->right){
		curr = curr->right;
	}
	return curr;
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
				root = NULL;
			}else if(curr->parent->left == curr){
				curr->parent->left = NULL;
			}else{
				curr->parent->right = NULL;
			}
			free(curr);
			curr = NULL;
		}else{
			node *toDel,*pos;
			if(curr->right){
				toDel = getSucc(pos = curr->right);
			}else{
				toDel = getPred(pos = curr->left);
			}
			curr->key = toDel->key;
			deletion(toDel->key,pos);
		}
	}
}

void inorder(node *curr,int depth){
	if(curr){
	inorder(curr->left,depth+1);
	printf("%d %d\n",curr->key,depth);
	inorder(curr->right,depth+1);}
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

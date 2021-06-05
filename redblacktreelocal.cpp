#include<stdio.h>
#include<stdlib.h>

#define RED 0
#define BLACK 1
#define LEFT 2
#define RIGHT 3


struct node {
	int key,color;
	node *left,*right,*parent;
};

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

int isRed(node *curr){
	return (curr && curr->color == RED) ? 1 : 0;
}

void rotate(node *curr, int flag,node **root) {
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
		*root = y;
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

node *find(int key,node *curr){
	if(!curr){
		return NULL;
	}
	
	if(curr->left && curr->key > key){
		
		return find(key,curr->left);
	}
	
	if(curr->right && curr->key < key){
		return find(key,curr->right);
	}
	

	return curr;
}

void fixViolation(node *curr,node **root) {
	node *parent,*uncle ,*grandparent;
	
	while (isRed(curr) && isRed(curr->parent)){ 
	
		parent = curr->parent;
		grandparent = parent->parent;
		uncle = (parent == grandparent->right) ? grandparent->left : grandparent->right;
		
		if(uncle && uncle->color == RED) {
			uncle->color = parent->color = BLACK;
			grandparent->color = RED;
			curr = grandparent;
			continue;
		}
		 
		if( grandparent->right == parent ) {
			if(parent->left == curr){
				rotate(parent,RIGHT,root);
				curr = parent;
				parent = curr->parent;
			}
			rotate(grandparent,LEFT,root);
		} else {
			if(parent->right == curr){
				rotate(parent,LEFT,root);
				curr = parent;
				parent = curr->parent;	
			}
			rotate(grandparent, RIGHT,root);
		}
		swapColor(parent,grandparent);
		curr = parent;
	}
	(*root)->color = BLACK;
}

void insertion(int key,node **root) {
	
	if(!*root){
		*root = createNode(key);
		(*root)->color = BLACK;
		return;
	}
	
	node *parent = find(key,*root);
	
	if(key == parent->key){
		return;
	}
	
	node *toAdd = createNode(key);
	if(parent->key > key){
		parent->left = toAdd;
	}else{
		parent->right = toAdd;
	}
	toAdd->parent = parent;
	
	fixViolation(toAdd,root);
}

void doubleBlack(node *curr, node **root){
	if(!curr->parent){ 
		return;
	}
	
	node *parent = curr->parent,*sibling = (curr->parent->left == curr) ? curr->parent->right : curr->parent->left; 
	if(!sibling){ 
		doubleBlack(parent,root);
		return;
	}
	
	if(isRed(sibling)){ 
		swapColor(parent,sibling);
		if(parent->left == sibling){
			rotate(parent,RIGHT,root);
		}else{
			rotate(parent,LEFT,root);
		}
		doubleBlack(curr,root);
		return;
	}
	
	if(isRed(sibling->left)){
		if(parent->left == sibling){
			sibling->left->color = sibling->color;
      		sibling->color = parent->color;
			rotate(parent,RIGHT,root);
		}else{
			sibling->left->color = parent->color;
			rotate(sibling,RIGHT,root);
			rotate(parent,LEFT,root);
		}
	}else if(isRed(sibling->right)){
		if(parent->right == sibling){
			sibling->right->color = sibling->color;
     		sibling->color = parent->color;
			rotate(parent,LEFT,root);
		}else{
			sibling->right->color = parent->color;
			rotate(sibling,LEFT,root);
			rotate(parent,RIGHT,root);
		}
	}else{
		
		sibling->color = RED;
		if(!isRed(parent)){
			doubleBlack(parent,root);
			return;
		}
	}
	parent->color = BLACK;
	
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

void deletion(int key,node *start,node **root){
	node *curr = find(key,start);

	if(curr->key != key){
		return;
	}
		
	if(!curr->right && !curr->left){
		if(!isRed(curr)){
			doubleBlack(curr,root);	
		}
		if(!curr->parent){
			*root = NULL;
			
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
		deletion(curr->key,pos,root);
	}
	
}

void inorder(node *curr,int depth){
	if(curr){
	inorder(curr->left,depth+1);
	printf("%d %d %s\n",curr->key,depth,(curr->color) ? "BLACK" : "RED");
	inorder(curr->right,depth+1);}
}

int main() {
	
	node *root = NULL;
	
	insertion(200,&root);
	insertion(300,&root);
	insertion(400,&root);
	insertion(700,&root);
	insertion(500,&root);
	insertion(600,&root);
	insertion(800,&root);
	insertion(900,&root);
	insertion(1000,&root);
	insertion(1100,&root);
	insertion(1200,&root);
	insertion(2200,&root);
	insertion(3300,&root);
	insertion(1400,&root);
	insertion(1500,&root);
	insertion(1600,&root);
	inorder(root,0);
	deletion(200,root,&root);
	deletion(300,root,&root);
	deletion(400,root,&root);
	deletion(700,root,&root);
	deletion(500,root,&root);
	deletion(600,root,&root);
	deletion(800,root,&root);
	deletion(900,root,&root);
	deletion(1000,root,&root);
	deletion(1100,root,&root);
	deletion(1200,root,&root);
	deletion(2200,root,&root);
	deletion(3300,root,&root);
	deletion(1400,root,&root);
	deletion(1500,root,&root);
	deletion(1600,root,&root);
	inorder(root,0);
}

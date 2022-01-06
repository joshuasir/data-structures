#include<stdio.h>
#include<stdlib.h>

#define RED 0
#define BLACK 1
#define LEFT 2
#define RIGHT 3
#define DBLACK 4

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

int isRed(node *curr){
	return (curr && curr->color == RED) ? 1 : 0;
}

int hasDblack(node *curr){
	return ((curr->left && curr->left->color == DBLACK) || (curr->right && curr->right->color == DBLACK));
}

node *clearDblack(node *doubleBlack,int key){
	if(doubleBlack && doubleBlack->key == key){
		if(doubleBlack->parent->left == doubleBlack){
			doubleBlack->parent->left = NULL;
		}else{
			doubleBlack->parent->right = NULL;
		}
		free(doubleBlack);
		doubleBlack = NULL;
	}else{
		doubleBlack->color = BLACK;
	}
	
	return doubleBlack;
	
}

void swapColor(node *a,node *b) {
	int temp = a->color;
	a->color = b->color;
	b->color = temp;
}

node *rotate(node *curr, int flag) {
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
	
	return y;
}

int isViolate(node **curr,node **parent,node **uncle,node *grandparent){
	
	if(isRed(grandparent->left)){
		*parent = grandparent->left;
		*uncle = grandparent->right;
		if(isRed((*parent)->left)){
			*curr = (*parent)->left;
		}else if(isRed((*parent)->right)){
			*curr = (*parent)->right;
		}
	}
	
	if(!(*curr) && isRed(grandparent->right)){
		*parent = grandparent->right;
		*uncle = grandparent->left;
		if(isRed((*parent)->left)){
			*curr = (*parent)->left;
		}else if(isRed((*parent)->right)){
			*curr = (*parent)->right;
		}
	}
	
	return (*curr) ? 1 : 0;
}

node *fixViolation(node *grandparent) {
	node *parent,*uncle ,*curr = NULL;
	
	if (isViolate(&curr,&parent,&uncle,grandparent)){ 
		
		if(isRed(uncle)) {
			uncle->color = parent->color = BLACK;
			grandparent->color = RED;
		}else{
			if( grandparent->right == parent ) {
				if(parent->left == curr){
					grandparent->right = rotate(parent,RIGHT);
				}
				grandparent = rotate(grandparent,LEFT);
				parent = grandparent->left;
			} else if( grandparent->left == parent ) {
				if(parent->right == curr){
					grandparent->left = rotate(parent,LEFT);
				}
				grandparent = rotate(grandparent, RIGHT);
				parent = grandparent->right;
			}
			swapColor(parent,grandparent);	
		}
	}
	if(!grandparent->parent){
		grandparent->color = BLACK;
	}
	
	return grandparent;
}

node *insertion(int key,node *curr) {
	if(!curr) {
		return createNode(key);
	}

	if(curr->key > key) {
		curr->left = insertion(key,curr->left);
		curr->left->parent = curr;
	} else if(curr->key < key) {
		curr->right = insertion(key,curr->right);
		curr->right->parent = curr;
	}
	
	return fixViolation(curr);
}


node *doubleBlack(node *parent,int key){
	
	if(!parent || !hasDblack(parent)){
		return parent;
	}

	node *curr = (parent->left &&  parent->left->color == DBLACK) ? parent->left : parent->right;
	node *sibling = (parent->left == curr) ? parent->right : parent->left; 

	if(!sibling){ 
		parent->color = (parent->parent) ? DBLACK : BLACK;
		curr = clearDblack(curr,key);
		return parent;
	}
	
	if(isRed(sibling)){ 
		swapColor(parent,sibling);
		if(parent->left == sibling){
			parent = rotate(parent,RIGHT);
			parent->right = doubleBlack(parent->right,key);
		}else{
			parent = rotate(parent,LEFT);
			parent->left = doubleBlack(parent->left,key);
		}
		return parent;
	}
	
	if(isRed(sibling->left) && parent->left == sibling){
		sibling->left->color = sibling->color;
      	sibling->color = parent->color;
      	parent->color = BLACK;
		parent = rotate(parent,RIGHT);
	}else if(isRed(sibling->right) && parent->right == sibling){
		sibling->right->color = sibling->color;
     	sibling->color = parent->color;
     	parent->color = BLACK;
		parent = rotate(parent,LEFT);
	}else if(isRed(sibling->left)){
		sibling->left->color = parent->color;
		parent->color = BLACK;
		parent->right = rotate(sibling,RIGHT);
		parent = rotate(parent,LEFT);
	}else if(isRed(sibling->right)){
		sibling->right->color = parent->color;
		parent->color = BLACK;
		parent->left = rotate(sibling,LEFT);
		parent = rotate(parent,RIGHT);
	}else{
		sibling->color = RED;
		parent->color = (!isRed(parent) && parent->parent) ? DBLACK : BLACK;
	}

	curr = clearDblack(curr,key);
	return parent;

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

node *deletion(int key,node *curr){
	if(!curr){
		return NULL;
	}
	
	if(curr->key > key){
		curr->left = deletion(key,curr->left);
	}else if(curr->key < key){
		curr->right = deletion(key,curr->right);
	}else{

		if(!curr->right && !curr->left){
			if(!isRed(curr) && curr->parent){
				curr->color = DBLACK;
				return curr;
			}

			if(!curr->parent){

			}else if(curr->parent->left == curr){
				curr->parent->left = NULL;
			}else{
				curr->parent->right = NULL;
			}

			free(curr);
			curr = NULL;
			
			return curr;
		}else{
			node *toDel;
			if(curr->right){			
				toDel = getSucc(curr->right);
				curr->key = toDel->key;
				curr->right = deletion(toDel->key,curr->right);
			}else{
				toDel = getPred(curr->left);
				curr->key = toDel->key;
				curr->left = deletion(toDel->key,curr->left);
			}
			key = toDel->key;
		}
	}
	return doubleBlack(curr,key);
}

void inorder(node *curr,int depth){
	if(curr){
	inorder(curr->left,depth+1);
	printf("%d %d %s\n",curr->key,depth,(curr->color) ? "BLACK" : "RED");
	inorder(curr->right,depth+1);}
}

int main() {
	node *root = NULL;
	root = insertion(200,root);
	root = insertion(300,root);
	root = insertion(400,root);
	root = insertion(700,root);
	root = insertion(500,root);
	root = insertion(600,root);
	root = insertion(800,root);
	root = insertion(900,root);
	root = insertion(1000,root);
	root = insertion(1100,root);
	root = insertion(1200,root);
	root = insertion(2200,root);
	root = insertion(3300,root);
	root = insertion(1400,root);
	root = insertion(1500,root);
	root = insertion(1600,root);
	inorder(root,0);
	root = deletion(200,root);
	root = deletion(300,root);
	root = deletion(400,root);
	root = deletion(700,root);
	root = deletion(500,root);
	root = deletion(600,root);
	root = deletion(800,root);
	root = deletion(900,root);
	root = deletion(1000,root);
	root = deletion(1100,root);
	root = deletion(1200,root);
	root = deletion(2200,root);
	root = deletion(3300,root);
	root = deletion(1400,root);
	root = deletion(1500,root);
	root = deletion(1600,root);
	puts("");
	inorder(root,0);
}
 

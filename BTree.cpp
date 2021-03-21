#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5
#define MIN 2

struct Node {
	int key[MAX + 2],ctr,leaf;
	Node *child[MAX+2];
} *root  = NULL;

Node *createNode(int key, Node *child);
int setKeyNode(int key, int *potKey, Node *node, Node **child);
void insertKey(int key, int pos, Node *leaf, Node *child);
void split(int key, int *potKey, int pos, Node *node, Node *child, Node **newNode);
void insertion(int key);

int find(Node *curr, int key);
int isLeaf(Node *curr);
void removeLeaf(int pos,Node *curr);
void fillFromPrev(int pos,Node *curr);
void fillFromNext(int pos,Node *curr);
void merge(int pos, Node *curr);
void fill(int pos, Node *curr);
void deletion(Node *curr,int key);
void inorder(Node *root);

Node *createNode(int key, Node *child) {
	Node *newNode = (Node*)malloc(sizeof(Node));
	memset(newNode->child,0,sizeof *newNode->child);
	newNode->child[0] = root;
	newNode->child[1] = child;
	newNode->key[1] = key;
	newNode->ctr = 1;
	newNode->leaf = !(child||root);
	return newNode;
}

void insertKey(int key, int pos, Node *leaf, Node *child) {
	int curr = leaf->ctr;

	while(curr > pos) {
		leaf->key[curr+1] = leaf->key[curr];
		leaf->child[curr+1] = leaf->child[curr];
		curr--;
	}

	leaf->key[curr+1] = key;
	leaf->child[curr+1] = child;
	leaf->leaf = !child;
	leaf->ctr++;
}

void split(int key, int *potKey, int pos, Node *node, Node *child, Node **newNode) {
	int median =  MIN + (pos > MIN);
	int j = median + 1;
	*newNode = (Node*)malloc(sizeof(Node));
	
	while (j <= MAX) {
		(*newNode)->key[j - median] = node->key[j];
		(*newNode)->child[j - median] = node->child[j];
		j++;
	}
	
	node->ctr = median;
	(*newNode)->ctr = MAX - median;
	
	if( pos <= MIN ) {
		insertKey(key, pos, node, child);
	}else{
		insertKey(key, pos - median, *newNode, child);
	}

	*potKey = node->key[node->ctr];
	(*newNode)->child[0] = node->child[node->ctr];
	node->ctr--;
}

int setKeyNode(int key, int *potKey, Node *node, Node **child) {
	int pos;
	if (!node) {
		*potKey = key;
		*child = NULL;
		return 1;
	}
	if (key<node->key[1]) {
		pos = 0;
	} else {
		pos = node->ctr;
		while (pos > 1 && key < node->key[pos]) {
			pos--;
		}
		if (key == node->key[pos]) {
			return 0;
		}
	}

	if (setKeyNode(key, potKey, node->child[pos], child)) {
		if (node->ctr < MAX) {
			insertKey(*potKey, pos, node, *child);
		} else {
			split(*potKey, potKey, pos, node, *child, child);
			return 1;
		}
	}
	return 0;
}

void insertion(int key) {
	int potKey;
	Node *child;
	if (setKeyNode(key, &potKey, root, &child)) {
		root = createNode(potKey, child);
	}
}

void inorder(Node *curr) {
	int i;
	if(curr) {
		for(i = 0; i < curr->ctr; i++) {
			inorder(curr->child[i]);
			printf("%d ", curr->key[i+1]);
		}
		inorder(curr->child[i]);
	}
}

int isLeaf(Node *curr) {
	return (curr)?curr->leaf:0;
}
int find(Node *curr, int key) {
	int i;
	for (i = 0; i+1<=curr->ctr && key > curr->key[i+1]; i++);
	return i+(i+1 <= curr->ctr && key == curr->key[i+1]);
}
void removeLeaf(int pos,Node *curr) {
	for(int i = pos+1; i<=curr->ctr; i++) {
		curr->key[i-1] = curr->key[i];
		curr->child[i-1] = NULL;
	}
	curr->child[curr->ctr--] = NULL;
}
void fillFromPrev(int pos,Node *curr) {
	Node *child = curr->child[pos];
	Node *sibling = curr->child[pos-1];

	for(int i = child->ctr+1; i>1; i--) {
		child->key[i] = child->key[i-1];
	}
	if(!isLeaf(child)) {
		for(int i = child->ctr+1; i>=1; i--) {
			child->child[i] = child->child[i-1];
		}
	}

	child->key[1] = curr->key[pos];
	curr->key[pos] = sibling->key[sibling->ctr];
	child->child[0] = sibling->child[sibling->ctr];
	sibling->child[sibling->ctr] = NULL;
	sibling->ctr--;
	child->ctr++;

}
void fillFromNext(int pos,Node *curr) {
	Node *child = curr->child[pos];
	Node *sibling = curr->child[pos+1];

	child->key[child->ctr+1] = curr->key[pos+1];
	curr->key[pos+1] = sibling->key[1];
	child->child[child->ctr+1] = sibling->child[0];

	for(int i = 1; i<sibling->ctr; i++) {
		sibling->key[i] = sibling->key[i+1];
	}
	if(!isLeaf(sibling)) {
		for(int i = 0; i<sibling->ctr; i++) {
			sibling->child[i] = sibling->child[i+1];
		}
	}
	sibling->child[sibling->ctr] = NULL;
	sibling->ctr--;
	child->ctr++;
}
int getPred(Node *curr) {
	while(!isLeaf(curr)) {
		curr = curr->child[curr->ctr];
	}
	return curr->key[curr->ctr];
}
int getSucc(Node *curr) {
	while(!isLeaf(curr)) {
		curr = curr->child[0];
	}
	return curr->key[1];
}
void merge(int pos, Node *curr) {
	Node *child = curr->child[pos];
	Node *sibling = curr->child[pos+1];

	child->key[MIN+1] = curr->key[pos+1];
	for(int i =1 ; i<=sibling->ctr; i++) {
		child->key[MIN+1+i] = sibling->key[i];
	}

	if(!isLeaf(child)) {
		for(int i =0 ; i<=sibling->ctr; i++) {
			child->child[MIN+1+i] = sibling->child[i];
			sibling->child[i] = NULL;
		}
	}

	for(int i = pos+1; i<curr->ctr; i++) {
		curr->key[i] = curr->key[i+1];
	}
	for(int i = pos+1; i<curr->ctr; i++) {
		curr->child[i] = curr->child[i+1];
	}
	curr->child[curr->ctr] = NULL;
	curr->ctr--;
	child->ctr+=sibling->ctr+1;
	free(sibling);
	sibling = NULL;

}
void fill(int pos, Node *curr) {
	if(pos+1<=curr->ctr&&curr->child[pos+1]->ctr > MIN) {
		fillFromNext(pos,curr);
	} else if(pos-1>=0&&curr->child[pos-1]->ctr > MIN) {
		fillFromPrev(pos,curr);
	} else {
		merge(pos-(pos!=0),curr);
	}
}
void removeNonLeaf(int pos, Node *curr) {
	int k = curr->key[pos];
	if(curr->child[pos-1]->ctr > MIN) {
		curr->key[pos] = getPred(curr->child[pos-1]);
		deletion(curr->child[pos-1],curr->key[pos]);
	} else if(curr->child[pos]->ctr > MIN) {
		curr->key[pos] = getSucc(curr->child[pos]);
		deletion(curr->child[pos],curr->key[pos]);
	} else {
		merge(pos-(pos!=0),curr);
		deletion(curr,k);
	}
}
void deletion(Node *curr,int key) {
	int index = find(curr,key);
	if(index > 0 && index <= curr->ctr && curr->key[index] == key) {
		if(isLeaf(curr)) {
			removeLeaf(index,curr);
		} else {
			removeNonLeaf(index,curr);
		}
	} else {
		if(!isLeaf(curr)) {
			if(curr->child[index]->ctr <= MIN) {
				fill(index,curr);
				index = find(curr,key);
			}
			deletion(curr->child[index],key);
		}
	}
}
int main() {
	insertion(1);
	insertion(3);
	insertion(7);
	insertion(10);
	insertion(11);
	insertion(13);
	insertion(14);
	insertion(15);
	insertion(18);
	insertion(16);
	insertion(19);
	insertion(24);
	insertion(25);
	insertion(26);
	insertion(21);
	insertion(4);
	insertion(5);
	insertion(20);
	insertion(22);
	insertion(2);
	insertion(17);
	insertion(12);
	insertion(6);
	inorder(root);
	puts("");
	deletion(root,6);
	inorder(root);
	puts("");
	deletion(root,13);
	inorder(root);
	puts("");
	deletion(root,7);
	inorder(root);
	puts("");
	deletion(root,4);
	inorder(root);
	puts("");
	deletion(root,2);
	inorder(root);
	puts("");
	deletion(root,16);
	inorder(root);
	puts("");
	return 0;
}

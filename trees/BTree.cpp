#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5
#define MIN 2

struct Node {
	int key[MAX + 2],ctr,leaf; // setiap node nanti nya akan memegang hingga tepat MAX - 1 key
 	Node *child[MAX+2]; 		// child yang dipegang hingga MAX
} *root = NULL; // initial root

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

Node *createNode(int key, Node *child) { // deklarasi node baru
	Node *newNode = (Node*)malloc(sizeof(Node)); 
	memset(newNode->child,0,sizeof *newNode->child);
	newNode->child[0] = root;
	newNode->child[1] = child;
	newNode->key[1] = key;
	newNode->ctr = 1;
	newNode->leaf = !(child||root); // bila node yang baru yang dibentuk tidak memiliki child/root maka akan ditandakan sebagai leaf
	return newNode;
}

void inorder(Node *curr) { // print in order tree
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
	return (curr)?curr->leaf:0; // validasi apakah sebuah node merupakan leaf
}


int find(Node *node, int key) {
	int pos = -1;
	if (key<node->key[1]) { // bila key lebi kecil dari seluru key pada node maka akan berada pada posisi array ke 0
		return 0;
	} else { // jika tidak traverse array satu per satu dan temukan lokasi yang tepat lebi kecil sama dengan key yang ingin kita cari
		pos = node->ctr;
		while (pos > 1 && key < node->key[pos]) {
			pos--;
		}
	}
	return pos;
}

void insertion(int key) {
	int potKey; // penunjuk key yang perlu diinsert, akan beruba bila ada split
	Node *child; // deklarasi child potential
	if (setKeyNode(key, &potKey, root, &child)) { // bila split dilakukan hinnga mencapai root 
		root = createNode(potKey, child); // pembentukan root baru dari key hasil split
	}
}

int setKeyNode(int key, int *potKey, Node *node, Node **child) {
	
	if (!node) { // bila sudah mencapai/melewati leaf
		*potKey = key; // inisialisasi key yang ingin kita input ke penunjuk
		*child = NULL; // inisialisasi child
		return 1; // indikasi ada key yang perlu diinsert
	}
	int pos = find(node,key); // cari posisi key
	if (key == node->key[pos]) { // bila key sudah ada hentikan proses insert
		return 0;
	}
	if (setKeyNode(key, potKey, node->child[pos], child)) { // rekursi traverse child, bila tidak ada child
		if (node->ctr < MAX) { // jika key pada node masih dapat ditambah
			insertKey(*potKey, pos, node, *child); // insert pada array
		} else {
			split(*potKey, potKey, pos, node, *child, child); // lakukan split 
			return 1; // indikasi kan masi ada key yang perlu diinsert, pada kasus ini x sebagai parent y dan z
			// child yang akan beserta value diinsert nanti nya adalah node baru (z) yang di tunjuk pointer child
			// sedangkan node lama(y) sudah dirujuk oleh key value index -1 dari index node x sebelumnya sebelum node x dibentuk
		}
	}
	
	return 0;
}


void insertKey(int key, int pos, Node *leaf, Node *child) {
	int curr = leaf->ctr;
	// lakukan iterasi traverse key pada node satu per satu membari mengeser value key dan child  untuk mendapatkan posisi yang tepat
	while(curr > pos) {
		leaf->key[curr+1] = leaf->key[curr];
		leaf->child[curr+1] = leaf->child[curr];
		curr--;
	}
	// letakan key dan child yang kita insert pada lokasi sesuai yang di tentukan pos
	leaf->key[curr+1] = key;
	leaf->child[curr+1] = child;
	// bila tidak ada child maka tandakan sebagai leaf
	leaf->leaf = !child;
	// tambahkan ukuran array node
	leaf->ctr++;
}

void split(int key, int *potKey, int pos, Node *node, Node *child, Node **newNode) {
	int median =  MIN + (pos > MIN); // tentukan nilai tengah, bila posisi key baru lebi besar dari minimum nilai tengah bergeser +1
	int j = median + 1; // tepat setela nilai tengah
	*newNode = (Node*)malloc(sizeof(Node)); // siapkan node baru
	
	while (j <= MAX) { // pindahkan key dan child dari index tengah +1 hingga MAX ke node baru
		(*newNode)->key[j - median] = node->key[j];
		(*newNode)->child[j - median] = node->child[j];
		j++;
	}
	
	node->ctr = median; // ukuran node lama menjadi tepat median
	(*newNode)->ctr = MAX - median; // sedangkan node baru dari median+1 hingga MAX
	
	if( pos <= MIN ) { // jika posisi yang ingin kita insert berada pada node kiri/lama
		insertKey(key, pos, node, child); // insert ke node lama
	}else{
		insertKey(key, pos - median, *newNode, child); // insert ke node baru
	}
	
	// sebagai referensi dari pseudocode y adalah node lama dan z adalah node baru 
	
	*potKey = node->key[node->ctr]; // deklarasi nilai x yang menjadi parent y dan z sebagai key yang ingin kita insert selanjut nya
	(*newNode)->child[0] = node->child[node->ctr]; // memindahkan child x 
	node->ctr--; // size berkurang
	
	// pada saat ini key yang ingin kita insert beruba menjadi x
}

int getPred(Node *curr) {
	while(!isLeaf(curr)) { // selama belum leaf
		curr = curr->child[curr->ctr]; // traverse child terujung
	}
	return curr->key[curr->ctr]; // return key terujung
}

int getSucc(Node *curr) { 
	while(!isLeaf(curr)) { // selama belum leaf
		curr = curr->child[0]; // traverse child terawal
	}
	return curr->key[1]; // return child terawal
}


void removeLeaf(int pos,Node *curr) {
	for(int i = pos+1; i<=curr->ctr; i++) { // bila berada pada leaf lakukan iterasi untuk menutup lokasi key yang di delete
		curr->key[i-1] = curr->key[i];
		curr->child[i-1] = NULL;
	}
	curr->child[curr->ctr--] = NULL; // kurangi ukuran key pada node
}


void fillFromPrev(int pos,Node *curr) {
	Node *child = curr->child[pos];
	Node *sibling = curr->child[pos-1];
	
	// geser key dan child dari node yang ingin ditamba/ sediakan tempat pada index terawal untuk node baru
	for(int i = child->ctr+1; i>1; i--) {
		child->key[i] = child->key[i-1];
	}
	if(!isLeaf(child)) {
		for(int i = child->ctr+1; i>=1; i--) {
			child->child[i] = child->child[i-1];
		}
	}
	
	// assign key parent pada lokasi pos menjadi key terawal
	child->key[1] = curr->key[pos];
	// assign key sibling terakhir menjadi key parent pada lokasi pos
	curr->key[pos] = sibling->key[sibling->ctr];
	// pindahkan child sibling terakhir ke child key child terawal
	child->child[0] = sibling->child[sibling->ctr];
	//kosongkan child sibling terujung/terakhir
	sibling->child[sibling->ctr] = NULL;
	
	// sesuaikan size sibling berkurang dan node yang kita fill bertamba
	sibling->ctr--;
	child->ctr++;

}
void fillFromNext(int pos,Node *curr) {
	Node *child = curr->child[pos];
	Node *sibling = curr->child[pos+1];
	
	// mengambil key dari parent pada lokasi pos menjadi key terakhir dari node yang ingin kita tambahkan
	child->key[child->ctr+1] = curr->key[pos+1];
	// assign nilai sibling terawal sebagai nilai key parent pada lokasi pos 
	curr->key[pos+1] = sibling->key[1];
	// pindahkan child sibling terawal menjadi child key child terakhir
	child->child[child->ctr+1] = sibling->child[0];


	// geser key key dan child child pada sibling untuk mengisi posisi key sibling yang telah dipindahkan/hapus
	for(int i = 1; i<sibling->ctr; i++) {
		sibling->key[i] = sibling->key[i+1];
	}
	if(!isLeaf(sibling)) {
		for(int i = 0; i<sibling->ctr; i++) {
			sibling->child[i] = sibling->child[i+1];
		}
	}
	
	// kosongkan lokasi key yang tidak terpakai
	sibling->child[sibling->ctr] = NULL;
	
	// sesuaikan size
	sibling->ctr--;
	child->ctr++;
}

void merge(int pos, Node *curr) {
	Node *child = curr->child[pos]; // (child kiri)
	Node *sibling = curr->child[pos+1]; // (child kanan)

	// letakan key parent tepat diakhiran child kiri
	child->key[MIN+1] = curr->key[pos+1];
	
	// pindahkan isi child kanan ke node child kiri
	for(int i =1 ; i<=sibling->ctr; i++) {
		child->key[MIN+1+i] = sibling->key[i];
	}
	// bila bukan leaf pindahkan juga child kanan
	if(!isLeaf(child)) {
		for(int i =0 ; i<=sibling->ctr; i++) {
			child->child[MIN+1+i] = sibling->child[i];
			sibling->child[i] = NULL;
		}
	}
	// geser key key and child child pada node parent yang sekarang parent telah menjadi bagian dari node kiri
	for(int i = pos+1; i<curr->ctr; i++) {
		curr->key[i] = curr->key[i+1];
	}
	for(int i = pos+1; i<curr->ctr; i++) {
		curr->child[i] = curr->child[i+1];
	}
	// bersihkan hubungan child parent dengan node sibling
	curr->child[curr->ctr] = NULL;
	
	// sesuaikan ukuran node, node parent berkurang 1 key dan jumla key pada child ditamba dari 1 parent dan seluru key sibling( child kanan )
	curr->ctr--;
	child->ctr+=sibling->ctr+1;
	
	// dump node sibling (child kanan)
	free(sibling);
	sibling = NULL;
}
void fill(int pos, Node *curr) {
	if(pos+1<=curr->ctr&&curr->child[pos+1]->ctr > MIN) { // node sibling kanan masi dapat dikurangi
		fillFromNext(pos,curr); // ambil node terawal dari sibling kanan
	} else if(pos-1>=0&&curr->child[pos-1]->ctr > MIN) { // node sibling kiri masi dapat dikurangi
		fillFromPrev(pos,curr); // ambil node terakhir dari sibling kiri
	} else { // bila kedua sibling ta dapat dikurangi lakukan merge
		merge(pos-(pos!=0),curr); // selama pos bukan 0 posisi -1 merge akan dilakukan dengan sibling next
	}
}
void removeNonLeaf(int pos, Node *curr) {
	int k = curr->key[pos];
	
	// untuk remove non leaf yang pertama dilakukan yaitu melakukan pengecekan untuk child kiri dan kanan key
	
	if(curr->child[pos-1]->ctr > MIN) { // bila key pada child kiri dapat dikurangi
		curr->key[pos] = getPred(curr->child[pos-1]);// ambil value key predecessor dari child kiri dan delete key predecessor tersebut pada child
		deletion(curr->child[pos-1],curr->key[pos]);
	} else if(curr->child[pos]->ctr > MIN) { // bila key pada child kanan dapat dikurangi
		curr->key[pos] = getSucc(curr->child[pos]); // ambil value key successor dari child kanan dan delete key successor tersebut pada child
		deletion(curr->child[pos],curr->key[pos]);
	} else { // bila kedua child tidak dapat dikurangi lakukan merge
		merge(pos-(pos!=0),curr); // selama pos bukan 0 posisi -1 merge akan dilakukan dengan sibling next
		deletion(curr,k); // lanjutkan delesi key pada node yang telah di merge
	}
}
void deletion(Node *curr,int key) {
	int pos = find(curr,key); // tentukan posisi 
	if(curr->key[pos] == key) { // bila key yang ingin di delete berada tepat pada node
		if(isLeaf(curr)) { // bila node adalah daun lakukan delesi biasa
			removeLeaf(pos,curr);
		} else { // bila tidak 
			removeNonLeaf(pos,curr);
		}
	} else {
		if(!isLeaf(curr)) { // jika bukan leaf dan node dimana kita berada tidak memiliki key yang ingin kita delete
			if(curr->child[pos]->ctr <= MIN) { // bila jumla key pada node yang sedang kita kunjungi melanggar syarat
				fill(pos,curr); // lakukan fill 
				pos = find(curr,key); // tentukan kembali lokasi key yang ingin kita cari bila ada dilakukan merge atau pergeseran akibat fill
			}
			deletion(curr->child[pos],key); // lanjutkan rekursi ke child untuk mencari key yang ingin kita delete
		}
	}
}
int main() {
	// test data
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

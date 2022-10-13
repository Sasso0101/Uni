#include <iostream>

using namespace std;

const int SIZE = 100;

/* Memory structure */
/* Cell 0: 0 - not allocated, 1 - allocated, 2 - allocated and root */
int initialize(int memory[]) {
	for (int i = 0; i < SIZE*4; i+=4) {
		memory[i] = 0;
		memory[i+1] = -1;
		memory[i+2] = -1;
		memory[i+3] = -1;
	}
	memory[SIZE*4] = -1; //Set index of root to -1
	return 0;
}

/* Returns index of first free cell in memory
   Returns -1 if not found
 */
int findFreeCell(int memory[]) {
	int i = 0;
	while(i < SIZE*4 && memory[i] != 0) {
		i += 4;
	}
	if (i >= SIZE*4) return -1;
	else return i;
}

/* Returns true if given index is valid */
bool isValid(int index) {
	if (index%4 == 0 && index >= 0 && index < SIZE*4) return true;
	else return false;
}

/* Returns true if the cell at given index is allocated
   Returns false if index is not valid or cell is not allocated
 */
bool isAllocated(int index, int memory[]) {
	if (isValid(index) && memory[index] != 0) return true;
	return false;
}

/* Return true if the cell at given index is root
   Return false if index is not valid or cell is not root
 */
int isRoot(int index, int memory[]) {
	if (isValid(index) && memory[SIZE*4] == index) return true;
	return false;
}

/* Returns index of root of tree */
int getRoot(int memory[]) {
	return memory[SIZE*4];
}

/* Add node and returns index of node
   Returns -1 if there are no free cells
   Returns -2 if left child doesn't exist or index is not valid
   Returns -3 if right child doesn't exist or index is not valid
*/
int addNode(int value, int memory[], int leftChild = -1, int rightChild = -1) {
	if (leftChild != -1 && !isAllocated(leftChild, memory))
		return -2;
	if (rightChild != -1 && !isAllocated(rightChild, memory))
		return -3;
	int cell = findFreeCell(memory);
	if (cell == -1) return -1;

	if (isRoot(leftChild, memory) || isRoot(rightChild, memory) || getRoot(memory) == -1) {
		memory[SIZE*4] = cell;
	}
	memory[cell] = 1; // Set cell as allocated
	memory[cell+1] = value;
	memory[cell+2] = leftChild;
	memory[cell+3] = rightChild;
	return cell;
}

/* Returns value of cell at given index
   Returns -1 if index is not valid or cell is not allocated
 */
int getValue(int index, int memory[]) {
	if (!isValid(index) || memory[index] == 0) return -1;
	return memory[index+1];
}

/* Returns index of left child
   Returns -1 if index is not valid or cell is not allocated or there is no left child
 */
int getLeftChild(int index, int memory[]) {
	if (!isValid(index) || memory[index] == 0 || memory[index+2] == -1) return -1;
	return memory[index+2];
}

/* Returns index of right child
   Returns -1 if index is not valid, cell is not allocated or there is no right child
 */
int getRightChild(int index, int memory[]) {
	if (!isValid(index) || memory[index] == 0 || memory[index+3] == -1) return -1;
	return memory[index+3];
}

/* Returns true if node is leaf */
bool isLeaf(int index, int memory[]) {
	if (isAllocated(index, memory) && (getLeftChild(index, memory) != 0 && getRightChild(index, memory) != 0)) return true;
	return false;
}

void printTree(int memory[], int index = -2) {
	if (index == -2) index = getRoot(memory);
	if (memory[index] == 0) return;
	printTree(memory, getLeftChild(index, memory));
	cout << " " << memory[index+1] << " ";
	printTree(memory, getRightChild(index, memory));
}

int main() {
	int memory[(SIZE*4)+1];
	initialize(memory);
	int leaf1 = addNode(10, memory);
	int leaf2 = addNode(20, memory, leaf1);
	int leaf3 = addNode(30, memory);
	int leaf4 = addNode(40, memory, leaf2, leaf3);
	printTree(memory);
}
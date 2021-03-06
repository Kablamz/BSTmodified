/*
  Darsh Lin
  CPSC 2430
  May 8, 2016
  Version 2

  Version 1: I wrote a bunch of stuff in my design document
  that I will follow
  and change the logic depending on circumstances. Until then,
  this will just be modified and edited until I finish it.

  Version 2: Version 2 will be a clean up that looks to see if
  every function has an empty condition as well as
  what to do with replicates and not being able to find
  certain objects in the tree to delete. This will also insert
  and const keywords wherever needed.
 */



#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <iomanip>

using namespace std;

#ifndef BST_H
#define BST_H

template<class T>

class BST {
 private:
  class BinNode {
  public:
	T data;
	BinNode * left;
	BinNode * right;
	int height;
	int level;

  BinNode() : left(NULL), right(NULL) {}
  BinNode(T item): data(item), left(NULL), right(NULL){}
  };
  typedef BinNode * BinNodePtr;
  BinNodePtr myRoot;

  //helper functions
  bool searchAct(BinNodePtr ptr, const T &item)const;
  void recursivePreorderAct(BinNodePtr ptr, ostream & out) const;
  void treeCopier(BinNodePtr &originalPtr, BinNodePtr &copyPtr);
  int findHeight(BinNodePtr ptr)const;
  void treeDeleter(BinNodePtr &delPtr);
  int recursiveLevelAct(BinNodePtr ptr, const T &item, int &level);
  void insertRecurAct(BinNodePtr &ptr, const T & item);
  bool searchTreeAct(BinNodePtr &ptr);
  void levelOrder(ostream &out, int indent, BinNodePtr ptr)const;

 public: //[x] to mark them as done
  BST(); //[x]
  BST(const BST<T> &original);//[x]
  ~BST();//[x]
  BST<T> & operator= (const BST<T> &rhs); //[x]
  bool isEmpty(); //[x]
  bool search(const T &item); //[x]
  void insert(const T &item); //[x]
  void remove(const T &item); //[x]
  int recursiveLevel(const T &item);//[x]
  int nonrecursiveLevel(const T &item);//[x]
  void levelTraversal(ostream &out);//[x]
  void recursivePreorder(ostream & out);//[x]
  void nonrecursivePreorder(ostream &out);//[x]
  void postorder(ostream &out); //[x]
  void levelOrder(ostream &out, int indent);
  int height(); //[x]
  void display(ostream &out) const; //[x]
  void insertRecur(const T &item);
  bool searchIterative(const T &item);
  int heightIterative();
  bool searchTree();
};
template<class T>
ostream& operator<< (ostream &out, const BST<T> &rhs); //[x]


//Constructor
template <class T>
BST<T>::BST():
myRoot(NULL)
{}

/*
  COPY CONSTRUCTOR
  This is to copy the original and make sure it is a deep copy with the same nodes.
  In order to do this successfully, it needs to be done using pre order.
  Pre order makes it so there is no need to shift when copying the nodes.
  Root -> Left -> Right
  This will be done recursively.

  Precondition: A BST is made and it starts out with nothing, but needs to be copied.
  Start off using an original pointer pointed to the original root and a NULL root that
  is going to be tracked with a copyPtr.

  Poscondition: Tracking every node recursively with Pre Order the original root gets to the end
  after the furthest right node and all the nodes should be made to the copy in the exact same
  positions but with different addresses.
  
 */
template<class T>
BST<T>::BST(const BST<T> &original)
{
  if(original.myRoot == NULL) //check if the original is empty
	{
	  cerr << "There is nothing to copy" << endl;
	}
  else {
  myRoot = NULL; //This does not affect original because original is const.
  //This is used for *this->myRoot.
  BST<T>::BinNodePtr originalPtr;

  originalPtr = original.myRoot; //original pointer to track source nodes

  treeCopier(originalPtr, myRoot);
  }
  
}
//PRE ORDER HELPER FUNCTION (COPY)
//To do the copy constructor and overloaded assignment operator recursively, they need a helper
template<class T>
void BST<T>::treeCopier(BST<T>::BinNodePtr & originalPtr,
						BST<T>::BinNodePtr & copyPtr)
//called this parameter copyptr to deal with the confusion on myRoot being *this->myRoot
{
  if(originalPtr != NULL) {
	copyPtr = new BST<T>::BinNode(originalPtr->data); //copy takes place on every recursion or on every time a root needs to be processed

	//Traveling processes
	if(originalPtr->left != NULL) {
	  treeCopier(originalPtr->left, copyPtr->left);
	}
	if(originalPtr->right != NULL) {
	  treeCopier(originalPtr->right, copyPtr->right);
	}
  }
}
/*
  DESTRUCTOR
  The destructor is responsible for the clean up at the end of the program deleting all the nodes for any trees that have been made
  Much like the copy constructor, this is going to be done in a similar way except it's the opposite with Post Order.
  Post order makes sure that the destructor does not delete a parent node first and make a mess with a child in the abyss.
  Left -> Right -> Root
  This will be done recursively

  Pre Condition: There are trees left in memory that needs to be freed. Only one pointer is needed because there is nothing to track
  and everything will be done once NULL is achieved.

  Post Condition: There is nothing left and the deletion pointer is equal to NULL thereby freeing everything possible.
 */
template<class T>
BST<T>::~BST()
{
  BST<T>::BinNodePtr ptr = myRoot;
  stack<BST<T>::BinNodePtr> st1;
  stack<BST<T>::BinNodePtr> st2;
  st1.push(myRoot);

  while(!st1.empty())
	{											\
	  ptr = st1.top();
	  st1.pop();
	  st2.push(ptr);

	  if(ptr->left != NULL)
		st1.push(ptr->left);
	  if(ptr->right != NULL)
		st1.push(ptr->right);
	}
  while(!st2.empty())
	{
	  ptr = st2.top();
	  delete ptr;
	  st2.pop();
	}
  ptr = NULL;
}
/*
  TREE DELETER HELPER FUNCTION
  This is a private helper function used to recursively delete everything at the end of a program
 */
template<class T>
void BST<T>::treeDeleter(BST<T>::BinNodePtr &delPtr)
{
  if(delPtr != NULL)
	{
	  if(delPtr->left != NULL) {
		treeDeleter(delPtr->left);
	  }
	  if(delPtr->right != NULL) {
		treeDeleter(delPtr->right);
	  }
	  delete delPtr;//the deletion happens here or in other words, where the Root happens at the end of Post Order
	}
}
/*
  OVERLOADED ASSIGNMENT OPERATOR
  Overloaded assignment operator is used to assign the data from one tree to another right to left.
  It works almost the same as the copy constructor except that it has a few framing processes.
  It needs to establish that the current tree (left hand side) is not the same as the right hand side.
  If it is the same, it will just skip the if statement and return the current tree.
  If it is inside the if statement(not the same), it will then delete the current tree and copy the rhs one.

  Pre condition: Check to see if the two trees are the same. If they are, return this one. If not, delete this one.

  Post condition: The nodes have been copied from the rhs recursively and should be identical except for addresses.
 */
template <class  T>
BST<T> & BST<T>::operator= (const BST<T> &rhs)
{
  if(this != &rhs) { //This is already a pointer to whatever is on the lhs, it just needs rhs address, not value so no *rhs
	this->~BST(); //Using this to make sure that it deletes the lhs
	
	myRoot = NULL; //Making root null is always good
	BST<T>::BinNodePtr originalPtr = rhs.myRoot; //something to pass into the copier

	treeCopier(originalPtr, myRoot);//Since they're almost the same, I'll just reuse this function
  }
  return *this;
}

/*
  IS EMPTY
  Simple function to check whether or not the tree is empty.

  Pre condition: Is myRoot NULL?

  Post condition: If it is return true, if not return false.
 */
template <class T>
bool BST<T>::isEmpty()
{
  return (myRoot == NULL);
}

/*
  SEARCH
  Search is used to look for an object containing an item entered by the user.
  it will return the private helper function which returns either a true or
  false statement.
  Search function can almost be a modified insert function, but instead of looking for NULL nodes, it looks for !NULL ones.
  It will use the same greater than or less than processes that insert uses to find the target.
  This will be done with recursion.
  
  Pre Condition: The item is not found and starts at root.

  Post condition: The item  is found returning a true statement, or the item is not returning a false statement.
 */
template <class T>
bool BST<T>::search(const T &item)//Recursive
{
  return searchAct(myRoot, item);
}

/*
  SEARCH HELPER FUNCTION
  Used to recursively look through the tree for the target.
 */
template <class T>
bool BST<T>::searchAct(BST<T>::BinNodePtr ptr, const T &item) const
{
  if(myRoot == NULL) {
	cerr << "Nothing in tree" << endl;
	return false;
  }
  else
	{
	  if(item == ptr->data) {
		cout << "Item " << item << " Found!" << endl;
		return true;
	  }
	  if(item < ptr->data) {
		if (ptr->left != NULL) {
		  return searchAct(ptr->left, item);
		}
		else {
		  cout << "Item " << item <<" does not exist in tree" << endl;
		  return false; //If the pointer has to go less than its left most node, it does not exist
		}
	  }
	  if(item > ptr->data) {
		if(ptr->right != NULL) {
		  return searchAct(ptr->right, item);
		}
		else {
		  cout << "Item " <<item << " does not exist in tree" << endl;
		  return false; //If the pointer has to go more than its right most node, it does not exist
		}
	  }
	}
  return false; //If the pointer has to do anything funny, the item does not exist
}

template <class T>
bool BST<T>::searchIterative(const T &item)
{
  if (myRoot == NULL) {
	cerr << "Nothing to search for" << endl;
	return false;
  }
  else
	{
	  BST<T>::BinNodePtr ptr = myRoot;

	  while(ptr != NULL)
		{
		  if(item < ptr->data) {
			ptr = ptr->left;
		  }
		  else if(item > ptr->data) {
			ptr = ptr->right;
		  }
		  else {
			return true;
		  }
		}
	  return false;
	}
}

template<class T>
bool BST<T>::searchTree()
{
  return searchTreeAct(myRoot);
}

template<class T>
bool BST<T>::searchTreeAct(BST<T>::BinNodePtr &ptr)
{
  if(ptr->left != NULL) {
	if(ptr->left->data < ptr->data) {
	  return searchTreeAct(ptr->left);
	}
	else return false;
  }
  if(ptr->right != NULL) {
	if(ptr->right->data > ptr ->data) {
	  return  searchTreeAct(ptr->right);
	}
	else return false;
  }
  return true;
}
/*
  INSERT
  Insert is used to get the data input from main and sort it depending on
  higher or lower than the previous data.
  The previous data is the parent.
  Since this is to be done non recursively, there needs to be a way to track parent nodes too.
  Parent nodes are always one behind the child node, so they should be established right after
  a loop back and before any operations.
  The new nodes also cannot be made until there is an available NULL spot.

  Pre condition: An item needs to be inserted.

  Post condition: Item less than parent needs to go left of the parent, item more needs to go right until there is
  a NULL spot for the new node to occupy.
  
 */

template<class T>
void BST<T>::insert(const T &item) //non recursive
{
  BST<T>::BinNodePtr parent;
  BST<T>::BinNodePtr child;
  if(myRoot == NULL) {
	myRoot = new BST<T>::BinNode(item);//When empty, the first insert will e //stablish itself
  }
  else {
	parent = myRoot;
	child = myRoot;
	while(child != NULL) {
	  parent = child; //parent follows child in search as child becomes parent when it moves on.

	  if(item < child->data) {
	  child = child->left;
	}

	  else if(item > child->data) {
	  child = child->right;
	  }
	  else {
		cerr << "Item " << item << " is a duplicate" << endl;
		return; //handle duplicates by just cutting off and returning.
	  }
 	}

	//int avlHeight;


	//This is where the actual insert takes place
	if(item < parent->data) {
	  parent->left = new BST<T>::BinNode(item);
	}
	else if(item > parent->data) {
	  parent->right = new BST<T>::BinNode(item);
	}
	/* avlHeight = findHeight(parent->left) - findHeight(parent->right); */
	/* cout << "AVL Height " << avlHeight << endl; */
	/* //AVL tree starts here */
	/* if(avlHeight >= 2 || avlHeight <= -2) */
	/*   { */
	/*     cout << "Tree unbalanced" << endl; */
	/*   } */
	
  }
}
template<class T>
void BST<T>::insertRecur(const T &item)
{  
  insertRecurAct(myRoot, item);
}

template<class T>
void BST<T>::insertRecurAct(BST<T>::BinNodePtr &ptr, const T &item)
{
  if(ptr == NULL) {
	if(myRoot == NULL) {
	  myRoot = new BST<T>::BinNode(item);
	}
	else {
	  ptr = new BST<T>::BinNode(item);
	}
  }
  else {
  if(item < ptr->data) {
	insertRecurAct(ptr->left, item);
  }
  else if(item > ptr->data) {
	insertRecurAct(ptr->right, item);
  }
  else if(item == ptr->data) {
	cerr << item << " is a duplicate" << endl;
	return;
  }
  }
}
/*
  REMOVE
  Remove is a tricky one, but it can be broken down.
  The first thing that must be done is to traverse the tree and have a child drag a parent one node behind.
  This will be done in a way similar to insert.
  Once the node is found, there are three categories to look at:
  0 child. 1 child. 2 child.
  
  From there the code goes as follows.
  0 child
  - From the parent node, establish whether the child is a left or right child.
  - Once established, break the link to the condemned child with NULL.
  - Delete the condemned child.

  1 child
  - Find out whether the child has a left or right child.
  - Establish whether left or right of parent.
  - Break link between parent and child
  - delete child.
  - establish link with grandchild through the former child's original link side.

  2 child
  - go to the child's right child (right grandchild).
  - move as far left as possible and hold the node information for that last node.
  * At the same time, make a tracker to find parent of this descendant.
  - Switch the information of held node with the targeted child.
  - Figure out if the farthest left of the right grand child has a right child.
  - If so, break the link between the parent of smallest right and itself.
  - Establish link between smallest right parent and smallest right child.
  - Delete smallest right.

  Pre condition: Item needs to be deleted and it needs to be filtered through remove to identify it's children.

  Post condition: Item is deleted from tree with all corresponding links attached to corresponding children.
*/


template<class T>
void BST<T>::remove(const T &item) //non recursive
{
  if(isEmpty()) {
	  cerr << "Nothing to delete" << endl;
	}
  else {
  BST<T>::BinNodePtr parent = myRoot;
  BST<T>::BinNodePtr child = myRoot;
  BST<T>::BinNodePtr delPtr;

  while(item != child->data) {//find the item
	parent = child; //just like in life, the child drags the parent
	if(item < child->data) {
	  child = child->left;
	}
	else if(item > child->data) {
	  child = child->right;
	}
  }
  //this is where the processing takes place to see the condition
  //this will also be where the while loop breaks out 
  if(child->left == NULL && child->right == NULL) //0 child
	{
	  //tests
	  /* cout << "this is what is going to be deleted " << child->data<< endl; */
	  /* cout << "this is the parent of it " << parent->data << endl; */
	  if(child == parent->left) {
		parent->left = NULL;
	  }
	  else if(child == parent->right) {
		parent->right = NULL;
	  }
	  delete child;
	}//0 child

  /*1 child (needs two seperate if/else statements that cannot be
	encaspsulated with each other */
  else if(child->left != NULL && child->right == NULL)
	{//tests
	  /* cout << "This is single child with left not empty" << endl; */
	  /* cout << "To be deleted " << child->data << endl; */
	  /* cout << "Parent of condemned "<< parent->data <<endl; */

	  if(child == parent->left) {
		parent->left = NULL;
		parent->left = child->left;
	  }
	  else if(child == parent->right) {
		parent->right = NULL;
		parent->right = child->left;
	  }
	  delete child;
	  /* cout << "Replacement " << parent->left->data << endl; */
	}
  else if(child->left == NULL && child->right != NULL)
	{//tests
	  /* cout << "This is single child with right not empty" << endl; */
	  /* cout << "To be deleted " << child->data << endl; */
	  /* cout << "Parent of condemned " << parent->data << endl; */

	  if(child == parent->left) {
		parent->left =NULL;
		parent->left = child->right;
	  }
	  else if(child == parent->right) {
		parent->right = NULL;
		parent->right = child->right;
	  }
	  delete child;

	  /* cout << "Replacement " << parent->right->data << endl; */
	}
  //end of 1 child

  //2 children
  //This one will have to get the lowest in the right side, so go down once to the right with a temp pointer and then go all the way left with the find min
  //might be better to replace the value than the actual node.
  else if(child->left != NULL && child->right != NULL)
	{//tests
	  /* cout <<"This is the two children removal" << endl; */
	  /* cout << "To be deleted " << child->data << endl; */
	  /* cout << "Parent of condemned "  << parent->data << endl; */
	  
	  BST<T>::BinNodePtr smallestRight; //holder for value to be switched
	  BST<T>::BinNodePtr SRParent; //in case there's a right child with
	  //the smallest right, I want to be able to replace it with that child
	  smallestRight = child->right;//start finding replacement
	  while(smallestRight->left != NULL) {
		SRParent = smallestRight; //holds place of parent in case there's a
		//right
		smallestRight = smallestRight->left;
	  }//by the end of this loop, smallest right should be found
	  
	  if(child == parent->left) {
		//cout << "It's a lefty!" << endl;
		parent->left->data = smallestRight->data;
	  }
	  else if(child == parent->right) {
		//cout << "It's a righty!" << endl;
		parent->right->data = smallestRight->data;
	  }

	  //replace smallest right with child if there is one.
	  if(smallestRight->right != NULL) {
		SRParent->left = smallestRight->right;
	  }
	  else {
		SRParent->left = NULL;
		//NULL out anything that's going to be deleted because of link
	  }
	  delete smallestRight;
	  /* cout << "Replacement: " << parent->right->data << endl; */
	  
	}//If there's a problem, most likely it is due to something going wrong during the switch at two children

	 
  }//end of the main else condition

  
}
/*
  RECURSIVE LEVEL
  This function is used to find an item and return the level that it is located.
  The function is going to work a lot like a search function except that it iterates a counter every time it moves down a level.
  The error handling will happen when the item hits a null making the counter into -1.

  Pre condition: The item needs to be found and it starts with myRoot and 0.

v  Post condition: item is found and the number of levels it traversed is counted. The item is not found and went too far left or right making the counter -1.
 */
template<class T>
int BST<T>::recursiveLevel(const T &item)
{
  if(isEmpty())
	{
	  cerr << "No recursive levels to go through" << endl;
	  return -1;
	}
  else
	{
  int level = 0;
  return recursiveLevelAct(myRoot, item, level); //0 is for the level
	}
}

template<class T>
int BST<T>::recursiveLevelAct(BST<T>::BinNodePtr ptr, const T &item, int &level)
{
  if(ptr == NULL) {
	level = -1;
	return level;
  }
  else {
	if(item < ptr->data) {
	  level++;
	  return recursiveLevelAct(ptr->left, item, level);
	}
	if(item > ptr->data) {
	  level++;
	  return recursiveLevelAct(ptr->right, item, level);
	}
	if(item == ptr->data) {
	  cout << "Found " << item << endl;
	  return level;
	}
  }
  cout << "Went through entire tree" <<endl;
  return -1;

}
/*
  NON RECURSIVE LEVEL
  The non recursive level works in a way quite similar to the recursive.
  The difference is that it is iterative and needs a condition to keep it going which I can make as if ptr exists.
  I think I can make it so that oncce a break condition comes in, I can just make ptr NULL to stop it, but a break seems like a more for sure thing.

  Pre condition: Same as recursion version

  Post condition: Same as recursion version
 */

template<class T>
int BST<T>::nonrecursiveLevel(const T &item)
{
  if(isEmpty()) {
	cerr<< "No non recursive levels to go through" << endl;
	return -1;
  }
  else {
  BST<T>::BinNodePtr ptr;
  int level = 0;
  
  ptr = myRoot;
  while (ptr) { //While there is still a ptr it will move

	//Found
	if(item == ptr->data) { //once found, just break out
	  return level;
	}

	//Left Side
	if(item < ptr->data) {
	  if(ptr->left != NULL) {
	  ptr = ptr->left;
	  level++;
	  }
	  else {
		return -1;
	  }
	}	 
	//Right Side
	if(item > ptr->data) {
	  if(ptr->right != NULL) {
	  ptr = ptr->right;
	  level++;
	  }
	  else {return -1;}
	}
  }
  }
  return -1;
}

/*
  LEVEL TRAVERSAL
  This is used to print out the tree by level starting from the root which is level 0
  The level traversal goes from top to bottom, left to right.
  This is non recursive so a Queue needs to be made.
  The first thing that needs to be done is to push the root into the queue to initialize it.
  While condition checks to see the q is not empty.
  The first thing is to print the front because we are going from top to bottom, the q will push top first.
  After displaying, push the children of each node that has been displayed.
  In the end, just pop out the front and wait for the next one to do the same steps.

  Pre condition: Queue is made and the tree's first node is pushed in.

  Post condition: After having displayed the nodes and pushing in the children, the queue should be empty after popping all displayed.
  
 */

template<class T>
void BST<T>::levelTraversal(ostream &out) //non recursive
{
  if(isEmpty())
	{
	  cerr << "No levels to traverse" << endl;
	  return;
	}
  queue<BST<T>::BinNodePtr> q;
  BST<T>::BinNodePtr ptr;
  q.push(myRoot);

  while(!q.empty()) {
	ptr = q.front();
	out << ptr->data << " "; //display the data for the front node on every iteration

	if(ptr->left != NULL) { //push in left child
	  q.push(ptr->left);
	}
	if(ptr->right != NULL) { //push in right child
	  q.push(ptr->right);
	}
	q.pop(); //pop out the front once it's children are pushed in
  }
}

/*
  RECURSIVE PRE ORDER
  For Preorder, the order that the function must print is Node->Left->Right.
  Root -> Left -> Right

  Pre condition: starts at root.

  Post condition: printing and going through all fo the lefts, it goes to the rights and prints until NULL.
*/
template <class T>
void BST<T>::recursivePreorder(ostream &out)
{
  recursivePreorderAct(myRoot, out);
  out << endl;
}

template<class T>
void BST<T>::recursivePreorderAct(BST<T>::BinNodePtr ptr, ostream & out)
const
{
  if(myRoot != NULL){
	 out << ptr->data << " ";
  if(ptr->left != NULL) {
	recursivePreorderAct(ptr->left, out); //follow the left if not empty
  }
  if(ptr->right != NULL) {
	recursivePreorderAct(ptr->right, out); //follow the right if n empty
  }
  }
  else {
	cerr << "Tree is empty" << endl;
  }
}
/*
  NON RECURSIVE PREORDER
  This non recursive version is actually rather neat and elegant.
  Since it goes in a stack instead of Root->Left->Right, it's Root->Right->Left for styles sake.
  Condition is stack isn't empty.
  First part is the root processing so make the ptr top, print it, and then pop it.
  After that, traverse right, then traverse left.

  Pre condition: root is pushed into stack.

  Post condition: Stack is empty. Because of the backwards way it was done, the stack, which goes top first, printed and popped in
  Root->Left->Right
  
 */

template <class T>
void BST<T>::nonrecursivePreorder(ostream &out)
{
  if (isEmpty()) {
	cerr<< "Tree is empty" << endl;
	return;
  }
  else
	{
  BST<T>::BinNodePtr ptr;
  stack<BST<T>::BinNodePtr> st;
  ptr = myRoot;
  st.push(myRoot);

  while(!st.empty())
	{
	  //root processing
	ptr = st.top();
	out << ptr->data << " ";
	st.pop();
	//Right
	if(ptr->right != NULL) {
	  st.push(ptr->right);
	}
	//Left
	if(ptr->left != NULL) {
	  st.push(ptr->left);
	}
	}
  }
}
/*
  POST ORDER
  There isn't much to say about this one as it is the opposite of Pre order.
  It will follow the same operations as pre order, except it has a second stack to receive the pops.
  The printing will also go to the second stack because when the first is done popping into the second,
  it should be flipped upside down ready to print.
  Also because of the flipping nature, the Pre order part will be done in the original Root->Left->Right so
  that the second stack can have the opposite.

  Pre condition: It's technically pushed in root, but I want to say that the pre order is all popped and pushed into
  the second stack successfully flipping it.

  Post condition: With everything flipped, just print and pop and there's a postorder.
 */

template<class T>
void BST<T>::postorder(ostream &out) //non recursive
{
  if(isEmpty()) {
	cerr << "No post to order" << endl;
	return;
  }
  else
	{
  BST<T>::BinNodePtr ptr;
  stack<BST<T>::BinNodePtr> st1; //Need one stack to process and organize the pops
  stack<BST<T>::BinNodePtr> st2; //The other to pop and print
  ptr = myRoot;
  st1.push(myRoot);
  while(!st1.empty())
	{
	  ptr = st1.top();
	  st2.push(ptr);
	  st1.pop();

	  if(ptr->left != NULL) { //This is processed in the left to right way because when st2 pops, it will be opposite
		st1.push(ptr->left);
	  }
	  if(ptr->right != NULL) {
		st1.push(ptr->right);
	  }
	}//Once the loop is done every element should have been popped and pushed to st2
  while(!st2.empty())
	{
	  ptr = st2.top();
	  out << ptr->data << " ";
	  st2.pop();
	}
	}

}


/*
  HEIGHT
  Height will be done recursively
  It will have three counters with the last one holding the actual height of the longest line to a leaf node.
  Through most of the process, the tallest variable will be attached to left, but if right ever exceeds left, it will switch.
  I had some trouble understanding this at first, but then I just remembered that recursion occurs in a stack so on it's way back
  and popping out, it will move onto other functions, so it won't add on the first go, but once it gets back to an operation in stack, it will add.
  At the end, a -1 was put on tree height because the recursion process seemed easier when including the root.

  Pre condition: Height is 0 at root.

  Post condition: Height is the length it takes to get from root to furthest leaf node.
 */
template<class T>
int BST<T>::height()
{
  int treeHeight;
  treeHeight = findHeight(myRoot);//doing this adds the root into the function and returns it as 1 for both left and right, so a subtraction is needed every time.

  return treeHeight - 1; //minus the root
}

template<class T>
int BST<T>::findHeight(BST<T>::BinNodePtr ptr) const
{
  int leftHeight = 0;
  int rightHeight = 0;
  int tallest = 0;

  if(ptr != NULL) //This si accounting for the node, so if node were to == 0. I would have to subtract 1 for the node
	{
	  //travelling processes that on pop back will move on to the addition processes
	  if(ptr->left != NULL) {
		//cout << "left " <<leftHeight << endl;
		leftHeight = findHeight(ptr->left);
	  }
	  if(ptr->right != NULL) {
		//cout << "right " << rightHeight << endl;
		rightHeight = findHeight(ptr->right);
	  }

	  //height addition processes
	  if(left >= right) {
		tallest = leftHeight + 1;
		//cout << "leftHeight " <<leftHeight << endl;
	  }
	  else {
		tallest = rightHeight + 1;
		//cout << "rightHeight " << rightHeight << endl;
	  }
	  
	}		
  return tallest;
}

template<class T>
int BST<T>::heightIterative()
{
  int height;
  if(isEmpty()) {
	height = 0;
  }
  else
	{
	  queue<BST<T>::BinNodePtr> q;
	  BST<T>::BinNodePtr ptr;
	  q.push(myRoot); //when it comes back here in queue, it should be 0
	  height = 0;
	  while(1)
		{
		  int nodeCount = q.size();

		  if(nodeCount == 0)
			return height -1;
		  else
			height++;
		  


		  while(nodeCount > 0)
			{
		  ptr = q.front();
		  q.pop();
		  if(ptr ->left != NULL) 
			q.push(ptr->left);
		  if(ptr->right != NULL)
			q.push(ptr->right);
		  nodeCount--;
			}
		}
	}
  return 0;
}
/*
  DISPLAY
  This works in conjunction with the overloaded output operator.
  I'm not even sure how th overloaded assignment even works without this sometimes.
  Anyway, this is initially going to be printed In Order depending on time constraints.
  In order is simple on paper but very tricky when done non recursively.
  Since the processes are done in the middle, it gives way to a lot of weird and undesired outcomes.
  So the best bet would have been to push all the way left while pushing nodes at the same time.
  Once it's done doing that, it can process the pushed nodes and then go for the right side.
  It's a weird one really.

  Pre condition: Root pushed in and going left

  Post condition: All nodes on the left have been pushed in, then processed and then the right.
 */
/* template<class T> */
/* void BST<T>::display (ostream & out) const //printing In Order, change depending on time constraints */
/* { */
/*   //Left -> Root -> Right */
/*   BST<T>::BinNodePtr ptr; */
/*   //BST<T>::BinNodePtr stackTop; //need a place holder for top of stack */
/*   stack<BST<T>::BinNodePtr> st; */
/*   bool end = 0;//starter and stopper */
/*   ptr = myRoot; */
/*   while(end == 0) { */
/* 	if(ptr != NULL) {//Push all the way left because that's the lowest */
/* 	  st.push(ptr); */
/* 	  ptr = ptr->left; */
/* 	} */
/* 	else {//When ptr does == NULL */
/* 	  if(!st.empty()) {//And stack isn't empty */
/* 		ptr = st.top(); */
/* 		st.pop(); */
/* 		out << ptr->data << " "; */
/* 		ptr = ptr->right; */
/* 	  } */
/* 		else { */
/* 		  end = 1; */
/* 		} */
/* 	} */
/*   } */
/* } */

template<class T>
void BST<T>::display (ostream & out) const
{
  queue<BST<T>::BinNodePtr> qu;
  queue<BST<T>::BinNodePtr> printerQ;
  BST<T>::BinNodePtr ptr;
  BST<T>::BinNodePtr checker; //checks the children nodes
  //int LRGuard = 0;
  int levelNodes = 2; //max nodes in a level
  int nodeCount = 0; // count the number of nodes in a level, ignore root
  int level = 1; //
  //int levelCount = 0;
  int spacer;
  int indent;
  int divider = 20;
  int height;
  int printerLevel = 0;
  myRoot->level = 0;
  qu.push(myRoot);

  height = findHeight(myRoot);
  out << "height: "<< height << endl;
  
  
  while(!qu.empty() && level <= height)
    {
      ptr = qu.front();
      //out << ptr->data << "||" << ptr->level << " ";
      printerQ.push(ptr);
      qu.pop();//on first run root gets popped anyways, so don't worry about its level


       if(nodeCount >= levelNodes) //This needs work
	{
	  levelNodes = levelNodes*2;
	  level++;
	  nodeCount = 0; //reset
	}
      checker = ptr->left;
      if(checker != NULL)
	{
	  nodeCount++;
	  checker->level = level;
	  qu.push(checker);
	}
      else if(checker == NULL)
	{
	  nodeCount++;
	}
      
      checker = ptr->right;
      if(checker != NULL)
	{
	  nodeCount++;
	  checker->level = level;
	  qu.push(checker);
	}
      else if(checker == NULL)
	{
	  nodeCount++;
	}   
    }
  indent = 40;
  spacer = indent;
  out << setw(indent);
  //Almost done, need to adjust this to print out at the right levels
    while(!printerQ.empty())
    {
      ptr = printerQ.front();
      printerQ.pop();

      if(ptr->level > printerLevel)
	{
	  out << endl;
	  printerLevel++;
	  spacer = indent;
	  indent = (indent/2);
	  out << setw(indent);
	}
      out << ptr->data << setw(spacer);
      
    }
}


//DONE!


/*
  OVERLOADED OUTPUT OPERATOR

  I honestly went back to P1 and just copied it, not much else to say.

  Pre condition: It's looking at the right hand side.

  Post condition: It prints the right hand side.
 */

template<class T>
ostream &operator<<(ostream &out, const BST<T> &rhs) //This will be printed In Order
{
  rhs.display(out);
  return out;
}
#endif

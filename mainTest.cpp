#include <iostream>
#include "bstTest.h"

using namespace std;

int main()
{

  BST<int>tree;


  tree.insertRecur(55);
  tree.insertRecur(44);
  tree.insertRecur(32);
  tree.insertRecur(88);
  tree.insertRecur(17);
  tree.insertRecur(49);
  tree.insertRecur(62);
  tree.insertRecur(88);
  tree.insertRecur(92);
  // tree.insertRecur(100);
  // tree.insertRecur(159);
  // tree.insertRecur(200);
  // tree.insertRecur(300);
  // tree.insertRecur(3);
  // tree.insertRecur(5);
  // tree.insertRecur(18);

  tree.display(cout);
  cout << endl;
  
  return 0;
  
}

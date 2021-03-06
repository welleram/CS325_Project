/*************************************************
 *CS325 Project Group 30
 *Amy Weller, Joseph Valencia, Matthew Toro 
 *Nearest Neighbor Greedy Algorithm
 *to solve travelling salesmen problem
 *Inputs:  Set of points
 *Outputs:  Shortest Hamiltonian Cycle
 *
 ***************************************************/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cstddef>
#include <stdlib.h>
#include <sstream>
#include <ctime>
#include <cmath>
#include <climits>
#include <assert.h>
#include <vector>
#include "compare.hpp"

// global variable used to keep track of the tree's depth in case of stack overflow run time errors
int depth = 0;

/*
function to initialize the binary search tree.
param tree
pre: tree is not null
post:	tree size is 0
root is null
*/
void initBSTree(struct BSTree *tree)
{
	tree->cnt = 0;
	tree->root = 0;
}

/*
function to create a binary search tree.
param: none
pre: none
post: tree->count = 0
tree->root = 0;
*/
struct BSTree*  newBSTree()
{
	struct BSTree *tree = (struct BSTree *)malloc(sizeof(struct BSTree));
	assert(tree != 0);

	initBSTree(tree);
	return tree;
}

/*
function to free the nodes of a binary search tree
param: node  the root node of the tree to be freed
pre: none
post: node and all descendants are deallocated
*/
void _freeBST(struct Node *node)
{
	if (node != 0) {
		_freeBST(node->left);
		_freeBST(node->right);
		free(node);
	}
}

/*
function to clear the nodes of a binary search tree
param: tree    a binary search tree
pre: tree ! = null
post: the nodes of the tree are deallocated
tree->root = 0;
tree->cnt = 0
*/
void clearBSTree(struct BSTree *tree)
{
	_freeBST(tree->root);
	tree->root = 0;
	tree->cnt = 0;
}

/*
function to deallocate a dynamically allocated binary search tree
param: tree   the binary search tree
pre: tree != null;
post: all nodes and the tree structure itself are deallocated.
*/
void deleteBSTree(struct BSTree *tree)
{
	clearBSTree(tree);
	free(tree);
}

/*
function to determine if  a binary search tree is empty.
param: tree    the binary search tree
pre:  tree is not null
*/
int isEmptyBSTree(struct BSTree *tree) { return (tree->cnt == 0); }

/*
function to determine the size of a binary search tree
param: tree    the binary search tree
pre:  tree is not null
*/
int sizeBSTree(struct BSTree *tree) { return tree->cnt; }

/*
recursive helper function to add a node to the binary search tree.
param:  cur	the current root node
val	the value to be added to the binary search tree
pre:	val is not null
*/
struct Node *_addNode(struct Node *cur, struct city* val, int threshold)
{
	// avoid stack overflow run time error by checking if depth is too big
	if (depth >= 4350)
		return NULL;
	struct Node* newNode;
	
	//if current node doesn't exist, make it
	if (!cur) 
	{
		newNode = (struct Node*)malloc(sizeof(struct Node));
		assert(newNode != 0);
		newNode->val = val;
		newNode->left = newNode->right = 0;
		return newNode;
	}
	
	//if value is less than current node's value, go down left side, otherwise go down right
	if (compare(val, (struct city*)cur->val, threshold) == -1) 
	{
		depth++;
		cur->left = _addNode(cur->left, val, threshold);
	}
	else
	{
		depth++;
		cur->right = _addNode(cur->right, val, threshold);
	}
	return cur;
}

/*
function to add a value to the binary search tree
param: tree   the binary search tree
val		the value to be added to the tree
pre:	tree is not null
val is not null
pose:  tree size increased by 1
tree now contains the value, val
*/
void addBSTree(struct BSTree *tree, struct city* val)
{
	tree->root = _addNode(tree->root, val, tree->threshold);
	if (depth >= 4350)
	{
		depth = 0;
		return;
	}
	depth = 0;
	tree->cnt++;
}


/*
function to determine if the binary search tree contains a particular element
param:	tree	the binary search tree
val		the value to search for in the tree
pre:	tree is not null
val is not null
post:	none
*/
int containsBSTree(struct BSTree *tree, struct city* val)
{
	struct Node* cur = tree->root;
	
	//keep going down until you find a null Node, then return 0 if not found, 1 if found
	while (cur != 0) 
	{
		if (compare(val, (struct city*)cur->val, tree->threshold) == 0)
			return 1;
		else if (compare(val, (struct city*)cur->val, tree->threshold) == 1)
			cur = cur->right;
		else
			cur = cur->left;
	}
	return 0;
}

/*
helper function to find the left most child of a node
return the value of the left most child of cur
param: cur		the current node
pre:	cur is not null
post: none
*/
TYPE _leftMost(struct Node *cur)
{
	//keep going down left side till you reach the end
	while (cur->left != 0) 
	{
		cur = cur->left;
	}
	return cur->val;
}


/*
recursive helper function to remove the left most child of a node
this function returns cur if its left child is NOT NULL. Otherwise,
it returns the right child of cur and free cur.
param: cur	the current node
pre:	cur is not null
post:	the left most node of cur is not in the tree
*/
/*----------------------------------------------------------------------------*/
struct Node *_removeLeftMost(struct Node *cur)
{
	struct Node* temp;
	
	//if you find the last node on the left side, remove it
	if (cur->left == 0) 
	{
		//if you find the last node on the left side, remove it
		temp = cur->right; //if you find the last node on the left side, remove it
		free(cur);
		cur = 0;
		return temp;
	}
	
	//keep going down left
	cur->left = _removeLeftMost(cur->left); 
	return cur;
}

/*
recursive helper function to remove a node from the tree
param:	cur	the current node
val	the value to be removed from the tree
pre:	val is in the tree
cur is not null
val is not null
*/
/*----------------------------------------------------------------------------*/
struct Node *_removeNode(struct Node *cur, struct city* val, int threshold)
{
	struct Node* temp;
	
	//if you find the node, remove it
	if (compare(val, (struct city*)cur->val, threshold) == 0) 
	{
		if (compareID(val, (struct city*)cur->val) == 0) 
		{
			//if it has a right node, replace it with the leftmost of the right side
			//otherwise replace it with the left side
			if (cur->right != 0) 
			{
				cur->val = _leftMost(cur->right);
				cur->right = _removeLeftMost(cur->right);
			}
			else 
			{
				temp = cur->left;
				free(cur);
				cur = 0;
				return temp;
			}
		}
		else {
			cur->right = _removeNode(cur->right, val, threshold);
		}
	}
	//keep going down left or right looking for the node
	else if (compare(val, (struct city*)cur->val, threshold) == -1) 
		cur->left = _removeNode(cur->left, val, threshold);
	else
		cur->right = _removeNode(cur->right, val, threshold);
	return cur;
}

/*
function to remove a value from the binary search tree
param: tree   the binary search tree
val		the value to be removed from the tree
pre:	tree is not null
val is not null
val is in the tree
pose:	tree size is reduced by 1
*/
void removeBSTree(struct BSTree *tree, struct city* val)
{
	if (containsBSTree(tree, val)) 
	{
		tree->root = _removeNode(tree->root, val, tree->threshold);
		tree->cnt--;
	}
}

void printNode(struct Node *cur) 
{
	if (cur == 0) 
		return;
	
	printNode(cur->left);
	/*Call print_type which prints the value of the TYPE*/
	print_type((struct city*)cur->val);
	printNode(cur->right);
}

void printTree(struct BSTree *tree) 
{
	if (tree == 0) 
		return;
	printNode(tree->root);
}

struct BSTree *buildBSTTree(struct city** pCity, int num, int threshold) 
{
	struct BSTree *tree = newBSTree();
	tree->threshold = threshold;
	for (int i = 0; i<num; i++) 
	{
		addBSTree(tree, pCity[i]);
	}

	return tree;
}

 /*****************************************************************
 *checkTime() uses time() from the C++ std library to check curr time
 *it is passed in the start time of the program
 * if time exceeds 180 seconds, program will exit
  ******************************************************************/
void checkTime(long long int start)
{
	long long int currTime = time(0);
	long long int secondsElapsed = (currTime - start);
	
	if(secondsElapsed >= 180)
	{ 
		// 180 seconds elapsed -> leave main lopp 
		cout<<"Time limit exceeded.   Exiting program"<<endl;
		exit (0);
	}
}

/*******************************************************************
*distance() takes in a pointer to two city structs and calculates
*the distance between their x and y coordinates.
*******************************************************************/
long long int distance(city* c1, city* c2)
{
	long long int xDistance = pow(c1->cityXCoord - c2->cityXCoord, 2);
	long long int yDistance = pow(c1->cityYCoord - c2->cityYCoord, 2);
	return  (long long int) round(sqrt(xDistance + yDistance));
}

/*******************************************************************
*nearestNeighbor() takes input of a vector of cities and an ofstream 
*it will call distance to calculate the nearest neighbor to the
*current city, mark it as visited and then move on to the
*nearest neighbor, repeating until the entire route is complete
*It will then write to the ofstream object the total length of the tour
* and then print all the city ID's in the order they were visited
*******************************************************************/
void nearestNeighbor(vector <city> &C, ofstream &outputFile)
{	 
	//run algorithm, be sure to call checkTime frequently.
	city* cur = &C[0];
	cur->visited = true;
	int index;
	long long int tour = 0;
	for (int i = 0; i < C.size(); i++)
	{
		
		long long int min = LLONG_MAX;
		for (int k = 0; k < C.size(); k++)
		{
			if (cur->cityID != C[k].cityID && C[k].visited != true)
			{
				long long int temp = distance(cur, &C[k]);
				if  (temp < min)
				{
					min = temp;
					cur->closestNeighbor = &C[k];
					cur->distanceToNeighbor = min;
					index = k;
				}
			}
		}
		tour+=cur->distanceToNeighbor;
		cur = &C[index];
		cur->visited = true;
	}
	
	//complete the circuit routing the last city back to the first
	cur->distanceToNeighbor = distance(cur, &C[0]);
	tour+=cur->distanceToNeighbor;
	cur->closestNeighbor = &C[0];
	 
	//place the algorithim's trip count into the first line of out file
	outputFile << tour;
	outputFile << endl;
	 
	cur = &C[0];
	for (int j = 0; j < C.size(); j++)
	{
		outputFile << cur->cityID;
		outputFile << endl;
		cur = cur->closestNeighbor;
	}
	outputFile.close(); 
 }

int nearestNeighborNoWriteout(vector <city> &C)
{
	//run algorithm, be sure to call checkTime frequently.
	city* cur = &C[0];
	cur->visited = true;
	int index;
	long long int tour = 0;
	for (int i = 0; i < C.size(); i++)
	{
		long long int min = LLONG_MAX;
		for (int k = 0; k < C.size(); k++)
		{
			if (cur->cityID != C[k].cityID && C[k].visited != true)
			{
				long long int temp = distance(cur, &C[k]);
				if (temp < min)
				{
					min = temp;
					cur->closestNeighbor = &C[k];
					cur->distanceToNeighbor = min;
					index = k;
				}
			}
		}
		tour += cur->distanceToNeighbor;
		cur = &C[index];
		cur->visited = true;
	}

	// complete the circuit routing the last city back to the first
	cur->distanceToNeighbor = distance(cur, &C[0]);
	tour += cur->distanceToNeighbor;
	cur->closestNeighbor = &C[0];
	return tour;
}
 
int main(int argc, char* argv[])
{
	//accept input from the command line
	char* inFileName = argv[1];
	
	//variable for start time to track count up to 180 seconds
	long long int start = time(0);
	cout<<"Start time is "<<start<<endl;
	
	 //read from file using ifstream
	ifstream inputFile;		
	inputFile.open(inFileName);
	
	//write to file using ofstream
	ofstream outputFile;
	
	//file name is apended to .tour
	string outFileName = string(inFileName) + ".tour";
	
	//open file for writing
	outputFile.open(outFileName.c_str());
	
	//this is the vector to store all cities on the route
	vector <city> route;	
	vector <city> route2;
	vector <city> tempRoute;
	
	//read untl the end of the file
	int cityValue;
	int xValue;
	int yValue;
	int i = 0;
	long long int sumDistance = 0;
	int j = 0;
	while (inputFile >> cityValue)
	{
		route.push_back(city());
		route2.push_back(city());

		inputFile >> xValue;
		inputFile >> yValue;
		
		route[i].cityID = cityValue;
		route[i].cityXCoord = xValue;
		route[i].cityYCoord = yValue;
		route[i].visited = false;
		
		if (i == 0)
		{
			tempRoute.push_back(city());
			tempRoute[0] = route[i];
		}
		if (i > 0 && i % 10 == 0) {
			sumDistance += distance(&tempRoute[0], &route[i]);
			tempRoute[0] = route[i];
			j++;
		}
		i++;
	}
	 
	struct city** pRoute = new struct city*[i];
	for (int h = 0; h < i; h++)
	{
		pRoute[h] = (struct city*)malloc(sizeof(struct city*));
		pRoute[h] = &route[h];
	}
	int average = sumDistance/j;
	int max = INT_MAX;
	int bestAverage = average;
	BSTree* myTree;
	int testAverage;
	for (int k = 0; k < 5; k++)
	{
		testAverage = average / 4 * (pow(2, k));
		myTree = buildBSTTree(pRoute, i, testAverage);
		if (myTree->cnt != i) {
		}
		else 
		{
			for (int h = 0; h < i; h++) 
			{
				city* temp = (city*)_leftMost(myTree->root);
				route2[h].cityID = (*temp).cityID;
				route2[h].cityXCoord = (*temp).cityXCoord;
				route2[h].cityYCoord = (*temp).cityYCoord;
				_removeNode(myTree->root, temp, testAverage);
			}
			int tour = nearestNeighborNoWriteout(route2);
			if (tour < max)
			{
				max = tour;
				bestAverage = testAverage;
			}
			route2 = route;
		}
	}
	
	myTree = buildBSTTree(pRoute, i, bestAverage);
	for (int h = 0; h < i; h++) 
	{
		city* temp = (city*)_leftMost(myTree->root);
		route2[h].cityID = (*temp).cityID;
		route2[h].cityXCoord = (*temp).cityXCoord;
		route2[h].cityYCoord = (*temp).cityYCoord;
		_removeNode(myTree->root, temp, bestAverage);
	}
	
	//call stub for algorithm.
	nearestNeighbor(route2, outputFile);
	
	long long int finish = time(0);
	cout<<"Finish time is "<<finish<<endl;
	long long int dur = finish - start;
	cout<<"Duration  is "<<dur<<endl;	 
	return 0;
}

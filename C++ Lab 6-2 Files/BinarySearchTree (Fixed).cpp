//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Luna
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// FIXED (1): Internal structure for tree node
struct Node {
	Bid bid;
	Node* left;
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right= nullptr;
	}

	// initialize a given bid
	Node(Bid aBid) : Node() {
		this->bid = aBid;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // FIXED (2a) Implement inserting a bid into the tree
	if (root == nullptr) {
		root = new Node(bid);
	} else {
		this->addNode(root, bid);
	}
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // FIXME (4a) Implement removing a bid from the tree
	this->removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FIXED (3) Implement searching the tree for a bid

	// start searching from the root
	Node* current = root;

	// keep looping downwards until bottom reached or bid found
	while (current != nullptr) {
		//if current node matches, return it
		if (current->bid.bidId.compare(bidId) == 0) {
			return current->bid;
		}
		//if bid is smaller than current then traverse left
		if (bidId.compare(current->bid.bidId) < 0) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

	Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // FIXED (2b) Implement inserting a bid into the tree

	//if node is larger than bid, add to left subtree
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(bid);
		} else {
			this->addNode(node->left, bid);
		}
	}
	//add to right subtree
	else {
		if (node->right == nullptr) {
			node->right = new Node(bid);
		} else {
			this->addNode(node->right, bid);
		}
	}
}
void BinarySearchTree::inOrder(Node* node) {
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
	// if this node is null then return (avoid crashing)
	if (node == nullptr) {
		return node;
	}

	// recurse down left subtree
	if (bidId.compare(node->bid.bidId) < 0) {
		node->left = removeNode(node->left, bidId);
	} else if (bidId.compare(node->bid.bidId) > 0){
		node->right = removeNode(node->right, bidId);
	} else {
		// no children so this is a leaf node
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// once child to the left
		else if (node->left != nullptr && node-> right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		// one child to the right
		else if (node->right != nullptr && node->left == nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		// two children
		else {
			Node* temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->bid = temp->bid;
			node->right = removeNode(node->right, temp->bid.bidId);
		}
	}
	return node;
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}

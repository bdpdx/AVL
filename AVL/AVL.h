//
//  AVL.h
//
//  Created by Brian Doyle on 4/14/14.
//  Copyright (c) 2014 Balance Software. All rights reserved.
//
//  Useful info about AVL trees available at:
//
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-impl.html
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-insert.html
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-delete.html


#ifndef __AVL_h__
#define __AVL_h__


#include <assert.h>
#include <stdlib.h>
#include <sys/errno.h>

enum AVLTraverseMethod {
	kAVLTraverseBreadthFirst,
	kAVLTraverseInfix,
	kAVLTraversePrefix,
	kAVLTraversePostfix
};

#ifndef kAVLMaxHeight
	// This AVL implementation may contain no more than 2^kAVLMaxHeight - 1 nodes.
	// No checking is performed so redefine kAVLMaxHeight if required.
	#define kAVLMaxHeight			32
#endif


template<typename K, typename V = void> class AVL {
	
protected:
	
	struct AVLNode;
	
public:
	
	// AVLComparator return value is to zero as lhs is to rhs
	typedef long (*AVLComparator)( const K &lhs, const K &rhs );
	// AVLTraverseCallback should return true to stop traversing
	typedef bool (*AVLTraverseCallback)( const K &key, V *value, void *context );
	
	AVL( AVLComparator comparator ) { _comparator = comparator; _root = NULL; }
	virtual ~AVL() { clear(); }
	
	void clear() { clear( _root ); _root = NULL; }
	bool find( const K &key, V **value = NULL ) const;
	void insert( const K &key, V *value = NULL );
	void remove( const K &key );
	void traverse( AVLTraverseCallback callback, void *context = NULL, AVLTraverseMethod method = kAVLTraverseInfix ) const { traverse( _root, callback, context, method ); }
	
protected:
	
	struct AVLNode {
		AVLNode( const K &key, V *value ) { _key = key; _height = 1; _left = _right = NULL; _value = value; }
		AVLNode &operator=( const AVLNode &rhs ) { _key = rhs._key; _left = rhs._left; _right = rhs._right; _value = rhs._value; return *this; }
		
		K							_key;
		long						_height;
		AVLNode *					_left;
		AVLNode *					_right;
		V *							_value;
	};
	
	struct AVLQueue {
		AVLQueue() { _head = NULL; _last = &_head; }
		
		struct AVLQueueNode {
			AVLQueueNode( AVLNode *node ) { _next = NULL; _node = node; }
			
			AVLQueueNode *			_next;
			AVLNode *				_node;
		};
		
		AVLNode *pop() { AVLQueueNode *head = _head; AVLNode *node; if ( head ) { if ( ! ( _head = _head->_next ) ) _last = &_head; node = head->_node; delete head; } else { node = NULL; } return node; }
		void push( AVLNode *node ) { *_last = new AVLQueueNode( node ); _last = &(*_last)->_next; }
		
		AVLQueueNode *				_head;
		AVLQueueNode **				_last;
	};
	
	void clear( AVLNode *root ) { if ( root ) { clear( root->_left ); clear( root->_right ); delete root; } }
	long height( AVLNode *node ) const { AVLNode *l = node->_left, *r = node->_right; long hl = l ? l->_height : 0, hr = r ? r->_height : 0; return 1 + ( hl > hr ? hl : hr ); }
	bool traverse( AVLNode *root, AVLTraverseCallback callback, void *context, AVLTraverseMethod method ) const;
	
#if ENABLE_AVL_UNIT_TESTS
	void verifyAVL() const { assert( verifyAVL( _root ) ); }
	bool verifyAVL( AVLNode *root ) const;
#endif
	
	AVLComparator					_comparator;
	AVLNode *						_root;
	
};

#pragma mark -

template<typename K, typename V> bool AVL<K,V>::find( const K &key, V **value ) const {
	long							c;
	AVLNode *						root;
	
	for ( root = _root; root; ) {
		c = _comparator( key, root->_key );
		
		if ( c < 0 ) root = root->_left;
		else if ( c > 0 ) root = root->_right;
		else {
			if ( value ) *value = root->_value;
			return true;
		}
	}
	
	return false;
}

template<typename K, typename V> void AVL<K,V>::insert( const K &key, V *value ) {
	long							c, index, height;
	K								k;
	AVLNode *						left, *node, *right, *x, *y, *z;
	AVLNode	*						path[ kAVLMaxHeight + 1 ];
	AVLNode **						root;
	
	for ( index = 0, root = &_root; ( node = *root ); ++index ) {
		path[ index ] = node;
		
		c = _comparator( key, node->_key );
		
		if ( c < 0 ) root = &node->_left;
		else if ( c > 0 ) root = &node->_right;
		else return;				// ignore duplicates
	}
	
	path[ index ] = *root = new AVLNode( key, value );
	
	for ( height = 2; index; ++height ) {
		x = path[ --index ];
		
		// if the height of x is <= 2 the tree at x is balanced
		if ( ( x->_height = height ) <= 2 ) continue;
		
		// since the height is >= 3, if either child is NULL or the absolute
		// difference in height between the children is > 1 the tree at x is unbalanced
		if ( ( left = x->_left ) && ( right = x->_right ) && ( c = left->_height - right->_height ) >= -1 && c <= 1 ) continue;

		y = path[ ++index ];
		z = path[ ++index ];
		
		--x->_height;
		--y->_height;
		
		if ( y == x->_left ) {
			if ( z == y->_left ) {
				k = x->_key;                    /*         x                y        */
				x->_key = y->_key;              /*        / \             /   \      */
				y->_key = k;                    /* 	     y   3           z     x     */
				x->_left = z;                   /*      / \      =>           / \    */
				y->_left = y->_right;           /*     z   2                 2   3   */
				y->_right = x->_right;          /*                                   */
				x->_right = y;                  /*                                   */
			} else {
				k = x->_key;                    /*       x                  z        */
				x->_key = z->_key;              /*      / \               /   \      */
				z->_key = k;                    /* 	   y   3             y     x     */
				y->_right = NULL;                /*    / \        =>     /       \    */		// y->_right = z->_left
//				z->_left = z->_right;           /*   0   z             0         3   */		// z->_left|right are NULL
				z->_right = x->_right;          /*                                   */
				x->_right = z;                  /*                                   */
			}
		} else {
			if ( z == y->_left ) {
				k = x->_key;                    /*     x                    z        */
				x->_key = z->_key;              /*    / \                 /   \      */
				z->_key = k;                    /* 	 0   y               x     y     */
				y->_left = NULL;                 /*      / \      =>     /       \    */		// y->_left = z->_right
//				z->_right = z->_left;           /*     z   3           0         3   */		// z->_left|right are NULL
				z->_left = x->_left;            /*                                   */
				x->_left = z;                   /*                                   */
			} else {
				k = x->_key;                    /*     x                    y        */
				x->_key = y->_key;              /*    / \                 /   \      */
				y->_key = k;                    /* 	 0   y               x     z     */
				x->_right = z;                  /*      / \      =>     / \          */
				y->_right = y->_left;           /*     1   z           0   1         */
				y->_left = x->_left;            /*                                   */
				x->_left = y;                   /*                                   */
			}
		}
		
		break;
	}
	
#if ENABLE_AVL_UNIT_TESTS
	verifyAVL();
#endif
}

template<typename K, typename V> void AVL<K,V>::remove( const K &key ) {
	long							c, heightLeft, heightRight, index;
	K								k;
	AVLNode *						left, *node, *right, *x, *y, *z;
	AVLNode	*						path[ kAVLMaxHeight + 1 ];
	AVLNode **						root, **successor;
	
	for ( index = 0, root = &_root; ( node = *root ); ++index ) {
		path[ index ] = node;
		
		c = _comparator( key, node->_key );
		
		if ( c < 0 ) root = &node->_left;
		else if ( c > 0 ) root = &node->_right;
		else goto found;
	}
	
	return;
	
found:
	
	// root now points to the node * to be deleted
	
	left = node->_left;
	right = node->_right;
	
	if ( ! left && ! right ) {
		*root = NULL;
		
		delete node;
	} else if ( ! left ) {
		*root = right;
		
		delete node;
	} else if ( ! right ) {
		*root = left;
		
		delete node;
	} else {
		// find node's successor: go right then all the way left
		path[ ++index ] = right;
		
		for ( successor = &node->_right; (*successor)->_left; successor = &(*successor)->_left ) {
			path[ ++index ] = (*successor)->_left;
		}
		
		node->_key = (*successor)->_key;
		node = *successor;
		
		*successor = (*successor)->_right;
		
		delete node;
	}
	
	// recompute height and rebalance if necessary
	
	while ( index ) {
		x = path[ --index ];
		
		left = x->_left;
		right = x->_right;
		
		if ( ! left && ! right ) {
			x->_height = 1;
			
			continue;
		} else if ( ! left ) {
			if ( ( x->_height = right->_height + 1 ) <= 2 ) continue;
			
			y = right;
		} else if ( ! right ) {
			if ( ( x->_height = left->_height + 1 ) <= 2 ) continue;
			
			y = left;
		} else {
			heightLeft = left->_height;
			heightRight = right->_height;
			
			x->_height = 1 + ( heightLeft > heightRight ? heightLeft : heightRight );
			
			if ( heightLeft > heightRight + 1 ) {
				y = left;
			} else if ( heightRight > heightLeft + 1 ) {
				y = right;
			} else {
				continue;
			}
		}
		
		left = y->_left;
		right = y->_right;
		
		z = left && ( ! right || left->_height > right->_height ) ? left : right;
		
		if ( y == x->_left ) {
			if ( z == y->_left ) {
				k = x->_key;                    /*         x                y        */
				x->_key = y->_key;              /*        / \             /   \      */
				y->_key = k;                    /* 	     y   3           z     x     */
				x->_left = z;                   /*      / \      =>     / \   / \    */
				y->_left = y->_right;           /*     z   2           0   1 2   3   */
				y->_right = x->_right;          /*    / \                            */
				x->_right = y;                  /*   0   1                           */
				
				left = z;
				right = y;
			} else {
				k = x->_key;                    /*       x                  z        */
				x->_key = z->_key;              /*      / \               /   \      */
				z->_key = k;                    /* 	   y   3             y     x     */
				y->_right = z->_left;           /*    / \        =>     / \   / \    */
				z->_left = z->_right;           /*   0   z             0   1 2   3   */
				z->_right = x->_right;          /*      / \                          */
				x->_right = z;                  /*     1   2                         */
				
				left = y;
				right = z;
			}
		} else {
			if ( z == y->_left ) {
				k = x->_key;                    /*     x                    z        */
				x->_key = z->_key;              /*    / \                 /   \      */
				z->_key = k;                    /* 	 0   y               x     y     */
				y->_left = z->_right;           /*      / \      =>     / \   / \    */
				z->_right = z->_left;           /*     z   3           0   1 2   3   */
				z->_left = x->_left;            /*    / \                            */
				x->_left = z;                   /*   1   2                           */
				
				left = z;
				right = y;
			} else {
				k = x->_key;                    /*     x                    y        */
				x->_key = y->_key;              /*    / \                 /   \      */
				y->_key = k;                    /* 	 0   y               x     z     */
				x->_right = z;                  /*      / \      =>     / \   / \    */
				y->_right = y->_left;           /*     1   z           0   1 2   3   */
				y->_left = x->_left;            /*        / \                        */
				x->_left = y;                   /*       2   3                       */
				
				left = y;
				right = z;
			}
		}
		
		left->_height = heightLeft = height( left );
		right->_height = heightRight = height( right );
		
		x->_height = 1 + ( heightLeft > heightRight ? heightLeft : heightRight );
	}
	
#if ENABLE_AVL_UNIT_TESTS
	verifyAVL();
#endif
}

template<typename K, typename V> bool AVL<K,V>::traverse( AVLNode *root, AVLTraverseCallback callback, void *context, AVLTraverseMethod method ) const {
	AVLQueue						queue;
	bool							stop;
	
	if ( ! root ) return false;

#if ENABLE_AVL_UNIT_TESTS
	root->_value = &root->_height;
#endif

	switch ( method ) {
		case kAVLTraversePrefix: {
			stop = callback( root->_key, root->_value, context );
			if ( ! stop ) stop = traverse( root->_left, callback, context, method );
			if ( ! stop ) stop = traverse( root->_right, callback, context, method );
		} break;
			
		case kAVLTraverseInfix: {
			stop = traverse( root->_left, callback, context, method );
			if ( ! stop ) stop = callback( root->_key, root->_value, context );
			if ( ! stop ) stop = traverse( root->_right, callback, context, method );
		} break;
			
		case kAVLTraversePostfix: {
			stop = traverse( root->_left, callback, context, method );
			if ( ! stop ) stop = traverse( root->_right, callback, context, method );
			if ( ! stop ) stop = callback( root->_key, root->_value, context );
		} break;
			
		case kAVLTraverseBreadthFirst: {
			while ( ! ( stop = callback( root->_key, root->_value, context ) ) ) {
				if ( root->_left ) queue.push( root->_left );
				if ( root->_right ) queue.push( root->_right );
				if ( ! ( root = queue.pop() ) ) break;
			}
		} break;
			
		default:					stop = true;		break;
	}
	
	return stop;
}

#if ENABLE_AVL_UNIT_TESTS

inline long AVLAbs( long n ) { return n < 0 ? -n : n; }

template<typename K, typename V> bool AVL<K,V>::verifyAVL( AVLNode *root ) const {
	if ( ! root ) return true;
	
	if ( ! root->_left && ! root->_right ) {
		return root->_height == 1;
	} else if ( ! root->_left ) {
		return root->_right->_height == root->_height - 1 && verifyAVL( root->_right );
	} else if ( ! root->_right ) {
		return root->_left->_height == root->_height - 1 && verifyAVL( root->_left );
	} else {
		return
			verifyAVL( root->_left ) &&
			verifyAVL( root->_right ) &&
			AVLAbs( root->_left->_height - root->_right->_height ) <= 1 &&
			( root->_left->_height == root->_height - 1 || root->_right->_height == root->_height - 1 );
	}
}

#endif


#endif // __AVL_h__

//
//  AVLFramework_OSXTests.swift
//  AVLFramework OSXTests
//
//  Created by Brian Doyle on 7/30/15.
//  Copyright © 2015 Balance Software. All rights reserved.
//

import XCTest

@testable import AVLFramework

class AVLFramework_OSXTests: XCTestCase {
    
    override func setUp() {
        super.setUp()
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }

	// MARK: - Insertion tests -

	func testInsert0() {
		let avl = AVL<Character, Void>()
		
		insert(avl, key: "a")

		expect(avl, infix: "a", breadth: "1:a")
	}

	func testInsert1() {
		var avl = AVL<Character, Void>()
		
		avl.insert( "a" )
		avl.insert( "b" )
		avl.insert( "c" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")

		avl = AVL<Character, Void>()

		avl.insert( "a" )
		avl.insert( "c" )
		avl.insert( "b" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")
		
		avl = AVL<Character, Void>()

		avl.insert( "b" )
		avl.insert( "a" )
		avl.insert( "c" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")
		
		avl = AVL<Character, Void>()

		avl.insert( "b" )
		avl.insert( "c" )
		avl.insert( "a" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")
		
		avl = AVL<Character, Void>()

		avl.insert( "c" )
		avl.insert( "a" )
		avl.insert( "b" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")
		
		avl = AVL<Character, Void>()
		
		avl.insert( "c" )
		avl.insert( "b" )
		avl.insert( "a" )
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c")
	}

	func testInsert2() {
		var avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("b")
		avl.insert("a")
		expect(avl, infix: "a,b,d,h,l", breadth: "3:h,2:b,1:l,1:a,1:d")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("b")
		avl.insert("c")
		expect(avl, infix: "b,c,d,h,l", breadth: "3:h,2:c,1:l,1:b,1:d")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("e")
		expect(avl, infix: "d,e,f,h,l", breadth: "3:h,2:e,1:l,1:d,1:f")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("g")
		expect(avl, infix: "d,f,g,h,l", breadth: "3:h,2:f,1:l,1:d,1:g")
	
		// test right-side
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("g")
		avl.insert("n")
		avl.insert("m")
		expect(avl, infix: "d,f,g,h,l,m,n", breadth: "3:h,2:f,2:m,1:d,1:g,1:l,1:n")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("g")
		avl.insert("n")
		avl.insert("o")
		expect(avl, infix: "d,f,g,h,l,n,o", breadth: "3:h,2:f,2:n,1:d,1:g,1:l,1:o")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("g")
		avl.insert("j")
		avl.insert("i")
		expect(avl, infix: "d,f,g,h,i,j,l", breadth: "3:h,2:f,2:j,1:d,1:g,1:i,1:l")
	
		avl = AVL<Character, Void>()
	
		avl.insert("h")
		avl.insert("d")
		avl.insert("l")
		avl.insert("f")
		avl.insert("g")
		avl.insert("j")
		avl.insert("k")
		expect(avl, infix: "d,f,g,h,j,k,l", breadth: "3:h,2:f,2:k,1:d,1:g,1:j,1:l")
	
		avl = AVL<Character, Void>()
	
		// sequential
		avl.insert("a")
		avl.insert("b")
		avl.insert("c")
		avl.insert("d")
		avl.insert("e")
		avl.insert("f")
		expect(avl, infix: "a,b,c,d,e,f", breadth: "3:d,2:b,2:e,1:a,1:c,1:f")
	
		avl = AVL<Character, Void>()
	
		// reverse sequential
		avl.insert("f")
		avl.insert("e")
		avl.insert("d")
		avl.insert("c")
		avl.insert("b")
		avl.insert("a")
		expect(avl, infix: "a,b,c,d,e,f", breadth: "3:c,2:b,2:e,1:a,1:d,1:f")
	}

	func testInsert3() {
		let avl = AVL<Character, Void>()
	
		avl.insert("d")
		avl.insert("b")
		avl.insert("n")
		avl.insert("c")
		avl.insert("j")
		avl.insert("q")
		avl.insert("i")
		avl.insert("k")
		expect(avl, infix: "b,c,d,i,j,k,n,q", breadth: "4:d,2:b,3:n,1:c,2:j,1:q,1:i,1:k")
	
		avl.insert("h")
		expect(avl, infix: "b,c,d,h,i,j,k,n,q", breadth: "4:d,2:b,3:j,1:c,2:i,2:n,1:h,1:k,1:q")
	}

	func testRemove0() {
		let avl = AVL<Character, Void>()
	
		avl.remove("a");
		expect(avl, infix: "", breadth: "");
	
		avl.insert("a");
		expect(avl, infix: "a", breadth: "1:a");
	
		avl.remove("b");
		expect(avl, infix: "a", breadth: "1:a");
	
		avl.remove("a");
		expect(avl, infix: "", breadth: "");
	}

	func testRemove1() {
		//    2:b
		//    / \
		//  1:a 1:c
	
		let avl = AVL<Character, Void>()
	
		avl.insert("a");
		avl.insert("b");
		avl.insert("c");
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c");
	
		avl.remove("a");
		expect(avl, infix: "b,c", breadth: "2:b,1:c");
	
		avl.insert("a");
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c");
	
		avl.remove("c");
		expect(avl, infix: "a,b", breadth: "2:b,1:a");
	
		avl.insert("c");
		expect(avl, infix: "a,b,c", breadth: "2:b,1:a,1:c");
	
		avl.remove("b");
		expect(avl, infix: "a,c", breadth: "2:c,1:a");
	}

	func testRemove2() {
		//      4:c                     4:d                    4:d
		//     /   \                   /   \                  /   \
		//   2:b   3:f               2:b   3:f              2:b   3:h
		//   /     /  \              /       \              /     /  \
		// 1:a   1:d  2:h     =>   1:a       2:h    =>    1:a   2:f  1:i
		//            / \                    / \                  \
		//          1:g 1:i                1:g 1:i                1:g
		//
		// delete c
		let avl = AVL<Character, Void>()
	
		avl.insert("c");
		avl.insert("b");
		avl.insert("f");
		avl.insert("a");
		avl.insert("d");
		avl.insert("h");
		avl.insert("g");
		avl.insert("i");
		expect(avl, infix: "a,b,c,d,f,g,h,i", breadth: "4:c,2:b,3:f,1:a,1:d,2:h,1:g,1:i");
	
		avl.remove("c");
		expect(avl, infix: "a,b,d,f,g,h,i", breadth: "4:d,2:b,3:h,1:a,2:f,1:i,1:g");
	}

	func testRemove3() {
		//         4:j                    4:m                   4:m                   3:c             3:m          2:e
		//       /     \                 /   \                 /   \                 /  \             / \          / \
		//     3:e     2:n             3:e    2:n            3:c    2:n            2:b  2:m         2:c 1:n      1:c 1:m
		//     / \     /  \            /  \     \            /  \     \            /    / \           \
		//   2:b 1:f 1:m  1:o   =>   2:b  1:f   1:o   =>   2:b  1:e   1:o   =>   1:a  1:e 1:n  =>     1:e    =>
		//   /  \                    /  \                  /
		// 1:a  1:c                1:a  1:c              1:a
		//
		// delete c
		let avl = AVL<Character, Void>()
	
		avl.insert("j");
		avl.insert("e");
		avl.insert("n");
		avl.insert("b");
		avl.insert("f");
		avl.insert("m");
		avl.insert("o");
		avl.insert("a");
		avl.insert("c");
		expect(avl, infix: "a,b,c,e,f,j,m,n,o", breadth: "4:j,3:e,2:n,2:b,1:f,1:m,1:o,1:a,1:c");
	
		avl.remove("j");
		expect(avl, infix: "a,b,c,e,f,m,n,o", breadth: "4:m,3:e,2:n,2:b,1:f,1:o,1:a,1:c");
	
		avl.remove("f");
		expect(avl, infix: "a,b,c,e,m,n,o", breadth: "4:m,3:c,2:n,2:b,1:e,1:o,1:a");
	
		avl.remove("o");
		expect(avl, infix: "a,b,c,e,m,n", breadth: "3:c,2:b,2:m,1:a,1:e,1:n");
	
		avl.remove("b");
		expect(avl, infix: "a,c,e,m,n", breadth: "3:c,1:a,2:m,1:e,1:n");
	
		avl.remove("a");
		expect(avl, infix: "c,e,m,n", breadth: "3:m,2:c,1:n,1:e");
	
		avl.remove("n");
		expect(avl, infix: "c,e,m", breadth: "2:e,1:c,1:m");
	}

	func testRemove4() {
		//         4:j                    4:m                   4:m                   3:c             3:m          2:e
		//       /     \                 /   \                 /   \                 /  \             / \          / \
		//     3:e     2:n             3:e    2:n            3:c    2:n            2:b  2:m         2:c 1:n      1:c 1:m
		//     / \     /  \            /  \     \            /  \     \            /    / \           \
		//   2:b 1:f 1:m  1:o   =>   2:b  1:f   1:o   =>   2:b  1:e   1:o   =>   1:a  1:e 1:n  =>     1:e    =>
		//   /  \                    /  \                  /
		// 1:a  1:c                1:a  1:c              1:a
		//
		// delete c
		let avl = AVL<Character, Void>()
	
		avl.insert("j");
		avl.insert("e");
		avl.insert("n");
		avl.insert("b");
		avl.insert("f");
		avl.insert("m");
		avl.insert("o");
		avl.insert("a");
		avl.insert("c");
		expect(avl, infix: "a,b,c,e,f,j,m,n,o", breadth: "4:j,3:e,2:n,2:b,1:f,1:m,1:o,1:a,1:c");
	
		avl.remove("j");
		expect(avl, infix: "a,b,c,e,f,m,n,o", breadth: "4:m,3:e,2:n,2:b,1:f,1:o,1:a,1:c");
	
		avl.remove("f");
		expect(avl, infix: "a,b,c,e,m,n,o", breadth: "4:m,3:c,2:n,2:b,1:e,1:o,1:a");
	
		avl.remove("o");
		expect(avl, infix: "a,b,c,e,m,n", breadth: "3:c,2:b,2:m,1:a,1:e,1:n");
	
		avl.remove("b");
		expect(avl, infix: "a,c,e,m,n", breadth: "3:c,1:a,2:m,1:e,1:n");
	
		avl.remove("a");
		expect(avl, infix: "c,e,m,n", breadth: "3:m,2:c,1:n,1:e");
	
		avl.remove("n");
		expect(avl, infix: "c,e,m", breadth: "2:e,1:c,1:m");
	}

	func testRemove5() {
		// test of single node remove that results in two node imbalances
		//
		//        5:m                     5:m                      4:p
		//     /      \                /      \                 /      \
		//   3:h      4:t            2:i      4:t             3:m      3:t
		//   / \      / \            / \      / \             / \      / \
		// 1:b 2:k  3:p 2:v    =>  1:h 1:k  3:p 2:v    =>   2:i 1:n  2:r 2:v
		//     /    / \   \                 / \   \         / \      /     \
		//   1:i  1:n 2:r 1:z             1:n 2:r 1:z     1:h 1:k  1:q     1:z
		//            /                       /
		//          1:q                     1:q
		//
		// delete b
	
		let avl = AVL<Character, Void>()
	
		avl.insert("m");
		avl.insert("h");
		avl.insert("t");
		avl.insert("b");
		avl.insert("k");
		avl.insert("p");
		avl.insert("v");
		avl.insert("i");
		avl.insert("n");
		avl.insert("r");
		avl.insert("z");
		avl.insert("q");
		expect(avl, infix: "b,h,i,k,m,n,p,q,r,t,v,z", breadth: "5:m,3:h,4:t,1:b,2:k,3:p,2:v,1:i,1:n,2:r,1:z,1:q");
	
		avl.remove("b");
		expect(avl, infix: "h,i,k,m,n,p,q,r,t,v,z", breadth: "4:p,3:m,3:t,2:i,1:n,2:r,2:v,1:h,1:k,1:q,1:z");
	}

	// MARK: - Helper methods -

	func insert<K: Comparable, V>(tree:AVL<K, V>, key: K, value: V? = nil) {
		tree.insert(key, value: value)

		XCTAssertTrue(verify(tree.root), "tree failed insert verification")
	}
	
	func expect<K: Comparable, V>(tree: AVL<K, V>, infix: String, breadth: String) {
		var actual = ""
		
		tree.traverse(tree.root, method: .Infix) {
			(key: K, value: V?, height: Int) in
			
			if !actual.isEmpty {
				actual += ","
			}
			actual += "\(key)"
			
			return false
		}
		
		XCTAssertEqual(actual, infix, "infix string doesn't match expected value")
		
		actual = ""
		
		tree.traverse(tree.root, method: .BreadthFirst) {
			(key: K, value: V?, height: Int) in
			
			if !actual.isEmpty {
				actual += ","
			}
			actual += "\(height):\(key)"
			
			return false
		}
		
		XCTAssertEqual(actual, breadth, "breadth string doesn't match expected value")
	}
	
	func remove<K: Comparable, V>(tree:AVL<K, V>, key: K) {
		tree.remove(key)

		XCTAssertTrue(verify(tree.root), "tree failed remove verification")
	}

	func verify<K: Comparable, V>(root: AVLNode<K, V>!) -> Bool {
		guard root != nil else {
			return true
		}
	
		if root.left == nil && root.right == nil {
			return root.height == 1
		} else if root.left == nil {
			return root.right.height == root.height - 1 && verify(root.right)
		} else if root.right == nil {
			return root.left.height == root.height - 1 && verify(root.left)
		} else {
			return
				verify(root.left) &&
				verify(root.right) &&
				abs(root.left.height - root.right.height) <= 1 &&
				(root.left.height == root.height - 1 || root.right.height == root.height - 1)
		}
	}
}

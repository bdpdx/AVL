//
//  AVL.swift
//  AVL
//
//  Created by Brian Doyle on 7/29/15.
//  Copyright © 2015 Balance Software. All rights reserved.
//
//  Useful info at:
//
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-impl.html
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-insert.html
//  http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Trees/AVL-delete.html

import Foundation

public enum AVLTraverseMethod {
	case BreadthFirst
	case Infix
	case InfixReverse
	case Postfix
	case Prefix
}

// Ideally this is a nested type inside AVL<K, V>, but as of Xcode 7b4 with Swift 2.0
// the compiler cannot handle it.  See http://stackoverflow.com/a/29011521/312594
class AVLNode<K: Comparable, V> {
	var key: K!
	var value: V?

	var height = 1					// leaves have a height of 1 and height increases as we move towards the root
	var left: AVLNode<K, V>!
	var right: AVLNode<K, V>!

	init(key: K? = nil, value: V? = nil) {
		self.key = key
		self.value = value
	}
	
	init(node: AVLNode<K, V>) {
		self.key = node.key
		self.value = node.value
		self.left = node.left
		self.right = node.right
	}
}

extension Comparable {
	// override compare() when a one-shot comparison is possible
	func compare(rhs: Self) -> Int {
		if self < rhs { return -1 }
		if self > rhs { return 1 }

		return 0
	}
}

public class AVL<K: Comparable, V> {
	typealias Node = AVLNode<K, V>

	public var capacity: Int		// capacity allows faster insertion for the first 2^capacity elements
	var root = Node()

	private var elements = 0
	
	// MARK: - Public methods -
	
	public init(capacity: Int = 16) {
		self.capacity = capacity
	}
	
	public func clear() {
		elements = 0

		root.key = nil
		root.value = nil
		root.left = nil
		root.right = nil
	}

	public var count: Int { return elements }
	
	public func find(key: K, inout value:V?) -> Bool {
		guard elements > 0 else {
			return false
		}
	
		var root: Node! = self.root
	
		while root != nil {
			let c = key.compare(root.key)
			
			if c < 0 {
				root = root.left
			} else if c > 0 {
				root = root.right
			} else {
				value = root.value
				return true
			}
		}
		
		return false
	}

	func height(node:Node) -> Int {
		let left = node.left
		let leftHeight = left?.height ?? 0
		let right = node.right
		let rightHeight = right?.height ?? 0
	
		return 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
	}

	public func insert(key: K, value: V? = nil) {
		guard elements > 0 else {
			++elements

			self.root.key = key
			self.root.value = value

			return
		}

		var c: Int
		let node = Node(key: key, value: value)
		var path = [Node]()
		var root = self.root
		
		path.reserveCapacity(capacity)
		
		for ;; {
			path.append(root)
		
			c = key.compare(root.key)
		
			if c < 0 {
				if root.left == nil {
					root.left = node
					break
				}
				root = root.left!
			} else if c > 0 {
				if root.right == nil {
					root.right = node
					break
				}
				root = root.right!
			} else {
				// key exists, so just replace its value
				root.value = value
				return
			}
		}

		++elements

		path.append(node)
		
		var k: K
		var v: V?
		var x, y, z: Node
		
		for var height = 2, index = path.count - 1; index > 0; ++height {
			x = path[--index]

			x.height = height
			
			// if the height of x is <= 2 the tree at x is balanced
			if height <= 2 { continue }

			// since the height is >= 3, if either child is nil or the absolute
			// difference in height between the children is > 1 the tree at x is unbalanced

			if let
				left = x.left,
				right = x.right
			{
				c = left.height - right.height
				
				if c >= -1 && c <= 1 { continue }
			}

			y = path[++index]
			z = path[++index]
			
			--x.height
			--y.height
			
			k = x.key
			v = x.value

			if y === x.left {
				if z === y.left {
					x.key = y.key                   /*         x                y        */
					x.value = y.value               /*        / \             /   \      */
					y.key = k                       /* 	     y   3           z     x     */
					y.value = v                     /*      / \      =>           / \    */
					x.left = z                      /*     z   2                 2   3   */
					y.left = y.right
					y.right = x.right
					x.right = y
				} else {
					x.key = z.key                   /*       x                  z        */
					x.value = z.value               /*      / \               /   \      */
					z.key = k                       /* 	   y   3             y     x     */
					z.value = v                     /*    / \        =>     /       \    */
					y.right = nil					/*   0   z             0         3   */     // y.right = z.left
//					z.left = z.right                                                            // z.left and z.right are nil
					z.right = x.right
					x.right = z
				}
			} else {
				if z === y.left {
					x.key = z.key                   /*     x                    z        */
					x.value = z.value               /*    / \                 /   \      */
					z.key = k                       /* 	 0   y               x     y     */
					z.value = v                     /*      / \      =>     /       \    */
					y.left = nil                    /*     z   3           0         3   */     // y.left = z.right
//					z.right = z.left                                                            // z.left and z.right are nil
					z.left = x.left
					x.left = z
				} else {
					x.key = y.key                   /*     x                    y        */
					x.value = y.value               /*    / \                 /   \      */
					y.key = k                       /* 	 0   y               x     z     */
					y.value = v                     /*      / \      =>     / \          */
					x.right = z                     /*     1   z           0   1         */
					y.right = y.left
					y.left = x.left
					x.left = y
				}
			}
			
			break
		}
	}

	public func remove(key: K) -> V? {
		guard elements > 0 else {
			return nil
		}

		var found = false
		var parent: (node: Node!, childIsLeft: Bool)!
		var path = [Node]()
		var root: Node! = self.root
		var value: V?

		path.reserveCapacity(capacity)

		while root != nil {
			path.append(root)
			
			let c = key.compare(root.key)
			
			if c < 0 {
				parent = (root, true)
				root = root.left
			} else if c > 0 {
				parent = (root, false)
				root = root.right
			} else {
				found = true
				break
			}
		}
		
		if !found { return nil }
		
		--elements
		
		value = root.value
		
		var left = root.left
		var right = root.right

		if parent == nil {
			root.key = nil
			root.value = nil
		}
		
		if left == nil && right == nil {
			if parent != nil {
				if parent.childIsLeft {
					parent.node.left = nil
				} else {
					parent.node.right = nil
				}
			}
		} else if left == nil {
			if parent != nil {
				if parent.childIsLeft {
					parent.node.left = right
				} else {
					parent.node.right = right
				}
			}
		} else if right == nil {
			if parent != nil {
				if parent.childIsLeft {
					parent.node.left = left
				} else {
					parent.node.right = left
				}
			}
		} else {
			// find node's successor: go right then all the way left
			parent = (root, false)
			path.append(right)
			
			var successor: Node = right
			
			for ; successor.left != nil; successor = successor.left {
				parent = (successor, true)
				path.append(successor.left)
			}

			root.key = successor.key
			root.value = successor.value

			if parent.childIsLeft {
				parent.node.left = successor.right
			} else {
				parent.node.right = successor.right
			}
		}
		
		// recompute height and rebalance if necessary

		var leftHeight, rightHeight: Int
		var k: K
		var v: V?
		var x, y, z: Node

		for var index = path.count - 1; index > 0; {
			x = path[--index]
			
			left = x.left
			right = x.right
			
			if left == nil && right == nil {
				x.height = 1
				continue
			} else if left == nil {
				x.height = right.height + 1
				
				if x.height <= 2 { continue }
				
				y = right
			} else if right == nil {
				x.height = left.height + 1
				
				if x.height <= 2 { continue }
				
				y = left
			} else {
				leftHeight = left.height
				rightHeight = right.height
				
				x.height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight )
				
				if leftHeight > rightHeight + 1 {
					y = left
				} else if rightHeight > leftHeight + 1 {
					y = right
				} else {
					continue
				}
			}
			
			left = y.left
			right = y.right
			
			z = left != nil && (right == nil || left.height > right.height) ? left : right

			k = x.key
			v = x.value

			if y === x.left {
				if z === y.left {
					x.key = y.key                   /*         x                y        */
					x.value = y.value               /*        / \             /   \      */
					y.key = k                       /* 	     y   3           z     x     */
					y.value = v                     /*      / \      =>     / \   / \    */
					x.left = z                      /*     z   2           0   1 2   3   */
					y.left = y.right                /*    / \                            */
					y.right = x.right               /*   0   1                           */
					x.right = y
					
					left = z
					right = y
				} else {
					x.key = z.key                   /*       x                  z        */
					x.value = z.value               /*      / \               /   \      */
					z.key = k                       /* 	   y   3             y     x     */
					z.value = v                     /*    / \        =>     / \   / \    */
					y.right = z.left                /*   0   z             0   1 2   3   */
					z.left = z.right                /*      / \                          */
					z.right = x.right               /*     1   2                         */
					x.right = z
					
					left = y
					right = z
				}
			} else {
				if z === y.left {
					x.key = z.key                   /*     x                    z        */
					x.value = z.value               /*    / \                 /   \      */
					z.key = k                       /* 	 0   y               x     y     */
					z.value = v                     /*      / \      =>     / \   / \    */
					y.left = z.right                /*     z   3           0   1 2   3   */
					z.right = z.left                /*    / \                            */
					z.left = x.left                 /*   1   2                           */
					x.left = z
					
					left = z
					right = y
				} else {
					x.key = y.key                   /*     x                    y        */
					x.value = y.value               /*    / \                 /   \      */
					y.key = k                       /* 	 0   y               x     z     */
					y.value = v                     /*      / \      =>     / \   / \    */
					x.right = z                     /*     1   z           0   1 2   3   */
					y.right = y.left                /*        / \                        */
					y.left = x.left                 /*       2   3                       */
					x.left = y
					
					left = y
					right = z
				}
			}
			
			leftHeight = height(left)
			rightHeight = height(right)
			
			left.height = leftHeight
			right.height = rightHeight
			
			x.height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight )
		}

		return value
	}

	// return true to from AVLTraverseCallback to abort traversal
#if TEST
	typealias AVLTraverseCallback = (key:K, value:V?, height: Int) -> Bool
#else
	typealias AVLTraverseCallback = (key:K, value:V?) -> Bool
#endif

	// if traverse() returns false each node was visited, otherwise the traversal was aborted
	public func traverse(method: AVLTraverseMethod = .Infix, callback: AVLTraverseCallback) -> Bool {
		return traverse(root, method: method, callback: callback)
	}

	func traverse(var root: Node!, method: AVLTraverseMethod, callback: AVLTraverseCallback) -> Bool {
		guard root != nil && root.key != nil else {
			return false
		}

		var stop: Bool
	
		switch method {
		case .BreadthFirst:
			var queue = [Node]()
			
			for ;; {
#if TEST
				stop = callback(key: root.key, value: root.value, height: root.height)
#else
				stop = callback(key: root.key, value: root.value)
#endif
				if stop { break }
				
				if root.left != nil {
					queue.append(root.left)
				}
				if root.right != nil {
					queue.append(root.right)
				}
				if queue.count == 0 {
					break
				}
				root = queue.removeAtIndex(0)
			}
		case .Infix:
			stop = traverse(root.left, method: method, callback: callback)
			if !stop {
#if TEST
				stop = callback(key: root.key, value: root.value, height: root.height)
#else
				stop = callback(key: root.key, value: root.value)
#endif
			}
			if !stop { stop = traverse(root.right, method: method, callback: callback) }
		case .InfixReverse:
			stop = traverse(root.right, method: method, callback: callback)
			if !stop {
#if TEST
				stop = callback(key: root.key, value: root.value, height: root.height)
#else
				stop = callback(key: root.key, value: root.value)
#endif
			}
			if !stop { stop = traverse(root.left, method: method, callback: callback) }
		case .Postfix:
			stop = traverse(root.left, method: method, callback: callback)
			if !stop { stop = traverse(root.right, method: method, callback: callback) }
			if !stop {
#if TEST
				stop = callback(key: root.key, value: root.value, height: root.height)
#else
				stop = callback(key: root.key, value: root.value)
#endif
			}
		case .Prefix:
#if TEST
			stop = callback(key: root.key, value: root.value, height: root.height)
#else
			stop = callback(key: root.key, value: root.value)
#endif
			if !stop { stop = traverse(root.left, method: method, callback: callback) }
			if !stop { stop = traverse(root.right, method: method, callback: callback) }
		}
		
		return stop
	}
}

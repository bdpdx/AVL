//
//  main.cpp
//  AVL
//
//  Created by Brian Doyle on 4/23/14.
//  Copyright (c) 2014 Balance Software. All rights reserved.
//

#include <assert.h>
#include <iostream>

using namespace std;

#define ENABLE_AVL_UNIT_TESTS       1

#import "AVL.h"

bool                                gError;

long compareChars( const char &lhs, const char &rhs ) {
    return lhs < rhs ? -1 : lhs > rhs ? 1 : 0;
}

bool traverseBreadth( const char &key, void *value, void *context ) {
    long                            height = *(long *) value;
    char **                         result = (char **) context;
    char *                          string;
    
    if ( **result ) {
        asprintf( &string, "%s,%ld:%c", *result, height, key );
        free( *result );
    } else {
        asprintf( &string, "%ld:%c", height, key );
    }
    
    *result = string;
    
    return false;
}
bool traverseInfix( const char &key, void *value, void *context ) {
    char **                         result = (char **) context;
    char *                          string;
    
    if ( **result ) {
        asprintf( &string, "%s,%c", *result, key );
        free( *result );
    } else {
        asprintf( &string, "%c", key );
    }
    
    *result = string;
    
    return false;
}

#pragma mark -

void expect( AVL<char> & avl, const char * infix, const char *breadth ) {
    char *                          s;
    
    asprintf( &s, "" );
    avl.traverse( traverseInfix, &s, kAVLTraverseInfix );
    if ( strcmp( s, infix ) ) {
        cerr << "infix result " << s << " does not match expected " << infix << '\n';
        gError = 1;
    }
    free( s );
    
    asprintf( &s, "" );
    avl.traverse( traverseBreadth, &s, kAVLTraverseBreadthFirst );
    if ( strcmp( s, breadth ) ) {
        cerr << "breadth first result " << s << " does not match expected " << breadth << '\n';
        gError = 1;
    }
    free( s );
}

void testInsert0() {
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'a' );
    expect( avl, "a", "1:a" );
}

void testInsert1() {
    AVL<char> *                     avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'c' );
    avl->insert( 'b' );
    avl->insert( 'a' );
    expect( *avl, "a,b,c", "2:b,1:a,1:c" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'c' );
    avl->insert( 'a' );
    avl->insert( 'b' );
    expect( *avl, "a,b,c", "2:b,1:a,1:c" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'a' );
    avl->insert( 'c' );
    avl->insert( 'b' );
    expect( *avl, "a,b,c", "2:b,1:a,1:c" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'a' );
    avl->insert( 'b' );
    avl->insert( 'c' );
    expect( *avl, "a,b,c", "2:b,1:a,1:c" );
    
    delete avl;
}

void testInsert2() {
    AVL<char> *                     avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'b' );
    avl->insert( 'a' );
    expect( *avl, "a,b,d,h,l", "3:h,2:b,1:l,1:a,1:d" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'b' );
    avl->insert( 'c' );
    expect( *avl, "b,c,d,h,l", "3:h,2:c,1:l,1:b,1:d" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'e' );
    expect( *avl, "d,e,f,h,l", "3:h,2:e,1:l,1:d,1:f" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'g' );
    expect( *avl, "d,f,g,h,l", "3:h,2:f,1:l,1:d,1:g" );
    
    delete avl;
    
    // test right-side
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'g' );
    avl->insert( 'n' );
    avl->insert( 'm' );
    expect( *avl, "d,f,g,h,l,m,n", "3:h,2:f,2:m,1:d,1:g,1:l,1:n" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'g' );
    avl->insert( 'n' );
    avl->insert( 'o' );
    expect( *avl, "d,f,g,h,l,n,o", "3:h,2:f,2:n,1:d,1:g,1:l,1:o" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'g' );
    avl->insert( 'j' );
    avl->insert( 'i' );
    expect( *avl, "d,f,g,h,i,j,l", "3:h,2:f,2:j,1:d,1:g,1:i,1:l" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    avl->insert( 'h' );
    avl->insert( 'd' );
    avl->insert( 'l' );
    avl->insert( 'f' );
    avl->insert( 'g' );
    avl->insert( 'j' );
    avl->insert( 'k' );
    expect( *avl, "d,f,g,h,j,k,l", "3:h,2:f,2:k,1:d,1:g,1:j,1:l" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    // sequential
    avl->insert( 'a' );
    avl->insert( 'b' );
    avl->insert( 'c' );
    avl->insert( 'd' );
    avl->insert( 'e' );
    avl->insert( 'f' );
    expect( *avl, "a,b,c,d,e,f", "3:d,2:b,2:e,1:a,1:c,1:f" );
    
    delete avl;
    
    avl = new AVL<char>( compareChars );
    
    // reverse sequential
    avl->insert( 'f' );
    avl->insert( 'e' );
    avl->insert( 'd' );
    avl->insert( 'c' );
    avl->insert( 'b' );
    avl->insert( 'a' );
    expect( *avl, "a,b,c,d,e,f", "3:c,2:b,2:e,1:a,1:d,1:f" );
    
    delete avl;
}

void testInsert3() {
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'd' );
    avl.insert( 'b' );
    avl.insert( 'n' );
    avl.insert( 'c' );
    avl.insert( 'j' );
    avl.insert( 'q' );
    avl.insert( 'i' );
    avl.insert( 'k' );
    expect( avl, "b,c,d,i,j,k,n,q", "4:d,2:b,3:n,1:c,2:j,1:q,1:i,1:k" );
    
    avl.insert( 'h' );
    expect( avl, "b,c,d,h,i,j,k,n,q", "4:d,2:b,3:j,1:c,2:i,2:n,1:h,1:k,1:q" );
}

void testRemove0() {
    AVL<char>                       avl( compareChars );
    
    avl.remove( 'a' );
    expect( avl, "", "" );
    
    avl.insert( 'a' );
    expect( avl, "a", "1:a" );
    
    avl.remove( 'b' );
    expect( avl, "a", "1:a" );
    
    avl.remove( 'a' );
    expect( avl, "", "" );
}

void testRemove1() {
    //    2:b
    //    / \
    //  1:a 1:c
    
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'a' );
    avl.insert( 'b' );
    avl.insert( 'c' );
    expect( avl, "a,b,c", "2:b,1:a,1:c" );
    
    avl.remove( 'a' );
    expect( avl, "b,c", "2:b,1:c" );
    
    avl.insert( 'a' );
    expect( avl, "a,b,c", "2:b,1:a,1:c" );
    
    avl.remove( 'c' );
    expect( avl, "a,b", "2:b,1:a" );
    
    avl.insert( 'c' );
    expect( avl, "a,b,c", "2:b,1:a,1:c" );
    
    avl.remove( 'b' );
    expect( avl, "a,c", "2:c,1:a" );
}

void testRemove2() {
    //      4:c                     4:d                    4:d
    //     /   \                   /   \                  /   \
    //   2:b   3:f               2:b   3:f              2:b   3:h
    //   /     /  \              /       \              /     /  \
    // 1:a   1:d  2:h     =>   1:a       2:h    =>    1:a   2:f  1:i
    //            / \                    / \                  \
    //          1:g 1:i                1:g 1:i                1:g
    //
    // delete c
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'c' );
    avl.insert( 'b' );
    avl.insert( 'f' );
    avl.insert( 'a' );
    avl.insert( 'd' );
    avl.insert( 'h' );
    avl.insert( 'g' );
    avl.insert( 'i' );
    expect( avl, "a,b,c,d,f,g,h,i", "4:c,2:b,3:f,1:a,1:d,2:h,1:g,1:i" );
    
    avl.remove( 'c' );
    expect( avl, "a,b,d,f,g,h,i", "4:d,2:b,3:h,1:a,2:f,1:i,1:g" );
}

void testRemove3() {
    //         4:j                    4:m                   4:m                   3:c             3:m          2:e
    //       /     \                 /   \                 /   \                 /  \             / \          / \
    //     3:e     2:n             3:e    2:n            3:c    2:n            2:b  2:m         2:c 1:n      1:c 1:m
    //     / \     /  \            /  \     \            /  \     \            /    / \           \
    //   2:b 1:f 1:m  1:o   =>   2:b  1:f   1:o   =>   2:b  1:e   1:o   =>   1:a  1:e 1:n  =>     1:e    =>
    //   /  \                    /  \                  /
    // 1:a  1:c                1:a  1:c              1:a
    //
    // delete c
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'j' );
    avl.insert( 'e' );
    avl.insert( 'n' );
    avl.insert( 'b' );
    avl.insert( 'f' );
    avl.insert( 'm' );
    avl.insert( 'o' );
    avl.insert( 'a' );
    avl.insert( 'c' );
    expect( avl, "a,b,c,e,f,j,m,n,o", "4:j,3:e,2:n,2:b,1:f,1:m,1:o,1:a,1:c" );
    
    avl.remove( 'j' );
    expect( avl, "a,b,c,e,f,m,n,o", "4:m,3:e,2:n,2:b,1:f,1:o,1:a,1:c" );
    
    avl.remove( 'f' );
    expect( avl, "a,b,c,e,m,n,o", "4:m,3:c,2:n,2:b,1:e,1:o,1:a" );
    
    avl.remove( 'o' );
    expect( avl, "a,b,c,e,m,n", "3:c,2:b,2:m,1:a,1:e,1:n" );
    
    avl.remove( 'b' );
    expect( avl, "a,c,e,m,n", "3:c,1:a,2:m,1:e,1:n" );
    
    avl.remove( 'a' );
    expect( avl, "c,e,m,n", "3:m,2:c,1:n,1:e" );
    
    avl.remove( 'n' );
    expect( avl, "c,e,m", "2:e,1:c,1:m" );
}

void testRemove4() {
    //         4:j                    4:m                   4:m                   3:c             3:m          2:e
    //       /     \                 /   \                 /   \                 /  \             / \          / \
    //     3:e     2:n             3:e    2:n            3:c    2:n            2:b  2:m         2:c 1:n      1:c 1:m
    //     / \     /  \            /  \     \            /  \     \            /    / \           \
    //   2:b 1:f 1:m  1:o   =>   2:b  1:f   1:o   =>   2:b  1:e   1:o   =>   1:a  1:e 1:n  =>     1:e    =>
    //   /  \                    /  \                  /
    // 1:a  1:c                1:a  1:c              1:a
    //
    // delete c
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'j' );
    avl.insert( 'e' );
    avl.insert( 'n' );
    avl.insert( 'b' );
    avl.insert( 'f' );
    avl.insert( 'm' );
    avl.insert( 'o' );
    avl.insert( 'a' );
    avl.insert( 'c' );
    expect( avl, "a,b,c,e,f,j,m,n,o", "4:j,3:e,2:n,2:b,1:f,1:m,1:o,1:a,1:c" );
    
    avl.remove( 'j' );
    expect( avl, "a,b,c,e,f,m,n,o", "4:m,3:e,2:n,2:b,1:f,1:o,1:a,1:c" );
    
    avl.remove( 'f' );
    expect( avl, "a,b,c,e,m,n,o", "4:m,3:c,2:n,2:b,1:e,1:o,1:a" );
    
    avl.remove( 'o' );
    expect( avl, "a,b,c,e,m,n", "3:c,2:b,2:m,1:a,1:e,1:n" );
    
    avl.remove( 'b' );
    expect( avl, "a,c,e,m,n", "3:c,1:a,2:m,1:e,1:n" );
    
    avl.remove( 'a' );
    expect( avl, "c,e,m,n", "3:m,2:c,1:n,1:e" );
    
    avl.remove( 'n' );
    expect( avl, "c,e,m", "2:e,1:c,1:m" );
}

void testRemove5() {
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
    
    AVL<char>                       avl( compareChars );
    
    avl.insert( 'm' );
    avl.insert( 'h' );
    avl.insert( 't' );
    avl.insert( 'b' );
    avl.insert( 'k' );
    avl.insert( 'p' );
    avl.insert( 'v' );
    avl.insert( 'i' );
    avl.insert( 'n' );
    avl.insert( 'r' );
    avl.insert( 'z' );
    avl.insert( 'q' );
    expect( avl, "b,h,i,k,m,n,p,q,r,t,v,z", "5:m,3:h,4:t,1:b,2:k,3:p,2:v,1:i,1:n,2:r,1:z,1:q" );
    
    avl.remove( 'b' );
    expect( avl, "h,i,k,m,n,p,q,r,t,v,z", "4:p,3:m,3:t,2:i,1:n,2:r,2:v,1:h,1:k,1:q,1:z" );
}

int main( int argc, const char *argv[] ) {
    cout << "starting AVL tests, errors will be reported on stderr\n";
    
    testInsert0();
    testInsert1();
    testInsert2();
    testInsert3();
    
    testRemove0();
    testRemove1();
    testRemove2();
    testRemove3();
    testRemove4();
    testRemove5();

    cout << "AVL tests completed\n";
    
    return gError;
}

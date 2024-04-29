#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cassert>
#include "rope.hpp"

using namespace std;

struct DiffChunk {
    string text;
    uint32_t pos;
    bool isInsertion;
};

vector<vector<uint32_t>> LevistanDistance (const string& str1, const string& str2) {
    uint32_t m = str1.size();
    uint32_t n = str2.size();

    vector<vector<uint32_t>> dp(m + 1, vector<uint32_t>(n + 1, 0));

    for (uint32_t i = 0; i <= m; i++)
        dp[i][0] = i;

    for (uint32_t j = 0; j <= n; j++)
        dp[0][j] = j;

    for (uint32_t i = 1; i <= m; i++) {
        for (uint32_t j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // Characters match
            } else {
                dp[i][j] = min( dp[i - 1][j]+1 ,  // Deletion of str1[i - 1]
                                dp[i][j - 1]+1 ); // Insertion of str2[j - 1]
            }
        }
    }

    return dp;
}

vector<DiffChunk> diff(string& str1, string& str2) {

    vector<DiffChunk> diffrences;
    vector<vector<uint32_t>> dp = LevistanDistance(str1, str2);

    uint32_t i = str1.size();
    uint32_t j = str2.size();

    string insertChunk = "";
    string deleteChunk = "";

    while (i > 0 || j > 0) {
        if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) { // Deletion
            if (!insertChunk.empty()) {
                // Insert
                reverse(insertChunk.begin(), insertChunk.end());
                diffrences.push_back({insertChunk, i, true});
                insertChunk = "";
            }
            deleteChunk = str1[i - 1] + deleteChunk;
            i--;
        }
        else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) { // Insertion
            if (!deleteChunk.empty()) {
                // Delete
                diffrences.push_back({deleteChunk, i, false});
                deleteChunk = "";
            }
            insertChunk += str2[j - 1];
            j--;
        }
        else {
            if (!insertChunk.empty()) {
                // Insert
                reverse(insertChunk.begin(), insertChunk.end());
                diffrences.push_back({insertChunk, i, true});
                insertChunk = "";
            }
            if (!deleteChunk.empty()) {
                // Delete
                diffrences.push_back({deleteChunk, i, false});
                deleteChunk = "";
            }
            i--;
            j--;
        }
    }

    if (!deleteChunk.empty()) {
        // Delete
        diffrences.push_back({deleteChunk, i, false});
    }

    if (!insertChunk.empty()) {
        // Insert
        reverse(insertChunk.begin(), insertChunk.end());
        diffrences.push_back({insertChunk, i, true});
    }
    

    //reverse(diffrences.begin(), diffrences.end());

    return diffrences;
}


void test_diff() {
    // Edge cases
    vector<pair<string, string>> edge_cases = {
        {"", ""},                           // Empty strings
        {"", "abc"},                        // First string is empty
        {"abc", ""},                        // Second string is empty
        {"a", "a"},                         // Same single character
        {"a", "b"},                         // Single character change
        {"abc", "def"},                     // Completely different strings
        {"abcdef", "abdef"},                // One character difference
        {"abcdef", "bcdef"},                // First character difference
        {"abcdef", "abcde"},                // Last character difference
        {"abcdef", "abcf"},                 // Last two characters difference
        {"abcdef", "bcdefg"},               // Additional character in the second string
        {"abcdefg", "bcdef"},               // Additional character in the first string
        {"abcdefg", "abdcef"},              // Multiple character swaps
        {"abcdef", "acdf"},                 // Random differences
        {"kitten", "sitting"},              // Example from the problem statement
        {"The quick brown fox jumps over the lazy dog.", "The quick black cat jumps over the lazy dog."},  // One word change
        {"The quick brown fox jumps over the lazy dog.", "The quick brown fox quickly jumps over the lazy dog."},  // Insertion in the middle
        {"The quick brown fox jumps over the lazy dog.", "The quick brown jumps over the lazy dog."},  // Deletion in the middle
        {"The quick brown fox jumps over the lazy dog.", "The quick brown jumps over the dog."},  // Deletion and insertion in the middle
        {"apple", "orange"},            // Completely different strings
        {"apple", "maple"},             // One character change
        {"apple", "appple"},            // Repeated character insertion
        {"apple", "ape"},               // Repeated character deletion
        {"apple", "ample"},             // Change in the middle
        {"apple", "app"},               // End deletion
        {"apple", "aapple"},            // Beginning insertion
    };

    cout << "Testing diff()..." << endl;
    cout << "------------------" << endl;

    for (auto& test_case : edge_cases) {
        string str1 = test_case.first;
        string str2 = test_case.second;

        cout << "Testing case: '" << str1 << "' to '" << str2 << "'" << endl;
        auto operations = diff(str1, str2);

        string result = str1;
        for (auto& op : operations) {
            if (op.isInsertion) {
                result.insert(op.pos, op.text);
            } else {
                result.erase(op.pos, op.text.size());
            }
        }

        assert(result == str2);
    }

    cout << "All tests passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void test_append() {
    Rope rope;

    cout << "Testing append()..." << endl;
    cout << "------------------" << endl;
    // Test appending to an empty rope
    rope.append("Hello", 5);
    assert(rope.toString() == "Hello");

    cout << "Rope After Appending Hello";
    rope.printTree();

    // Test appending to a non-empty rope
    rope.append(" World", 6);
    assert(rope.toString() == "Hello World");

    cout << "Rope After Appending World";
    rope.printTree();

    cout << "Test append passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void test_prepend() {
    Rope rope;

    cout << "Testing prepend()..." << endl;
    cout << "------------------" << endl;
    // Test prepending to an empty rope
    rope.prepend("World", 5);
    assert(rope.toString() == "World");

    cout << "Rope after prepending World";
    rope.printTree();

    // Test prepending to a non-empty rope
    rope.prepend("Hello ", 6);
    assert(rope.toString() == "Hello World");

    cout << "Rope after prepending Hello";
    rope.printTree();

    cout << "Test prepend passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void test_insert() {
    Rope rope("Hello", 5);

    cout << "Testing insert()..." << endl;
    // Test inserting into an empty rope
    rope.insert(0, "World", 5);
    assert(rope.toString() == "WorldHello");

    cout << "Rope after inserting World" ;
    rope.printTree();

    // Test inserting into the middle of a rope
    rope.insert(5, " ", 1);
    assert(rope.toString() == "World Hello");

    cout << "Rope after inserting space ' ' ";
    rope.printTree();

    // Test inserting at the end of a rope
    rope.insert(11, "!", 1);
    assert(rope.toString() == "World Hello!");

    cout << "Rope after inserting ! ";
    rope.printTree();

    cout << "Test insert passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void test_remove() {
    Rope rope("Hello World!", 12);

    cout << "Testing remove()..." << endl;
    cout << "------------------" << endl;
    // Test removing from the beginning of a rope
    rope.remove(0, 6);
    assert(rope.toString() == "World!");

    cout << "Rope after removing 'Hello' ";
    rope.printTree();

    // Test removing from the middle of a rope
    rope.remove(3, 1);
    assert(rope.toString() == "Word!");

    cout << "Rope after removing l ";
    rope.printTree();

    // Test removing from the end of a rope
    rope.remove(4, 2);
    assert(rope.toString() == "Word");

    cout << "Rope after removing '!' ";
    rope.printTree();

    cout << "Test remove passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void test_cut() {
    Rope rope("Hello World!", 12);

    cout << "Rope before cut: "; rope.printTree();

    cout << "Testing cut()..." << endl;
    cout << "------------------" << endl;
    // Test cutting from the beginning of a rope
    Rope cut_rope1 = rope.cut(0, 5);
    assert(cut_rope1.toString() == "Hello");

    cout << "Rope after cutting 'Hello' ";
    cut_rope1.printTree();

    // Test cutting from the middle of a rope
    Rope cut_rope2 = rope.cut(6, 5);
    assert(cut_rope2.toString() == "World");

    cout << "Rope after cutting 'World' ";
    cut_rope2.printTree();

    // Test cutting from the end of a rope
    Rope cut_rope3 = rope.cut(6, 6);
    assert(cut_rope3.toString() == "World!");

    cout << "Rope after cutting 'World!' ";
    cut_rope3.printTree();

    cout << "Test cut passed successfully!" << endl;
    cout << "------------------" << endl << endl;

    /*
    cout << "Rope after cut: "; rope.printTree();
    cout << "Deleted cut_rope1" << endl;
    delete &cut_rope1;
    */

    cout << "Delete cut_rope2" << endl;
    delete &cut_rope2;

    cout << "Delete cut_rope3" << endl;
    delete &cut_rope3;

    cout << "Delete Rope" << endl;
    delete &rope;
}

void test_paste() {
    Rope rope1("Hello", 5);
    Rope rope2(" World", 6);

    cout << "Testing paste()..." << endl;
    cout << "------------------" << endl;
    // Test pasting at the beginning of a rope
    rope1.paste(0, rope2);
    assert(rope1.toString() == " WorldHello");

    cout << "Rope after pasting ' World' ";
    rope1.printTree();

    // Test pasting in the middle of a rope
    rope1.paste(5, rope2);
    assert(rope1.toString() == " World WorldHello");

    cout << "Rope after pasting ' World' ";
    rope1.printTree();

    // Test pasting at the end of a rope
    rope1.paste(12, rope2);
    assert(rope1.toString() == " World WorldHello World");

    cout << "Rope after pasting ' World' ";
    rope1.printTree();

    cout << "Test paste passed successfully!" << endl;
    cout << "------------------" << endl << endl;
}

void run_tests() {
    cout << "Running Rope tests... " << endl;
    cout << "***************************" << endl << endl;

    test_cut();
    test_append();
    test_prepend();
    test_insert();
    test_remove();
    test_paste();

    cout << "All tests passed!" << endl;
    cout << "*************************" << endl << endl;
}

int main() {
    /*
    const char* str= "Hello, World!";

    Rope* rope = new Rope(str, strlen(str));
    cout << rope->toString() << endl;
    rope->printTree();

    str = " Trying to append some stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " more \n stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " testing is it working";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " more \nlets stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " testing is it working mess";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append [1]";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append [\n]";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = "Trying to insert some stuff ";
    //rope->prepend(str, strlen(str));
    rope->insert(0,str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " Lets try to insert. ";
    //rope->prepend(str, strlen(str));
    rope->insert(0,str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " @lets see if we can insert in the middle@ ";
    rope->insert(52, str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " &l-e-t-s s-e-e i-f w-e c-a-n i-n-s-e-r-t i-n t-h-e m-i-d-d-l-e& ";
    rope->insert(49+3+3, str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;
    
    cout << "Rope to string:- " << endl;
    cout << rope->toString() << endl;
    

    delete rope;
    */

    test_diff();
    run_tests();

    return 0;
}
#include <chrono>
#include "rope.hpp"
#include "stringBuilder.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>

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

    cout << "____________________________" << endl;
    cout << "Testing diff()..." << endl;

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
    cout << "-------------------------------" << endl << endl;
}

void test_append() {
    Rope rope;

    cout << "____________________________" << endl;
    cout << "Testing append()..." << endl;
    // Test appending to an empty rope
    rope.append("Hello", 5);
    assert(rope.toString() == "Hello");

    // Test appending to a non-empty rope
    rope.append(" World", 6);
    assert(rope.toString() == "Hello World");

    cout << "Test append passed successfully!" << endl;
    cout << "----------------------------------" << endl << endl;
}

void test_prepend() {
    Rope rope;

    cout << "________________________________" << endl;
    cout << "Testing prepend()..." << endl;
    
    // Test prepending to an empty rope
    rope.prepend("World", 5);
    assert(rope.toString() == "World");

    // Test prepending to a non-empty rope
    rope.prepend("Hello ", 6);
    assert(rope.toString() == "Hello World");

    cout << "Test prepend passed successfully!" << endl;
    cout << "----------------------------------" << endl << endl;
}

void test_insert() {
    Rope rope("Hello", 5);

    cout << "____________________________" << endl;
    cout << "Testing insert()..." << endl;
    // Test inserting into an empty rope
    rope.insert(0, "World", 5);
    assert(rope.toString() == "WorldHello");

    // Test inserting into the middle of a rope
    rope.insert(5, " ", 1);
    assert(rope.toString() == "World Hello");

    // Test inserting at the end of a rope
    rope.insert(11, "!", 1);
    assert(rope.toString() == "World Hello!");

    cout << "Test insert passed successfully!" << endl;
    cout << "--------------------------------" << endl << endl;
}

void test_remove() {
    Rope rope("Hello World!", 12);

    cout << "___________________________" << endl;
    cout << "Testing remove()..." << endl;
    
    // Test removing from the beginning of a rope
    rope.remove(0, 6);
    assert(rope.toString() == "World!");

    // Test removing from the middle of a rope
    rope.remove(3, 1);
    assert(rope.toString() == "Word!");

    // Test removing from the end of a rope
    rope.remove(4, 2);
    assert(rope.toString() == "Word");

    cout << "Test remove passed successfully!" << endl;
    cout << "--------------------------------" << endl << endl;
}

void test_cut() {
    Rope rope("Hello World!", 12);

    cout << "___________________________" << endl;
    cout << "Testing cut()..." << endl;
    // Test cutting from the beginning of a rope
    Rope cut_rope1 = *rope.cut(0, 5);
    assert(cut_rope1.toString() == "Hello");

    // Test cutting from the middle of a rope
    Rope cut_rope2 = *rope.cut(6, 5);
    assert(cut_rope2.toString() == "World");

    // Test cutting from the end of a rope
    Rope cut_rope3 = *rope.cut(6, 6);
    assert(cut_rope3.toString() == "World!");

    cout << "Test cut passed successfully!" << endl;
    cout << "------------------------------" << endl << endl;
}

void test_paste() {
    Rope* rope1 = new Rope("Hello", 5);
    Rope* rope2 = new Rope(" World", 6);

    cout << "______________________________" << endl;
    cout << "Testing paste()..." << endl;
    // Test pasting at the beginning of a rope
    rope1->paste(0, rope2);
    assert(rope1->toString() == " WorldHello");

    // Test pasting in the middle of a rope
    rope1->paste(6, rope2);
    assert(rope1->toString() == " World WorldHello");

    // Test pasting at the end of a rope
    rope1->paste(18, rope2);
    assert(rope1->toString() == " World WorldHello World");

    cout << "Test paste passed successfully!" << endl;
    cout << "--------------------------------" << endl << endl;
}

void run_tests() {
    cout << "*****************************************" << endl;
    cout << "Running Rope tests... " << endl;
    

    test_append();
    test_prepend();
    test_insert();
    test_remove();
    test_cut();
    test_paste();

    cout << "All tests passed!" << endl;
    cout << "*****************************************" << endl << endl;
}

struct Blocks {
    uint32_t pos;
    string text;

    Blocks(uint32_t pos, string text) : pos(pos), text(text){}
};

vector<Blocks> generateRandomBlocks (uint32_t totalSizeKB, uint32_t chunkSizeKB, uint32_t len)
{   
    uint32_t totalSizeBytes = totalSizeKB * 1024;
    uint32_t chunkSizeBytes = chunkSizeKB * 1024;

    vector<Blocks> blocks;
    string fullText;
    srand(time(0));
    
    string words [] = {"Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit",
             "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore", "et", "dolore",
             "magna", "aliqua", "Ut", "enim", "ad", "minim", "veniam", "quis", "nostrud",
             "exercitation", "ullamco", "laboris", "nisi", "ut", "aliquip", "ex", "ea", "commodo",
             "consequat", "Duis", "aute", "irure", "dolor", "in", "reprehenderit", "in", "voluptate",
             "velit", "esse", "cillum", "dolore", "eu", "fugiat", "nulla", "pariatur", "Excepteur",
             "sint", "occaecat", "cupidatat", "non", "proident", "sunt", "in", "culpa", "qui",
             "officia", "deserunt", "mollit", "anim", "id", "est","laborum"}; 

    while (fullText.size() < totalSizeBytes) {
        uint32_t remainingBytes = totalSizeBytes - blocks.size();
        uint32_t size = min(remainingBytes, chunkSizeBytes);

        string chunk;

        while (chunk.size() < size) {
            int length = sizeof(words) / sizeof(words[0]);
            int wordIndex = rand() % length;
            string word = words[wordIndex];
            chunk += word;
            chunk += ' ';
        }

        fullText += chunk;

        uint32_t pos = rand() % len;

        blocks.emplace_back(pos, chunk);
    }

    return blocks;
}

vector<pair<uint32_t,uint32_t>> generateRemovalPos (uint32_t totalSizeKB, uint32_t chunkSizeKB, uint32_t len)
{   
    uint32_t totalSizeBytes = totalSizeKB * 1024;
    uint32_t chunkSizeBytes = chunkSizeKB * 1024;

    srand(time(0));

    uint32_t totalChunksSize = 0;
    vector<pair<uint32_t,uint32_t>> Pos;
    
    while (totalChunksSize < totalSizeBytes && len > chunkSizeBytes) {
        uint32_t startPos = rand() % (len-chunkSizeBytes-1);
        uint32_t endPos = startPos + chunkSizeBytes;
        totalChunksSize += chunkSizeBytes;
        len -= chunkSizeBytes;
        Pos.push_back({startPos, endPos});
    }

    return Pos;
}

void benchmark(const string files[], const int num_files) {

    cout << "Starting benchmark for " << num_files << " file(s)..." << endl;

    for (int i = 0; i < num_files; i++) {
        string file = files[i];
        
        ifstream f (file);
        uint32_t totalSizeKB = f.seekg(0, ios::end).tellg() / 1024;

        cout << "*********************************************************************************************************************************************" << endl;
        cout << "File: " << file << " of size (" << totalSizeKB << "KB) "<< endl << endl;
        cout << "___________________________________________________________________________________________________________________________________________" << endl;
        cout << "Loading file:- " << file << "..." << endl;
        
        auto start = chrono::high_resolution_clock::now();
        StringBuilder sb = StringBuilder();
        sb.load(file);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Loading benchmark done for file" << file << " StringBuilder took " << duration.count() << " nanoseconds." << endl;

        start = chrono::high_resolution_clock::now();
        Rope* rope = new Rope(file.c_str());
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Loading benchmark done for file" << file << " of size (" << totalSizeKB << "KB) Rope took " << duration.count() << " nanoseconds." << endl;

        cout << "Testing equality...";
        assert(sb.toString() == rope->toString());
        cout << "Passed!" << endl;

        cout << "--------------------------------------------------------------------------------------------------------------------------------------------" << endl;

        cout << "_____________________________________________________________________________________________________________________________________________" << endl;
        cout << "Appending Benchmark:- " << file << "..." << endl;

        int num_blocks = 30;
        vector<Blocks> blocks = generateRandomBlocks(totalSizeKB*num_blocks, totalSizeKB, rope->getLength());

        start = chrono::high_resolution_clock::now();
        for (Blocks block : blocks) {
            sb.append(block.text);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Appending benchmark done for file" << file << " StringBuilder took " << duration.count() << " nanoseconds." << endl;

        start = chrono::high_resolution_clock::now();
        for (Blocks block : blocks) {
            rope->append(block.text.c_str(), block.text.length());
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Appending benchmark done for file" << file << " Rope took " << duration.count() << " nanoseconds." << endl;

        cout << "Testing equality...";
        assert(sb.toString() == rope->toString());
        cout << "Passed!" << endl;

        cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;


        cout << "_____________________________________________________________________________________________________________________________________________" << endl;
        cout << "Insertion Benchmark:- " << file << "..." << endl;

        num_blocks = 30;
        blocks = generateRandomBlocks(totalSizeKB*num_blocks, totalSizeKB, rope->getLength());

        start = chrono::high_resolution_clock::now();
        for (Blocks block : blocks) {
            sb.insert(block.pos, block.text);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Insertion benchmark done for file" << file << " StringBuilder took " << duration.count() << " nanoseconds." << endl;

        start = chrono::high_resolution_clock::now();
        for (Blocks block : blocks) {
            rope->insert(block.pos, block.text.c_str(), block.text.length());
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Insertion benchmark done for file" << file << " Rope took " << duration.count() << " nanoseconds." << endl;

        cout << "Testing equality...";
        assert(sb.toString() == rope->toString());
        cout << "Passed!" << endl;

        cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;


        cout << "_____________________________________________________________________________________________________________________________________________" << endl;
        cout << "Removal Benchmark:- " << file << "..." << endl;

        num_blocks = 30;
        vector<pair<uint32_t,uint32_t>> Pos = generateRemovalPos(totalSizeKB*num_blocks, totalSizeKB, rope->getLength());

        start = chrono::high_resolution_clock::now();
        for (auto pos : Pos) {
            sb.remove(pos.first, pos.second-pos.first);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Removal benchmark done for file" << file << " StringBuilder took " << duration.count() << " nanoseconds." << endl;

        start = chrono::high_resolution_clock::now();
        for (auto pos : Pos) {
            rope->remove(pos.first, pos.second-pos.first);
        }
        stop = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
        cout << "Removal benchmark done for file" << file << " Rope took " << duration.count() << " nanoseconds." << endl;

        cout << "Testing equality...";
        assert(sb.toString() == rope->toString());
        cout << "Passed!" << endl;

        cout << "----------------------------------------------------------------------------------------------------------------------------------------------" << endl;


        cout << "*********************************************************************************************************************************************" << endl;

        delete rope;
    };


    cout << "Benchmarking done!" << endl;

}

void demo() {
    cout << "Starting demo..." << endl<<endl;
    cout << "*****************************************************************************************************************" << endl;

    string fullText;

    string str = "Hello, World!";
    
    fullText.append(str.c_str(), str.length());

    Rope* rope = new Rope(str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl;

    str = " Trying to append some stuff";
    fullText.append(str.c_str(), str.length());
    rope->append(str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl << endl;

    str = "Lets try to prepend some stuff ";
    fullText.insert(0, str.c_str(), str.length());
    rope->prepend(str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl << endl;

    str = " Does New Line Work? \n If this is on new line it is working";
    fullText.append(str.c_str(), str.length());
    rope->append(str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl << endl;

    str = " Trying to insert some stuff ";
    fullText.insert(44, str.c_str(), str.length());
    rope->insert(44,str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl;


    str = " @lets see if we can insert in the middle@ ";
    fullText.insert(52, str.c_str(), str.length());
    rope->insert(52, str.c_str(), str.length());
    rope->printTree();
    cout << rope->toString() << endl << endl;
    
    cout << endl << "Rope to string:- " << endl;
    cout << rope->toString() << endl << endl;

    cout << "Testing equality ....  ";
    assert(rope->toString() == fullText);
    cout << "Passed!" << endl;
    cout << "*****************************************************************************************************************" << endl;
    
    delete rope;
}

int main() {

    string currentDirecotry = filesystem::current_path().string();
    string fileDirectory = currentDirecotry + "\\benchmark_files\\";

    const string filenames [] = {
        fileDirectory + "small.txt",
        fileDirectory + "medium.txt"
        //fileDirectory + "large.txt"
    };

    test_diff();
    run_tests();
    benchmark(filenames, 2);
    demo();

    return 0;
}
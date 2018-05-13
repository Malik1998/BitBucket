#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <string.h>
#include <set>
#include <algorithm>
#include <queue>

const size_t alph = 255;

/*
 * The Node of tree from AHO Karasik - alogorithm
 * Aho small crucian algorithm
 *
 */

struct Node {
    Node();
    Node(Node *parent, char Letter);
    Node* SufLink; // SuffLink to the Node
    Node* Parent; // Link to the parent of Node
    Node* Up = nullptr; // Link to upper, evristic lnk
    char Letter; // Letter that node takes
    bool IsLetter = false; // Bool find that shows if node is leaf or not
    std::vector<int> Ends; // Links to other Nodes From where we can continue our search
    Node* BackEdge[alph]; // Links to leafs from where we can continues
    std::shared_ptr<Node> DirectEdge[alph]; // Edges that goes from this node
};

Node::Node(): SufLink(this), Parent(this) {
    for (int i = 0; i < alph; i++) {
        BackEdge[i] = nullptr;
        DirectEdge[i] =  nullptr;
    }
}

Node::Node(Node *parent, char Letter):SufLink(nullptr) {
    this->Parent = parent;
    this->Letter = Letter;
    for (int i = 0; i < alph; i++) {
        BackEdge[i] = nullptr;
        DirectEdge[i] =  nullptr;
    }
}

/*
 * Template problem solver written via Aho Karasik Algorithm
 */
class TemplatesProblemSolver {
public:
    /*
     * Get the big string and makes tree from it
     */
    explicit TemplatesProblemSolver(const std::string& pattern);
    /*
     * Find indexes of text entering to pattern text
     */
    std::vector<int> FindTheText(const std::string& text);

private:
    /*
     * private function that counts the answer to FindText function
     */
    const std::vector<int> CountTextEnter(std::vector<int> &CountOfPatterns);
    /*
     * Function that helps to intilize the tree with pattern string, it adds some word to the tree
     */
    void AddWordToForest(std::shared_ptr<Node>& cur, const std::string &word, int add_index, int index, int terminate);
    /*
     * Add all sufflinks to the tree
     */
    void FindAllSufLinks(Node *CurNode);

    /*
     * Link to the root Node
     */
    std::shared_ptr<Node> root;
    /*
     * Length of the pattern length
     */
    int pattern_size;
    /*
     * Count of small strings from patten string that not contains '?' symbol
     */
    int pattern_count;
};

TemplatesProblemSolver::TemplatesProblemSolver(const std::string& pattern) {
    root = std::make_shared<Node>();
    pattern_size = pattern.length();
    pattern_count = 0;
    int cur = pattern_size + 3; // Так как никогда это невозможное значение
    for (int i = 0; i < pattern_size; i++) {
        if (cur == (pattern_size + 3) && pattern[i] != '?') {
            cur = i;
        } else if (pattern[i] == '?' && cur != (pattern_size + 3)) {
            pattern_count++;
            AddWordToForest(root, pattern, i - 1, cur, i);
            cur = pattern_size + 3;
        }
    }
    if (cur != (pattern_size + 3)) {
        pattern_count++;
        AddWordToForest(root, pattern, pattern_size - 1, cur, pattern_size);
    }
    FindAllSufLinks(root.get());
}


void TemplatesProblemSolver::AddWordToForest(std::shared_ptr<Node> &cur, const std::string &word, int add_index, int index, int terminate) {
    if (index == terminate) {
        cur->IsLetter = true;
        cur->Ends.push_back(add_index);
    } else {
        if (cur->DirectEdge[word[index]] != nullptr) {
            AddWordToForest(cur->DirectEdge[word[index]], word, add_index, index + 1, terminate);
        } else {
            std::shared_ptr<Node> NewNode(new Node(cur.get(), word[index]));
            cur->DirectEdge[word[index]] = NewNode;
            cur->BackEdge[word[index]] = NewNode.get();
            AddWordToForest(NewNode, word, add_index, index + 1, terminate);
        }
    }
}

void TemplatesProblemSolver::FindAllSufLinks(Node *CurNode) {
    std::queue<Node *> que;
    que.push(CurNode);
    CurNode->Up = root.get();
    while (!que.empty()) {
        Node *TempNode = que.front();
        que.pop();
        if (TempNode->SufLink->IsLetter && TempNode->Up == nullptr)
            TempNode->Up = TempNode->SufLink;
        else if (TempNode->Up == nullptr)
            TempNode->Up = TempNode->SufLink->Up;

        for (int i = 0; i < alph; i++) {
            if (TempNode->DirectEdge[i] == nullptr) {
                if (TempNode == root.get()) {
                    TempNode->BackEdge[i] = root.get();
                } else {
                    TempNode->BackEdge[i] = TempNode->SufLink->BackEdge[i];
                }
            }
            if (TempNode->DirectEdge[i] != nullptr) {
                que.push(TempNode->DirectEdge[i].get());
                if (TempNode == root.get()) {
                    TempNode->DirectEdge[i]->SufLink = root.get();
                } else {
                    TempNode->DirectEdge[i]->SufLink = TempNode->SufLink->BackEdge[i];
                }
            }
        }
    }

}

std::vector<int> TemplatesProblemSolver::FindTheText(const std::string &text) {
    Node* curNode = root.get();
    std::vector<int> indexes(text.length());
    for (int i = 0; i < text.length(); ++i) {
        curNode = curNode->BackEdge[text[i]];
        Node* cr = curNode;
        while (cr != root.get()) {
            for (auto ind : cr->Ends) {
                if (i - ind >= 0)
                    indexes[i - ind]++;
            }
            cr = cr->Up;
        }
    }

    return CountTextEnter(indexes);
}

const std::vector<int> TemplatesProblemSolver::CountTextEnter(std::vector<int> &CountOfPatterns) {
    std::vector<int> answer_indexes;
    for (int j = 0; j + pattern_size - 1 < CountOfPatterns.size(); ++j) {
        if (CountOfPatterns[j] == pattern_count) {
            answer_indexes.push_back(j);
        }
    }
    return answer_indexes;
}

/*
 * Class that used Temlate ProblemSolver to find all Enters to pattern
 *
 */
class PatterFinder {
public:
    /*
     * Get pattern string and intilize TemlateProblemSolver
     */
    explicit PatterFinder(const std::string& pattern);
    const std::vector<int> Finder(const std::string &text);
private:
    /*
     * Link to TemplateProblemSolver class that solves out problemes
     */
    std::shared_ptr<TemplatesProblemSolver> ahoKarasikAlgorithm;
};

PatterFinder::PatterFinder(const std::string& pattern) {
    ahoKarasikAlgorithm = std::make_shared<TemplatesProblemSolver>(pattern);
}

const std::vector<int> PatterFinder::Finder(const std::string &text) {
    return ahoKarasikAlgorithm->FindTheText(text);
}


int main() {
    std::ios_base::sync_with_stdio(0); // To make programm work quicker, we swithc out synchronization
    std::string pattern;
    std::string text;
    std::cin >> pattern; // read pattern string
    std::cin >> text; // read text that where we nust find our pattern
    if (pattern.length() > text.length()) {
        return 0;
    }
    PatterFinder patterFinder = PatterFinder(pattern);
    const std::vector<int>& indexes = patterFinder.Finder(text);
    for (int i = 0; i < indexes.size(); i++) {
        std::cout << indexes[i] << " ";
    }
}

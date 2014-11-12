#include <iostream> 
#include <map>
#include <queue>
#include <assert.h>
#include <unordered_map>

using namespace std; 

typedef unordered_map<char, struct TrieNode*> NextMap;

struct TrieNode{  
    TrieNode *fail;       //失败指针
    NextMap* next;
    bool isLeaf;
    TrieNode(){           //构造函数初始化
        fail = NULL; 
        next = NULL;
        isLeaf = false;
    } 
};

class Automation {
    public:
        Automation() {
            root_ = new TrieNode();
        }
        ~Automation(){}
    public:
        void insert(const char* word, size_t len) {
            typename NextMap::iterator iter;
            TrieNode* node = root_;
            TrieNode* newnode;
            for(size_t i = 0; i < len; i++) {
                if(node->next == NULL) {
                    node->next = new NextMap();
                }
                iter = node->next->find(word[i]);
                if(iter != node->next->end()) {
                    node = iter->second;
                } else {
                    newnode = new TrieNode();
                    (*(node->next))[word[i]] = newnode;
                    node = newnode;
                }
            }
            node->isLeaf = true;
        }

        // bfs
        void build() {
            queue<TrieNode*> que;
            assert(!root_->isLeaf);
            assert(root_->next);
            root_->fail = NULL;
            for(typename NextMap::iterator iter = root_->next->begin(); iter != root_->next->end(); iter++) {
                iter->second->fail = root_;
                que.push(iter->second);
            }
            TrieNode* back = NULL;
            typename NextMap::iterator backiter;
            while(!que.empty()) {
                TrieNode * now = que.front();
                que.pop();
                if(now->next == NULL) {
                    continue;
                }
                for(typename NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
                    back = now->fail;
                    while(back != NULL) {
                        backiter = back->next->find(iter->first); 
                        if(backiter != back->next->end()) {
                            iter->second->fail = backiter->second;
                            break;
                        }
                        back = back->fail;
                    }
                    if(back == NULL) {
                        iter->second->fail = root_;
                    }
                    que.push(iter->second);
                }
            }
        }
        void query(const char * sentence, size_t len) const {
            const TrieNode * now = root_;
            typename NextMap::const_iterator iter;
            size_t count = 0;
            for (size_t i = 0; i < len; i++) {
                while( now != root_ && (now->next == NULL || (iter = now->next->find(sentence[i])) == now->next->end())) {
                    now = now->fail;
                }
                if(now->next == NULL || (iter = now->next->find(sentence[i])) == now->next->end()) {
                    now = root_;
                    continue;
                }
                now = iter->second;
                const TrieNode * temp = now;
                while(temp != root_) {
                    if (temp->isLeaf) {
                        count ++;
                    }
                    temp = temp->fail;
                    assert(temp);
                }
            }
            cout << count << endl;
        }
    private:
        TrieNode * root_;
};


int main(){ 
    Automation automationer;
    //const char* keywords[] = {"she", "he", "say", "shr", "her"};
    const char* keywords[] = {"she", "he", "say", "ays", "aysa"};
    for(size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        automationer.insert(keywords[i], strlen(keywords[i]));
    }
    automationer.build();
    //const char* sentence = "yasherhs";
    const char* sentence = "saysay";
    automationer.query(sentence, strlen(sentence));
    return 0; 
}


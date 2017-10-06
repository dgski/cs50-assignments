
typedef struct node
{
    bool is_word;
    struct node* letters[27];
} node;

node* create(void);
void flag_is_word(node* element);
node* insert_letter(node* element, char letter);
void insert_raw_letter(node* element, int index);
bool search_trie(node* head, char* search_word);
void insert_word(node* head, char* input_word);
void free_trie(node* head);

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

Node *InsertKey(Node *root, int k, Node *tree, int i);
int GetHeight(Node *root);

int main() {
    int n;
    if(!scanf("%d", &n))
        return EXIT_FAILURE;
    if(n <= 0) {
        printf("0");
        return EXIT_SUCCESS;
    }
    Node *tree = malloc(sizeof(Node) * n);
    Node *root = NULL;
    int value;
    for(int i = 0; i < n; i++) {
        if(!scanf("%d", &value))
            return 0;
        root = InsertKey(root, value, tree, i);
    }
    printf("%d", GetHeight(root));
    free(tree);
    return EXIT_SUCCESS;
}

int GetHeight(Node *root) {
    return root ? root->height : 0;
}

void FixHeight(Node *root) {
    int height_l = GetHeight(root->left);
    int height_r = GetHeight(root->right);
    root->height = (height_l > height_r ? height_l : height_r) + 1;
}

Node *RightSubtreeRotation(Node *root) {
    Node *root_new = root->left;
    root->left = root_new->right;
    root_new->right = root;
    FixHeight(root);
    FixHeight(root_new);
    return root_new;
}

Node *LeftSubtreeRotation(Node *root) {
    Node *root_new = root->right;
    root->right = root_new->left;
    root_new->left = root;
    FixHeight(root);
    FixHeight(root_new);
    return root_new;
}

int GetSubtreeHeightDifference(Node *root) {
    return GetHeight(root->left) - GetHeight(root->right);
}

Node *BalanceTree(Node *root) {
    if(GetSubtreeHeightDifference(root) == -2) {
        if(GetSubtreeHeightDifference(root->right) > 0)
            root->right = RightSubtreeRotation(root->right);
        return LeftSubtreeRotation(root);
    }
    if(GetSubtreeHeightDifference(root) == 2) {
        if(GetSubtreeHeightDifference(root->left) < 0)
            root->left = LeftSubtreeRotation(root->left);
        return RightSubtreeRotation(root);
    }
    FixHeight(root);
    return root;
}

Node *InsertKey(Node *root, int k, Node *tree, int i) {
    if(!root) {
        tree[i].key = k;
        tree[i].height = 1;
        tree[i].left = NULL;
        tree[i].right = NULL;
        root = &tree[i];
        return root;
    }
    if(k < root->key)
        root->left = InsertKey(root->left, k, tree, i);
    else
        root->right = InsertKey(root->right, k, tree, i);
    return BalanceTree(root);
}

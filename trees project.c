#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5
#define MIN_SIZE 2
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_PAN_LENGTH 20
#define MAX_BANK_LENGTH 50

typedef enum
{
    NOT_FOUND,
    FOUND
} Status_code;

// Structure for Aadhar list
typedef struct AadharNode
{
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    int aadharNumber;

} AadharNode;

typedef struct PanNode
{
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char panNumber[MAX_PAN_LENGTH];
    int aadharNumber;
    struct PanNode *multi_pan;
} PanNode;

typedef struct BankAccountNode
{
    char name[MAX_NAME_LENGTH];
    char panNumber[MAX_PAN_LENGTH];
    char bank[MAX_BANK_LENGTH];
    int accountNumber;
    struct BankAccountNode *multi_bank;
    double depositedAmount;

} BankAccountNode;

typedef struct LPGNode
{
    char name[MAX_NAME_LENGTH];
    int accountNumber;
    char panNumber[MAX_PAN_LENGTH];
    char subsidyStatus[4]; // "YES" or "NO"
    BankAccountNode *bank_ptr;
} LPGNode;

typedef struct TreeNode_Aadhar
{
    AadharNode *data[MAX_SIZE];
    struct TreeNode_Aadhar *children[MAX_SIZE + 1];
    struct TreeNode_Aadhar *parent;
    int no_of_nodes;
} TreeNode_Aadhar;

typedef struct TreeNode_Pan
{
    PanNode *data[MAX_SIZE];
    struct TreeNode_Pan *children[MAX_SIZE + 1];
    struct TreeNode_Pan *parent;
    int no_of_nodes;
} TreeNode_Pan;

typedef struct TreeNode_Bank
{
    BankAccountNode *data[MAX_SIZE];
    struct TreeNode_Bank *children[MAX_SIZE + 1];
    struct TreeNode_Bank *parent;
    int no_of_nodes;
} TreeNode_Bank;

typedef struct TreeNode_LPG
{
    LPGNode *data[MAX_SIZE];
    struct TreeNode_LPG *children[MAX_SIZE + 1];
    struct TreeNode_LPG *parent;
    int no_of_nodes;
} TreeNode_LPG;

AadharNode *createAadharNode(char *name, char *address, int aadharNumber)
{
    AadharNode *newNode = (AadharNode *)malloc(sizeof(AadharNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    strcpy(newNode->address, address);
    newNode->aadharNumber = aadharNumber;
    return newNode;
}
TreeNode_Aadhar *createTreeNode_aadhar()
{
    TreeNode_Aadhar *newNode = (TreeNode_Aadhar *)malloc(sizeof(TreeNode_Aadhar));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->no_of_nodes = 0;
    newNode->parent = NULL;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        newNode->data[i] = NULL;
        newNode->children[i] = NULL;
    }
    newNode->children[MAX_SIZE] = NULL;
    return newNode;
}
TreeNode_Aadhar *splitNode_Aadhar(TreeNode_Aadhar *root, AadharNode *newNode)
{
    TreeNode_Aadhar *newRoot;
    if (root->parent == NULL)
    {
        newRoot = createTreeNode_aadhar();
        newRoot->data[0] = root->data[MIN_SIZE];
        root->data[MIN_SIZE] = NULL;
        root->no_of_nodes--;
        // creating childrens 0 , 1
        newRoot->children[0] = root;
        newRoot->children[1] = createTreeNode_aadhar();
        newRoot->children[0]->parent = newRoot;
        newRoot->children[1]->parent = newRoot;

        for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
        {
            newRoot->children[1]->data[i - MIN_SIZE - 1] = newRoot->children[0]->data[i];
            newRoot->children[0]->data[i];
            newRoot->children[1]->no_of_nodes++;
            newRoot->children[0]->no_of_nodes--;
        }
        newRoot->no_of_nodes = 1;
        root = newRoot;
    }
    else
    {
        TreeNode_Aadhar *parent = root->parent;
        if (parent->no_of_nodes < MAX_SIZE)
        {
            int i = parent->no_of_nodes;
            while (i > 0 && parent->data[i - 1]->aadharNumber > root->data[MIN_SIZE]->aadharNumber)
            {
                i--;
            }
            for (int a = parent->no_of_nodes - 1; a >= i; a--)
            {
                parent->data[a + 1] = parent->data[a];
                parent->children[a + 2] = parent->children[a + 1];
            }
            int index = i;
            parent->data[i] = root->data[MIN_SIZE];
            root->data[MIN_SIZE] = NULL;
            root->no_of_nodes--;
            // creating childrens 0 , 1
            parent->children[i] = root;
            parent->children[i + 1] = createTreeNode_aadhar();
            parent->children[i]->parent = parent;
            parent->children[i + 1]->parent = parent;

            for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
            {
                parent->children[index + 1]->data[i - MIN_SIZE - 1] = parent->children[index]->data[i];
                parent->children[index]->data[i];
                parent->children[index + 1]->no_of_nodes++;
                parent->children[index]->no_of_nodes--;
            }
            parent->no_of_nodes++;
        }
        else
        {
            root = splitNode_Aadhar(parent, newNode);
        }
    }
    return root;
}
TreeNode_Aadhar *insertNode_Aadhar(TreeNode_Aadhar *root, AadharNode *newNode)
{
    if (root == NULL)
    {
        root = createTreeNode_aadhar();
        root->data[root->no_of_nodes] = newNode;
        root->no_of_nodes++;
    }
    else
    {
        if (root->no_of_nodes < MAX_SIZE)
        {
            int i = root->no_of_nodes;
            while (i > 0 && root->data[i - 1]->aadharNumber > newNode->aadharNumber)
            {
                i--;
            }
            if (root->children[i] == NULL)
            {
                for (int a = root->no_of_nodes - 1; a >= i; a--)
                {
                    root->data[a + 1] = root->data[a];
                }
                root->data[i] = newNode;
                root->no_of_nodes++;

                while (root->parent != NULL)
                {
                    root = root->parent;
                }
            }
            else
            {
                root = insertNode_Aadhar(root->children[i], newNode);
            }
        }
        else
        {
            root = splitNode_Aadhar(root, newNode);
            while (root->parent != NULL)
            {
                root = root->parent;
            }
            root = insertNode_Aadhar(root, newNode);
        }
    }
    return root;
}
void printAadharList(TreeNode_Aadhar *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            printAadharList(root->children[i]);
            printf("Aadhar Number: %d | Name: %s | Address: %s\n", root->data[i]->aadharNumber, root->data[i]->name, root->data[i]->address);
        }
        printAadharList(root->children[root->no_of_nodes]);
    }
}
void freeBTree_Aadhar(TreeNode_Aadhar *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            freeBTree_Aadhar(root->children[i]);
            free(root->data[i]);
        }
        freeBTree_Aadhar(root->children[root->no_of_nodes]);
        free(root);
    }
}
Status_code find_Aadhar(TreeNode_Aadhar *root, int new_Aadhar)
{
    Status_code sc = NOT_FOUND;
    if (root == NULL)
    {
        sc = NOT_FOUND;
    }
    else
    {
        int i = root->no_of_nodes;
        while (i > 0 && root->data[i - 1]->aadharNumber >= new_Aadhar)
        {
            i--;
        }

        if (i != root->no_of_nodes && root->data[i]->aadharNumber == new_Aadhar)
        {
            sc = FOUND;
        }
        else
        {
            if (root->children[i] == NULL)
            {
                sc = NOT_FOUND;
            }
            else
            {
                sc = find_Aadhar(root->children[i], new_Aadhar);
            }
        }
    }
    return sc;
}

PanNode *createPanNode(char *name, char *address, char *panNumber, int aadharNumber)
{
    PanNode *newNode = (PanNode *)malloc(sizeof(PanNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    strcpy(newNode->address, address);
    strcpy(newNode->panNumber, panNumber);
    newNode->aadharNumber = aadharNumber;
    newNode->multi_pan = NULL;
    return newNode;
}
TreeNode_Pan *createTreeNode_Pan()
{
    TreeNode_Pan *newNode = (TreeNode_Pan *)malloc(sizeof(TreeNode_Pan));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->no_of_nodes = 0;
    newNode->parent = NULL;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        newNode->data[i] = NULL;
        newNode->children[i] = NULL;
    }
    newNode->children[MAX_SIZE] = NULL;
    return newNode;
}
TreeNode_Pan *splitNode_Pan(TreeNode_Pan *root, PanNode *newNode)
{
    TreeNode_Pan *newRoot;
    if (root->parent == NULL)
    {
        newRoot = createTreeNode_Pan();
        newRoot->data[0] = root->data[MIN_SIZE];
        root->data[MIN_SIZE] = NULL;
        root->no_of_nodes--;
        // creating childrens 0 , 1
        newRoot->children[0] = root;
        newRoot->children[1] = createTreeNode_Pan();
        newRoot->children[0]->parent = newRoot;
        newRoot->children[1]->parent = newRoot;

        for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
        {
            newRoot->children[1]->data[i - MIN_SIZE - 1] = newRoot->children[0]->data[i];
            newRoot->children[0]->data[i];
            newRoot->children[1]->no_of_nodes++;
            newRoot->children[0]->no_of_nodes--;
        }
        newRoot->no_of_nodes = 1;
        root = newRoot;
    }
    else
    {
        TreeNode_Pan *parent = root->parent;
        if (parent->no_of_nodes < MAX_SIZE)
        {
            int i = parent->no_of_nodes;
            while (i > 0 && parent->data[i - 1]->aadharNumber > root->data[MIN_SIZE]->aadharNumber)
            {
                i--;
            }
            for (int a = parent->no_of_nodes - 1; a >= i; a--)
            {
                parent->data[a + 1] = parent->data[a];
                parent->children[a + 2] = parent->children[a + 1];
            }
            int index = i;
            parent->data[i] = root->data[MIN_SIZE];
            root->data[MIN_SIZE] = NULL;
            root->no_of_nodes--;
            // creating childrens 0 , 1
            parent->children[i] = root;
            parent->children[i + 1] = createTreeNode_Pan();
            parent->children[i]->parent = parent;
            parent->children[i + 1]->parent = parent;

            for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
            {
                parent->children[index + 1]->data[i - MIN_SIZE - 1] = parent->children[index]->data[i];
                parent->children[index]->data[i];
                parent->children[index + 1]->no_of_nodes++;
                parent->children[index]->no_of_nodes--;
            }
            parent->no_of_nodes++;
        }
        else
        {
            root = splitNode_Pan(parent, newNode);
        }
    }
    return root;
}
TreeNode_Pan *insertNode_Pan(TreeNode_Pan *root, PanNode *newNode)
{
    if (root == NULL)
    {
        root = createTreeNode_Pan();
        root->data[root->no_of_nodes] = newNode;
        root->no_of_nodes++;
    }
    else
    {
        if (root->no_of_nodes < MAX_SIZE)
        {
            int i = root->no_of_nodes;
            while (i > 0 && root->data[i - 1]->aadharNumber > newNode->aadharNumber)
            {
                i--;
            }
            if (i != 0 && root->data[i - 1]->aadharNumber == newNode->aadharNumber)
            {
                while (root->data[i - 1]->multi_pan != NULL)
                {
                    root->data[i - 1] = root->data[i - 1]->multi_pan;
                }
                root->data[i - 1]->multi_pan = newNode;
                while (root->parent != NULL)
                {
                    root = root->parent;
                }
            }
            else
            {
                if (root->children[i] == NULL)
                {
                    for (int a = root->no_of_nodes - 1; a >= i; a--)
                    {
                        root->data[a + 1] = root->data[a];
                    }
                    root->data[i] = newNode;
                    root->no_of_nodes++;

                    while (root->parent != NULL)
                    {
                        root = root->parent;
                    }
                }
                else
                {
                    root = insertNode_Pan(root->children[i], newNode);
                }
            }
        }
        else
        {
            root = splitNode_Pan(root, newNode);
            while (root->parent != NULL)
            {
                root = root->parent;
            }
            root = insertNode_Pan(root, newNode);
        }
    }
    return root;
}
void printPanList(TreeNode_Pan *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            printPanList(root->children[i]);
            if (root->data[i]->multi_pan != NULL)
            {
                if (root->data[i]->multi_pan != NULL)
                {
                    PanNode *ptr;
                    ptr = root->data[i];
                    printf("Aadhar Number: %d | Name: %s | Address: %s | PAN: %s\n", ptr->aadharNumber, ptr->name, ptr->address, ptr->panNumber);
                    ptr = ptr->multi_pan;
                    while (ptr->multi_pan != NULL)
                    {
                        printf("                                                                                            | PAN: %s\n", ptr->panNumber);
                        ptr = ptr->multi_pan;
                    }
                    printf("                                                                                            | PAN: %s\n", ptr->panNumber);
                }
            }
            else
            {
                printf("Aadhar Number: %d | Name: %s | Address: %s | PAN: %s\n", root->data[i]->aadharNumber, root->data[i]->name, root->data[i]->address, root->data[i]->panNumber);
            }
        }
        printPanList(root->children[root->no_of_nodes]);
    }
}
void freeBTree_Pan(TreeNode_Pan *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            freeBTree_Pan(root->children[i]);
            free(root->data[i]);
        }
        freeBTree_Pan(root->children[root->no_of_nodes]);
        free(root);
    }
}
Status_code find_Aadhar_in_Pan(TreeNode_Pan *root, int Aadhar)
{
    Status_code sc = NOT_FOUND;
    if (root == NULL)
    {
        sc = NOT_FOUND;
    }
    else
    {
        int i = root->no_of_nodes;
        while (i > 0 && root->data[i - 1]->aadharNumber >= Aadhar)
        {
            i--;
        }

        if (i != root->no_of_nodes && root->data[i]->aadharNumber == Aadhar)
        {
            sc = FOUND;
        }
        else
        {
            if (root->children[i] == NULL)
            {
                sc = NOT_FOUND;
            }
            else
            {
                sc = find_Aadhar_in_Pan(root->children[i], Aadhar);
            }
        }
    }
    return sc;
}

BankAccountNode *createBankAccountNode(char *name, char *panNumber, char *bank, int accountNumber, double depositedAmount)
{
    BankAccountNode *newNode = (BankAccountNode *)malloc(sizeof(BankAccountNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    strcpy(newNode->panNumber, panNumber);
    strcpy(newNode->bank, bank);
    newNode->accountNumber = accountNumber;
    newNode->depositedAmount = depositedAmount;
    newNode->multi_bank = NULL;
    return newNode;
}
TreeNode_Bank *createTreeNode_Bank()
{
    TreeNode_Bank *newNode = (TreeNode_Bank *)malloc(sizeof(TreeNode_Bank));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->no_of_nodes = 0;
    newNode->parent = NULL;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        newNode->data[i] = NULL;
        newNode->children[i] = NULL;
    }
    newNode->children[MAX_SIZE] = NULL;
    return newNode;
}
TreeNode_Bank *splitNode_Bank(TreeNode_Bank *root, BankAccountNode *newNode)
{
    TreeNode_Bank *newRoot;
    if (root->parent == NULL)
    {
        newRoot = createTreeNode_Bank();
        newRoot->data[0] = root->data[MIN_SIZE];
        root->data[MIN_SIZE] = NULL;
        root->no_of_nodes--;
        // creating childrens 0 , 1
        newRoot->children[0] = root;
        newRoot->children[1] = createTreeNode_Bank();
        newRoot->children[0]->parent = newRoot;
        newRoot->children[1]->parent = newRoot;

        for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
        {
            newRoot->children[1]->data[i - MIN_SIZE - 1] = newRoot->children[0]->data[i];
            newRoot->children[0]->data[i];
            newRoot->children[1]->no_of_nodes++;
            newRoot->children[0]->no_of_nodes--;
        }
        newRoot->no_of_nodes = 1;
        root = newRoot;
    }
    else
    {
        TreeNode_Bank *parent = root->parent;
        if (parent->no_of_nodes < MAX_SIZE)
        {
            int i = parent->no_of_nodes;
            while (i > 0 && strcmp(parent->data[i - 1]->panNumber, root->data[MIN_SIZE]->panNumber) > 0)
            {
                i--;
            }
            for (int a = parent->no_of_nodes - 1; a >= i; a--)
            {
                parent->data[a + 1] = parent->data[a];
                parent->children[a + 2] = parent->children[a + 1];
            }
            int index = i;
            parent->data[i] = root->data[MIN_SIZE];
            root->data[MIN_SIZE] = NULL;
            root->no_of_nodes--;
            // creating childrens 0 , 1
            parent->children[i] = root;
            parent->children[i + 1] = createTreeNode_Bank();
            parent->children[i]->parent = parent;
            parent->children[i + 1]->parent = parent;

            for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
            {
                parent->children[index + 1]->data[i - MIN_SIZE - 1] = parent->children[index]->data[i];
                parent->children[index]->data[i];
                parent->children[index + 1]->no_of_nodes++;
                parent->children[index]->no_of_nodes--;
            }
            parent->no_of_nodes++;
        }
        else
        {
            root = splitNode_Bank(parent, newNode);
        }
    }
    return root;
}
TreeNode_Bank *insertNode_Bank(TreeNode_Bank *root, BankAccountNode *newNode)
{
    if (root == NULL)
    {
        root = createTreeNode_Bank();
        root->data[root->no_of_nodes] = newNode;
        root->no_of_nodes++;
    }
    else
    {
        if (root->no_of_nodes < MAX_SIZE)
        {
            int i = root->no_of_nodes;
            while (i > 0 && strcmp(root->data[i - 1]->panNumber, newNode->panNumber) > 0)
            {
                i--;
            }

            if (i != 0 && strcmp(root->data[i - 1]->panNumber, newNode->panNumber) == 0)
            {
                while (root->data[i - 1]->multi_bank != NULL)
                {
                    root->data[i - 1] = root->data[i - 1]->multi_bank;
                }
                root->data[i - 1]->multi_bank = newNode;
                while (root->parent != NULL)
                {
                    root = root->parent;
                }
            }
            else
            {
                if (root->children[i] == NULL)
                {
                    for (int a = root->no_of_nodes - 1; a >= i; a--)
                    {
                        root->data[a + 1] = root->data[a];
                    }
                    root->data[i] = newNode;
                    root->no_of_nodes++;

                    while (root->parent != NULL)
                    {
                        root = root->parent;
                    }
                }
                else
                {
                    root = insertNode_Bank(root->children[i], newNode);
                }
            }
        }
        else
        {
            root = splitNode_Bank(root, newNode);
            while (root->parent != NULL)
            {
                root = root->parent;
            }
            root = insertNode_Bank(root, newNode);
        }
    }
    return root;
}
void printBankList(TreeNode_Bank *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            printBankList(root->children[i]);
            if (root->data[i]->multi_bank != NULL)
            {
                BankAccountNode *ptr;
                ptr = root->data[i];
                printf("PAN: %s | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->panNumber, ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);
                ptr = ptr->multi_bank;
                while (ptr->multi_bank != NULL)
                {
                    printf("              | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);

                    ptr = ptr->multi_bank;
                }
                printf("               | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);
            }
            else
            {
                printf("PAN: %s | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", root->data[i]->panNumber, root->data[i]->accountNumber, root->data[i]->name, root->data[i]->bank, root->data[i]->depositedAmount);
            }
        }
        printBankList(root->children[root->no_of_nodes]);
    }
}
void freeBTree_Bank(TreeNode_Bank *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            freeBTree_Bank(root->children[i]);
            free(root->data[i]);
        }
        freeBTree_Bank(root->children[root->no_of_nodes]);
        free(root);
    }
}
Status_code find_pan_in_bank_for_LPG(TreeNode_Bank *root, char *pan, BankAccountNode **bank_ptr)
{
    Status_code sc = NOT_FOUND;
    if (root == NULL)
    {
        sc = NOT_FOUND;
    }
    else
    {
        int i = root->no_of_nodes;
        while (i > 0 && strcmp(root->data[i - 1]->panNumber, pan) >= 0)
        {
            i--;
        }

        if (i != root->no_of_nodes && strcmp(root->data[i]->panNumber, pan) >= 0)
        {
            sc = FOUND;
            printf("%x", root->data[i]);

            *bank_ptr = root->data[i];
            printf("hi");
        }
        else
        {
            if (root->children[i] == NULL)
            {
                sc = NOT_FOUND;
            }
            else
            {
                sc = find_pan_in_bank_for_LPG(root->children[i], pan, bank_ptr);
            }
        }
    }
    return sc;
}

LPGNode *createLPGNode(char *name, int accountNumber, char *panNumber, char *subsidyStatus)
{
    LPGNode *newNode = (LPGNode *)malloc(sizeof(LPGNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->accountNumber = accountNumber;
    strcpy(newNode->panNumber, panNumber);
    strcpy(newNode->subsidyStatus, subsidyStatus);
    newNode->bank_ptr = NULL;
    return newNode;
}
TreeNode_LPG *createTreeNode_LPG()
{
    TreeNode_LPG *newNode = (TreeNode_LPG *)malloc(sizeof(TreeNode_LPG));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->no_of_nodes = 0;
    newNode->parent = NULL;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        newNode->data[i] = NULL;
        newNode->children[i] = NULL;
    }
    newNode->children[MAX_SIZE] = NULL;
    return newNode;
}
TreeNode_LPG *splitNode_LPG(TreeNode_LPG *root, LPGNode *newNode)
{
    TreeNode_LPG *newRoot;
    if (root->parent == NULL)
    {
        newRoot = createTreeNode_LPG();
        newRoot->data[0] = root->data[MIN_SIZE];
        root->data[MIN_SIZE] = NULL;
        root->no_of_nodes--;

        newRoot->children[0] = root;
        newRoot->children[1] = createTreeNode_LPG();
        newRoot->children[0]->parent = newRoot;
        newRoot->children[1]->parent = newRoot;

        for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
        {
            newRoot->children[1]->data[i - MIN_SIZE - 1] = newRoot->children[0]->data[i];
            newRoot->children[0]->data[i];
            newRoot->children[1]->no_of_nodes++;
            newRoot->children[0]->no_of_nodes--;
        }
        newRoot->no_of_nodes = 1;
        root = newRoot;
    }
    else
    {
        TreeNode_LPG *parent = root->parent;
        if (parent->no_of_nodes < MAX_SIZE)
        {
            int i = parent->no_of_nodes;
            while (i > 0 && strcmp(root->data[i - 1]->panNumber, newNode->panNumber) > 0)
            {
                i--;
            }
            for (int a = parent->no_of_nodes - 1; a >= i; a--)
            {
                parent->data[a + 1] = parent->data[a];
                parent->children[a + 2] = parent->children[a + 1];
            }
            int index = i;
            parent->data[i] = root->data[MIN_SIZE];
            root->data[MIN_SIZE] = NULL;
            root->no_of_nodes--;

            parent->children[i] = root;
            parent->children[i + 1] = createTreeNode_LPG();
            parent->children[i]->parent = parent;
            parent->children[i + 1]->parent = parent;

            for (int i = MIN_SIZE + 1; i < MAX_SIZE; i++)
            {
                parent->children[index + 1]->data[i - MIN_SIZE - 1] = parent->children[index]->data[i];
                parent->children[index]->data[i];
                parent->children[index + 1]->no_of_nodes++;
                parent->children[index]->no_of_nodes--;
            }
            parent->no_of_nodes++;
        }
        else
        {
            root = splitNode_LPG(parent, newNode);
        }
    }
    return root;
}
TreeNode_LPG *insertNode_LPG(TreeNode_LPG *root, LPGNode *newNode)
{
    if (root == NULL)
    {
        root = createTreeNode_LPG();
        root->data[root->no_of_nodes] = newNode;
        root->no_of_nodes++;
    }
    else
    {
        if (root->no_of_nodes < MAX_SIZE)
        {
            int i = root->no_of_nodes;
            while (i > 0 && strcmp(root->data[i - 1]->panNumber, newNode->panNumber) > 0)
            {
                i--;
            }
            if (root->children[i] == NULL)
            {
                for (int a = root->no_of_nodes - 1; a >= i; a--)
                {
                    root->data[a + 1] = root->data[a];
                }
                root->data[i] = newNode;
                root->no_of_nodes++;
                while (root->parent != NULL)
                {
                    root = root->parent;
                }
            }
            else
            {
                root = insertNode_LPG(root->children[i], newNode);
            }
        }
        else
        {
            root = splitNode_LPG(root, newNode);
            while (root->parent != NULL)
            {
                root = root->parent;
            }
            root = insertNode_LPG(root, newNode);
        }
    }
    return root;
}
void printLPGList(TreeNode_LPG *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            printLPGList(root->children[i]);
            printf("Name: %20s | Account Number: %d | Pan Number: %s | Subsidy Status: %s\n", root->data[i]->name, root->data[i]->accountNumber, root->data[i]->panNumber, root->data[i]->subsidyStatus);
        }
        printLPGList(root->children[root->no_of_nodes]);
    }
}
void freeBTree_LPG(TreeNode_LPG *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            freeBTree_LPG(root->children[i]);
            free(root->data[i]);
        }
        freeBTree_LPG(root->children[root->no_of_nodes]);
        free(root);
    }
}

TreeNode_Aadhar *read_aadharlist(TreeNode_Aadhar *root)
{
    FILE *file = fopen("aadhar_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    { // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        char address[MAX_ADDRESS_LENGTH];
        long long int aadharNumber;
        int read = 0;
        TreeNode_Aadhar *root_prev = NULL;
        do
        {
            read = fscanf(file, "%lld | %[^|] | %[^|] | ", &aadharNumber, name, address);
            if (read == 3)
            {
                AadharNode *newNode = createAadharNode(name, address, aadharNumber);
                root = insertNode_Aadhar(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}
TreeNode_Pan *read_panlist(TreeNode_Pan *root)
{
    FILE *file = fopen("pan_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    {
        // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        char address[MAX_ADDRESS_LENGTH];
        char panNumber[MAX_PAN_LENGTH];
        long long int aadharNumber;
        int read = 0;

        do
        {
            read = fscanf(file, "%d | %[^|] | %[^|]  | %[^|] |", &aadharNumber, name, address, panNumber);
            if (read == 4)
            {
                PanNode *newNode = createPanNode(name, address, panNumber, aadharNumber);
                root = insertNode_Pan(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}
TreeNode_Bank *read_bankaccountlist(TreeNode_Bank *root)
{
    FILE *file = fopen("bankaccount_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    {
        // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        char panNumber[MAX_PAN_LENGTH];
        char bank[MAX_BANK_LENGTH];
        long long int accountNumber;
        double depositedAmount;
        int read = 0;
        do
        {
            read = fscanf(file, " %[^|] | %[^|] | %lld | %lf | %[^|] | ", name, bank, &accountNumber, &depositedAmount, panNumber);
            if (read == 5)
            {
                BankAccountNode *newNode = createBankAccountNode(name, panNumber, bank, accountNumber, depositedAmount);
                root = insertNode_Bank(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}
TreeNode_Bank *read_bankaccountlist_1(TreeNode_Bank *root)
{
    FILE *file = fopen("bankaccount1_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    {
        // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        char panNumber[MAX_PAN_LENGTH];
        char bank[MAX_BANK_LENGTH];
        long long int accountNumber;
        double depositedAmount;
        int read = 0;
        do
        {
            read = fscanf(file, " %[^|] | %[^|] | %lld | %lf | %[^|] | ", name, bank, &accountNumber, &depositedAmount, panNumber);
            if (read == 5)
            {
                BankAccountNode *newNode = createBankAccountNode(name, panNumber, bank, accountNumber, depositedAmount);
                root = insertNode_Bank(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}
TreeNode_Bank *read_bankaccountlist_2(TreeNode_Bank *root)
{
    FILE *file = fopen("bankaccount2_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    {
        // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        char panNumber[MAX_PAN_LENGTH];
        char bank[MAX_BANK_LENGTH];
        long long int accountNumber;
        double depositedAmount;
        int read = 0;
        do
        {
            read = fscanf(file, " %[^|] | %[^|] | %lld | %lf | %[^|] | ", name, bank, &accountNumber, &depositedAmount, panNumber);
            if (read == 5)
            {
                BankAccountNode *newNode = createBankAccountNode(name, panNumber, bank, accountNumber, depositedAmount);
                root = insertNode_Bank(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}
TreeNode_LPG *read_lpglist(TreeNode_LPG *root)
{
    FILE *file = fopen("lpg_data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    else
    {
        // Read data from the file and insert it into the linked list
        char name[MAX_NAME_LENGTH];
        int accountNumber;
        char panNumber[MAX_PAN_LENGTH];
        char subsidyStatus[MAX_NAME_LENGTH];
        int read = 0;
        do
        {
            read = fscanf(file, "%s | %[^|] | %d | %s ", panNumber, name, &accountNumber, subsidyStatus);
            if (read == 4)
            {
                LPGNode *newNode = createLPGNode(name, accountNumber, panNumber, subsidyStatus);
                root = insertNode_LPG(root, newNode);
            }
        } while (!feof(file));

        // Close the file
        fclose(file);
    }
    return root;
}

void Aadhar_without_pan(TreeNode_Aadhar *root, TreeNode_Pan *root_p)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            Aadhar_without_pan(root->children[i], root_p);
            Status_code sc = find_Aadhar_in_Pan(root_p, root->data[i]->aadharNumber);
            if (sc == NOT_FOUND)
            {
                printf("Aadhar Number: %d, Name: %s, Address: %s\n", root->data[i]->aadharNumber, root->data[i]->name, root->data[i]->address);
            }
        }
        Aadhar_without_pan(root->children[root->no_of_nodes], root_p);
    }
}
void print_People_With_Multiple_PAN(TreeNode_Pan *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            print_People_With_Multiple_PAN(root->children[i]);

            if (root->data[i]->multi_pan != NULL)
            {
                PanNode *ptr;
                ptr = root->data[i];

                printf("Aadhar Number: %d | Name: %s | Address: %s | PAN: %s\n", ptr->aadharNumber, ptr->name, ptr->address, ptr->panNumber);
                ptr = ptr->multi_pan;

                while (ptr->multi_pan != NULL)
                {
                    printf("                                                                                            | PAN: %s\n", ptr->panNumber);

                    ptr = ptr->multi_pan;
                }
                printf("                                                                                            | PAN: %s\n", ptr->panNumber);
            }
        }
        print_People_With_Multiple_PAN(root->children[root->no_of_nodes]);
    }
}
void print_People_With_Multiple_Bank(TreeNode_Bank *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            print_People_With_Multiple_Bank(root->children[i]);
            if (root->data[i]->multi_bank != NULL)
            {
                BankAccountNode *ptr;
                ptr = root->data[i];
                printf("PAN: %s | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->panNumber, ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);
                ptr = ptr->multi_bank;
                while (ptr->multi_bank != NULL)
                {
                    printf("              | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);

                    ptr = ptr->multi_bank;
                }
                printf("               | Account Number: %d | Name: %20s | Bank: %20s | Amount: %.2lf\n", ptr->accountNumber, ptr->name, ptr->bank, ptr->depositedAmount);
            }
        }
        print_People_With_Multiple_Bank(root->children[root->no_of_nodes]);
    }
}
TreeNode_Bank *merge_Bank_Tree(TreeNode_Bank *root_b2, TreeNode_Bank *root_b1)
{
    TreeNode_Bank *final_root;
    if (root_b2 != NULL)
    {
        for (int i = 0; i < root_b2->no_of_nodes; i++)
        {
            merge_Bank_Tree(root_b2->children[i], root_b1);
            final_root = insertNode_Bank(root_b1, root_b2->data[i]);
        }
        merge_Bank_Tree(root_b2->children[root_b2->no_of_nodes], root_b1);
    }
    return final_root;
}
void print_in_the_range(TreeNode_Aadhar *root, int upper_value, int lower_value)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            print_in_the_range(root->children[i], upper_value, lower_value);
            if (root->data[i]->aadharNumber >= lower_value && root->data[i]->aadharNumber <= upper_value)
            {
                printf("Aadhar Number: %d | Name: %s | Address: %s\n", root->data[i]->aadharNumber, root->data[i]->name, root->data[i]->address);
            }
        }
        print_in_the_range(root->children[root->no_of_nodes], upper_value, lower_value);
    }
}
void link_lpg_to_bank(TreeNode_LPG *root, TreeNode_Bank *root_b)
{
    BankAccountNode **bank_ptr=NULL;
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            link_lpg_to_bank(root->children[i], root_b);
            printf("%x", **bank_ptr);

            Status_code sc = find_pan_in_bank_for_LPG(root_b, root->data[i]->panNumber, bank_ptr);
            // printf("%s", root->data[i]->panNumber);
            if (sc == FOUND)
            {
                root->data[i]->bank_ptr = *bank_ptr;
                printf("Name: %20s | Account Number: %d | Pan Number: %s | Subsidy Status: %s\n", root->data[i]->name, root->data[i]->accountNumber, root->data[i]->panNumber, root->data[i]->subsidyStatus);
            }
        }
        link_lpg_to_bank(root->children[root->no_of_nodes], root_b);
    }
}
void printLPG_bank_List(TreeNode_LPG *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            printLPG_bank_List(root->children[i]);
            if (root->data[i]->bank_ptr != NULL)
            {
                printf("Name: %20s | Account Number: %d | Pan Number: %s | Subsidy Status: %s\n", root->data[i]->name, root->data[i]->accountNumber, root->data[i]->panNumber, root->data[i]->subsidyStatus);
            }

            printLPG_bank_List(root->children[root->no_of_nodes]);
        }
    }
}
Status_code find_Aadhar_in_Pan_and_compare(TreeNode_Pan *root, int Aadhar, char *name)
{
    Status_code sc = NOT_FOUND;
    if (root == NULL)
    {
        sc = NOT_FOUND;
    }
    else
    {
        int i = root->no_of_nodes;
        while (i > 0 && root->data[i - 1]->aadharNumber >= Aadhar)
        {
            i--;
        }

        if (i != root->no_of_nodes && root->data[i]->aadharNumber == Aadhar)
        {
            if (strcmp(root->data[i]->name, name) != 0)
            {
                printf("Name in pan_list :%s\n", root->data[i]->name);
                sc = FOUND;
            }
        }
        else
        {
            if (root->children[i] == NULL)
            {
                sc = NOT_FOUND;
            }
            else
            {
                sc = find_Aadhar_in_Pan_and_compare(root->children[i], Aadhar, name);
            }
        }
    }
    return sc;
}
void print_Inconsistent_Data(TreeNode_Aadhar *root, TreeNode_Pan *root_p)
{
    if (root != NULL)
    {
        for (int i = 0; i < root->no_of_nodes; i++)
        {
            print_Inconsistent_Data(root->children[i], root_p);
            Status_code sc = find_Aadhar_in_Pan_and_compare(root_p, root->data[i]->aadharNumber, root->data[i]->name);
            if (sc == FOUND)
            {
                printf("Aadhar Number: %d | Name: %s | Address: %s\n", root->data[i]->aadharNumber, root->data[i]->name, root->data[i]->address);
            }
        }
        print_Inconsistent_Data(root->children[root->no_of_nodes], root_p);
    }
}
void print_People_With_High_Savings_And_LPGSubsidy()
{
}

int main()
{
    // Initialize head pointers of linked lists
    Status_code sc;

    // Read data from files and insert it into the linked lists
    TreeNode_Aadhar *root_a = NULL;
    root_a = read_aadharlist(root_a);
    // printAadharList(root_a);

    TreeNode_Pan *root_p = NULL;
    root_p = read_panlist(root_p);
    // printPanList(root_p);

    TreeNode_Bank *root_b = NULL;
    root_b = read_bankaccountlist(root_b);
    // printBankList(root_b);

    TreeNode_LPG *root_lpg = NULL;
    root_lpg = read_lpglist(root_lpg);

    TreeNode_Bank *root_b1 = NULL, *root_b2 = NULL, *merge_root = NULL;
    root_b1 = read_bankaccountlist_1(root_b1);
    root_b2 = read_bankaccountlist_2(root_b2);

    // printBankList(root_b);

    // head_pan = read_panlist(head_pan);
    // head_bank = read_bankaccountlist(head_bank);
    // head_lpg = read_lpglist_sorted(head_lpg);
    // head_bank1 = read_bankaccountlist1(head_bank1);
    // head_bank2 = read_bankaccountlist2(head_bank2);

    // Print data stored in the linked lists
    printf("Aadhar data stored in the B-Tree:\n");

    printf("PAN data stored in the B-Tree (sorted by Aadhar number):\n");

    printf("Bank Account data stored in the B-Tree (sorted by PAN number):\n");

    printf("LPG data stored in the B-Tree (sorted by Account no):\n");
    int upper_value, lower_value;
    int i;
    do
    {
        double amount = 0.0;
        printf("ENTER 1:\t Print data of people having Aadhaar numbers but no PAN numbers\n");
        printf("ENTER 2:\t Print data of people having Multiple PAN numbers\n");
        printf("ENTER 3:\t Print data of people having Multiple Bank Accounts\n");
        printf("ENTER 4:\t Print BANK data of people having LPG subsidy\n");
        printf("ENTER 5:\t To print people with higher AMOUNT and have subsidy\n");
        printf("ENTER 6:\t To print inconsistent Data\n");
        printf("ENTER 7:\t To Merge two Bank Trees\n");
        printf("ENTER 8:\t To print values in a given range\n");
        printf("ENTER 9:\t To print Aadhar Tree\n");
        printf("ENTER 10:\t To print PAN  Tree\n");
        printf("ENTER 11:\t To print Bank Tree\n");
        printf("ENTER 12:\t To print LPG  Tree\n");
        printf("ENTER 13:\t *TO EXIT*\n");

        printf("ENTER your choice: ");
        scanf("%d", &i);

        switch (i)
        {
        case 1:
            Aadhar_without_pan(root_a, root_p);
            break;
        case 2:
            print_People_With_Multiple_PAN(root_p);
            break;
        case 3:
            print_People_With_Multiple_Bank(root_b);
            break;
        case 4:
            link_lpg_to_bank(root_lpg, root_b);
            printLPG_bank_List(root_lpg);
            break;
        case 5:
            printf("Enter the amount to check:");
            scanf("%lf", &amount);
            print_People_With_High_Savings_And_LPGSubsidy();
            break;
        case 6:
            print_Inconsistent_Data(root_a, root_p);
            break;
        case 7:
            merge_root = merge_Bank_Tree(root_b2, root_b1);
            printBankList(merge_root);
            break;
        case 8:
            printf("Enter upper_bound:");
            scanf("%d", &upper_value);
            printf("Enter lower_bound:");
            scanf("%d", &lower_value);
            print_in_the_range(root_a, upper_value, lower_value);
            break;

        case 9:
            printAadharList(root_a);

            break;
        case 10:
            printPanList(root_p);

            break;
        case 11:
            printBankList(root_b);
            break;

        case 12:
            printLPGList(root_lpg);
            break;
        case 13:
            printf("\n---------------EXIT-----------------\n");
            break;
        default:
            if (i != 13)
            {
                printf("Enter correct option\n");
                break;
            }
        }
    } while (i != 13);

    // Free memory allocated for linked list nodes

    freeBTree_LPG(root_lpg);
    freeBTree_Aadhar(root_a);
    freeBTree_Pan(root_p);
    freeBTree_Bank(root_b);
    freeBTree_Bank(root_b1);
    freeBTree_Bank(root_b2);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct Node_Group {
    int group_id;
    int group_size;
    char group_name[100];
    int Weekly_Group_Goal;
    int *member_id;
    struct Node_Group *next;
} Node_Group;

// Function to read data from file and create a Node_Group
Node_Group *readFromFile(FILE *file) {
    Node_Group *newNode = (Node_Group *)malloc(sizeof(Node_Group));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Read data from file
    if (fscanf(file, "%d %d %99s %d", &newNode->group_id, &newNode->group_size, newNode->group_name, &newNode->Weekly_Group_Goal) != 4) {
        free(newNode);
        return NULL; // Failed to read data
    }

    // Allocate memory for member_id array
    newNode->member_id = (int *)malloc(newNode->group_size * sizeof(int));
    if (!newNode->member_id) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Read member_id values
    for (int i = 0; i < newNode->group_size; i++) {
        if (fscanf(file, "%d", &newNode->member_id[i]) != 1) {
            free(newNode->member_id);
            free(newNode);
            return NULL; // Failed to read member_id values
        }
    }

    newNode->next = NULL;
    return newNode;
}

// Function to print the data from the Node_Group
void printNode(Node_Group *node) {
    printf("Group ID: %d\n", node->group_id);
    printf("Group Size: %d\n", node->group_size);
    printf("Group Name: %s\n", node->group_name);
    printf("Weekly Group Goal: %d\n", node->Weekly_Group_Goal);
    printf("Member IDs: ");
    for (int i = 0; i < node->group_size; i++) {
        printf("%d ", node->member_id[i]);
    }
    printf("\n\n");
}

int main() {
    FILE *file = fopen("d.txt", "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Node_Group *head = NULL;
    Node_Group *current = NULL;

    // Read data from file and create linked list of Node_Group
    while (1) {
        Node_Group *newNode = readFromFile(file);
        if (!newNode) {
            break; // Break if failed to read data
        }

        if (!head) {
            head = newNode;
            current = head;
        } else {
            current->next = newNode;
            current = newNode;
        }
    }

    // Close the file
    fclose(file);

    // Print the data
    Node_Group *temp = head;
    while (temp != NULL) {
        printNode(temp);
        temp = temp->next;
    }

    // Free allocated memory
    temp = head;
    while (temp != NULL) {
        Node_Group *next = temp->next;
        free(temp->member_id);
        free(temp);
        temp = next;
    }

    return EXIT_SUCCESS;
}

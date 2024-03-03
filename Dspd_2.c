#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_Individual {
    int id;
    char name[100];
    int age;
    int daily_goal;
    int weekly_step_count[7];
    int ingroup=0;
    struct Node_Individual* next;
} Node_Individual;

typedef struct Node_Group{
    int group_id;
    int group_size;
    char group_name[100];
    int Weekly_Group_Goal;
    int * member_id;
    struct Node_Group *next;
}Node_Group;

void printList(Node_Individual* head) {
    while (head != NULL) {
        printf("ID: %d\n", head->id);
        printf("Name: %s\n", head->name);
        printf("Age: %d\n", head->age);
        printf("Daily Goal: %d\n", head->daily_goal);
        printf("Weekly Step Count: ");
        for (int i = 0; i < 7; ++i) {
            printf("%d ", head->weekly_step_count[i]);
        }
        printf("\n\n");

        head = head->next;
    }
}

int main() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Node_Individual* head = NULL;
    Node_Individual* current = NULL;

    while (1) {
        Node_Individual* newNode = (Node_Individual*)malloc(sizeof(Node_Individual));
        if (newNode == NULL) {
            perror("Memory allocation error");
            return 1;
        }

        int result = fscanf(file, "%d %s %d %d %d %d %d %d %d %d %d",
                            &newNode->id, newNode->name, &newNode->age, &newNode->daily_goal,
                            &newNode->weekly_step_count[0], &newNode->weekly_step_count[1],
                            &newNode->weekly_step_count[2], &newNode->weekly_step_count[3],
                            &newNode->weekly_step_count[4], &newNode->weekly_step_count[5],
                            &newNode->weekly_step_count[6]);

        if (result == EOF) {
            free(newNode);
            break;
        } else if (result != 11) {
            fprintf(stderr, "Error reading data from file\n");
            free(newNode);
            break;
        }

        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = newNode;
        }
    }

    fclose(file);
    printList(head);

    while (head != NULL) {
        Node_Individual* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

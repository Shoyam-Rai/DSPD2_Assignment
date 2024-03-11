#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_Individual {
    int id;
    char name[100];
    int age;
    int daily_goal;
    int weekly_step_count[7];
    int ingroup;
    struct Node_Individual* next;
} Node_Individual;

typedef struct Node_Group{
    int group_id;
    int group_size;
    char group_name[100];
    int Weekly_Group_Goal;
   //int * member_id;
    Node_Individual ** individuals;
    struct Node_Group *next;
}Node_Group;

typedef struct TotalStepCount {
    int id;
    char name[100];
    int total_steps;
    struct TotalStepCount *next;
} TotalStepCount;

typedef struct LeaderboardNode {
    char group_name[100];
    int total_steps;
    int rank;
    struct LeaderboardNode *next;
} LeaderboardNode;
 
  Node_Individual * readFromIndividualFile(FILE * file,Node_Individual * headIndividual){
 
    headIndividual = NULL;
    Node_Individual* current = NULL;

    while (1) {
        Node_Individual* newNode = (Node_Individual*)malloc(sizeof(Node_Individual));
        if (newNode == NULL) {
            perror("Memory allocation error");
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
        newNode->ingroup=0;

        if (headIndividual == NULL) {
            headIndividual = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = newNode;
        }
    }

    fclose(file);
    printf("input of indiviuals taken\n");


return headIndividual;
    
}

Node_Group *readFromFile(FILE *file,Node_Individual* IndividualList) {
    Node_Group *newNode = (Node_Group *)malloc(sizeof(Node_Group));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }


    if (fscanf(file, "%d %d %99s %d", &newNode->group_id, &newNode->group_size, newNode->group_name, &newNode->Weekly_Group_Goal) != 4) {
        free(newNode);
        return NULL; 
    }


    newNode->individuals = (Node_Individual* *)malloc(newNode->group_size * sizeof(Node_Individual*));
    if (!newNode->individuals) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < newNode->group_size; i++) {
        int id;
        int r=fscanf(file, "%d", &id);
        Node_Individual* ptr= IndividualList;
        while(ptr->id!=id && ptr!=NULL)
        {
            ptr=ptr->next;
        }
        if(ptr==NULL)
        {
            printf("individual with id %d is not registered\n",id);
            newNode->group_size--;
        }
        else
        {
            newNode->individuals[i]=ptr;
        }
        if(r!=1)
        {
            free(newNode->individuals);
            free(newNode);
            return NULL; 
        }
    }

    newNode->next = NULL;
    return newNode;
}

Node_Group * readFromGroupFile(FILE * file2,FILE *file, Node_Individual * headIndividual , Node_Group * headGroup){
   Node_Group * current2=NULL;
   headGroup=NULL;
  while (1) {
        Node_Group *newNode2 = readFromFile(file2,headIndividual);


        if (!newNode2) {
            break; 
        }
     
    for (int i = 0; i < newNode2->group_size; i++) {
        int memberId = newNode2->individuals[i]->id;

        Node_Individual *tempIndividual = headIndividual;
        while (tempIndividual != NULL) {
            if (tempIndividual->id == memberId) {
                tempIndividual->ingroup = 1;
                break; 
            }
            tempIndividual = tempIndividual->next;
        }
    }
        if (!headGroup ) {
            headGroup  = newNode2;
            current2 = headGroup ;
        } else {
            current2->next = newNode2;
            current2 = newNode2;
        }
    }
    fclose(file);
printf("Input of Group Taken:\n");
    return headGroup;
}

void printIndividualList(Node_Individual* head) {
    while (head != NULL) {
        printf("ID: %d\n", head->id);
        printf("Name: %s\n", head->name);
        printf("Age: %d\n", head->age);
        printf("Daily Goal: %d\n", head->daily_goal);
        printf("Is the Individual in any group? %d\n", head->ingroup);
        printf("Weekly Step Count: ");
        for (int i = 0; i < 7; ++i) {
            printf("%d ", head->weekly_step_count[i]);
        }
        printf("\n\n");

        head = head->next;
    }
}

void printGroupList(Node_Group *node) {
    printf("Group ID: %d\n", node->group_id);
    printf("Group Size: %d\n", node->group_size);
    printf("Group Name: %s\n", node->group_name);
    printf("Weekly Group Goal: %d\n", node->Weekly_Group_Goal);
    printf("Member IDs: ");
    for (int i = 0; i < node->group_size; i++) {
        printf("%d ", node->individuals[i]->id);
    }
    printf("\n\n");
}

void printGroup(Node_Group * headGroup){
    Node_Group *temp = headGroup ;
    while (temp != NULL) {
        printGroupList(temp);
        temp = temp->next;
    }
}

void writeToFile(FILE *file, Node_Individual *node) {
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    while (node != NULL) {
        fprintf(file, "%d %s %d %d", node->id, node->name, node->age, node->daily_goal);
        for (int i = 0; i < 7; i++) {
            fprintf(file, " %d", node->weekly_step_count[i]);
        }
        fprintf(file, "\n");

        node = node->next;
    }
    fclose(file);
}


void writeGroupToFile(Node_Group *node) {
    FILE *file = fopen("d.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (node != NULL) {
        fprintf(file, "%d %d %s %d", node->group_id, node->group_size, node->group_name, node->Weekly_Group_Goal);
        
        for (int i = 0; i < node->group_size; i++) {
            fprintf(file, " %d", node->individuals[i]->id);
        }
        
        fprintf(file, "\n");

        node = node->next;
    }

    fclose(file);
}

int isIdExist(Node_Individual* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            return 1; 
        }
        head = head->next;
    }
    return 0; 
}

int isIdExistGroup(Node_Group* head, int id) {
    while (head != NULL) {
        if (head->group_id == id) {
            return 1; 
        }
        head = head->next;
    }
    return 0; 
}

Node_Individual * Add_Person(Node_Individual *node){
    Node_Individual* nodeToInsert = (Node_Individual*)malloc(sizeof(Node_Individual));
    printf("Enter the details for the node to be inserted:\n");
    printf("Enter id:\n");
    scanf("%d",&nodeToInsert->id);

    if (isIdExist(node, nodeToInsert->id)==1) {
        printf("Error: ID already exists. Please choose a different ID.\n");
        free(nodeToInsert);
        return node;
    }
    printf("Enter name:\n");
    scanf("%s",&nodeToInsert->name);
    printf("Enter age:\n");
    scanf("%d",&nodeToInsert->age);
    printf("Enter daily goal:\n");
    scanf("%d",&nodeToInsert->daily_goal);
    printf("Enter weekly step count:\n");
    for(int i=0;i<7;i++){
        scanf("%d",&nodeToInsert->weekly_step_count[i]);
    }
    
    Node_Individual *prev=NULL;
    Node_Individual *head=node;
    while(head!=NULL && nodeToInsert->id>head->id){
        prev=head;
        head=head->next;
    }
    if(prev==NULL){
        nodeToInsert->next=node;
        node=nodeToInsert;
    }
    else{
        nodeToInsert->next=prev->next;
        prev->next=nodeToInsert;
    }

    printf("Individual added Successfully!\n");

    FILE *file = fopen("dspd_file_individual.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    writeToFile(file, node);

    return node;

}
Node_Group *Create_Group(Node_Group *node, Node_Individual *individualList) {
    

    Node_Group *groupToInsert = (Node_Group *)malloc(sizeof(Node_Group));
    if (groupToInsert == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    printf("Enter the group details:\n");
    printf("Enter group_id:\n");
    scanf("%d", &groupToInsert->group_id);

    if (isIdExistGroup(node, groupToInsert->group_id)==1) {
        printf("Error: ID already exists. Please choose a different ID.\n");
        free(groupToInsert);
        return node;
    }
    printf("Enter group_size:\n");
    scanf("%d", &groupToInsert->group_size);
    printf("Enter group_name:\n");
    scanf("%s", groupToInsert->group_name);
    printf("Enter Weekly_Group_Goal:\n");
    scanf("%d", &groupToInsert->Weekly_Group_Goal);

    groupToInsert->individuals = (Node_Individual **)malloc(groupToInsert->group_size * sizeof(Node_Individual*));
    if (groupToInsert->individuals == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    printf("Enter Member_id of group_members (each separated by space):\n");
    int k=0;
    int id;

    for (int i = 0; i < groupToInsert->group_size; i++) {
        scanf("%d", &id);
        
        Node_Individual *temp = individualList;
        int found = 0;

        while (temp != NULL) {
            if (temp->id == id && temp->ingroup == 0) {
                found = 1;
                groupToInsert->individuals[k]=temp;
                k++;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            fprintf(stderr, "Error: Member ID %d not found or already in a group.\n", id);
            free(groupToInsert->individuals);
            free(groupToInsert);
            return node;
        }
    }

   
    Node_Group *prev=NULL;
    Node_Group *head=node;
    while(head!=NULL && groupToInsert->group_id>head->group_id){
        prev=head;
        head=head->next;
    }
    if(prev==NULL){
        groupToInsert->next=node;
        node=groupToInsert;
    }
    else{
        groupToInsert->next=prev->next;
        prev->next=groupToInsert;
    }
    writeGroupToFile(node);
    printf("Group added successfully!\n");
    return node;
}



int calculateTotalSteps(const Node_Individual *individual) {
    int total = 0;
    for (int i = 0; i < 7; i++) {
        total += individual->weekly_step_count[i];
    }
    return total;
}

TotalStepCount *insertSorted(TotalStepCount *head,int id, const char *name, int total_steps) {
    TotalStepCount *newNode = (TotalStepCount *)malloc(sizeof(TotalStepCount));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newNode->id=id;
    strcpy(newNode->name, name);
    newNode->total_steps = total_steps;

    if (head == NULL || total_steps > head->total_steps) {
        newNode->next = head;
        head = newNode;
    } else {
        TotalStepCount *current = head;
        while (current->next != NULL && total_steps <= current->next->total_steps) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    return head;
}


void Get_top_3(Node_Individual *node) {
    TotalStepCount *totalStepCountHead = NULL;

    while (node != NULL) {
        int allGreater = 1;
        for (int i = 0; i < 7; i++) {
            if (node->weekly_step_count[i] <= node->daily_goal) {
                allGreater = 0;
                break;
            }
        }

        if (allGreater) {
            int total_steps = calculateTotalSteps(node);
            totalStepCountHead = insertSorted(totalStepCountHead, node->id, node->name, total_steps);
        }

        node = node->next;
    }

    TotalStepCount *current = totalStepCountHead;
    int count = 0;

    while (current != NULL && count < 3) {
        printf("ID: %d, Name: %s, Total Steps: %d\n", current->id ,current->name, current->total_steps);
        current = current->next;
        count++;
    }
    if(count<3)
    {
        printf("only these individuals completed their daily step goals!\n");
    }
    while (totalStepCountHead != NULL) {
        TotalStepCount *temp = totalStepCountHead;
        totalStepCountHead = totalStepCountHead->next;
        free(temp);
    }
}

void Check_group_achievement(int group_id, Node_Individual *individualList, Node_Group *groupList) {
    int totalWeeklyCount = 0;


    Node_Group *currentGroup = groupList;
    while (currentGroup != NULL && currentGroup->group_id != group_id) {
        currentGroup = currentGroup->next;
    }

    if (currentGroup != NULL) {
        for (int i = 0; i < currentGroup->group_size; i++) {
            int memberId = currentGroup->individuals[i]->id;
            Node_Individual* current=currentGroup->individuals[i];
            if (current != NULL) {
                int individualWeeklyCount = calculateTotalSteps(current);
                totalWeeklyCount += individualWeeklyCount;

                printf("Member: %s, Weekly Count: %d\n", current->name, individualWeeklyCount);
            } else {
                printf("Member with ID %d not found.\n", memberId);
            }
        }

        if (totalWeeklyCount >= currentGroup->Weekly_Group_Goal) {
            printf("Weekly goal met for Group %d!\n", currentGroup->group_id);
        } else {
            printf("Weekly goal not met for Group %d.\n", currentGroup->group_id);
        }
    } else {
        printf("Group with ID %d not found.\n", group_id);
    }
}



LeaderboardNode *insertSorted2(LeaderboardNode *head, const char *group_name, int total_steps) {
    LeaderboardNode *newNode = (LeaderboardNode *)malloc(sizeof(LeaderboardNode));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->group_name, group_name);
    newNode->total_steps = total_steps;

    if (head == NULL || total_steps > head->total_steps) {
        newNode->next = head;
        head = newNode;
    } else {
        LeaderboardNode *current = head;
        while (current->next != NULL && total_steps <= current->next->total_steps) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    return head;
}

void Generate_leader_board(Node_Group *groupList, Node_Individual *individualList) {
    LeaderboardNode *leaderboardHead = NULL;

    while (groupList != NULL) {
        int totalGroupSteps = 0;

        for (int i = 0; i < groupList->group_size; i++) {
            int memberId = groupList->individuals[i]->id;

            Node_Individual *current = individualList;
            while (current != NULL && current->id != memberId) {
                current = current->next;
            }

            if (current != NULL) {
                int individualWeeklyCount = calculateTotalSteps(current);
                totalGroupSteps += individualWeeklyCount;
            } else {
                printf("Member with ID %d not found.\n", memberId);
            }
        }

        leaderboardHead = insertSorted2(leaderboardHead, groupList->group_name, totalGroupSteps);

        groupList = groupList->next;
    }

    LeaderboardNode *currentLeader = leaderboardHead;

    while (currentLeader != NULL) {
        printf("Group Name: %s, Total Steps: %d\n", currentLeader->group_name, currentLeader->total_steps);
        currentLeader = currentLeader->next;
    }

    while (leaderboardHead != NULL) {
        LeaderboardNode *temp = leaderboardHead;
        leaderboardHead = leaderboardHead->next;
        free(temp);
    }
}

LeaderboardNode * Get_leader_head(Node_Group *groupList, Node_Individual *individualList) {
    LeaderboardNode *leaderboardHead = NULL;

    while (groupList != NULL) {
        int totalGroupSteps = 0;

        for (int i = 0; i < groupList->group_size; i++) {
            int memberId = groupList->individuals[i]->id;

            Node_Individual *current = individualList;
            while (current != NULL && current->id != memberId) {
                current = current->next;
            }

            if (current != NULL) {
                int individualWeeklyCount = calculateTotalSteps(current);
                totalGroupSteps += individualWeeklyCount;
            } else {
                printf("Member with ID %d not found.\n", memberId);
            }
        }

        leaderboardHead = insertSorted2(leaderboardHead, groupList->group_name, totalGroupSteps);

        groupList = groupList->next;
    }

return leaderboardHead;
}

int Get_Rank(LeaderboardNode *leaderboardHead, const char *group_name) {
    LeaderboardNode *currentLeader = leaderboardHead;
    int rank = 1;

    while (currentLeader != NULL) {
        if (strcmp(currentLeader->group_name, group_name) == 0) {
            return rank;
        }

        rank++;
        currentLeader = currentLeader->next;
    }

    // Return -1 or any other value to indicate that the group name was not found
    return -1;
}

void Check_individual_rewards(int id,Node_Individual * node){
 TotalStepCount *totalStepCountHead = NULL;

    while (node != NULL) {
        int allGreater = 1;
        for (int i = 0; i < 7; i++) {
            if (node->weekly_step_count[i] <= node->daily_goal) {
                allGreater = 0;
                break;
            }
        }

        if (allGreater) {
            int total_steps = calculateTotalSteps(node);
            totalStepCountHead = insertSorted(totalStepCountHead,node->id, node->name, total_steps);
        }

        node = node->next;
    }

    TotalStepCount *current = totalStepCountHead;
    int count = 1;

    while (current != NULL && count <= 3) {
        if(id==current->id){
        printf("Name: %s, Total Steps: %d\n", current->name, current->total_steps);
        printf("Rank=%d\n",count);
        if(count==1){
            printf("100 points\n");
        }
        else if(count==2){
            printf("75 points\n");
        }
        else if(count==3){
            printf("50 points\n");
        }
        break;
        }
        current = current->next;
        count++;
    }
    if(count>3){
        printf("Sorry!,You're not in Top 3:\n");
    }

    while (totalStepCountHead != NULL) {
        TotalStepCount *temp = totalStepCountHead;
        totalStepCountHead = totalStepCountHead->next;
        free(temp);
    }


}

void deleteFromNodeGroup(Node_Group *group, Node_Individual *individualList, int idToDelete) {
    int indexToDelete = -1;
    for (int i = 0; i < group->group_size; i++) {
        if (group->individuals[i]->id == idToDelete) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete != -1) {
        for (int i = indexToDelete; i < group->group_size - 1; i++) {
            group->individuals[i] = group->individuals[i + 1];
        }
        group->group_size--;

        Node_Individual *current = individualList;
        while (current != NULL && current->id != idToDelete) {
            current = current->next;
        }

        if (current != NULL) {
            current->ingroup = 0;
        }

    } 
}

Node_Individual *Delete_individual(Node_Individual *headIndividual, Node_Group *headGroup, int idToDelete) {
    Node_Group *currentGroup = headGroup;
    while (currentGroup != NULL) {
        deleteFromNodeGroup(currentGroup,headIndividual, idToDelete);
        currentGroup = currentGroup->next;
    }

    Node_Individual *prev=NULL;
    Node_Individual *curr=headIndividual;

    while(curr->id!=idToDelete){
        prev=curr;
        curr=curr->next;
    }

    if(prev==NULL){
        Node_Individual *temp=prev;
        headIndividual=headIndividual->next;
        free(temp);
    }
    else{
        prev->next=curr->next;
        free(curr);
    }

    FILE *file = fopen("dspd_file_individual.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    writeToFile(file, headIndividual);
    writeGroupToFile(headGroup);

    return headIndividual;
}

Node_Group *Delete_group(Node_Individual *headIndividual, Node_Group *headGroup, int group_id) {
    Node_Group *prevGroup = NULL;
    Node_Group *currentGroup = headGroup;


    while (currentGroup != NULL && currentGroup->group_id != group_id) {
        prevGroup = currentGroup;
        currentGroup = currentGroup->next;
    }


    if (currentGroup != NULL) {
        for (int i = 0; i < currentGroup->group_size; i++) {
            int memberId = currentGroup->individuals[i]->id;

            Node_Individual *currentIndividual = headIndividual;
            while (currentIndividual != NULL && currentIndividual->id != memberId) {
                currentIndividual = currentIndividual->next;
            }

            if (currentIndividual != NULL) {
                currentIndividual->ingroup = 0;
            }
        }

        if (prevGroup == NULL) {
            headGroup = currentGroup->next;
        } else {
            prevGroup->next = currentGroup->next;
        }

        printf("Deleted group with ID %d.\n", group_id);
        free(currentGroup);
    } else {
        printf("Group with ID %d not found.\n", group_id);
    }

    writeGroupToFile(headGroup);
    return headGroup;
}

void Display_group_info(Node_Group *groupList, Node_Individual *individualList, int group_id) {
    Node_Group *currentGroup = groupList;
    while (currentGroup != NULL && currentGroup->group_id != group_id) {
        currentGroup = currentGroup->next;
    }

    if (currentGroup != NULL) {
        printf("Group ID: %d\n", currentGroup->group_id);
        printf("Group Name: %s\n", currentGroup->group_name);
        printf("Group Size: %d\n", currentGroup->group_size);
        printf("Weekly Group Goal: %d\n", currentGroup->Weekly_Group_Goal);

        printf("\nMembers in the group:\n");
        for (int i = 0; i < currentGroup->group_size; i++) {
            int memberId = currentGroup->individuals[i]->id;

            Node_Individual *currentIndividual = individualList;
            while (currentIndividual != NULL && currentIndividual->id != memberId) {
                currentIndividual = currentIndividual->next;
            }

            if (currentIndividual != NULL) {
                printf("Member ID: %d\n", currentIndividual->id);
                printf("Name: %s\n", currentIndividual->name);
                printf("Age: %d\n", currentIndividual->age);
                printf("Daily Goal: %d\n", currentIndividual->daily_goal);
                printf("Weekly Step Count: ");
                for (int j = 0; j < 7; j++) {
                    printf("%d ", currentIndividual->weekly_step_count[j]);
                }
                printf("\n");
            }
        }

        
        printf("\nGroup Rank Information:\n");
        LeaderboardNode * node=Get_leader_head(groupList,individualList);
        int a=Get_Rank(node,currentGroup->group_name);
        printf("%d",a);
    } else {
        printf("Group with ID %d not found.\n", group_id);
    }
}

void Suggest_Daily_Goal_Top3(Node_Individual *node) {
    TotalStepCount *totalStepCountHead = NULL;

    while (node != NULL) {
        int allGreater = 1;
        for (int i = 0; i < 7; i++) {
            if (node->weekly_step_count[i] <= node->daily_goal) {
                allGreater = 0;
                break;
            }
        }

        if (allGreater) {
            int total_steps = calculateTotalSteps(node);
            totalStepCountHead = insertSorted(totalStepCountHead, node->id, node->name, total_steps);
        }

        node = node->next;
    }

    TotalStepCount *current = totalStepCountHead;
    int count = 0;
    int totalTop3Steps = 0;

    while (current != NULL && count < 3) {
        totalTop3Steps += current->total_steps;
        current = current->next;
        count++;
    }

    int suggestedDailyGoal = (totalTop3Steps / 3) / 7;

    printf("Suggested Daily Goal for appearing in the top 3: %d\n", suggestedDailyGoal);


    while (totalStepCountHead != NULL) {
        TotalStepCount *temp = totalStepCountHead;
        totalStepCountHead = totalStepCountHead->next;
        free(temp);
    }
}
Node_Group* Merge_groups(Node_Group* groupList, int Group_ID_1, int Group_ID_2) {
    Node_Group *group1 = NULL;
    Node_Group *group2 = NULL;
    Node_Group *head = groupList;
    Node_Group* ans = groupList;

    while (groupList != NULL) {
        if (groupList->group_id == Group_ID_1) {
            group1 = groupList;
        } else if (groupList->group_id == Group_ID_2) {
            group2 = groupList;
        }

        groupList = groupList->next;
    }

    if (group1 == NULL || group2 == NULL) {
        printf("One or both of the specified groups not found.\n");
        return ans;
    }

    Node_Group *newGroup = (Node_Group *)malloc(sizeof(Node_Group));
    if (newGroup == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newGroup->group_id = Group_ID_1;
    newGroup->group_size = group1->group_size + group2->group_size;

    if (newGroup->group_size > 5) {
        printf("Error: Merged group exceeds the maximum allowed size.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(newGroup->group_name, sizeof(newGroup->group_name), "%s%s", group1->group_name, group2->group_name);

    newGroup->Weekly_Group_Goal = group1->Weekly_Group_Goal + group2->Weekly_Group_Goal;

    newGroup->individuals = (Node_Individual **)malloc(newGroup->group_size * sizeof(Node_Individual *));
    if (newGroup->individuals == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < group1->group_size; i++) {
        newGroup->individuals[i] = group1->individuals[i];
    }

    for (int j = 0; j < group2->group_size; j++, i++) {
        newGroup->individuals[i] = group2->individuals[j];
    }

    newGroup->next = NULL;

    if (group1 == ans) {
        ans = newGroup;
    } else {
        while (head->next != group1) {
            head = head->next;
        }
        head->next = newGroup;
    }

    while (head->next != group2) {
        head = head->next;
    }

    head->next = group2->next;

    writeGroupToFile(ans);

    printf("Groups merged successfully.\n");

    return ans;
}



int main() {
    FILE *file = fopen("dspd_file_individual.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

   Node_Individual * headIndividual=readFromIndividualFile(file,headIndividual);


    FILE *file2 = fopen("dspd_file_group.txt", "r");
    if (!file2) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    Node_Group *headGroup = readFromGroupFile(file2,file,headIndividual,headGroup);

    
  
    printf("Group list is as follows:\n");
    printGroup(headGroup);
    

    printf("Individuals are as follows:\n");
    printIndividualList(headIndividual);
     
        int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add Person\n");
        printf("2. Create Group\n");
        printf("3. Get Top 3 Individuals\n");
        printf("4. Check Group Achievement\n");
        printf("5. Generate Leaderboard\n");
        printf("6. Check Individual Rewards\n");
        printf("7. Delete Individual\n");
        printf("8. Delete Group\n");
        printf("9. Merge Groups\n");
        printf("10. Display Group Info\n");
        printf("11. Suggest Goal Update\n");
        printf("12. Print Individual Info:\n");
        printf("13. Print Group Info\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:{
                headIndividual = Add_Person(headIndividual);
                break;
            }
            case 2:{
                headGroup = Create_Group(headGroup, headIndividual);
                break;
            }
            case 3:{
                Get_top_3(headIndividual);
                break;
            }
            case 4:{
                printf("Enter group_id:\n");
                int g;
                scanf("%d",&g);
                Check_group_achievement(g,headIndividual,headGroup);
                break;
            }
            case 5:{
                Generate_leader_board(headGroup, headIndividual);
                break;
            }
            case 6:{
                int a;
                printf("Enter individual ID: ");
                scanf("%d", &a);
                Check_individual_rewards(a,headIndividual);
                break;
            }
            case 7:{
                
                int idToDeleteindividual;
                printf("Enter individual ID to delete: ");
                scanf("%d", &idToDeleteindividual);
                headIndividual = Delete_individual(headIndividual, headGroup, idToDeleteindividual);
                break;
            }
            case 8:{
                int idToDeletegroup;
                printf("Enter group ID to delete: ");
                scanf("%d", &idToDeletegroup);
                headGroup = Delete_group(headIndividual,headGroup,idToDeletegroup);
                break;
            }
            case 9:{
                int groupID1, groupID2;
                printf("Enter first group ID to merge: ");
                scanf("%d", &groupID1);
                printf("Enter second group ID to merge: ");
                scanf("%d", &groupID2);
                Merge_groups(headGroup, groupID1, groupID2);
                break;
            }
            case 10:{
                int groupIDToDisplayInfo;
                printf("Enter group ID to display info: ");
                scanf("%d", &groupIDToDisplayInfo);
                Display_group_info(headGroup, headIndividual, groupIDToDisplayInfo);
                break;
            }
            case 11:{
                Suggest_Daily_Goal_Top3(headIndividual);
                break;
            }
            case 12:{
                 printf("Individuals are as follows:\n");
                 printIndividualList(headIndividual);
                 break;
            }
            case 13:{
                printf("Group list is as follows:\n");
                printGroup(headGroup);
                break;
            }
            case 0:{
                printf("Exiting program.\n");
                break;
            }
            default:{
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }

    } while (choice != 0);


    return 0;
}

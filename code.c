// Name : Lara Saleh Abu Asfour
// ID : 1221484
// Section Number : 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 50

int islistsorted = 0;

// naming for pointers (linked lists)
typedef struct TownList* TownPTR;
typedef struct DistrictList* DistrictPTR;

// struct to contain a town information
struct Town {
    char name[MAX_LEN];
    int population;
};

// struct to contain a town linked list
struct TownList {
    struct Town town;
    TownPTR next;
};

// struct to contain a district information
struct District {
    char name[MAX_LEN];
    TownPTR towns;
};


// struct to contain a district linked list
struct DistrictList {
    struct District district;
    DistrictPTR next;
    DistrictPTR prev;
};

// returns a town node (linked list with one node only and next = NULL)
TownPTR createTown(char *name, int population) {
    TownPTR newTown = (TownPTR)malloc(sizeof(struct TownList));
    if (!newTown) {
        printf("Memory allocation failed for town\n");
        return NULL;
    }

    strncpy(newTown->town.name, name, MAX_LEN - 1);
    newTown->town.name[MAX_LEN - 1] = '\0';
    newTown->town.population = population;
    newTown->next = NULL;
    return newTown;
}


// returns a district node (linked list with one node only and next = NULL)
DistrictPTR createDistrict(char *name) {
    DistrictPTR newDistrict = (DistrictPTR)malloc(sizeof(struct DistrictList));
    if (!newDistrict) {
        printf("Memory allocation failed for district\n");
        return NULL;
    }

    strncpy(newDistrict->district.name, name, MAX_LEN - 1);
    newDistrict->district.name[MAX_LEN - 1] = '\0';
    newDistrict->district.towns = NULL;
    newDistrict->next = NULL;
    newDistrict->prev = NULL;
    return newDistrict;
}

// insertion of new town in the towns linked list of the desired district
void insertTown(DistrictPTR district, TownPTR town) {
    if (!district || !town) {
        printf("Invalid district or town\n");
        return;
    }

    if (district->district.towns == NULL) {
        district->district.towns = town;
    } else {
        TownPTR temp = district->district.towns;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = town;
    }
}

// insertion of a new district in the double linked list of the districts
void insertDistrict(DistrictPTR *head, DistrictPTR district) {
    if (!district) {
        printf("Invalid district\n");
        return;
    }
    if (*head == NULL) {
        *head = district;
    } else {
        DistrictPTR temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = district;
        district->prev = temp;
    }
}


// searches for district node by name and returns the node if found, or NULL if not found
DistrictPTR SearchForDistrict(DistrictPTR head, char *name) {
    DistrictPTR temp = head;
    while (temp != NULL) {
        if (strcmp(temp->district.name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// searches for town node by name and returns the node if found, or NULL if not found
TownPTR SearchForTown(DistrictPTR head, char *name) {
    DistrictPTR temp = head;
    while (temp != NULL) {
        TownPTR currentTown = temp->district.towns;
        while (currentTown != NULL) {
            if (strcmp(currentTown->town.name, name) == 0) {
                return currentTown;
            }
            currentTown = currentTown->next;
        }
        temp = temp->next;
    }
    return NULL;
}

// to remove last space character from a string
void removeSpaces(char* str) {
    int index, i;
    index = -1;
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }
        i++;
    }
    str[index + 1] = '\0';
}


// to print the districts in a format like the file format
void PrintFileFormat(DistrictPTR head) {
    if(head == NULL) {
        printf("The list of districts is empty, Please load the data from a file.\n");
        return;
    }

    DistrictPTR currentDistrict = head;
    while(currentDistrict != NULL) {
        TownPTR t = currentDistrict->district.towns;
        while(t != NULL) {
            printf("%s|%s|%d\n", currentDistrict->district.name, t->town.name, t->town.population);
            t = t->next;
        }
        currentDistrict = currentDistrict->next;
    }
}

// to print the districts and towns in another format with giving more information
void printDistricts(DistrictPTR head) {
     if (head == NULL) {
        printf("The list of districts is empty, Please load the data from a file.\n");
        return;
    }

    DistrictPTR currentDistrict = head;
    int districtCount = 0;
    int totalTowns = 0;

    while (currentDistrict != NULL) {
        districtCount++;
        printf("District %d: '%s'\n", districtCount, currentDistrict->district.name);

        int townCount = 0;

        TownPTR currentTown = currentDistrict->district.towns;
        if (currentTown == NULL) {
            printf("- Currently, there are no towns registered in this district.\n");
        }

        while (currentTown != NULL) {
            townCount++;
            totalTowns++;
            printf("- Town %d: '%s', with a population of %d.\n", townCount, currentTown->town.name, currentTown->town.population);
            currentTown = currentTown->next;
        }

        printf("\n");
        currentDistrict = currentDistrict->next;
    }

    printf("Summary:\n");
    printf("Total districts: %d\n", districtCount);
    printf("Total towns: %d\n", totalTowns);
    printf("\n");
}

// findmax for radix sort
int FindMaxDistrictNameLength(DistrictPTR head) {
    int MaximumLength = 0;
    while (head != NULL) {
        int len = strlen(head->district.name);
        if (len > MaximumLength) {
            MaximumLength = len;
        }
        head = head->next;
    }
    return MaximumLength;
}


// counting sort for radix sort
void CountingSortByCharacter(DistrictPTR* head, int exp, int MaximumLength, int listSize) {
    DistrictPTR output[listSize];
    int count[256] = {0};

    DistrictPTR current = *head;
    for (int i = 0; current != NULL; i++) {
        int charIndex = MaximumLength - exp - 1;
        char ch = charIndex < strlen(current->district.name) ? current->district.name[charIndex] : 0;
        count[(unsigned char)ch]++;
        current = current->next;
    }

    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    for (int i = listSize - 1; i >= 0; i--) {
        current = *head;
        for (int j = 0; j < i; j++) current = current->next;

        int charIndex = MaximumLength - exp - 1;
        char ch = charIndex < strlen(current->district.name) ? current->district.name[charIndex] : 0;
        output[count[(unsigned char)ch] - 1] = current;
        count[(unsigned char)ch]--;
    }

    for (int i = 0; i < listSize; i++) {
        if (i < listSize - 1) output[i]->next = output[i + 1];
        if (i > 0) output[i]->prev = output[i - 1];
    }
    output[0]->prev = NULL;
    output[listSize - 1]->next = NULL;
    *head = output[0];
}

// radix sort function
void RadixSortDistricts(DistrictPTR* head) {
    if (head == NULL || (*head) == NULL) {
        printf("List is empty and nothing is performed\n");
        return;
    }

    int MaximumLength = FindMaxDistrictNameLength(*head);
    int listSize = 0;
    DistrictPTR current = *head;

    while (current != NULL) {
        listSize++;
        current = current->next;
    }

    for (int exp = 0; exp < MaximumLength; exp++) {
        CountingSortByCharacter(head, exp, MaximumLength, listSize);
    }
}

// to swap two towns in a linked list
void SwapTowns(TownPTR a, TownPTR b) {
    struct Town temp = a->town;
    a->town = b->town;
    b->town = temp;
}

// sorting towns based on population once
void SortTowns(TownPTR* headRef) {
    int swapped;
    TownPTR ptr1;
    TownPTR lptr = NULL;

    if (*headRef == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = *headRef;

        while (ptr1->next != lptr) {
            if (ptr1->town.population > ptr1->next->town.population) {
                SwapTowns(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

// looping through towns and sorting based on population
void SortTownsInAllDistricts(DistrictPTR head) {
    DistrictPTR currentDistrict = head;
    while (currentDistrict != NULL) {
        SortTowns(&(currentDistrict->district.towns));
        currentDistrict = currentDistrict->next;
    }
}

// to read array of characters from user, this way I used to prevent the problems coming from stdin while reading array of chars
void ReadChar(char* input, int length) {
    fflush(stdin);
    fgets(input, length, stdin);
    fflush(stdin);
    strtok(input, "\n");
}

// to change population for a specific town
void ChangePopulation(DistrictPTR head) {
    char name[MAX_LEN];

    printf("District Name: ");
    ReadChar(name, MAX_LEN);

    DistrictPTR district = SearchForDistrict(head, name);

    if(district == NULL) {
        printf("This district does not exist\n\n");
        return;
    }

    char town[MAX_LEN];
    printf("Town Name: ");
    ReadChar(town, MAX_LEN);

    TownPTR townNode = SearchForTown(district, town);

    if(townNode == NULL) {
        printf("This town does not exist\n\n");
        return;
    }

    int newpopulation = 0;
    printf("New population: ");
    scanf("%d", &newpopulation);

    if(newpopulation < 0) {
        printf("Population can't be less than 0\n\n");
        return;
    }

    townNode->town.population = newpopulation;
    printf("Population changed successfully\n\n");
}


// calculate population for palestine, find min and max population towns
void Calculate(DistrictPTR head) {
    int population = 0;
    int min = head->district.towns->town.population;
    int max = head->district.towns->town.population;
    char MinTown[MAX_LEN], MaxTown[MAX_LEN];
    strcpy(MinTown, head->district.towns->town.name);
    strcpy(MaxTown, head->district.towns->town.name);

    DistrictPTR temp = head;
    while(temp != NULL) {
        TownPTR towns = temp->district.towns;
        while(towns != NULL) {
            population += towns->town.population;
            if(towns->town.population < min) {
                min = towns->town.population;
                strcpy(MinTown, towns->town.name);
            }

            if(towns->town.population > max) {
                max = towns->town.population;
                strcpy(MaxTown, towns->town.name);
            }
            towns = towns->next;
        }
        temp = temp->next;
    }
    printf("\n");
    printf("Palestine population is: %d\n", population);
    printf("Minimum population is %d, for town %s\n", min, MinTown);
    printf("Maximum population is %d, for town %s\n\n", max, MaxTown);
}

// calculate population for a given district
int CalculatePopulation(DistrictPTR district) {
    TownPTR towns = district->district.towns;
    int population = 0;
    while(towns != NULL) {
        population+= towns->town.population;
        towns = towns->next;
    }
    return population;
}

// loop through districts and print calculated population for each one
void CalculateDistrictsOnly(DistrictPTR head) {
    DistrictPTR temp = head;
    while(temp != NULL) {
        printf("%s District: %d\n", temp->district.name, CalculatePopulation(temp));
        temp = temp->next;
    }
}

// load data from file
void LoadInputFile(DistrictPTR *palestineDistricts, char *filename) {
    FILE *file = fopen("districts.txt", "r");
    if (!file) {
        printf("Could not open file %s for reading.\n", "districts.txt");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *districtName = strtok(line, "|");
        char *townName = strtok(NULL, "|");
        char *populationStr = strtok(NULL, "|");

        if (districtName != NULL && townName != NULL && populationStr != NULL) {
            removeSpaces(districtName);
            removeSpaces(townName);
            removeSpaces(populationStr);
            int population = atoi(populationStr);

            DistrictPTR district = SearchForDistrict(*palestineDistricts, districtName);
            if (district == NULL) {
                district = createDistrict(districtName);
                insertDistrict(palestineDistricts, district);
            }

            TownPTR town = createTown(townName, population);
            insertTown(district, town);
        }
    }

    fclose(file);
}

// delete a town from the linked list
void DeleteTown(TownPTR* head, TownPTR position) {
    if(*head == position) {
        TownPTR temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    TownPTR temp = *head;
    while(temp->next != position) {
        temp = temp->next;
    }

    if(temp != NULL) {
        temp->next = position->next;
        free(position);
    }
}

// delete a district from the double linked list
void DeleteDistrict(DistrictPTR* head, DistrictPTR district) {
    if(*head == district) {
        DistrictPTR temp = *head;
        *head = (*head)->next;
        (*head)->prev = NULL;
        free(temp);
        return;
    }

    DistrictPTR temp = *head;
    while(temp->next != district) {
        temp = temp->next;
    }

    if(temp != NULL) {
        temp->next = district->next;
        DistrictPTR next = temp->next;
        next->prev = temp;
        free(district);
    }
}

// add new district from user
void AddNewDistrict(DistrictPTR* head) {
    char name[MAX_LEN];
    printf("Enter the full district name please: ");

    ReadChar(name, MAX_LEN);

    DistrictPTR district = SearchForDistrict(*head, name);
    if(district != NULL) {
        printf("\nThis district exists\n\n");
        return;
    }
    else
    {
        district = createDistrict(name);
        insertDistrict(head, district);
        printf("\nInserted %s successfully\n", name);
    }
}


// add new town from user
void AddNewTown(DistrictPTR head) {
    char name[MAX_LEN];
    printf("District name: ");
    ReadChar(name, MAX_LEN);

    DistrictPTR district = SearchForDistrict(head, name);

    if(district == NULL) {
        printf("\nDistrict does not exist\n\n");
        return;
    }
    else {
        char Town[MAX_LEN];
        printf("New Town name: ");
        ReadChar(Town, MAX_LEN);
        if(SearchForTown(district, Town) != NULL) {
            printf("This town exists\n\n");
            return;
        } else {
            int population = 0;
            printf("Population: ");
            scanf("%d", &population);
            if(population < 0)
            {
                printf("Population can't be less than 0\n\n");
                return;
            }
            insertTown(district, createTown(Town, population));
            printf("Town %s with population %d inserted in %s district\n\n", Town, population, name);
        }
    }
}

// delete town from a specific district
void DeleteTownFromDistrict(DistrictPTR head) {
    char name[MAX_LEN];
    char town[MAX_LEN];

    printf("District Name: ");
    ReadChar(name, MAX_LEN);
    DistrictPTR district = SearchForDistrict(head, name);
    if(district == NULL) {
        printf("\nDistrict does not exist\n\n");
        return;
    }
    printf("\nTown Name: ");
    ReadChar(town, MAX_LEN);
    TownPTR townNode = SearchForTown(district, town);
    if(townNode == NULL) {
        printf("\nTown does not exist\n\n");
        return;
    }

    DeleteTown(&(district->district.towns), townNode);
}

// delete a towns linked list
void destroy(TownPTR* head) {
    TownPTR current = *head;
    TownPTR next;

    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

// to delete a full district with it's towns list
void DeleteCompleteDistrict(DistrictPTR* head) {
    char name[MAX_LEN];

    printf("District Name: ");
    ReadChar(name, MAX_LEN);

    DistrictPTR district = SearchForDistrict(*head, name);

    if(district == NULL) {
        printf("\nThis district does not exist\n\n");
        return;
    }

    destroy(&(district->district.towns));
    DeleteDistrict(head, district);
    printf("\nDistrict %s is deleted\n\n", name);
}


// print districts data to output file
void SaveToOutputFile(DistrictPTR head) {
    char filename[MAX_LEN];
    ReadChar(filename, MAX_LEN);

    FILE* file = fopen("sorted_districts.txt", "w");

    if(file == NULL){
        printf("Error opening the file\n\n");
        return;
    }

    DistrictPTR temp = head;

    while(temp != NULL) {
        fprintf(file, "%s District, Population = %d\n", temp->district.name, CalculatePopulation(temp));
        TownPTR towns = temp->district.towns;
        while(towns != NULL) {
            fprintf(file, "%s, %d\n", towns->town.name, towns->town.population);
            towns = towns->next;
        }
        temp = temp->next;
    }
}

int main()
{
    DistrictPTR palestineDistricts = NULL;
    char filename[MAX_LEN];
    int operation = 0;

    while(operation != 14) {
        printf("Please choose an operation\n");
        printf("1- Load the input file (Palestinian districts and their town with population).\n");
        printf("2- Print the loaded information before sorting (as in the input file format).\n");
        printf("3- Sort the districts alphabetically using Radix sort.\n");
        printf("4- Sort the towns for each district based on population in ascending order.\n");
        printf("5- Print the sorted information.\n");
        printf("6- Add a new district to the list of sorted districts (and sort the list).\n");
        printf("7- Add a new town to a certain district.\n");
        printf("8- Delete a town from a specific district.\n");
        printf("9- Delete a complete district; here, you must delete all towns belonging to that district before deleting it.\n");
        printf("10- Calculate the population of Palestine, the max and min town population.\n");
        printf("11- Print the districts and their total population (without towns details). \n");
        printf("12- Change the population of a town, after prompting the user to enter the district name.\n");
        printf("13- Save to output file.\n");
        printf("14- Exit.\n");

        scanf("%d", &operation);

        if(operation == 1)
        {
            LoadInputFile(&palestineDistricts, filename);
            printf("\n");
            continue;
        }

        if(operation == 2)
        {
            printf("\n");
            if(islistsorted == 1)
            {
                printf("The following data has been sorted by you, it will not have the same order from the first run\n");
            }
            PrintFileFormat(palestineDistricts);
            printf("\n");
            continue;
        }

        if(operation == 3)
        {
            RadixSortDistricts(&palestineDistricts);
            islistsorted = 1;
            printf("\nLinked list sorted successfully\n");
            continue;
        }

        if(operation == 4)
        {
            SortTownsInAllDistricts(palestineDistricts);
            islistsorted = 1;
            printf("\nTowns sorted successfully\n");
            continue;
        }

        if(operation == 5)
        {
            printf("\n");
            if(islistsorted)
                printDistricts(palestineDistricts);
            else
                printf("You haven't sorted the list yet. please sort it before reaching this option\n");
            printf("\n");
            continue;
        }

        if(operation == 6)
        {
            AddNewDistrict(&palestineDistricts);
            RadixSortDistricts(&palestineDistricts);
            islistsorted = 1;
            printf("\nList is sorted\n\n");
            continue;
        }

        if(operation == 7)
        {
            AddNewTown(palestineDistricts);
            continue;
        }

        if(operation == 8)
        {
            DeleteTownFromDistrict(palestineDistricts);
            continue;
        }

        if(operation == 9)
        {
            DeleteCompleteDistrict(&palestineDistricts);
            continue;
        }

        if(operation == 10)
        {
            Calculate(palestineDistricts);
            continue;
        }

        if(operation == 11)
        {
            printf("\n");
            CalculateDistrictsOnly(palestineDistricts);
            printf("\n");
            continue;
        }

        if(operation == 12)
        {
            ChangePopulation(palestineDistricts);
            continue;
        }

        if(operation == 13)
        {
            SaveToOutputFile(palestineDistricts);
            printf("Saved Successfully\n\n");
            continue;
        }
        if(operation != 14)
            printf("No operation related to number '%d', please choose a valid operation number between (1-14)\n", operation);
    }

    return 0;
}


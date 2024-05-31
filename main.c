#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
} FoodItem;

void loadRecords(FoodItem records[], int *count);
void saveRecords(FoodItem records[], int count);
void viewRecords(FoodItem records[], int count);
void addRecord(FoodItem records[], int *count);
void searchRecords(FoodItem records[], int count);
void updateRecord(FoodItem records[], int count);
void deleteRecord(FoodItem records[], int *count);
int findRecordById(FoodItem records[], int count, int id);

int main() {
    FoodItem records[MAX_RECORDS];
    int count = 0;
    int choice;

    loadRecords(records, &count);

    do {
        printf("\nFood Court Management System\n");
        printf("1. View Records\n");
        printf("2. Add Record\n");
        printf("3. Search Records\n");
        printf("4. Update Record\n");
        printf("5. Delete Record\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid choice. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                viewRecords(records, count);
                break;
            case 2:
                addRecord(records, &count);
                break;
            case 3:
                searchRecords(records, count);
                break;
            case 4:
                updateRecord(records, count);
                break;
            case 5:
                deleteRecord(records, &count);
                break;
            case 6:
                saveRecords(records, count);
                printf("Records saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void loadRecords(FoodItem records[], int *count) {
    FILE *file = fopen("foodcourt.dat", "rb");
    if (file == NULL) {
        printf("No existing records found. Starting fresh.\n");
        return;
    }
    if (fread(count, sizeof(int), 1, file) != 1) {
        printf("Error reading record count.\n");
        fclose(file);
        return;
    }
    if (fread(records, sizeof(FoodItem), *count, file) != (size_t)*count) {
        printf("Error reading records.\n");
    }
    fclose(file);
}

void saveRecords(FoodItem records[], int count) {
    FILE *file = fopen("foodcourt.dat", "wb");
    if (file == NULL) {
        printf("Error saving records.\n");
        return;
    }
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        printf("Error writing record count.\n");
    }
    if (fwrite(records, sizeof(FoodItem), count, file) != (size_t)count) {
        printf("Error writing records.\n");
    }
    fclose(file);
}

void viewRecords(FoodItem records[], int count) {
    printf("\nFood Items:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Price: $%.2f\n", records[i].id, records[i].name, records[i].price);
    }
}

void addRecord(FoodItem records[], int *count) {
    if (*count >= MAX_RECORDS) {
        printf("Cannot add more records. Storage full.\n");
        return;
    }

    FoodItem newItem;
    printf("Enter ID: ");
    if (scanf("%d", &newItem.id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input. Record not added.\n");
        return;
    }

    if (findRecordById(records, *count, newItem.id) != -1) {
        printf("ID already exists. Record not added.\n");
        return;
    }

    printf("Enter name: ");
    while (getchar() != '\n');
    if (fgets(newItem.name, NAME_LEN, stdin) == NULL) {
        printf("Error reading name. Record not added.\n");
        return;
    }
    newItem.name[strcspn(newItem.name, "\n")] = '\0';
    printf("Enter price: ");
    if (scanf("%f", &newItem.price) != 1 || newItem.price < 0) {
        while (getchar() != '\n');
        printf("Invalid price. Record not added.\n");
        return;
    }

    records[*count] = newItem;
    (*count)++;
    printf("Record added.\n");
}

void searchRecords(FoodItem records[], int count) {
    int id;
    printf("Enter ID to search: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input. Search aborted.\n");
        return;
    }
    int index = findRecordById(records, count, id);
    if (index != -1) {
        printf("ID: %d, Name: %s, Price: $%.2f\n", records[index].id, records[index].name, records[index].price);
    } else {
        printf("Record not found.\n");
    }
}

void updateRecord(FoodItem records[], int count) {
    int id;
    printf("Enter ID to update: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input. Update aborted.\n");
        return;
    }
    int index = findRecordById(records, count, id);
    if (index == -1) {
        printf("Record not found.\n");
        return;
    }

    printf("Enter new name: ");
    while (getchar() != '\n');
    if (fgets(records[index].name, NAME_LEN, stdin) == NULL) {
        printf("Error reading name. Update aborted.\n");
        return;
    }
    records[index].name[strcspn(records[index].name, "\n")] = '\0';

    printf("Enter new price: ");
    if (scanf("%f", &records[index].price) != 1 || records[index].price < 0) {
        while (getchar() != '\n');
        printf("Invalid price. Update aborted.\n");
        return;
    }

    printf("Record updated.\n");
}

void deleteRecord(FoodItem records[], int *count) {
    int id;
    printf("Enter ID to delete: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input. Deletion aborted.\n");
        return;
    }
    int index = findRecordById(records, *count, id);
    if (index == -1) {
        printf("Record not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        records[i] = records[i + 1];
    }
    (*count)--;
    printf("Record deleted.\n");
}

int findRecordById(FoodItem records[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (records[i].id == id) {
            return i;
        }
    }
    return -1;
}

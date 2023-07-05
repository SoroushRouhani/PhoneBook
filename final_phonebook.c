
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

struct person {
    char fullName[35];
    char address[50];
    long int mobileNo;
    char mail[100];
};

void menu();
void backToMenu();
void addRecord();
void listRecord();
void modifyRecord();
void deleteRecord();
void searchRecord();

int main() {
    menu();
    return 0;
}

void menu() {

    printf("\t\t**********WELCOME TO PHONEBOOK*************\n\n");
    printf("\t\t\t  MENU\t\t\n\n");
    printf("\t1. Add New   \t2. List   \t3. Exit  \n\t4. Modify \t5. Search\t6. Delete \n\n");

    switch (getchar()) {
        case '1': addRecord(); break;
        case '2': listRecord(); break;
        case '3': exit(0); break;
        case '4': modifyRecord(); break;
        case '5': searchRecord(); break;
        case '6': deleteRecord(); break;
        default:
            backToMenu();
    }
}

void backToMenu(){
	printf("\n\n-Please press ENTER key to return to the menu.");
	getchar();   // Wait for user input (ENTER key)
	system(CLEAR_SCREEN);
    menu();
}

void addRecord() {
    system(CLEAR_SCREEN);
	getchar();  // Read and discard the newline character
	
    FILE *file;
    struct person p;
    file = fopen("contacts_db", "ab+");
    
    printf("\n Enter Fullname: ");
    scanf("%34s", p.fullName);
    
    printf("\nEnter the address: ");
    scanf("%49s", p.address);
    
    printf("\nEnter phone no.:");
    scanf("%ld", &p.mobileNo);
    
    printf("\nEnter e-mail:");
    scanf("%99s", p.mail);
    getchar();  // Read and discard the newline character
	
    fwrite(&p, sizeof(p), 1, file);

    printf("\n-Record saved");

    fclose(file);
    backToMenu();
}

void listRecord() {
	system(CLEAR_SCREEN);
	getchar();  // Read and discard the newline character
	
    struct person p;
    FILE *file;
    file = fopen("contacts_db", "rb");
    if (file == NULL) {
        printf("\nFile opening error: No contact Has entered yet!");
		backToMenu();
    }
    while (fread(&p, sizeof(p), 1, file) == 1) {
        printf("\nRecord is: \n\n ");
        printf("\nFullname=%s \nAdress=%s \nMobileNo=%ld \nE-mail=%s",
               p.fullName, p.address, p.mobileNo, p.mail);
    }
    fclose(file);
	backToMenu();
}

void searchRecord() {
	system(CLEAR_SCREEN);
	getchar();  // Read and discard the newline character
	
    struct person p;
    FILE *file;
    char name[100];
    int found = 0;  // Flag to track if a matching record is found


    file = fopen("contacts_db", "rb");
    if (file == NULL) {
        printf("\nFile opening error: No contact has entered yet!");
        backToMenu();
    }
	
	printf("\nEnter the name of person to search: \n");
    scanf("%99s", name);
	getchar();   // Read and discard the newline character
    
    while (fread(&p, sizeof(p), 1, file) == 1) {
        if (strcmp(p.fullName, name) == 0) {    // 0 means the two strings are equal
            printf("\nDetail Information About %s", name);
            printf("\nFullname=%s \nAdress=%s \nMobileNo=%ld \nE-mail=%s",
                   p.fullName, p.address, p.mobileNo, p.mail);
            found = 1; // Set the flag to indicate a match is found
            break;     // No need to continue searching
        }
    }

    fclose(file);

    if (!found) {
            printf("There is no one with this name!");
    }

    backToMenu();
}

void deleteRecord() {
	system(CLEAR_SCREEN);
	getchar();  // Read and discard the newline character
	
    struct person p;
    FILE *f, *ft;
    int flag = 0; // Initialize flag to 0
    char name[100];
	
    f = fopen("contacts_db", "rb");
    if (f == NULL) {
        printf("\nFile opening error: No contact has entered yet!");
        backToMenu();
    } else {
        ft = fopen("contacts_temp", "wb+");
        if (ft == NULL) {
            printf("\nFile opening error: can not create teml file.");
        } else {
			printf("\nEnter contact's fullname:");
            scanf("%s", name);
			getchar();   // Read and discard the newline character
			
            while (fread(&p, sizeof(p), 1, f) == 1) {  // while!! it checks for each record
                if (strcmp(p.fullName, name) != 0){
					fwrite(&p, sizeof(p), 1, ft);  // the names do not match
				} else{
					flag = 1;   // if the names match, set just flag to 1
				}     
            }

            if (flag != 1) {    // We didn't have any match names
                printf("\nNo contact's record to delete.");
                remove("contacts_temp");
            } else {        // flag has changed to 1 >>> we had at least one match name
                fclose(f);     // Close the original file before deleting
                fclose(ft);    // Close the temporary file before renaming
                remove("contacts_db");
                rename("contacts_temp", "contacts_db");
                printf("\nRecord deleted successfully.");
            }
        }
    }
    backToMenu();
}

void modifyRecord() {
	system(CLEAR_SCREEN);
	getchar();  // Read and discard the newline character
	
    FILE *f;
    int flag = 0;
    struct person p, s;
    char name[50];
	
    f = fopen("contacts_db", "rb+");
    if (f == NULL) {
        printf("\nFile opening error: No contact has entered yet!");
        backToMenu();
    } else {
		
        printf("\nEnter contact's name to modify:\n");
		scanf("%s", name);
		getchar(); // Read and discard the newline character

        while (fread(&p, sizeof(p), 1, f) == 1) {
            if (strcmp(name, p.fullName) == 0) {
                printf("\nEnter name:");
                fgets(s.fullName, sizeof(s.fullName), stdin);
                s.fullName[strcspn(s.fullName, "\n")] = '\0';

                printf("\nEnter the address:");
                fgets(s.address, sizeof(s.address), stdin);
                s.address[strcspn(s.address, "\n")] = '\0';

                printf("\nEnter phone no:");
                scanf("%ld", &s.mobileNo);
                getchar(); // Read and discard the newline character

                printf("\nEnter e-mail:");
                fgets(s.mail, sizeof(s.mail), stdin);
                s.mail[strcspn(s.mail, "\n")] = '\0';

                fseek(f, -sizeof(p), SEEK_CUR);
                fwrite(&s, sizeof(p), 1, f);
                flag = 1;
                break;
            }
        }

        if (flag == 1) {
            printf("\nContact's data is modified");
        } else {
            printf("\nContact's data is not found");
        }
        fclose(f);
    }
    backToMenu();
}
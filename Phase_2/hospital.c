#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"

/**
 * The main entry point of the program, initializing data structures,
 * loading previously saved patient data, and managing the menu system.
 *
 * @return int Returns 0 upon successful program termination.
 */
int main()
{
    struct LinkedList patientList = {NULL, 0};
    int doctorSchedule[MAX_DAYS][MAX_SECTIONS_OF_DAY];
    char doctorNames[MAX_DOCTORS][MAX_CHAR_LENGTH];
    int doctorNum = 0;
    int input = 1;

    // Checking if the doctorSchedule cell is 0; if so, set it to -1 to indicate no doctor assigned
    for (int i = 0; i < MAX_DAYS; i++)
    {
        for (int j = 0; j < MAX_SECTIONS_OF_DAY; j++)
        {
            if (doctorSchedule[i][j] == 0)
            {
                doctorSchedule[i][j] = -1;
            }
        }
    }

    // Loads previously saved patients from file, if available
    loadSaveFile(&patientList);

    while (input != 6)
    {
        printf("\nMAIN MENU\n"
            "1. Add Patient Record\n"
            "2. Reporting and Analytics Menu\n"
            "3. Search Patient Menu\n"
            "4. Discharge Patient\n"
            "5. Manage Doctor Schedule\n"
            "6. Exit\n"
            "Please input your option:\n");

        scanf("%d", &input);
        getchar();

        if (input < 0 || input > 6)
        {
            printf("Please input a valid option\n");
            continue;
        }

        switch (input)
        {
        case 1:
            addPatient(&patientList);
            break;
        case 2:
            ReportsAndAnalyticsMenu(&patientList, doctorNames, doctorSchedule, doctorNum);
            break;
        case 3:
            searchPatientMenu(&patientList);
            break;
        case 4:
            dischargePatient(&patientList);
            break;
        case 5:
            manageDoctorShifts(doctorNames, doctorSchedule, &doctorNum);
            break;
        case 6:
            writeAllPatientsToSaveFile(&patientList);
            break;
        }
    }

    // Frees the memory allocated for each listNode and its patient data
    struct listNode* current = patientList.head;

    while (current != NULL)
    {
        struct listNode* nextNode = current->next;

        //These are all the data that we malloc'd so we are
        //now deallocating them with free
        free(current->patient->dischargeDate);
        free(current->patient->admissionDate);
        free(current->patient);
        free(current);

        current = nextNode;
    }
}

/**
 * Replaces all spaces in a string with underscores.
 *
 * @param string The string in which spaces will be replaced.
 */
void replaceSpacesWithUnderscores(char* string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == 0)
        {
            break;
        }

        if (string[i] == ' ')
        {
            string[i] = '_';
        }
    }
}

/**
 * Replaces all underscores in a string with spaces.
 *
 * @param string The string in which underscores will be replaced.
 */
void replaceUnderscoresWithSpaces(char* string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == 0)
        {
            break;
        }

        if (string[i] == '_')
        {
            string[i] = ' ';
        }
    }
}

/**
 * Loads patient data from 'saveFile.txt' into the linked list, if the file exists.
 * If the file doesn't exist, creates a new one instead.
 *
 * @param patientList Pointer to the LinkedList to populate with patient data.
 */
void loadSaveFile(struct LinkedList* patientList)
{
    FILE* fptr;

    fptr = fopen("saveFile.txt", "r");

    if (fptr == NULL)
    {
        fopen("saveFile.txt", "w");
    }
    else
    {
        int id;
        char name[50];
        int age;
        char diagnosis[50];
        int roomNumber;
        char admittedDateString[50];
        char dischargedDateString[10];

        fscanf(fptr, "%d %s %d %s %d %s %s",
               &id, name, &age, diagnosis, &roomNumber, admittedDateString, dischargedDateString);

        //Repeats until it hits the end of the file
        while (!feof(fptr))
        {
            struct Date* dischargedDate = NULL;
            struct Date* admittedDate = NULL;

            //Check if we have a discharged date in the line
            //If so, parse it and put it inside our dischargedDate Date pointer
            if (strcmp(dischargedDateString, "null") != 0)
            {
                parseDate(&dischargedDate, dischargedDateString);
            }

            //MUST have an admittedDateString to be parsed and put inside
            //admittedDate Date pointer
            parseDate(&admittedDate, admittedDateString);

            //Once all the data is ready, we can create a patient with the data fetched
            //from the file and append it to the linkedlist
            createPatientToList(patientList, id, name, age, diagnosis, roomNumber, admittedDate, dischargedDate);

            //Preparing the next line to be read
            fscanf(fptr, "%d %s %d %s %d %s %s",
                   &id, name, &age, diagnosis, &roomNumber, admittedDateString, dischargedDateString);
        }

        printf("Successfully loaded data\n");
        fclose(fptr);
    }
}

/**
 * Appends a SINGLE patient's data to 'saveFile.txt'.
 *
 * @param id Unique identifier for the patient.
 * @param name Name of the patient.
 * @param age Age of the patient.
 * @param diagnosis Patient's diagnosis or condition.
 * @param roomNumber The patient's assigned room number.
 * @param dateAdmitted Pointer to a Date struct for the admission date.
 */
void writePatientToSaveFile(const int id,
                            char name[50],
                            const int age,
                            char diagnosis[50],
                            const int roomNumber,
                            const struct Date* dateAdmitted)
{
    FILE* fptr = fopen("saveFile.txt", "a");
    char dateAdmittedString[50];

    dateToString(dateAdmitted, dateAdmittedString);

    //Ensures that the string are supposed to be connected so that
    //it can be properly parsed by spaces
    replaceSpacesWithUnderscores(name);
    replaceSpacesWithUnderscores(diagnosis);

    fprintf(fptr, "%d %s %d %s %d %s %s\n",
            id, name, age, diagnosis, roomNumber, dateAdmittedString, "null");

    fclose(fptr);
}

/**
 * Overwrites 'saveFile.txt' with ALL patients currently in the linked list.
 *
 * @param patientList Pointer to the LinkedList containing patient data to write.
 */
void writeAllPatientsToSaveFile(const struct LinkedList* patientList)
{
    FILE* fptr = fopen("saveFile.txt", "w");
    const struct listNode* currentNode = patientList->head;

    while (currentNode != NULL)
    {
        char admittedDateString[10] = "null";
        char dischargedDateString[10] = "null";
        char name[50];
        char diagnosis[50];

        //Convert from its day, month, and year property into a string date format (dd/mm/yyyy)
        dateToString(currentNode->patient->admissionDate, admittedDateString);
        dateToString(currentNode->patient->dischargeDate, dischargedDateString);

        strcpy(name, currentNode->patient->name);
        strcpy(diagnosis, currentNode->patient->diagnosis);

        //Ensures that the string are supposed to be connected so that
        //it can be properly parsed by spaces
        replaceSpacesWithUnderscores(name);
        replaceSpacesWithUnderscores(diagnosis);

        //Prints to the file
        fprintf(fptr, "%d %s %d %s %d %s %s\n",
                currentNode->patient->id, name,
                currentNode->patient->age, diagnosis,
                currentNode->patient->roomNumbers,
                admittedDateString,
                dischargedDateString);

        currentNode = currentNode->next;
    }

    fclose(fptr);
}

/**
 * Checks whether the specified ID already exists in the linked list.
 * Ensures unique Ids across all patients
 *
 * @param patientList Pointer to the LinkedList of patients.
 * @param id The patient ID to validate.
 * @return 1 if the ID is found, 0 otherwise.
 */
int validateId(const struct LinkedList* patientList,
               const int id)
{
    const struct listNode* current = patientList->head;

    while (current != NULL)
    {
        if (current->patient->id == id)
        {
            return 1;
        }

        current = current->next;
    }

    return 0;
}

/**
 * Prompts user input to add a new patient record, validating each field,
 * then creates and saves the new record.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void addPatient(struct LinkedList* patientList)
{
    int id;
    char name[50];
    int age;
    char diagnosis[50];
    int roomNumber;
    struct Date* admittedDate;

    int idValid = 0;
    int nameValid = 0;
    int ageValid = 0;
    int diagnosisValid = 0;
    int roomNumberValid = 0;

    //Id input
    while (idValid == 0)
    {
        printf("\nEnter in the patient Id:\n");
        scanf("%d", &id);
        getchar();

        if (id < 0 || validateId(patientList, id))
        {
            printf("\nInvalid input or Id is taken. Please try again\n");
            continue;
        }

        idValid = 1;
    }

    //Name input
    while (nameValid == 0)
    {
        printf("\nEnter in the patient name:\n");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline

        if (strlen(name) == 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        nameValid = 1;
    }

    //Age input
    while (ageValid == 0)
    {
        printf("\nEnter in the age of the patient:\n");
        scanf("%d", &age);
        getchar();

        if (age < 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        ageValid = 1;
    }

    //Diagnosis input
    while (diagnosisValid == 0)
    {
        printf("\nEnter in the diagnosis name:\n");
        fgets(diagnosis, 50, stdin);
        diagnosis[strcspn(diagnosis, "\n")] = 0;

        if (strlen(diagnosis) == 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        diagnosisValid = 1;
    }

    //Room number input
    while (roomNumberValid == 0)
    {
        printf("\nEnter in the room number:\n");
        scanf("%d", &roomNumber);
        getchar();

        if (roomNumber < 0)
        {
            printf("\nInvalid input. Please try again\n");
            continue;
        }

        roomNumberValid = 1;
    }

    admittedDate = malloc(sizeof(struct Date));
    if (admittedDate == NULL)
    {
        printf("Could not allocate memory for admittedDate when adding a patient\n");
        return;
    }

    promptDateInitialization(admittedDate);

    createPatientToList(patientList, id, name, age, diagnosis, roomNumber, admittedDate, NULL);
    writePatientToSaveFile(id, name, age, diagnosis, roomNumber, admittedDate);
}

/**
 * Inserts a new patient node in ascending order of ID into the patientList.
 *
 * @param patientList Pointer to the LinkedList.
 * @param id Unique identifier for the patient.
 * @param name Name of the patient.
 * @param age Age of the patient.
 * @param diagnosis Diagnosis or condition of the patient.
 * @param roomNumber The assigned room number.
 * @param admittedDate Pointer to the Date struct for admission.
 * @param dischargeDate Pointer to the Date struct for discharge (may be NULL).
 */
void createPatientToList(struct LinkedList* patientList,
                         int id,
                         char* name,
                         int age,
                         char* diagnosis,
                         int roomNumber,
                         struct Date* admittedDate,
                         struct Date* dischargeDate)
{
    struct listNode* createdNode = malloc(sizeof(struct listNode));

    if (createdNode == NULL)
    {
        printf("Could not allocate memory to create a node");
        return;
    }

    createdNode->next = NULL;
    createdNode->patient = malloc(sizeof(struct Patient));
    if (createdNode->patient == NULL)
    {
        printf("Could not allocate memory for a new patient");
        return;
    }

    //To ensure the strings are just connected and not seperated by spaces
    //so that it would be properly parsed when it is later read
    replaceUnderscoresWithSpaces(name);
    replaceUnderscoresWithSpaces(diagnosis);

    //Initializing our node and the patient attached to said node we allocated memory for
    createdNode->patient->id = id;
    createdNode->patient->age = age;
    createdNode->patient->roomNumbers = roomNumber;
    strcpy(createdNode->patient->name, name);
    strcpy(createdNode->patient->diagnosis, diagnosis);
    createdNode->patient->admissionDate = admittedDate;
    createdNode->patient->dischargeDate = dischargeDate;

    struct listNode* current = patientList->head;
    struct listNode* prevNode = NULL;

    // If no head exists, the newly created node becomes the head
    if (patientList->head == NULL)
    {
        patientList->head = createdNode;
        patientList->nodeAmount++;
        return;
    }

    //Performing an insertion sort within the list with the patient's Id
    while (current != NULL)
    {
        if (id < current->patient->id)
        {
            if (prevNode == NULL)
            {
                // Inserting at the beginning
                createdNode->next = patientList->head;
                patientList->head = createdNode;
            }
            else
            {
                // Inserting between prevNode and current
                prevNode->next = createdNode;
                createdNode->next = current;
            }

            patientList->nodeAmount++;
            return;
        }

        prevNode = current;
        current = current->next;
    }


    // Append to the end if all existing IDs are smaller
    prevNode->next = createdNode;
    patientList->nodeAmount++;
}

/**
 * Discharges a patient by setting their discharge date if they haven't been discharged already.
 *
 * @param patientList Pointer to the LinkedList containing patients.
 */
void dischargePatient(const struct LinkedList* patientList)
{
    if (patientList->nodeAmount == 0)
    {
        printf("\nPlease register a patient first.\n");
        return;
    }

    struct Date* dischargeDate = malloc(sizeof(struct Date));
    int id = 0;

    if (dischargeDate == NULL)
    {
        printf("Could not allocated memory for date");
        return;
    }

    printf("\nPlease input the patient's Id to be discharged:\n");
    scanf("%d", &id);

    printf("Please fill the date fields below for discharge date:");
    promptDateInitialization(dischargeDate);

    struct listNode* prevNode = NULL;
    struct listNode* currentNode = patientList->head;

    //Loop through all the nodes until it hits null
    while (currentNode != NULL)
    {
        // Matches the given ID
        if (currentNode->patient->id == id)
        {
            // Check if already discharged
            if (currentNode->patient->dischargeDate != NULL)
            {
                printf("The patient is already discharged\n");
                return;
            }

            //If not discharged, we will add the discharge date to them
            currentNode->patient->dischargeDate = dischargeDate;

            printf("\nSuccessfully discharged patient #%d\n", id);
            return;
        }

        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    printf("No patient with that Id was found\n");
}

/**
 * Provides a submenu to manage doctor schedules: adding doctors or assigning them to shifts.
 *
 * @param doctorNames 2D array storing doctor names.
 * @param doctorSchedule 2D array representing days and shifts.
 * @param doctorNum Pointer to an integer tracking how many doctors exist.
 */
void manageDoctorShifts(char doctorNames[][MAX_CHAR_LENGTH],
                        int doctorSchedule[][MAX_SECTIONS_OF_DAY],
                        int* doctorNum)
{
    int input = 0;
    while (input != 3)
    {
        printf("\nDOCTOR SCHEDULE MENU\n"
            "1. Add Doctors\n"
            "2. Schedule Doctor Shifts\n"
            "3. Go Back\n"
            "Please input your option:\n");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            addDoctors(doctorNames, doctorNum);
            break;
        case 2:
            scheduleDoctors(doctorNames, doctorSchedule, *doctorNum);
            break;
        case 3:
            break;
        default:
            printf("Invalid input. Try again.\n");
        }
    }
}

/**
 * Prompts the user to add a new doctor name into the system, with basic validation.
 *
 * @param doctors 2D array where each row stores a single doctor's name.
 * @param doctorNum Pointer to the current count of doctors.
 */
void addDoctors(char doctors[][MAX_CHAR_LENGTH],
                int* doctorNum)
{
    int isInvalid = 1;
    while (isInvalid)
    {
        printf("Enter name for Doctor:\n");
        getchar();
        fgets(doctors[*doctorNum], MAX_CHAR_LENGTH, stdin);
        doctors[*doctorNum][strcspn(doctors[*doctorNum], "\n")] = 0; // Remove newline

        if (strlen(doctors[*doctorNum]) == 0)
        {
            printf("Invalid name. Try again.\n");
            continue;
        }

        (*doctorNum)++;
        isInvalid = 0;
    }
}

/**
 * Assigns an existing doctor to a specific day and shift slot.
 *
 * @param doctors 2D array of doctor names.
 * @param doctorSchedule 2D array storing shift assignments (by index).
 * @param doctorNum Number of doctors currently available.
 */
void scheduleDoctors(char doctors[][MAX_CHAR_LENGTH],
                     int doctorSchedule[][MAX_SECTIONS_OF_DAY],
                     int doctorNum)
{
    if (doctorNum == 0)
    {
        printf("No doctors have been added yet. Please add doctors first.\n");
        return;
    }

    printf("\nAvailable Doctors:\n");
    for (int i = 0; i < doctorNum; i++)
    {
        printf("%d. %s\n", i + 1, doctors[i]);
    }

    int doctorIndex;
    printf("Enter the number of the doctor to schedule (1 - %d): ", doctorNum);
    scanf("%d", &doctorIndex);

    if (doctorIndex < 1 || doctorIndex > doctorNum)
    {
        printf("Invalid doctor selection.\n");
        return;
    }

    doctorIndex -= 1; // convert to 0-based index

    int day, section;

    printf("Scheduling %s.\n", doctors[doctorIndex]);
    printf("Enter day (0=Sunday, 6=Saturday): ");
    scanf("%d", &day);
    printf("Enter section of the day (0=Morning, 1=Afternoon, 2=Evening): ");
    scanf("%d", &section);

    if (day < 0 || day >= MAX_DAYS || section < 0 || section >= MAX_SECTIONS_OF_DAY)
    {
        printf("Invalid input for day or section.\n");
        return;
    }

    if (doctorSchedule[day][section] != -1)
    {
        printf("Warning: There is already a doctor scheduled for that time slot.\n");
    }

    //Set that time slot within one of the day of the week to the
    //doctor's index
    doctorSchedule[day][section] = doctorIndex;
    printf("Successfully scheduled %s on day %d, section %d.\n", doctors[doctorIndex], day, section);
}

/**
 * Displays the Reporting and Analytics menu, allowing the user to choose different reports.
 *
 * @param patientList Pointer to the LinkedList of patients.
 * @param doctorNames 2D array of doctor names.
 * @param doctorShifts 2D array storing day/shift doctor assignments.
 * @param numDoctors Current number of registered doctors.
 */
void ReportsAndAnalyticsMenu(const struct LinkedList* patientList,
                             char doctorNames[][MAX_CHAR_LENGTH],
                             int doctorShifts[][MAX_SECTIONS_OF_DAY],
                             int numDoctors)
{
    int input = 0;

    while (input != 6)
    {
        printf("\nREPORTING AND ANALYTICS\n"
            "1. View All Patients\n"
            "2. Total Patients admitted in a day, week, or month\n"
            "3. Patients Discharged by Day, Week, And Month\n"
            "4. Doctor Utilization Report\n"
            "5. Room Usage Statistics\n"
            "6. Go back\n"
            "Please input your option:\n");

        scanf("%d", &input);

        switch (input)
        {
        case 1:
            viewAllPatients(patientList);
            break;
        case 2:
            generatePatientsAdmittedReport(patientList);
            break;
        case 3:
            generateDischargedPatientsReport(patientList);
            break;
        case 4:
            generateDoctorReport(doctorNames, doctorShifts, numDoctors);
            break;
        case 5:
            generateRoomUsageReport(patientList);
            break;
        default: ;
        }
    }
}

/**
 * Prints all patients in the linked list, including their admission and discharge dates.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void viewAllPatients(const struct LinkedList* patientList)
{
    if (patientList->nodeAmount == 0)
    {
        printf("\nPlease register a patient first\n");
        return;
    }

    printf("\n");

    struct listNode* currentNode = patientList->head;

    //Loop through until we hit a null node
    while (currentNode != NULL)
    {
        struct Patient* nodePatient = currentNode->patient;
        char dischargedDateString[50] = "Not Discharged";
        char admittedDateString[50];

        //Convert all dates to a readable format (dd/mm/yyyy)
        dateToString(nodePatient->dischargeDate, dischargedDateString);
        dateToString(nodePatient->admissionDate, admittedDateString);

        printf(
            "Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d | Date Admitted: %s | Date discharged: %s\n",
            nodePatient->id, nodePatient->name, nodePatient->age, nodePatient->diagnosis, nodePatient->roomNumbers,
            admittedDateString, dischargedDateString);

        //Reset the dischargedDateString to an empty string
        strcpy(dischargedDateString, "");
        currentNode = currentNode->next;
    }

    printf("\n");
}

/**
 * Generates a report of total patients admitted, broken down by year and month, and writes it to 'PatientAdmittedReport.txt'.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void generatePatientsAdmittedReport(const struct LinkedList* patientList)
{
    FILE* fptr;
    struct listNode* current;
    int yearlyCounts[MAX_YEARS] = {0};
    int monthlyCounts[MAX_YEARS][MAX_MONTHS] = {{0}};
    int uniqueYears[MAX_YEARS];
    int yearIndex = 0;
    int found, i, j;

    fptr = fopen("PatientAdmittedReport.txt", "w");
    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    current = patientList->head;

    while (current != NULL)
    {
        struct Date admDate = *current->patient->admissionDate;
        found = 0;

        for (i = 0; i < yearIndex; i++)
        {
            if (uniqueYears[i] == admDate.year)
            {
                yearlyCounts[i]++;
                monthlyCounts[i][admDate.month - 1]++;
                found = 1;
                break;
            }
        }

        if (!found && yearIndex < MAX_YEARS)
        {
            uniqueYears[yearIndex] = admDate.year;
            yearlyCounts[yearIndex] = 1;
            monthlyCounts[yearIndex][admDate.month - 1] = 1;
            yearIndex++;
        }

        current = current->next;
    }

    fprintf(fptr, "Patients Admission Report\n\n");
    for (i = 0; i < yearIndex; i++)
    {
        fprintf(fptr, "Year: %d\n", uniqueYears[i]);
        fprintf(fptr, "Total Patients Admitted: %d\n", yearlyCounts[i]);
        fprintf(fptr, "Patients Per Month:\n");
        for (j = 0; j < MAX_MONTHS; j++)
        {
            fprintf(fptr, "  Month %2d: %d\n", j + 1, monthlyCounts[i][j]);
        }
        fprintf(fptr, "Average Patients Per Month: %.2f\n\n", (float)yearlyCounts[i] / MAX_MONTHS);
    }

    fclose(fptr);
    printf("Report generated successfully!\n");
}

/**
 * Allows the user to input a target discharge date, then generates a report listing
 * all patients discharged on that date, saved to 'DischargedPatientsReport.txt'.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void generateDischargedPatientsReport(const struct LinkedList* patientList)
{
    struct Date* targetDate;
    FILE* fptr;
    struct listNode* current;
    int found = 0;

    targetDate = malloc(sizeof(struct Date));

    if (targetDate == NULL)
    {
        printf("Could not allocate memory for the target date.\n");
        return;
    }

    //Getting the discharge date form the user
    do
    {
        printf("Please input the year discharged:\n");
        scanf("%d", &(targetDate->year));

        printf("Please input the month discharged:\n");
        scanf("%d", &(targetDate->month));

        printf("Please input the day discharged:\n");
        scanf("%d", &(targetDate->day));
    }
    while (!validateDate(targetDate->day, targetDate->month, targetDate->year));

    fptr = fopen("DischargedPatientsReport.txt", "w");
    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fptr, "Patients Discharged on %d-%d-%d\n\n", targetDate->day, targetDate->month, targetDate->year);

    current = patientList->head;
    while (current != NULL)
    {
        const struct Date* disDate;

        disDate = current->patient->dischargeDate;

        if (disDate != NULL && disDate->day == targetDate->day &&
            disDate->month == targetDate->month &&
            disDate->year == targetDate->year)
        {
            fprintf(fptr, "ID: %d | Name: %s | Age: %d | Diagnosis: %s | Room: %d\n",
                    current->patient->id, current->patient->name, current->patient->age,
                    current->patient->diagnosis, current->patient->roomNumbers);
            found = 1;
        }

        current = current->next;
    }

    if (!found)
    {
        fprintf(fptr, "No patients were discharged on this date.\n");
    }

    free(targetDate);
    fclose(fptr);
    printf("Discharge report generated successfully!\n");
}

/**
 * Generates a doctor utilization report by scanning how many shifts each doctor covered,
 * saving the results to 'DoctorUtilizationReport.txt'.
 *
 * @param doctorNames 2D array of doctor names.
 * @param doctorShifts 2D array storing each doctor's index for each day/shift combination.
 * @param numDoctors Number of doctors currently registered in the system.
 */
void generateDoctorReport(char doctorNames[][MAX_CHAR_LENGTH],
                          int doctorShifts[][MAX_SECTIONS_OF_DAY],
                          int numDoctors)
{
    FILE* fptr;
    int i, j;
    int weeklyShiftTotal;

    fptr = fopen("DoctorUtilizationReport.txt", "w");
    if (fptr == NULL)
    {
        printf("Error: Could not open DoctorUtilizationReport.txt\n");
        return;
    }

    fprintf(fptr, "Doctor Utilization Report (Week)\n");
    fprintf(fptr, "===============================\n\n");

    for (i = 0; i < numDoctors; i++)
    {
        weeklyShiftTotal = 0;
        for (j = 0; j < MAX_DAYS; j++)
        {
            for (int k = 0; k < MAX_SECTIONS_OF_DAY; k++)
            {
                if (doctorShifts[j][k] == i)
                {
                    weeklyShiftTotal++;
                }
            }
        }
        fprintf(fptr, "Doctor: %s | Total Shifts: %d\n", doctorNames[i], weeklyShiftTotal);
    }

    fclose(fptr);
    printf("Doctor utilization report generated successfully.\n");
}

/**
 * Generates a Room Usage Report showing how many patients occupy each room,
 * along with lists of underutilized or overutilized rooms.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void generateRoomUsageReport(const struct LinkedList* patientList)
{
    FILE* fptr;
    struct listNode* current;
    int roomUsage[MAX_ROOMS] = {0}; // Index represents room number, value represents occupancy count
    int i;

    fptr = fopen("RoomUsageReport.txt", "w");
    if (fptr == NULL)
    {
        printf("Error opening file for room usage report.\n");
        return;
    }

    current = patientList->head;

    // For each patient, increment the usage of their assigned room
    while (current != NULL)
    {
        int roomNum = current->patient->roomNumbers;
        if (roomNum >= 0 && roomNum < MAX_ROOMS)
        {
            roomUsage[roomNum]++;
        }
        current = current->next;
    }

    fprintf(fptr, "Room Usage Report\n==================\n");
    fprintf(fptr, "Room\tUsage Count\n");
    for (i = 0; i < MAX_ROOMS; i++)
    {
        if (roomUsage[i] > 0)
        {
            fprintf(fptr, "%d\t%d\n", i, roomUsage[i]);
        }
    }

    fprintf(fptr, "\nUnderutilized Rooms (usage < %d):\n", UNDERUTILIZED_THRESHOLD);
    for (i = 0; i < MAX_ROOMS; i++)
    {
        if (roomUsage[i] > 0 && roomUsage[i] < UNDERUTILIZED_THRESHOLD)
        {
            fprintf(fptr, "Room %d: %d patients\n", i, roomUsage[i]);
        }
    }

    fprintf(fptr, "\nOverutilized Rooms (usage > %d):\n", OVERUTILIZED_THRESHOLD);
    for (i = 0; i < MAX_ROOMS; i++)
    {
        if (roomUsage[i] > OVERUTILIZED_THRESHOLD)
        {
            fprintf(fptr, "Room %d: %d patients\n", i, roomUsage[i]);
        }
    }

    fclose(fptr);
    printf("Room usage report generated successfully.\n");
}

/**
 * Displays a menu to search patients by ID or name.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void searchPatientMenu(struct LinkedList* patientList)
{
    int input = 0;

    while (input != 3)
    {
        printf("SEARCH PATIENT MENU\n"
            "1. Search by Id\n"
            "2. Search by Name\n"
            "3. Go back\n"
            "Please input your option:\n");

        scanf("%d", &input);

        switch (input)
        {
        case 1:
            searchPatientByID(patientList);
            break;
        case 2:
            searchPatientByName(patientList);
            break;
        }
    }
}

/**
 * Searches patients by an integer ID, printing the patient's details if found.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void searchPatientByID(struct LinkedList* patientList)
{
    const struct listNode* currentNode;
    int id;

    id = -1;

    while (id < 0)
    {
        printf("\nPlease input the Patient's Id:\n");
        scanf("%d", &id);

        if (id < 0)
        {
            printf("Please input a valid Id");
        }
    }

    currentNode = patientList->head;

    while (currentNode != NULL)
    {
        if (id == currentNode->patient->id)
        {
            printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d\n",
                   currentNode->patient->id, currentNode->patient->name, currentNode->patient->age,
                   currentNode->patient->diagnosis, currentNode->patient->roomNumbers);
            return;
        }
        // You should also move to the next node to continue searching beyond the first
        currentNode = currentNode->next;
    }

    printf("Patient was not found");
}

/**
 * Searches patients by name, printing the details of each matching patient.
 *
 * @param patientList Pointer to the LinkedList of patients.
 */
void searchPatientByName(const struct LinkedList* patientList)
{
    char name[50];
    int patientFound = 0;
    struct listNode* currentNode;

    printf("\nEnter Patient Name:\n");

    getchar();
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    currentNode = patientList->head;
    while (currentNode != NULL)
    {
        if (strcmp(name, currentNode->patient->name) == 0)
        {
            printf("Id: %d | Name: %s | Age: %d | Diagnosis: %s | Room Number: %d\n",
                   currentNode->patient->id, currentNode->patient->name, currentNode->patient->age,
                   currentNode->patient->diagnosis, currentNode->patient->roomNumbers);
            patientFound = 1;
        }

        currentNode = currentNode->next;
    }

    if (!patientFound)
    {
        printf("No patients were found\n");
    }
}

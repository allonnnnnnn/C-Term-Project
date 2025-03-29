#include <stdio.h>
#include <string.h>

/**
 * Function prototypes for patient and doctor management
 * Includes adding, viewing, searching, and discharging patients
 * Also includes managing doctors and their schedules
 */
void addPatient(int patientID[],
                char patientName[][50],
                int patientAge[],
                char patientDiagnosis[][50],
                int patientRoomNumber[],
                int *totalPatients);

void viewRecords(int patientID[],
                 char patientName[][50],
                 int patientAge[],
                 char patientDiagnosis[][50],
                 int patientRoomNumber[],
                 int totalPatients);

void searchPatient(int patientID[],
                   char patientName[][50],
                   int patientAge[],
                   char patientDiagnosis[][50],
                   int patientRoomNumber[],
                   int totalPatients);

void searchPatientByID(int patientID[],
                       char patientName[][50],
                       int patientAge[],
                       char patientDiagnosis[][50],
                       int patientRoomNumber[],
                       int totalPatients);

void searchPatientByName(int patientID[],
                         char patientName[][50],
                         int patientAge[],
                         char patientDiagnosis[][50],
                         int patientRoomNumber[],
                         int totalPatients);

void dischargePatient(int patientID[],
                      char patientName[][50],
                      int patientAge[],
                      char patientDiagnosis[][50],
                      int patientRoomNumber[],
                      int *totalPatients);

void manageDoctors(int doctorsSchedule[][3],
                   char doctors[][50],
                   int *totalDoctors);

void addDoctor(char doctors[][50],
               int *totalDoctors);

void viewAllDoctors(char doctors[][50],
                    int *totalDoctors);

void manageDoctorsSchedule(int doctorsSchedule[][3],
                           int totalDoctors);

void viewDoctorsSchedule(char doctors[][50],
                         int doctorsSchedule[][3]);

/**
 * Entry point of the program
 */
int main(void)
{
    int patientID[50];
    char patientName[50][50];
    int patientAge[50];
    char patientDiagnosis[50][50];
    int patientRoomNumber[50];
    int doctorSchedule[7][3];
    char doctors[21][50];

    int totalDoctors = 0;
    int totalPatients = 0;
    int input = 1;

    //Initialize the array to no doctors being scheduled (-1 in this case)
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            doctorSchedule[i][j] = -1;
        }
    }

    //System loop
    while (input != 6)
    {
        printf("\n1. Add Patient Record "
            "\n2. View All Patients"
            "\n3. Search Patient by ID or Name"
            "\n4. Discharge Patient"
            "\n5. Manage Doctor Schedule"
            "\n6. Exit"
            "\nPlease input your option: ");

        scanf("%d", &input);
        getchar();

        switch (input)
        {
            case 1:
                addPatient(patientID,
                           patientName,
                           patientAge,
                           patientDiagnosis,
                           patientRoomNumber,
                           &totalPatients);
                break;
            case 2:
                viewRecords(patientID,
                            patientName,
                            patientAge,
                            patientDiagnosis,
                            patientRoomNumber,
                            totalPatients);
                break;
            case 3:
                searchPatient(patientID,
                              patientName,
                              patientAge,
                              patientDiagnosis,
                              patientRoomNumber,
                              totalPatients);
                break;
            case 4:
                dischargePatient(patientID,
                                 patientName,
                                 patientAge,
                                 patientDiagnosis,
                                 patientRoomNumber,
                                 &totalPatients);
                break;
            case 5:
                manageDoctors(doctorSchedule,
                              doctors,
                              &totalDoctors);
                break;
            case 6:
                break;
        }
    }

    return 0;
}

/**
 * Displays all patients currently stored in the system
 */
void viewRecords(int patientID[],
                 char patientName[][50],
                 int patientAge[],
                 char patientDiagnosis[][50],
                 int patientRoomNumber[],
                 int totalPatients)
{
    printf("\nID\t|\t\tName\t\t |\tAge \t|\tDiagnosis\t|\tRoom Number\t\n");
    for (int i = 0; i < totalPatients; i++)
    {
        printf("%d\t|\t\t%s\t\t |\t%d \t|\t%s\t|\t%d\n",
               patientID[i],
               patientName[i],
               patientAge[i],
               patientDiagnosis[i],
               patientRoomNumber[i]);
    }
    printf("\n");
}

/**
 * Allows the user to search for a patient by either ID or Name
 */
void searchPatient(int patientID[],
                   char patientName[][50],
                   int patientAge[],
                   char patientDiagnosis[][50],
                   int patientRoomNumber[],
                   int totalPatients)
{
    if (totalPatients == 0)
    {
        printf("\nNo patients are added. Please add a patient first.");
        return;
    }

    int input = 0;

    while (input == 0)
    {
        printf("\nHow would you like to search for a patient:");
        printf("\n1. Search by patient id");
        printf("\n2. Search by patient name\n");
        scanf("%d", &input);
        getchar();

        if (input < 1 || input > 2)
        {
            printf("\nInvalid input, please try again");
            continue;
        }

        if (input == 1)
        {
            searchPatientByID(patientID,
                              patientName,
                              patientAge,
                              patientDiagnosis,
                              patientRoomNumber,
                              totalPatients);
        } else
        {
            searchPatientByName(patientID,
                                patientName,
                                patientAge,
                                patientDiagnosis,
                                patientRoomNumber,
                                totalPatients);
        }
    }
}

/**
 * Searches patients by id
 */
void searchPatientByID(int patientID[],
                       char patientName[][50],
                       int patientAge[],
                       char patientDiagnosis[][50],
                       int patientRoomNumber[],
                       int totalPatients)
{
    int input = 0;
    int index = 0;
    int doesExist = 0;

    printf("\nPlease input the patient id you want to get: ");
    scanf("%d", &input);
    getchar();


    for (int i = 0; i < totalPatients; i++)
    {
        if (input == patientID[i])
        {
            index = i;
            doesExist = 1;
        }
    }

    if (!doesExist)
    {
        printf("\nPatient not found.");
        return;
    }

    printf("\nPatient's name: %s"
           "\nPatient's Age: %d"
           "\nPatient's Diagnosis: %s"
           "\nPatient's Room Number: %d",
           patientName[index],
           patientAge[index],
           patientDiagnosis[index],
           patientRoomNumber[index]);
}

/**
 * Searches patients by name
 */
void searchPatientByName(int patientID[],
                         char patientName[][50],
                         int patientAge[],
                         char patientDiagnosis[][50],
                         int patientRoomNumber[],
                         int totalPatients)
{
    char name[50];
    int index = -1;
    printf("\nEnter Patient Name:");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    for (int i = 0; i < totalPatients; i++)
    {
        if (strcmp(patientName[i], name) == 0)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        printf("\nPatient found: \nID: %d \nName: %s \nAge: %d \nDiagnosis: %s, \nRoom Number: %d\n",
               patientID[index],
               patientName[index],
               patientAge[index],
               patientDiagnosis[index],
               patientRoomNumber[index]);
    } else
    {
        printf("\nPatient not found.");
    }
}

/**
 * Adds a patient to the system with an ID, name, age
 * diagnosis, and room number
 */
void addPatient(int patientID[],
                char patientName[][50],
                int patientAge[],
                char patientDiagnosis[][50],
                int patientRoomNumber[],
                int *totalPatients)
{
    int id;
    char name[50];
    int age;
    char diagnosis[50];
    int roomNumber;

    int idValid = 0;
    int nameValid = 0;
    int ageValid = 0;
    int diagnosisValid = 0;
    int roomNumberValid = 0;

    while (idValid == 0)
    {
        printf("\nEnter in the patient Id: ");
        scanf("%d", &id);
        getchar();
        idValid = 1;
    }

    while (nameValid == 0)
    {
        printf("\nEnter in the patient name: ");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline
        nameValid = 1;
    }

    while (ageValid == 0)
    {
        printf("\nEnter in the age of the patient: ");
        scanf("%d", &age);
        getchar();
        ageValid = 1;
    }

    while (diagnosisValid == 0)
    {
        printf("\nEnter in the diagnosis name: ");
        fgets(diagnosis, 50, stdin);
        diagnosis[strcspn(diagnosis, "\n")] = 0;
        diagnosisValid = 1;
    }

    while (roomNumberValid == 0)
    {
        printf("\nEnter in the room number: ");
        scanf("%d", &roomNumber);
        getchar();
        roomNumberValid = 1;
    }

    patientID[*totalPatients] = id;
    strcpy(patientName[*totalPatients], name);
    patientAge[*totalPatients] = age;
    strcpy(patientDiagnosis[*totalPatients], diagnosis);
    patientRoomNumber[*totalPatients] = roomNumber;

    (*totalPatients)++;
}

/**
 * Removes a patient from the system by their ID
 */
void dischargePatient(int patientID[],
                      char patientName[][50],
                      int patientAge[],
                      char patientDiagnosis[][50],
                      int patientRoomNumber[],
                      int *totalPatients)
{
    if (totalPatients == 0)
    {
        printf("\nThere are no registered patients. Please make some patients.");
        return;
    }

    int id;
    int patientIndex = 0;
    int foundPatient = 0;

    printf("\nPlease input the patient's id that is going to be discharged: ");
    scanf("%d", &id);
    getchar();

    while (foundPatient == 0)
    {
        for (int i = 0; i < *totalPatients; i++)
        {
            if (patientID[i] == id)
            {
                foundPatient = 1;
                patientIndex = i;
            }
        }

        if (foundPatient == 0)
        {
            printf("\nDid not find patient. Please try again: ");
            scanf("%d", &id);
            getchar();
        }
    }

    // Shifting down every item from the patientIndex to totalPatients
    for (int i = patientIndex; i <= *totalPatients; i++)
    {
        patientID[i] = patientID[i + 1];
        strcpy(patientName[i], patientName[i + 1]);
        patientAge[i] = patientAge[i + 1];
        strcpy(patientDiagnosis[i], patientDiagnosis[i + 1]);
        patientRoomNumber[i] = patientRoomNumber[i + 1];
    }
    (*totalPatients)--;

    printf("\nRemoved patient #%d", id);
}

/**
 * Provides options to add/view doctors and manage schedules
 */
void manageDoctors(int doctorSchedule[][3],
                   char doctors[][50],
                   int *totalDoctors)
{
    int input = 0;
    while (input != 5)
    {
        printf("\n1. Add a doctor"
            "\n2. View all doctors"
            "\n3. Manage Schedule"
            "\n4. View Doctors Schedule"
            "\n5. Exit\n");

        scanf("%d", &input);
        getchar();

        if (input < 1 || input > 5)
        {
            continue;
        }

        switch (input)
        {
            case 1:
                addDoctor(doctors,
                          totalDoctors);
                break;
            case 2:
                viewAllDoctors(doctors,
                               totalDoctors);
                break;
            case 3:
                manageDoctorsSchedule(doctorSchedule,
                                      *totalDoctors);
                break;
            case 4:
                viewDoctorsSchedule(doctors,
                                    doctorSchedule);
                break;
        }
    }
}

/**
 * Adds a new doctor to the system
 */
void addDoctor(char doctors[][50],
               int *totalDoctors)
{
    char doctorsName[50];

    printf("\nPlease input the doctors name: ");
    fgets(doctorsName, 50, stdin);

    printf("\nSuccessfully added a doctor");
    strcpy(doctors[*totalDoctors], doctorsName);

    (*totalDoctors)++;
}

/**
 * Views all doctors in a table-like format
 */
void viewAllDoctors(char doctors[][50],
                    int *totalDoctors)
{
    printf("\nID\t|\t\tName\t\t");

    for (int i = 0; i < *totalDoctors; i++)
    {
        printf("\n%d\t|\t\t%s",
               i + 1,
               doctors[i]);
    }
}

/**
 * Allows modifying doctor schedules
 */
void manageDoctorsSchedule(int doctorsSchedule[][3],
                           int totalDoctors)
{
    if (totalDoctors <= 0)
    {
        printf("\nPlease create a doctor first");
        return;
    }

    int timeslot = 0;
    int weekday = 0;
    int userInput = -1;

    printf("Enter 1 to add a doctor to the time table or 2 to remove a doctor: ");
    scanf("%d", &userInput);
    getchar();

    while (userInput < 1 || userInput > 2)
    {
        printf("Please enter a valid input:");
        scanf("%d", &userInput);
        getchar();
    }

    if (userInput == 1)
    {
        int doctorId = -1;

        printf("\nPlease input the doctor Id: ");
        scanf("%d", &doctorId);

        if (doctorId < 1 || doctorId > totalDoctors)
        {
            printf("\nNo doctor found. ");
            scanf("%d", &doctorId);

            return;
        }

        printf(
            "\nPlease input the day of week (1 for Monday, 2 for Tuesday, 3 for Wednesday, etc., to 7 for Sunday) you want the doctor to work at. In a number: ");
        scanf("%d", &weekday);
        getchar();

        while (weekday < 1 || weekday > 7)
        {
            printf("\nPlease input a valid weekday: ");
            scanf("%d", &weekday);
            getchar();
        }

        printf("\nPlease input the time slot you want the doctor to work at. "
            "\n1 for morning, 2 for afternoon, and 3 for evening: ");
        scanf("%d", &timeslot);
        getchar();


        while (weekday < 1 || weekday > 3)
        {
            printf("\nPlease input a valid slot: ");
            scanf("%d", &timeslot);
            getchar();
        }

        doctorsSchedule[weekday - 1][timeslot - 1] = doctorId;
        printf("\nUpdated the schedule!");
    } else
    {
        printf("\nPlease input the weekday (1-7) you want to remove a doctor from. Enter in a number: ");
        scanf("%d", &weekday);
        getchar();

        while (weekday < 1 || weekday > 7)
        {
            printf("\nPlease input a valid weekday: ");
            scanf("%d", &weekday);
            getchar();
        }

        printf("\nPlease input the time slot you want to remove a doctor from. "
            "\n1 for morning, 2 for afternoon, and 3 for evening: ");
        scanf("%d", &timeslot);
        getchar();

        while (weekday < 1 || weekday > 3)
        {
            printf("\nPlease input a valid slot: ");
            scanf("%d", &timeslot);
            getchar();
        }

        if (doctorsSchedule[weekday - 1][timeslot - 1] == -1)
        {
            printf("\nNo doctor is appointed to this day and timeslot");
        } else
        {
            doctorsSchedule[weekday - 1][timeslot - 1] = -1;
            printf("\nTimeslot cleared");
        }
    }
}

/**
 * Displays the weekly schedule of doctors
 */
void viewDoctorsSchedule(char doctors[][50],
                         int doctorsSchedule[][3])
{
    for (int i = 0; i < 7; i++)
    {
        char dayOfTheWeek[50];
        switch (i)
        {
            case 0:
                strcpy(dayOfTheWeek, "Monday");
                break;
            case 1:
                strcpy(dayOfTheWeek, "Tuesday");
                break;
            case 2:
                strcpy(dayOfTheWeek, "Wednesday");
                break;
            case 3:
                strcpy(dayOfTheWeek, "Thursday");
                break;
            case 4:
                strcpy(dayOfTheWeek, "Friday");
                break;
            case 5:
                strcpy(dayOfTheWeek, "Saturday");
                break;
            case 6:
                strcpy(dayOfTheWeek, "Sunday");
                break;
        }
        printf("\n%s:\n", dayOfTheWeek);
        for (int j = 0; j < 3; j++)
        {
            if (doctorsSchedule[i][j] == -1)
            {
                printf("No doctors scheduled\t\t|");
                continue;
            }
            printf("%s\t\t|",
                   doctors[doctorsSchedule[i][j] - 1]);
        }
    }
    printf("\n");
}

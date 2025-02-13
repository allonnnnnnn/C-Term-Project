#include <stdio.h>
#include <string.h>

int patientID[50];
char patientName[50][50];
int patientAge[50];
char patientDiagnosis[50][50];
int patientRoomNumber[50];
char doctorSchedule[7][3];

int totalPatients = 0;

void addPatient();
void viewRecords();
void searchPatient();
void searchPatientByID();
void searchPatientByName();
void dischargePatient();
void manageDoctorSchedule();

int main(void) {
    int input = 1;

    while (input != 6) {
        printf("\n1. Add Patient Record \n"
            "2. View All Patients\n"
            "3. Search Patient by ID\n"
            "4. Discharge Patient\n"
            "5. Manage Doctor Schedule\n"
            "6. Exit\n");

        scanf("%d", &input);
        getchar();

        switch (input) {
            case 1:
                addPatient();
                break;
            case 2:
                viewRecords();
                break;
            case 3:
                searchPatient();
                break;
            case 4:
                dischargePatient();
                break;
            case 5:
                manageDoctorSchedule();
                break;
            case 6:
                break;
        }
    }

    return 0;
}

void viewRecords() {
    printf("\nID\t|\t\tName\t\t |\tAge \t|\tDiagnosis\t|\tRoom Number\t\n");
    for (int i = 0; i < totalPatients; i++) {
        printf("%d\t|\t\t%s\t\t |\t%d \t|\t%s\t|\t%d\n",
               patientID[i],
               patientName[i],
               patientAge[i],
               patientDiagnosis[i],
               patientRoomNumber[i]);
    }
    printf("\n");
}

void searchPatient() {
    int input;
    int task = 0;

    while (task == 0) {
        printf("How would you like to search for a patient:");
        printf("\n1. search by patient id");
        printf("\n2. search by patient name\n");
        scanf("%d", &input);
        getchar();
        if (input < 1 || input > 2) {
            printf("Invalid input, please try again");
            continue;
        }

        if (input == 1) {
            searchPatientByID();
            task = 1;
        } else {
            searchPatientByName();
            task = 1;
        }
    }
}

void searchPatientByID() {
    int input = 0;
    int doesExist = 0;

    printf("Please input the patient id you want to get");
    scanf("%d", &input);
    getchar();

    while (doesExist == 0) {
        for (int i = 0; i < totalPatients; i++) {
            if (input == patientID[i]) {
                doesExist = 1;
            }
        }

        if (!doesExist) {
            printf("Invalid input. Please try again");

            scanf("%d", &input);
            getchar();
        }
    }

    printf("\nPatient's name: %s"
           "\nPatient's Age: %d"
           "\nPatient's Diagnosis: %s"
           "\nPatient's Room Number: %d",
           patientName[input],
           patientAge[input],
           patientDiagnosis[input],
           patientRoomNumber[input]);
}

void searchPatientByName() {
    char name[50];
    int index = -1;
    printf("\nEnter Patient Name:");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    for (int i = 0; i < totalPatients; i++) {
        if (strcmp(patientName[i], name) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        printf("Patient found: ID: %d Name: %s, Age: %d, Diagnosis: %s, Room Number: %d\n", patientID[index],
               patientName[index], patientAge[index], patientDiagnosis[index], patientRoomNumber[index]);
    } else {
        printf("Patient not found.\n");
    }
}

void addPatient() {
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

    while (idValid == 0) {
        printf("Enter in the patient Id: ");
        scanf("%d", &id);
        getchar();
        idValid = 1;
    }

    while (nameValid == 0) {
        printf("Enter in the patient name: ");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline
        nameValid = 1;
    }

    while (ageValid == 0) {
        printf("Enter in the age of the patient: ");
        scanf("%d", &age);
        getchar();
        ageValid = 1;
    }

    while (diagnosisValid == 0) {
        printf("Enter in the diagnosis name: ");
        fgets(diagnosis, 50, stdin);
        diagnosis[strcspn(diagnosis, "\n")] = 0;
        diagnosisValid = 1;
    }

    while (roomNumberValid == 0) {
        printf("Enter in the room number: ");
        scanf("%d", &roomNumber);
        getchar();
        roomNumberValid = 1;
    }

    patientID[totalPatients] = id;
    strcpy(patientName[totalPatients], name);
    patientAge[totalPatients] = age;
    strcpy(patientDiagnosis[totalPatients], diagnosis);
    patientRoomNumber[totalPatients] = roomNumber;
    totalPatients++;
}

void dischargePatient() {
    if (totalPatients == 0) {
        printf("There are no registered patients. Please make some patients.");
        return;
    }

    int id;
    int patientIndex;
    int foundPatient = 0;

    printf("Please input the patient's id that is going to be discharged: \n");
    scanf("%d", &id);

    while (foundPatient == 0) {
        for (int i = 0; i < totalPatients; i++) {
            if (patientID[i] == id) {
                foundPatient = 1;
                patientIndex = i;
            }
        }

        if (foundPatient == 0) {
            printf("Did not find patient. Please try again: \n");
            scanf("%d", &id);
        }
    }

    // Shifting down every item from the patientIndex to totalPatients
    for (int i = patientIndex; i <= totalPatients; i++) {
        patientID[i] = patientID[i + 1];
        strcpy(patientName[i], patientName[i + 1]);
        patientAge[i] = patientAge[i + 1];
        strcpy(patientDiagnosis[i], patientDiagnosis[i + 1]);
        patientRoomNumber[i] = patientRoomNumber[i + 1];
        strcpy(doctorSchedule[i], doctorSchedule[i + 1]);
    }
    totalPatients--;

    printf("Removed patient #%d\n", id);
}

void manageDoctorSchedule() {

}




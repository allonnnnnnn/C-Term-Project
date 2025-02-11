#include <stdio.h>
#include <string.h>

int patientID[50];
char patientName[50][50];
int patientAge[50];
char patientDiagnosis[50][50];
int roomNumber[50];
char doctorSchedule[7][3];

void addPatient();
void viewRecords();
void searchPatientByID();
void dischargePatient();
void searchPatientByName();
void manageDoctorSchedule();

int main(void)
{
    printf("1. Add Patient Record \n"
        "2. View All Patients\n"
        "3. Search Patient by ID\n"
        "4. Discharge Patient\n"
        "5. Manage Doctor Schedule\n"
        "6. Exit\n");

    const int input;

    scanf("%d", input);

    switch (input)
    {
    case 1:
        addPatient();
        break;
    case 2:
        viewRecords();
        break;
    case 3:
        searchPatientByID();
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

    return 0;
}

void viewRecords()
{
    for (int i = 0; i < 50; i++)
    {

    }
}

void addPatient()
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
        printf("Enter in the patient Id:");
        scanf("%d", id);
        idValid = 1;
    }

    while (nameValid == 0)
    {
        printf("Enter in the patient name:");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0; // Remove newline
        nameValid = 1;
    }

    while (ageValid == 0)
    {
        printf("Enter in the age of the patient:");
        scanf("%d", age);
        ageValid = 1;
    }

    while (diagnosisValid == 0)
    {
        printf("Enter in the diagnosis name:");
        fgets(diagnosis, 50, stdin);
        diagnosis[strcspn(diagnosis, "\n")];
    }
}

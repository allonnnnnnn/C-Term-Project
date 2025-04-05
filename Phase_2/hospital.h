//
// Created by allan on 2025-03-28.
//

#ifndef HOSPITAL_H
#define HOSPITAL_H

#define MAX_CHAR_LENGTH 200
#define MAX_SECTIONS_OF_DAY 3
#define MAX_YEARS 10
#define MAX_MONTHS 12
#define MAX_ROOMS 100
#define UNDERUTILIZED_THRESHOLD 2
#define OVERUTILIZED_THRESHOLD 10
#define MAX_DOCTORS 10
#define MAX_DAYS 7

struct Patient
{
    int id;
    char name[MAX_CHAR_LENGTH];
    int age;
    char diagnosis[MAX_CHAR_LENGTH];
    int roomNumbers;
    struct Date* admissionDate;
    struct Date* dischargeDate;
};

struct DoctorShift
{
    char doctorName[MAX_CHAR_LENGTH];
    int weekNumber;
    int dayOfWeek; // 0-6
};

struct Date
{
    int day;
    int month;
    int year;
};

struct listNode
{
    struct Patient* patient;
    struct listNode* next;
};

struct LinkedList
{
    struct listNode* head;
    int nodeAmount;
};

void addPatient(struct LinkedList* patientList);

void createPatientToList(struct LinkedList* patientList,
                         int id,
                         char* name,
                         int age,
                         char* diagnosis,
                         int roomNumber,
                         struct Date* admittedDate,
                         struct Date* dischargeDate);

void writePatientToSaveFile(const int id,
                            char name[50],
                            const int age,
                            char diagnosis[50],
                            const int roomNumber,
                            struct Date* dateAdmitted);

int validateDate(int day,
                 int month,
                 int year);

int initializeDate(struct Date* date,
                   int day,
                   int month,
                   int year);

void promptDateInitialization(struct Date* date);

void dateToString(const struct Date* date,
                  char* string);

void parseDate(struct Date** date,
               char* dateString);

void loadSaveFile(struct LinkedList* patientList);

void writeAllPatientsToSaveFile(const struct LinkedList* patientList);

void ReportsAndAnalyticsMenu(const struct LinkedList* patientList,
                             char doctorNames[][MAX_CHAR_LENGTH],
                             int doctorShifts[][MAX_SECTIONS_OF_DAY],
                             int numDoctors);

void generatePatientsAdmittedReport(const struct LinkedList* patientList);

void generateDischargedPatientsReport(const struct LinkedList* patientList);

void generateRoomUsageReport(const struct LinkedList* patientList);

void generateDoctorReport(char doctorNames[][MAX_CHAR_LENGTH],
                          int doctorShifts[][MAX_SECTIONS_OF_DAY],
                          int numDoctors);

void viewAllPatients(const struct LinkedList* patientList);

void searchPatientMenu(struct LinkedList* patientList);

void searchPatientByID(struct LinkedList* patientList);

void searchPatientByName(const struct LinkedList* patientList);

void dischargePatient(const struct LinkedList* patientList);

void manageDoctorShifts(char doctorNames[][MAX_CHAR_LENGTH],
                        int doctorSchedule[][MAX_SECTIONS_OF_DAY],
                        int* doctorNum);

void addDoctors(char doctors[][MAX_CHAR_LENGTH],
                int* doctorNum);

void scheduleDoctors(char doctors[][MAX_CHAR_LENGTH],
                     int doctorSchedule[][MAX_SECTIONS_OF_DAY],
                     int doctorNum);

#endif //HOSPITAL_H

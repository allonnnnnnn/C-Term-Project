//
// Created by allan on 2025-03-28.
//

#ifndef HOSPITAL_H
#define HOSPITAL_H

#define MAX_LENGTH 200

struct Patient
{
    int id;
    char name[MAX_LENGTH];
    int age;
    char diagnosis[MAX_LENGTH];
    int roomNumbers;
};

struct listNode
{
    struct Patient* patient;
    struct listNode* next;
};

void addPatient(struct Patient ***patients,
                int* patientAmount,
                int* maxPatientAmount);

void viewRecords(const struct Patient patients[],
                 const int* patientAmount);

void searchPatient(const struct Patient patients[],
                   const int* patientAmount);

void searchPatientByID(const struct Patient patients[],
                       const int* patientAmount);

void searchPatientByName(const struct Patient patients[],
                         const int* patientAmount);

void dischargePatient(const struct Patient patients[],
                      const int* patientAmount);

void manageDoctors();

void addDoctor();

void viewAllDoctors();

void manageDoctorsSchedule();

void viewDoctorsSchedule();


#endif //HOSPITAL_H

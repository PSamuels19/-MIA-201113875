#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <time.h>

//####### INICIO ESTRUCTURA PARTICIONES #########
typedef struct Partition{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}particion;

//####### FIN ESTRUCTURA PARTICIONES #########


//####### INICIO ESTRUCTURA MASTER BOOT RECORD #########
typedef struct MasterBootRecord{
 int mbr_tam;
 time_t mbr_fecha_creacion;
 int mbr_disk_signature;
 particion mbr_partition_[4];
}MBR;

//####### FIN ESTRUCTURA MASTER BOOT RECORD #########


//####### INICIO ESTRUCTURA EXTENDED BOOT RECORD #########
typedef struct ExtendedBootRecord{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

//####### FIN ESTRUCTURA EXTENDED BOOT RECORD #########


//####### INICIO ESTRUCTURA PARA MONTAR #########
typedef struct Mount{
    char id[4];
    char path[250];
    char name[50];
    int DiskNumber;
    int part;
    int estado;
}montar;

//####### FIN ESTRUCTURA PARA MONTAR #########

#endif // ESTRUCTURAS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "citas.h"

void solicitarCita(int id_paciente) {
    printf("\n--- Solicitar Nueva Cita ---\n");
    
    Cita nuevaCita;
    nuevaCita.id_paciente = id_paciente;
    
    printf("Fecha (dd/mm/aaaa): ");
    scanf("%s", nuevaCita.fecha);
    printf("Hora (hh:mm): ");
    scanf("%s", nuevaCita.hora);
    printf("Motivo: ");
    scanf(" [^\n]", nuevaCita.motivo);
    strcpy(nuevaCita.estado, "programada");
    
    printf("Cita programada para el %s a las %s\n", nuevaCita.fecha, nuevaCita.hora);
}

void modificarCita(int id_cita) {
    printf("\n--- Modificar Cita ---\n");
    printf("Modificando cita con ID: %d\n", id_cita);
    
}

void cancelarCita(int id_cita) {
    printf("\n--- Cancelar Cita ---\n");
    printf("Cita con ID: %d ha sido cancelada\n", id_cita);
    
}

void listarCitasPaciente(int id_paciente) {
    printf("\n--- Mis Citas Programadas ---\n");
    printf("Mostrando citas para paciente con ID: %d\n", id_paciente);
    
}

void listarCitasMedico(int id_medico) {
    printf("\n--- Citas Programadas ---\n");
    printf("Mostrando citas para médico con ID: %d\n", id_medico);
    
}
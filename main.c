#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "operations.h"

void exit_(sensor *sensor_array, int nr_of_sensors)
{
//functie pentru eliberarea memoriei alocate
//la finalul programului
	for (int i = 0; i < nr_of_sensors; ++i) {
		free(sensor_array[i].sensor_data);
		free(sensor_array[i].operations_idxs);
	}
	free(sensor_array);
}

void sortare(sensor **sensor_array,	int nr_of_sensors)
{
//sortez vectorul astfel incat senzorii de tip PMU sa aiba prioritate
sensor *sensor_array_aux = malloc(sizeof(sensor)*(nr_of_sensors));
	int pmu_index = 0, tire_index = 0;
	for (int i = 0; i < nr_of_sensors; ++i)
		if ((*sensor_array)[i].sensor_type == PMU) {
			sensor_array_aux[pmu_index] = (*sensor_array)[i];
			pmu_index++;
		}
	for (int i = 0; i < nr_of_sensors; ++i)
		if ((*sensor_array)[i].sensor_type == TIRE) {
			sensor_array_aux[pmu_index+tire_index] = (*sensor_array)[i];
			tire_index++;
		}
	for (int i = 0; i < nr_of_sensors; ++i) {
	(*sensor_array)[i] = sensor_array_aux[i];
	}
	free(sensor_array_aux);
	//Aloc mai intai memorie pentru un alt array de senzori auxiliar .
	//Apoi,parcurg array-ul original,iar daca senzorului curent este de tip PMU
	//il adaug in array-ul auxiliar .
	//Daca senzorul curent este de tip TIRE il adaug
	//in array-ul auxiliar dupa senzorii de tip PMU .
	//La final array-ul auxiliar sortat
	//il copiez in array-ul original si
	// eliberez memoria alocata pt array-ul auxiliar.
}

void print_simple(sensor *sensor_array, int nr_of_sensors, int index)
{

	if (index < 0 || index >= nr_of_sensors) {
		printf("Index not in range!\n");
		return;
	}
	if (sensor_array[index].sensor_type == TIRE) {
	printf("Tire Sensor\n");
	printf("Pressure: %.2f\n",
	((tire_sensor *)sensor_array[index].sensor_data)->pressure);
	printf("Temperature: %.2f\n",
	((tire_sensor *)sensor_array[index].sensor_data)->temperature);
	printf("Wear Level: %d%%\n",
	((tire_sensor *)sensor_array[index].sensor_data)->wear_level);
	if (((tire_sensor *)sensor_array[index].sensor_data)->performace_score != 0)
		printf("Performance Score: %d\n",
		((tire_sensor *)sensor_array[index].sensor_data)->performace_score);
	else
		printf("Performance Score: Not Calculated\n");

	}
	if (sensor_array[index].sensor_type == PMU) {
	printf("Power Management Unit\n");
	printf("Voltage: %.2f\n",
	((power_management_unit *)sensor_array[index].sensor_data)->voltage);
	printf("Current: %.2f\n",
	((power_management_unit *)sensor_array[index].sensor_data)->current);
	printf("Power Consumption: %.2f\n",
	((power_management_unit *)
	sensor_array[index].sensor_data)->power_consumption);
	printf("Energy Regen: %d%%\n",
	((power_management_unit *)sensor_array[index].sensor_data)->energy_regen);
	printf("Energy Storage: %d%%\n",
	((power_management_unit *)sensor_array[index].sensor_data)->energy_storage);
	}
}

void stergere(sensor **sensor_array, int index, int *nr_of_sensors)
{
//functie pentru stergerea unui element dintr un vector
sensor temp = (*sensor_array)[index];
for (int i = index; i < (*nr_of_sensors-1); ++i) {
	(*sensor_array)[i] = (*sensor_array)[i+1]; }
free(temp.sensor_data);
free(temp.operations_idxs);
(*nr_of_sensors)--;
//Sterg elementul de la indexul dat prin mutarea elementelor
//din dreapta spre stanga si reduc nr total de senzori.
//Eliberez memoria alocata pt datele senzorului sters si pt indecsi operatiilor
//senzorului respectiv
}

void clear(sensor **sensor_array, int *nr_of_sensors)
{
	//verific daca un senzor contine valori eronate si daca contine il sterg
	for (int i = 0; i < (*nr_of_sensors); ++i) {
		if ((*sensor_array)[i].sensor_type == TIRE) {
			if (((tire_sensor *)
			(*sensor_array)[i].sensor_data)->pressure > 28 ||
			((tire_sensor *)(*sensor_array)[i].sensor_data)->pressure < 19 ||
			((tire_sensor *)
			(*sensor_array)[i].sensor_data)->temperature > 120 ||
			((tire_sensor *)(*sensor_array)[i].sensor_data)->temperature < 0 ||
			((tire_sensor *)(*sensor_array)[i].sensor_data)->wear_level > 100 ||
			((tire_sensor *)(*sensor_array)[i].sensor_data)->wear_level < 0) {
			stergere(sensor_array, i, nr_of_sensors);
			i--; }
		} else if ((*sensor_array)[i].sensor_type == PMU) {
			if (((power_management_unit *)
				(*sensor_array)[i].sensor_data)->voltage > 20 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->voltage < 10 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->current > 100 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->current < -100 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->power_consumption > 1000 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->power_consumption < 0 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->energy_regen > 100 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->energy_regen < 0 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->energy_storage > 100 ||
				((power_management_unit *)
				(*sensor_array)[i].sensor_data)->energy_storage < 0) {
				stergere(sensor_array, i, nr_of_sensors);
				i--; }
		}
	}
}

void analyze(sensor *sensor_array, int index)
{
	//vectorul de functii
	void (*operations[8])(void *);
	//initializez vectorul cu functiile disponibilie
	get_operations((void **)operations);
	//parcurg vectorul de indecsi si apelez functiile corespunzatoare
	for (int i = 0; i < sensor_array[index].nr_operations; ++i) {
		int idx = sensor_array[index].operations_idxs[i];
		operations[idx](sensor_array[index].sensor_data);
	}
	//Aceasta functie primeste ca input un pointer la un array de senzori
	// si un index la pozitia senzorului in array ul dat .
	// Initializez vectorul de functii
	//apeland functia get_operations si parcurg vectorul de indecsi din
	//senzorul dat,apeland functiile corespunzatoare din vectorul de functii.
	//Pt a apela functia corespunzatoare
	//folosesc indicele salvat in vectorul de indecsi idx .
	//Apelez apoi functia respectiva ,
	//dandu i ca argument datele senzorului (sensor_data).
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("Missing file");
	}
	FILE *in = fopen(argv[1], "rb");
	int nr_of_sensors;
	int s_type;
	fread(&nr_of_sensors, sizeof(int), 1, in);
	sensor *sensor_array = malloc(sizeof(sensor)*nr_of_sensors);
	for (int i = 0 ; i < nr_of_sensors ; ++i) {
		fread(&s_type, sizeof(int), 1, in);
		if (s_type == 1) {
			//daca este de tip PMU , citesc datele corespunzatoare
		sensor_array[i].sensor_type = PMU;
		power_management_unit pmu;
		fread(&pmu.voltage, sizeof(float), 1, in);
		fread(&pmu.current, sizeof(float), 1, in);
		fread(&pmu.power_consumption, sizeof(float), 1, in);
		fread(&pmu.energy_regen, sizeof(int), 1, in);
		fread(&pmu.energy_storage, sizeof(int), 1, in);
		sensor_array[i].sensor_data =
		malloc(sizeof(power_management_unit));
		memcpy(sensor_array[i].sensor_data, &pmu,
		sizeof(power_management_unit));
		} else if (s_type == 0) {
			//daca este de tip TIRE citesc datele corespunzatoare
		sensor_array[i].sensor_type = TIRE;
		tire_sensor tire;
		fread(&tire.pressure, sizeof(float), 1, in);
		fread(&tire.temperature, sizeof(float), 1, in);
		fread(&tire.wear_level, sizeof(int), 1, in);
		fread(&tire.performace_score, sizeof(int), 1, in);
		sensor_array[i].sensor_data = malloc(sizeof(tire_sensor));
		memcpy(sensor_array[i].sensor_data, &tire, sizeof(tire_sensor));
		}
		fread(&sensor_array[i].nr_operations, sizeof(int), 1, in);
		sensor_array[i].operations_idxs =
		malloc(sensor_array[i].nr_operations * sizeof(int));
		for (int j = 0; j < sensor_array[i].nr_operations; ++j)
			fread(&sensor_array[i].operations_idxs[j], sizeof(int), 1, in);
	}
	fclose (in);
	sortare(&sensor_array, nr_of_sensors);
	int index;
	char comanda[100];
	while (1) {
		scanf("%s", comanda);
		if (strcmp (comanda, "print") == 0) {
			scanf("%d", &index);
			print_simple(sensor_array, nr_of_sensors, index);
		} else if (strcmp (comanda, "exit") == 0) {
			exit_(sensor_array, nr_of_sensors);
			break;
		} else if (strcmp (comanda, "analyze") == 0) {
			scanf("%d", &index);
			analyze(sensor_array, index);
		} else if (strcmp (comanda, "clear") == 0) {
			clear (&sensor_array, &nr_of_sensors);
		}
	}
	return 0;
}

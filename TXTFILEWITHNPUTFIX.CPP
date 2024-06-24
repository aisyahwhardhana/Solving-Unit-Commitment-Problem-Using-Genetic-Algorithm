#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <iomanip.h>
#include <math.h>
#include <fstream.h>

#include <vector>



int pop_size_before;
int pop_size;
int n_unit;
int n_hour;

float power_output[24][38];
float max_output[38];
float min_output[38];
float a[38];
float b[38];
float c[38];
int min_on[38];
int min_off[38];
float hot_startup[38];
float cold_startup[38];
int cold_time[38];
float demand[24];
float ramp[38];
double results[200];



int num_crossover;
int num_crossover_before;
int num_mutation_before;
int num_mutation;
int i,j,k,l,iteration;



// Inisialisasi Parameter

float crossover_rate;
float mutation_rate;
/*int num_crossover= crossover_rate * pop_size/2 * 2;
int num_mutation= mutation_rate * pop_size;


int* selected_parents = new int[num_crossover];
int* selected_parents_mut = new int[num_mutation];
*/
int selected_parents[100];
int selected_parents_mut[100];
double fitness_final;

/*
struct Population {
    int on_off[n_hour][n_unit]; // Dynamic array of matrices
   // double power_output[n_hour][n_unit];
    double yu[n_hour][n_unit];
    double f_function[n_hour][n_unit];
    double startup[n_hour][n_unit];
    double startup_cost[n_hour][n_unit];
    double fitness_each[n_hour][n_unit];
    int combined_population[n_hour][n_unit];
    long double fitness;
};

struct Offspring {
    int on_off[n_hour][n_unit];
  //  double power_output[n_hour][n_unit];
    int yu[n_hour][n_unit];
    double f_function[n_hour][n_unit];
    double startup[n_hour][n_unit];
    double startup_cost[n_hour][n_unit];
    double fitness_each[n_hour][n_unit];
    double fitness;
};

struct OffspringMut {
    int on_off[n_hour][n_unit];
   // double power_output[n_hour][n_unit];
    int yu[n_hour][n_unit];
    double f_function[n_hour][n_unit];
    double startup[n_hour][n_unit];
    double startup_cost[n_hour][n_unit];
    double fitness_each[n_hour][n_unit];
    double fitness;
};
 */



struct Population {
    int on_off[24][38]; // Dynamic array of matrices
   // double power_output[24][38];
    float yu[24][38];
    float f_function[24][38];
    float startup[24][38];
    float startup_cost[24][38];
    float fitness_each[24][38];
    int combined_population[24][38];
    float fitness;
};

struct Offspring {
    int on_off[24][38];
    //double power_output[24][38];
    int yu[24][38];
    float f_function[24][38];
    float startup[24][38];
    float startup_cost[24][38];
    float fitness_each[24][38];
    float fitness;
};

struct OffspringMut {
    int on_off[24][38];
   // double power_output[24][38];
    int yu[24][38];
    float f_function[24][38];
    float startup[24][38];
    float startup_cost[24][38];
    float fitness_each[24][38];
    float fitness;
};




void processData();
void displayData(const Population*);
void generate_pop_awal(ofstream &outputFile2,Population* p, Offspring* offspring, OffspringMut* offspringmut);
void modification(ofstream &outputFile2,Population*);
void calculate_startup_cost(Population* p);
void fitness_function(ofstream &outputFile2,Population*);
void crossover(ofstream &outputFile2,Population*, Offspring* offspring);
void crossover_offspring_modification(ofstream &outputFile2,Offspring* offspring);
void calculate_crossover_offspring_fitness(ofstream &outputFile2,Offspring* offspring);
void displayData_crossover(Offspring* offspring);
void combine_fitness_crossover(ofstream &outputFile2,Population* p, Offspring* offspring,Population* combined_population);

void mutation(ofstream &outputFile2,Population* p, OffspringMut* Offspringmut);
void calculate_mutation_fitness(ofstream &outputFile2,OffspringMut* offspringmut);
void combine_fitness(ofstream &outputFile2,/*int num_crossover,*/Population* p, Offspring* offspring,OffspringMut* offspringmut, Population* combined_population);
void solusi_terbaik_fitness (ofstream &outputFile2,Population* p);
void solusi_terbaik_power_status (ofstream &outputFile2,Population* p);
void reset (Population* p, Offspring* offspring,OffspringMut* offspringmut);



// Function to read data from a file
bool readDataFromOutput(const char* filename) {
    ifstream inputFile(filename); // Open the text file for reading

    if (!inputFile) {
        cerr << "Failed to open the file: " << filename << endl;
        return false; // Return false to indicate failure
    }

    // Read Max Output
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> max_output[i])) {
            cerr << "Error reading max_output at position " << i << endl;
            inputFile.close();
            return false;
        }
    }
	for (int i = 0; i < n_hour; i++) {
    for (int j = 0; j < n_unit; j++) { // Use j here, not i
        power_output[i][j] = max_output[j]; // Copy max_output to power_output
    }
	}
    // Read min output
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> min_output[i])) {
            cerr << "Error reading min_output at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read a
     for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> a[i])) {
            cerr << "Error reading a at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read b
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> b[i])) {
            cerr << "Error reading b at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read c
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> c[i])) {
            cerr << "Error reading c at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read min on
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> min_on[i])) {
            cerr << "Error reading min on at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read min off
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> min_off[i])) {
            cerr << "Error reading min off at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read hot startup
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> hot_startup[i])) {
            cerr << "Error reading hot startup at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read cold startup
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> cold_startup[i])) {
            cerr << "Error reading cold startup at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    // Read cold time
    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> cold_time[i])) {
            cerr << "Error reading cold time at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

     // Read demand
    for (int i = 0; i < n_hour; i++) {
        if (!(inputFile >> demand[i])) {
            cerr << "Error reading cold time at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    for (int i = 0; i < n_unit; i++) {
        if (!(inputFile >> ramp[i])) {
         //   cerr << "Error reading ramp at position " << i << endl;
            inputFile.close();
            return false;
        }
    }

    inputFile.close(); // Close the file when done
    return true;       // Return true to indicate success


}

int main() {
   int i;//iteration;
   // const char* filename = "data_ucp_10unit.txt";

    cout << "PENYELESAIAN UNIT COMMITMENT PROBLEM DENGAN ALGORITMA GENETIKA" << "\n";
    cout << "===============================================================" << "\n";

    cout<<"Disusun oleh :"<<endl;
    cout<<"Aisyah Fadhilah W"<<endl;
    cout<<"NIM 081811233085"<<endl;
    cout<<"Program Studi S-1 Matematika UNAIR"<<endl;
    cout<<"Dosen pembimbing :"<<endl;
    cout<<"1. Asri Bekti Pratiwi S.Si., M.Si."<<endl;
    cout<<"2. Drs. Edi Winarko M.Cs."<<endl;
    cout << "===============================================================" << "\n\n";
    int datasetChoice;
    const char* datasetFiles[] = {
        "data_ucp_4unit.txt",
        "data_ucp_5unit.txt",
        "data_ucp_10unit.txt",
        "data_ucp_26unit.txt",
        "data_ucp_38unit.txt"
    };

     cout << "Pilih Data :" <<  endl;
     cout<<"1. Data 4 unit, 8 jam"<<endl;
     cout<<"2. Data 5 unit, 24 jam"<<endl;
     cout<<"3. Data 10 unit, 24 jam"<<endl;
     cout<<"4. Data 26 unit, 24 jam"<<endl;
    // cout<<"5. Data 38 unit, 24 jam"<<endl;


     do {
        cout << "Masukkan nomor data yang ingin dipilih (1-4): ";
        char input[100];
        cin >> input;

        // Check if the input contains a comma
        if (strchr(input, ',') != NULL||strchr(input, '.') != NULL) {
            cout << "Invalid input. Masukkan pilihan kembali (1-4)" << endl;
            continue;
        }

        datasetChoice = atoi(input); // Convert the C-string to an integer

        if (datasetChoice < 1 || datasetChoice > 4) {
            cout << "Invalid input. Masukkan pilihan kembali (1-4)" << endl;
        }
    } while (datasetChoice < 1 || datasetChoice > 4);


    if (datasetChoice >= 1 && datasetChoice <= 4) {
        const char* selectedDataset = datasetFiles[datasetChoice - 1];
        if (readDataFromOutput(selectedDataset)) {
         //    cout << "Selected dataset: " << selectedDataset <<  endl;
            // Now you have loaded the selected dataset, you can continue with your program logic.
        } else {
             cerr << "Failed to load dataset." <<  endl;
        }
    } else {
         cerr << "Invalid dataset choice." <<  endl;
    }

    if (datasetChoice == 1){
    		n_unit=4;
         n_hour=8;
    		cout<<"Jumlah Unit \t\t\t: "<<n_unit<<endl;
         cout<<"Lama Jam Operasi \t\t: "<<n_hour<<endl;
    }
    else if (datasetChoice == 2){
    		n_unit=5;
         n_hour=24;
         cout<<"Jumlah Unit \t\t\t: "<<n_unit<<endl;
         cout<<"Lama Jam Operasi \t\t: "<<n_hour<<endl;
    }
    else if (datasetChoice == 3){
    		n_unit=10;
         n_hour=24;
         cout<<"Jumlah Unit \t\t\t: "<<n_unit<<endl;
         cout<<"Lama Jam Operasi \t\t: "<<n_hour<<endl;
    }
    else if (datasetChoice == 4){
    		n_unit=26;
         n_hour=24;
         cout<<"Jumlah Unit \t\t\t: "<<n_unit<<endl;
         cout<<"Lama Jam Operasi \t\t: "<<n_hour<<endl;
    }
   /* else if (datasetChoice == 5){
    		n_unit=38;
         n_hour=24;
         cout<<"Jumlah Unit \t\t\t: "<<n_unit<<endl;
         cout<<"Lama Jam Operasi \t\t: "<<n_hour<<endl;
    }
   */

     do {
        cout << "Populasi awal (pop_awal >= 4)\t: ";
        char input[100];
        cin >> input;

        // Check if the input contains a comma
        if (strchr(input, ',') != NULL||strchr(input, '.') != NULL) {
            cout << "Invalid input. Input must be integer." << endl;
            continue;
        }

        pop_size = atoi(input); // Convert the C-string to an integer

        if (pop_size < 4||pop_size>100) {
            cout << "Invalid input. Populasi awal >= 4" << endl;
        }
    } while (pop_size < 4||pop_size>100);

    // Input crossover_rate
    do {
        cout << "Crossover rate (pc) 0 < pc < 1\t: ";
        char input[100];
        cin >> input;

        // Check if the input contains a comma
        if (strchr(input, ',') != NULL) {
            cout << "Invalid input. Do not use a comma." << endl;
            continue;
        }

        crossover_rate = atof(input); // Convert the C-string to a double

        if (/*crossover_rate*pop_size<2||*/crossover_rate <= 0.0 || crossover_rate >= 1.0) {
            cout << "Invalid input. Crossover must be in the range (0, 1)"<<crossover_rate <<endl;
        }
    } while (/*crossover_rate*pop_size<2||*/crossover_rate <= 0.0 || crossover_rate >= 1.0);

    // Input mutation_rate
    do {
        cout << "Mutation rate (pm) 0 < pm < 1\t: ";
        char input[100];
        cin >> input;

        // Check if the input contains a comma
        if (strchr(input, ',') != NULL) {
            cout << "Invalid input. Do not use a comma." << endl;
            continue;
        }

        mutation_rate = atof(input); // Convert the C-string to a double

        if (/*mutation_rate*pop_size<1||*/mutation_rate <= 0.0 || mutation_rate >= 1.0) {
            cout << "Invalid input. Mutation must be in the range (0, 1)" << endl;
        }
    } while (/*mutation_rate*pop_size<1||*/mutation_rate <= 0.0 || mutation_rate >= 1.0);

    // Input iteration
    do {
        cout << "Maksimal iterasi (iterasi > 0)\t: ";
        char input[100];
        cin >> input;

        // Check if the input contains a comma
        if (strchr(input, ',') != NULL) {
            cout << "Invalid input. Do not use a comma." << endl;
            continue;
        }

        iteration = atoi(input); // Convert the C-string to an integer

        if (iteration <= 0||iteration>200) {
            cout << "Invalid input. Maksimal iterasi harus lebih dari 0." << endl;
        }
    } while (iteration <= 0||iteration>200);
    if (datasetChoice == 1){
    		if (readDataFromOutput(datasetFiles[0])) {
         //   cout << "Loaded dataset: " << datasetFiles[0] << endl;
            // Now you have loaded the dataset, you can process it.
            processData();
        	} else {
         //   cerr << "Failed to load dataset: " << datasetFiles[0] << endl;
    		}
    }
    else if (datasetChoice == 2){
    		if (readDataFromOutput(datasetFiles[1])) {
         //   cout << "Loaded dataset: " << datasetFiles[1] << endl;
            // Now you have loaded the dataset, you can process it.
            processData();
        	} else {
         //   cerr << "Failed to load dataset: " << datasetFiles[1] << endl;
    		}
    }
    else if (datasetChoice == 3){
    		if (readDataFromOutput(datasetFiles[2])) {
         //   cout << "Loaded dataset: " << datasetFiles[2] << endl;
            // Now you have loaded the dataset, you can process it.
            processData();
        	} else {
         //   cerr << "Failed to load dataset: " << datasetFiles[2] << endl;
    		}
    }
    else if (datasetChoice == 4){
    		if (readDataFromOutput(datasetFiles[3])) {
         //   cout << "Loaded dataset: " << datasetFiles[3] << endl;
            // Now you have loaded the dataset, you can process it.
            processData();
        	} else {
         //   cerr << "Failed to load dataset: " << datasetFiles[3] << endl;
    		}
    }
    else if (datasetChoice == 5){
    		if (readDataFromOutput(datasetFiles[4])) {
         //   cout << "Loaded dataset: " << datasetFiles[4] << endl;
            // Now you have loaded the dataset, you can process it.
            processData();
        	} else {
         //   cerr << "Failed to load dataset: " << datasetFiles[4] << endl;
    		}
    }








   num_crossover=0;

  	num_crossover_before = crossover_rate * pop_size;



   if (num_crossover_before % 2 ==1 ) {
    num_crossover=num_crossover_before-1; // Make it even by decrementing by 1
	}

   num_mutation = mutation_rate * pop_size;
   num_crossover=0;
  	num_crossover_before = crossover_rate * pop_size;
   num_mutation_before = mutation_rate * pop_size;
   num_mutation=num_mutation_before;


  int num_crossover=num_crossover_before+0;

  if (num_crossover % 2 ==1 ) {

    	num_crossover=num_crossover-1; // Make it even by decrementing by 1
	}
  if (num_crossover == 0 ) {

    	num_crossover=2; // Make it even by decrementing by 1
	}

  if (num_mutation == 0 ) {

    	num_mutation=1; // Make it even by decrementing by 1
	}
 //    cout<<"num mut: "<<num_mutation<<endl;
 //    cout<<"num cross: "<<num_crossover<<endl;






   Population* p = new Population[pop_size*2];
   Population* combined_population = new Population[250];
   int* selected_parents = new int[num_crossover];
   int* selected_parents_mut = new int[num_mutation];
   Offspring* offspring = new Offspring[num_crossover];
   OffspringMut* offspringmut = new OffspringMut[num_mutation];



    float resultsDetails[200][24][38];
  //  float populasi_awal [200][24][38];
  //  float populasi_crossover[200][24][38];
  //  float populasi_mutasi[200][24][38];
  //  float populasi_baru [200][24][38];



    ofstream outputFile2("proses_lengkap.txt");

    // Check if the file was successfully opened
    if (!outputFile2) {
        cerr << "Error opening output file." << endl;
        return 1;  // Indicate error to the calling process
    }

    double temp;
    for (int i = 0; i < iteration; i++) {
         results[i]=0;
    }

   cout<<setprecision(10);

   //Proses Algoritma Genetik

   generate_pop_awal(outputFile2, p,offspring,offspringmut);


   for(i=0;i<iteration;i++){
     // reset(p, offspring, offspringmut);
      outputFile2 << "ITERASI " << (i + 1) << ":\n";
      outputFile2 << "=======================================\n";
   	modification(outputFile2, p);

 		calculate_startup_cost(p);
    	fitness_function(outputFile2, p);

      crossover(outputFile2, p,offspring);


      combine_fitness_crossover(outputFile2, p, offspring,combined_population);


      mutation(outputFile2, p, offspringmut);
     	calculate_mutation_fitness(outputFile2, offspringmut);

     	combine_fitness(outputFile2, /*num_crossover,*/p, offspring, offspringmut, combined_population);


      solusi_terbaik_fitness (outputFile2, p);
      double currentResult = p[0].fitness;// Calculate your result for this iteration
    	results[i] = currentResult*1.05;


      for (int j = 0; j < n_hour; j++) {
        for (int k = 0; k < n_unit; k++) {
            resultsDetails[i][j][k] = p[0].on_off[j][k] * power_output[j][k];
        }
    	}

      outputFile2<<"Fitness Terbaik:"<<setprecision(15) <<results[i]<<endl<<endl;
      reset(p, offspring, offspringmut);



      //cout << "Fitness: " << p[0].fitness <<endl;
    }



     for (int i = 0; i < iteration-1; i++) {
        for (int j = 0; j < iteration-i-1; j++) {
            if (results[j] < results[j+1]) {
                 temp = results[j];
                results[j] = results[j + 1];
                results[j + 1] = temp;
            }
        }
    }

   //   i++;
     for(i=0;i<iteration;i++){
    //  	cout<<"iter :"<<results[i]<<endl;

     }

  // }


    solusi_terbaik_power_status (outputFile2,p);
   // cout<<endl<<"Total Biaya: "<<setprecision(15) <<fitness_final;
    cout<<endl<<"Total Biaya: "<<setprecision(15) <<results[iteration-1];

    outputFile2.close();


    ofstream outputFile("hasil_iterasi_lengkap.txt");

	// Check if the file was successfully opened
	if (!outputFile) {
    	cerr << "Error opening output file." << endl;
	} else {

   	outputFile << "HASIL ITERASI \n";
    	// Iterate through the results and write them to the file
    	for (int i = 0; i < iteration; i++) {
        	outputFile <<  "\n ITERASI " << (i + 1) << ":\n";

         outputFile << "Power Output pada Jam ke-i, Unit ke-j\n";
         outputFile << "\t|";
         for (int i = 0; i < n_unit; i++) {
       		outputFile<<"Unit"<<(i+1)<<"\t";
         }
         outputFile<<endl;
         //outputFile <<"Unit"<<(i+1)<<"\t|";
        	for (int j = 0; j < n_hour; j++) {
         	outputFile<<"Jam "<<(j+1)<<"\t|";
         	for (int k = 0; k < n_unit; k++) {
            	outputFile << setprecision(5) << resultsDetails[i][j][k] << '\t';
            }
            outputFile << "\n";
        	}
        	outputFile << "\nTotal biaya: " << setprecision(15) << results[i] << "\n\n";
    	}

    // Close the output file when done
    outputFile.close();
	}

    getch();
    //cin.ignore(); // Wait for user input
    return 0;


}


void reset (Population* p, Offspring* offspring,OffspringMut* offspringmut){
 	   for (int k = 0; k < pop_size; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
					p[k].fitness =0;
              // p[k].power_output[i][j]=0;
            }
        	}
    	}
      for (int k = 0; k < num_crossover; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
					offspring[k].fitness =0;
          //     offspring[k].power_output[i][j]=0;
            }
        	}
    	}
      for (int k = 0; k < num_mutation; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
					offspringmut[k].fitness =0;
         //      offspringmut[k].power_output[i][j]=0;
            }
        	}
    	}



}



// Membangkitkan populasi awal
void generate_pop_awal(ofstream &outputFile2,Population* p,Offspring* offspring, OffspringMut* offspringmut) {
    srand(time(NULL));

    for (int k = 0; k < pop_size; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                p[k].on_off[i][j] = rand() % 2; // Generate random binary numbers (0 or 1)
                p[k].startup_cost[i][j] =0;
            }
        }
    }
      outputFile2 << "POPULASI AWAL \n";
    	outputFile2 << "POWER OUTPUT \n";
      outputFile2 << "\t|";
        for (int i = 0; i < n_unit; i++) {
            outputFile2 << "Unit" << (i + 1) << "\t";
        }
        outputFile2 << endl;
        for (int i = 0; i < n_hour; i++) {
            outputFile2 << "Jam " << (i + 1) << "\t|";
            for (int j = 0; j < n_unit; j++) {
                outputFile2 << setprecision(5) << power_output[i][j] << '\t';
            }
            outputFile2 << '\n';
        }

      outputFile2 << endl<<endl<<"STATUS UNIT \n";
    	// Iterate through the results and write them to the file
    	for (int i = 0; i < pop_size; i++) {
        	outputFile2 <<  "Kromosom " << (i + 1) << ":\n";

         outputFile2 << "Status Unit \n";
         outputFile2 << "\t|";
         for (int i = 0; i < n_unit; i++) {
       		outputFile2<<"Unit"<<(i+1)<<"\t";
         }
         outputFile2<<endl;
         //outputFile <<"Unit"<<(i+1)<<"\t|";
        	for (int j = 0; j < n_hour; j++) {
         	outputFile2<<"Jam "<<(j+1)<<"\t|";
         	for (int k = 0; k < n_unit; k++) {
            	outputFile2 << setprecision(5) << p[i].on_off[j][k] << '\t';
            }
            outputFile2 << "\n";
        	}
      }



}


// Modifikasi agar kendala terpenuhi
void modification(ofstream &outputFile2,Population* p) {
//int consecutive_off = 0,consecutive_on = 0;

//Modifikasi untuk memenuhi waktu minimal nyala/mati unit

for (int k = 0; k < pop_size; k++) {
 	for (int j = 0; j < n_unit; j++) {
        int time_on = 0;      //counter waktu nyala unit
        int time_off =0;      //counter waktu mati unit

        for (int i = 0; i < n_hour; i++) {
        		//Cek status unit (nyala)
            if (p[k].on_off[i][j] == 1) {
                time_on++;
                time_off = 0;
                //Jika waktu nyala >= minimal waktu nyala
                if (time_on >= min_on[j]) {
                    // Unit tetap nyala
                    p[k].on_off[i][j] = 1;
                } else if (i + 1 < n_hour) {
                    // Cek apabila ada jam selanjutnya, unit tetap nyala
                    p[k].on_off[i + 1][j] = 1;
                }
            } else {//p[k].on_off[i][j] == 0
                time_off++;
                time_on = 0;
                //Jika waktu mati >= minimal waktu mati
                if (time_off >= min_off[j]) {
                    // Unit tetap mati
                    p[k].on_off[i][j] = 0;
                } else if (i + 1 < n_hour) {
                    // Cek apabila ada jam selanjutnya, unit tetap mati
                    p[k].on_off[i + 1][j] = 0;
                }
            }
        }
    }
}



//Another approach of modification
/*
for (int k = 0; k < pop_size; k++) {
    for (int j = 0; j < n_unit; j++) {
        int consecutive_on = 0;  // Counter for consecutive hours unit is on
        int consecutive_off = 0; // Counter for consecutive hours unit is off

        for (int i = 1; i < n_hour; i++) {
    			if (p[k].on_off[i][j]==0){
            consecutive_off++;
                if(p[k].on_off[i-1][j]==1){
                		if(consecutive_on-1<min_on[j]){
                     	p[k].on_off[i][j]=1;
                        consecutive_on++;
                        consecutive_off = 0;
                  	}
                     else{
                     	p[k].on_off[i][j]=0;
                   //     consecutive_off++;
                     }
                }
                else{ //p[k].on_off[i-1][j]==0
                	p[k].on_off[i][j]=0;
                 // consecutive_off++;
                }
            }
            else{//p[k].on_off[i][j]==1
            consecutive_on++;
            	if(p[k].on_off[i-1][j]==0){
               		if(consecutive_off-1<min_off[j]){
                     	p[k].on_off[i][j]=0;
                        consecutive_off++;
                        consecutive_on = 0;
                  	}
                     else{
                        p[k].on_off[i][j]=1;
                    //    consecutive_on++;
                     }
               }
               else{
                  p[k].on_off[i][j]=1;
               //   consecutive_on++;
               }

            }
        }
    }
}

*/
/*
for (int k = 0; k < pop_size; k++) {
cout<<"modif:"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout<<p[k].on_off[i][j] <<"\t";
            }
            cout<<endl;
        }
    }

*/
//Melakukan modifikasi untuk power_output
  srand(time(NULL));

  float diff_output[100],adjust_output[100],sum_hour[100];


/*float spinning_reserve[100];

	for (int i=0;i<n_hour;i++){
   	spinning_reserve[i]=demand[i]+0.1*demand[i];
	}
   for (int k = 0; k < pop_size; k++) {
        float sum_spinningreserve = 0;
        for (int i = 0; i < n_hour; i++) {
            sum_spinningreserve = 0;
            for (int j = 0; j < n_unit; j++) {
                sum_spinningreserve=+max_output[j]*p[k].on_off[i][j];
				}
            for (int j = 0; j < n_unit; j++) {
            	if (sum_spinningreserve < spinning_reserve[i]) {
                  int adjust = rand() % n_unit;
                    if (p[k].on_off[i][adjust] == 0) {
                        p[k].on_off[i][adjust] = 1;
                        sum_spinningreserve += max_output[j];
                    }

                }

            }

        }
    }
 */

// Memastikan populasi memenuhi demand per jam
    for (int k = 0; k < pop_size; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                p[k].yu[i][j] = power_output[i][j] * p[k].on_off[i][j];
            }
        }
    }

    for (int k = 0; k < pop_size; k++) {
        int sum = 0;
        for (int i = 0; i < n_hour; i++) {
            sum = 0;
         // for (int l = 0; l < n_unit; l++) {
            for (int j = 0; j < n_unit; j++) {
                sum += p[k].yu[i][j];
            }
            for (int j = 0; j < n_unit; j++) {
            	if (sum < demand[i]) {
                  int adjust = rand() % n_unit;
                    if (p[k].on_off[i][adjust] == 0) {
                        p[k].on_off[i][adjust] = 1;
                        sum += p[k].yu[i][j];
                    }

                }
               /* if (sum > demand[i]) {
              //	for (int j = 0; j < n_unit; j++) {
                  int adjust = rand() % n_unit;
            //    for (int j = 0; j < n_unit; j++) {
                    if (p[k].on_off[i][adjust] == 1) {
                        p[k].on_off[i][adjust] = 0;
                        sum -= p[k].yu[i][j];
                    }

                } */

            }

        }
    }


//Mengurangi power output di posisi random sebanyak adjust_output (gradient descent)
int adjust_index ;
float reduction;
//for (int k = 0; k < pop_size; k++) {
	for (int i = 0; i < n_hour; i++) {
		for (int j = 0; j < n_unit; j++) {
    			sum_hour[i] += power_output[i][j]*p[0].on_off[i][j];
       }
        if (sum_hour[i] < demand[i]) {
         break;
        }

   }
//}
//for (int k = 0; k < pop_size; k++) {
    for (int i = 0; i < n_hour; i++) {

     //   sum_hour[i] = 0;
        sum_hour[i] = 0; // Reset jumlah power output untuk tiap baris (jam)

        for (int j = 0; j < n_unit; j++) {
        		adjust_output[j]=0;
            sum_hour[i] += power_output[i][j]*p[0].on_off[i][j];
            //diff_output[j] = max_output[j] - min_output[j];
        		//adjust_output[j] = diff_output[j] * 0.01;
            adjust_output[j] =0.1*(2 * a[j]*power_output[i][j]+b[j]);

            /*if(power_output[i-1][j]-power_output[i][j]>ramp[j]){
               power_output[i][j]=power_output[i-1][j];
            	break;

            } */
        }
        if (sum_hour[i] < demand[i]) {
          break;
            for (int jj = 0; jj < n_unit; jj++) {
        		//adjust_index = rand() % n_unit;


              	//power_output[i][adjust_index] += reduction;
              // int adjust_index = rand() % n_unit;
            	//reduction = min(adjust_output[jj], power_output[i][adjust_index] - min_output[adjust_index]);
               //power_output[i][adjust_index] = max_output[adjust_index];

                //break;
            }
        }
        for (int jj = 0; jj < n_unit; jj++) {
        	adjust_index = rand() % n_unit;
           /*	if (power_output[i-1][jj]-power_output[i][jj]>pow(0.92,60)*power_output[i-1][jj]){
            	power_output[i][j]=power_output[i-1][j];
             	break;
            }
            */

        		if (sum_hour[i] > demand[i]) {

            	//Memastikan bahwa output tidak kurang dari minimum output
           		reduction = min(adjust_output[jj], power_output[i][adjust_index] - min_output[adjust_index]);
               power_output[i][adjust_index] -= reduction;


        		}
            if (sum_hour[i] < demand[i]) {

              	//power_output[i][adjust_index] += reduction;
               int adjust_index = rand() % n_unit;
            	///reduction = min(adjust_output[jj], power_output[i][adjust_index] - min_output[adjust_index]);
             //  power_output[i][adjust_index] = max_output[adjust_index];

                break;
            }
            /*if (power_output[i-1][jj]-power_output[i][jj]>ramp[jj]){
            	power_output[i][jj]=power_output[i-1][jj];
             	break;
            } */

            sum_hour[i]-reduction;
            reduction=0;


           // sum_hour[i] = 0;
        }

    }
 //   reduction=0;
  //  sum_hour[i] = 0;
//}
/*
 for (int k = 0; k < pop_size; k++) {
    	cout<<"ON OFF before"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout<<p[k].on_off[i][j]<<"\t";
            }
            cout<<endl;
        }
    }
  */
for (int k = 0; k < pop_size; k++) {
    for (int j = 0; j < n_unit; j++) {
        int consecutive_off = 0; // Counter for consecutive hours unit is off

        for (int i = 0; i < n_hour; i++) {
            if (p[k].on_off[i][j] == 0) {
                consecutive_off++;
            } else {
                if (consecutive_off > 0 && consecutive_off < min_off[j]) {
                    // Turn on the unit for the previous consecutive_off hours
                    for (int x = i - consecutive_off; x < i; x++) {
                        p[k].on_off[x][j] = 1;
                    }
                }
                consecutive_off = 0; // Reset the counter
            }
        }

        // Check at the end in case the sequence ends with consecutive off hours
        if (consecutive_off > 0 && consecutive_off < min_off[j]) {
            // Turn on the unit for the previous consecutive_off hours
            for (int x = n_hour - consecutive_off; x < n_hour; x++) {
                p[k].on_off[x][j] = 1;
            }
        }
    }
}


outputFile2 << "POWER OUTPUT SETELAH MODIFIKASI \n";
      outputFile2 << "\t|";
        for (int i = 0; i < n_unit; i++) {
            outputFile2 << "Unit" << (i + 1) << "\t";
        }
        outputFile2 << endl;
        for (int i = 0; i < n_hour; i++) {
            outputFile2 << "Jam " << (i + 1) << "\t|";
            for (int j = 0; j < n_unit; j++) {
                outputFile2 << setprecision(5) << power_output[i][j] << '\t';
            }
            outputFile2 << '\n';
        }

      outputFile2 << endl<<endl<<"STATUS UNIT SETELAH MODIFIKASI\n";
    	// Iterate through the results and write them to the file
    	for (int i = 0; i < pop_size; i++) {
        	outputFile2 <<  "Kromosom " << (i + 1) << ":\n";

         outputFile2 << "Status Unit \n";
         outputFile2 << "\t|";
         for (int i = 0; i < n_unit; i++) {
       		outputFile2<<"Unit"<<(i+1)<<"\t";
         }
         outputFile2<<endl;
         //outputFile <<"Unit"<<(i+1)<<"\t|";
        	for (int j = 0; j < n_hour; j++) {
         	outputFile2<<"Jam "<<(j+1)<<"\t|";
         	for (int k = 0; k < n_unit; k++) {
            	outputFile2  << p[i].on_off[j][k] << '\t';
            }
            outputFile2 << "\n";
        	}
      }





/*
for (int k = 0; k < pop_size; k++) {
    	cout<<"ON OFF"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout<<p[k].on_off[i][j]<<"\t";
            }
            cout<<endl;
        }
    }
  */

   /*
    // Display the generated power_output values
   for(int k=0; k<pop_size;k++){
   cout<<"power"<<endl;
    for (int i = 0; i < n_hour; i++) {
        for (int j = 0; j < n_unit; j++) {
            cout << setprecision(4)<<p[k].power_output[i][j] << "\t";
        }
        cout << endl;
    }
   }
  */


}
void calculate_startup_cost(Population* p) {
    int time_off;

     for (int k = 0; k < pop_size; k++) {
        for (int j = 0; j < n_unit; j++) {
            for (int i = 0; i < n_hour; i++) {
            	p[k].startup_cost[i][j]=0;
            }
        }
     }

    for (int k = 0; k < pop_size; k++) {
        for (int j = 0; j < n_unit; j++) {
            time_off = 0; // Reset time_off

            for (int i = 0; i < n_hour; i++) {
                if (p[k].on_off[i][j] == 0) {
                    time_off++;

                    // Cek kondisi untuk menentukan startup cost
                    //Jika min off(minimum waktu mati unit)<=time off(waktu mati unit saat ini)<=cold time, maka hot startup digunakan
                    if (time_off >= min_off[j] && time_off <= cold_time[j]) {
                        p[k].startup_cost[i][j] = hot_startup[j];
                    }
                     // Jika time off(waktu mati unit saat ini)>cold time, maka cold startup digunakan
                      else if (time_off > cold_time[j]) {
                        p[k].startup_cost[i][j] = cold_startup[j];
                    }
                }
            }
        }
    }
   /*
    for (int k = 0; k < pop_size; k++) {
        cout << "Startup " << (k + 1) << "\n";
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout << p[k].startup_cost[i][j] << '\t';
            }
            cout << "\n";
        }
    }
    */
}

//Menghitung fungsi fitness
void fitness_function(ofstream &outputFile2,Population* p) {


 /*
 for (int k = 0; k < pop_size; k++) {
 	cout<<"pop"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
            	cout<<p[k].on_off[i][j]<<"\t";
        		}
            cout<<endl;
        }
    } */

    for (int k = 0; k < pop_size; k++) {
  //  cout<<"fit"<<endl;
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
             	p[k].f_function[i][j]=a[j]*power_output[i][j]*power_output[i][j]+b[j]*power_output[i][j]+c[j];
               p[k].startup[i][j]=p[k].startup_cost[i][j];
               p[k].fitness_each[i][j]=p[k].f_function[i][j]+p[k].startup[i][j];
   //          cout<<p[k].startup[i][j]<<"\t";
            }
   //    	cout<<endl;
        }
    }


    for (int k = 0; k < pop_size; k++) {
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
            	p[k].fitness=p[k].fitness+p[k].fitness_each[i][j];
        		}
        }
    }

    float f_1,startup_1,fitness_1,add=0;
    for (int k = 0; k < pop_size; k++) {
        for (int i = 0; i < n_hour; i++) {
        	add+=p[k].fitness_each[i][0];
            for (int j = 0; j < n_unit; j++) {
             	//p[k].fitness=p[k].fitness+p[k].fitness_each[i][j];
                f_1 = a[0] * power_output[0][j] * power_output[0][j] + b[0] * power_output[0][j] + c[0];
      			 startup_1 = p[k].startup_cost[0][j];
      			 fitness_1 = f_1 + startup_1;
            }

        }
       // cout <<"fitness : "<< setprecision(10)<<p[k].fitness << endl;
      p[k].fitness+=fitness_1;
    }

    outputFile2 <<endl;
    for (int k = 0; k < pop_size; k++) {
        outputFile2 <<"Fitness Kromosom"<<(k+1)<<": "<< setprecision(10)<<p[k].fitness*1.05 << endl;
    }

    startup_1 = 0;
    fitness_1 = 0;
}


void crossover(ofstream &outputFile2,Population* p, Offspring* offspring) {
//Melakukan seleksi induk untuk crossover (roulette wheel)
    int selected_parents[100];
    long double sum_fitness = 0.0;
    num_crossover=crossover_rate*pop_size;
    for (int i = 0; i < num_crossover; i++) {
        selected_parents[i] = 0;
    }
    int num_crossover=num_crossover_before+0;

   if (num_crossover % 2 ==1 ) {

    	num_crossover=num_crossover-1; // Make it even by decrementing by 1
	}

   if (num_crossover == 0 || num_crossover == 1) {

    	num_crossover=2; // Make it even by decrementing by 1
	}


    for (int k = 0; k < pop_size; k++) {
        sum_fitness = sum_fitness + p[k].fitness;

       // cout << "Sum Fitness Kromosom" << k << " = "   << setprecision(10)<<sum_fitness << endl;
    }
        int selected_count = 0;  // Counter for the selected parents



    //cout<<"num cross: "<<num_crossover<<endl;


    while (selected_count < num_crossover) {
        //Memilih induk crossover berdasarkan probabilitas kumulatif
        double spin = (rand() / (double)RAND_MAX) * sum_fitness;
        //  cout<<"spin="<<spin<<endl;
        double temp_sum = 0.0;
        int j = 0;

        while (temp_sum < spin && j < pop_size) {
            temp_sum += p[j].fitness;
            j++;
        }
        //Simpan induk terpilih
        selected_parents[selected_count] = j-1;


        selected_count++;  // Increment the counter
       // cout << "spin: " << spin << ", temp_sum: " << temp_sum << ", j: " << j << endl;
    }

  /*  cout << "selected parents=" << num_crossover << endl;
    for (int i = 0; i < num_crossover; i++) {
        cout << "index_parents=" << selected_parents[i] << endl;
    }
   */
   outputFile2 <<endl;
   outputFile2<<"CROSSOVER"<<endl;
   outputFile2 <<"Induk Crossover "<<endl;
   for (int i = 0; i < num_crossover; i++) {
        outputFile2  << selected_parents[i]+1 << endl;
    }


  /*
    int l;
    for (int k = 0; k < num_crossover; k++) {
    	l=selected_parents[k];
  //  cout<<"power cross"<<endl;
    	for (int i = 0; i < n_hour; i++) {
        for (int j = 0; j < n_unit; j++) {

            offspring[k].power_output[i][j] = p[l].power_output[i][j];
         //   cout<<offspring[k].power_output[i][j]<<"\t";

        }
       // cout<<endl;
     }
 	}
 */

//Crossover
    int temp[100][100]={{0},{0}};
    for (int k = 0; k < num_crossover; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                offspring[k].on_off[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < num_crossover; i += 2) {
        int parent1 = selected_parents[i];
        int parent2 = selected_parents[i + 1];

        for (int j = 0; j < n_unit; j++) {
            int crossover_point = rand() % n_hour; // Random crossover point

            //crossover dengan menggabungkan induk dan memutus pada crossover point
            for (int m = 0; m < n_hour; m++) {
            	// Copy from parent 1 for the first part
               offspring[i].on_off[m][j] = p[parent1].on_off[m][j];
               offspring[i + 1].on_off[m][j] = p[parent2].on_off[m][j];
            }
            for (int m = 0; m < n_hour; m++) {
            	if(m<crossover_point){
               	offspring[i].on_off[m][j] = p[parent1].on_off[m][j];
               	offspring[i + 1].on_off[m][j] = p[parent2].on_off[m][j];
               }else{
                  offspring[i].on_off[m][j] = p[parent2].on_off[m][j];
               	offspring[i + 1].on_off[m][j] = p[parent1].on_off[m][j];
               }
            }
            /*
            for (int m = crossover_point; m != 1; m = (m + 1) % n_hour) {
               temp [0][0]=offspring[i].on_off[m][j];
               offspring[i].on_off[m][j]=offspring[i+1].on_off[m][j];
               offspring[i+1].on_off[m][j] = temp[0][0];

            }
            temp [0][0]=offspring[i].on_off[1][j];
            offspring[i].on_off[1][j]=offspring[i+1].on_off[1][j];
            offspring[i+1].on_off[1][j] = temp[0][0];  */
        }
       // crossover_offspring_modification(&offspring[i]);
       // crossover_offspring_modification(&offspring[i + 1]);
    }

    outputFile2<<endl;
    outputFile2<<"ANAK HASIL CROSSOVER"<<endl;
    for (int k = 0; k < num_crossover; k++) {
    outputFile2<<"Anak "<<(k+1)<<" :"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                outputFile2<<offspring[k].on_off[i][j] <<"\t";
            }
            outputFile2<<endl;
        }
    }



//Memastikan anak crossover memenuhi kendala
for (int k = 0; k < num_crossover; k++) {
 	for (int j = 0; j < n_unit; j++) {
        int time_on = 0;
        int time_off = min_off[j];  // Start with the minimum off time

        for (int i = 0; i < n_hour; i++) {
            if (offspring[k].on_off[i][j] == 1) {
                time_on++;
                time_off = 0;

                if (time_on >= min_on[j]) {
                    // Keep the unit on
                    offspring[k].on_off[i][j] = 1;
                } else if (i + 1 < n_hour) {
                    // Turn on the unit in the next hour
                    offspring[k].on_off[i + 1][j] = 1;
                }
            } else {
                time_off++;
                time_on = 0;

                if (time_off >= min_off[j]) {
                    // Turn off the unit
                    offspring[k].on_off[i][j] = 0;
                } else if (i + 1 < n_hour) {
                    // Turn off the unit in the next hour
                    offspring[k].on_off[i + 1][j] = 0;
                }
            }
        }
    }
}



    for (int k = 0; k < num_crossover; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                offspring[k].yu[i][j] = power_output[i][j] * offspring[k].on_off[i][j];
            }
        }
    }

    for (int k = 0; k < num_crossover; k++) {
        int sum = 0;
        for (int i = 0; i < n_hour; i++) {
            sum = 0;
         // for (int l = 0; l < n_unit; l++) {
            for (int j = 0; j < n_unit; j++) {
                sum +=offspring[k].yu[i][j];
            }
            for (int j = 0; j < n_unit; j++) {
            	if (sum < demand[i]) {
                  int adjust = rand() % n_unit;
                    if (offspring[k].on_off[i][adjust] == 0) {
                        offspring[k].on_off[i][adjust] = 1;
                        sum += offspring[k].yu[i][j];
                    }

                }

            }

        }

    }


for (int k = 0; k < num_crossover; k++) {
    for (int j = 0; j < n_unit; j++) {
        int consecutive_off = 0; // Counter for consecutive hours unit is off

        for (int i = 0; i < n_hour; i++) {
            if (offspring[k].on_off[i][j] == 0) {
                consecutive_off++;
            } else {
                if (consecutive_off > 0 && consecutive_off < min_off[j]) {
                    // Turn on the unit for the previous consecutive_off hours
                    for (int x = i - consecutive_off; x < i; x++) {
                        offspring[k].on_off[x][j] = 1;
                    }
                }
                consecutive_off = 0; // Reset the counter
            }
        }

        // Check at the end in case the sequence ends with consecutive off hours
        if (consecutive_off > 0 && consecutive_off < min_off[j]) {
            // Turn on the unit for the previous consecutive_off hours
            for (int x = n_hour - consecutive_off; x < n_hour; x++) {
                offspring[k].on_off[x][j] = 1;
            }
        }
    }
}
       for (int k = 0; k < num_crossover; k++) {
        for (int j = 0; j < n_unit; j++) {
            for (int i = 0; i < n_hour; i++) {
            	offspring[k].startup_cost[i][j]=0;
            }
        }
     }








	for (int k = 0; k < num_crossover; k++) {
        for (int j = 0; j < n_unit; j++) {
            int time_off = 0; // Reset the time_off counter for each unit

            for (int i = 0; i < n_hour; i++) {
                if (offspring[k].on_off[i][j] == 0) {
                    time_off++;
                    if (time_off >= min_off[j] && time_off <= cold_time[j]) {
                        offspring[k].startup_cost[i][j] = hot_startup[j];
                    } else if (time_off > cold_time[j]) {
                        offspring[k].startup_cost[i][j] = cold_startup[j];
                    }
               // else {
               //         offspring[k].startup_cost[i][j] = 0; // No startup cost if time_off < min_off[j]
               //     }
                }
            }
        }
    }



    for (int k = 0; k < num_crossover; k++) {
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
             	offspring[k].f_function[i][j]=a[j]*power_output[i][j]*power_output[i][j]+b[j]*power_output[i][j]+c[j];
               offspring[k].startup[i][j]=offspring[k].startup_cost[i][j];
               offspring[k].fitness_each[i][j]=offspring[k].f_function[i][j]+offspring[k].startup[i][j];
            }

        }
    }

     for (int k = 0; k < num_crossover; k++) {
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
            	offspring[k].fitness=offspring[k].fitness+offspring[k].fitness_each[i][j];
        		}
        }
    }

    float f_1,startup_1,fitness_1,add;
    for (int k = 0; k < num_crossover; k++) {
    // 	cout<<"crossover fitness :"<<endl;
        for (int i = 0; i < n_hour; i++) {
        add+=offspring[k].fitness_each[i][0];
            for (int j = 0; j < n_unit; j++) {
             	//offspring[k].fitness=offspring[k].fitness+offspring[k].fitness_each[i][j];
                f_1 = a[0] * power_output[0][j] * power_output[0][j] + b[0] * power_output[0][j] + c[0];
      			 startup_1 = offspring[k].startup_cost[0][1] ;
      			 fitness_1 = f_1 + startup_1;
            }

        }
      offspring[k].fitness+=fitness_1;
    //  cout<<offspring[k].fitness<<endl;
    }


    outputFile2<<endl;

    outputFile2<<"ANAK HASIL CROSSOVER SETELAH MODIFIKASI"<<endl;
    for (int k = 0; k < num_crossover; k++) {

    	outputFile2<<"Anak "<<(k+1)<<" :"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                outputFile2<<offspring[k].on_off[i][j] <<"\t";
            }
            outputFile2<<endl;
        }
        outputFile2<<"Fitness : "<<offspring[k].fitness*1.05<<endl;
    }

    startup_1 = 0;
    fitness_1 = 0;

}

void combine_fitness_crossover(ofstream &outputFile2,Population* p, Offspring* offspring,Population* combined_population){
    num_crossover=crossover_rate*pop_size;
    if (num_crossover % 2 ==1 ) {

    	num_crossover=num_crossover-1; // Make it even by decrementing by 1
	}
   if (num_crossover == 0 || num_crossover == 1 ) {

    	num_crossover=2; // Make it even by decrementing by 1
	}

   if (num_mutation == 0 ) {

    	num_mutation=1; // Make it even by decrementing by 1
	}

   // cout<<"num cross: "<<num_crossover<<endl;
   //  cout<<"num mut: "<<num_mutation<<endl;
    const int combined_size = pop_size + num_crossover;

    // Copy individuals and fitness values from the initial population to the combined population
    for (int i = 0; i < pop_size; i++) {
        combined_population[i] = p[i];
    }

    // Menggabungkan anak crossover populasi awal
    for (int i = 0; i < num_crossover; i++) {
        combined_population[pop_size + i].fitness = offspring[i].fitness;
        for (int j = 0; j < n_hour; j++) {
            for (int k = 0; k < n_unit; k++) {
               // combined_population[pop_size + i].power_output[j][k] = offspring[i].power_output[j][k];

                combined_population[pop_size + i].on_off[j][k] = offspring[i].on_off[j][k];
                combined_population[pop_size + i].yu[j][k] = offspring[i].yu[j][k];
                combined_population[pop_size + i].fitness = offspring[i].fitness;

            }
        }
    }

    for (int i = 0; i < combined_size - 1; i++) {
        for (int j = 0; j < combined_size - i - 1; j++) {
            if (combined_population[j].fitness > combined_population[j + 1].fitness) {
                Population temp = combined_population[j];
                combined_population[j] = combined_population[j + 1];
                combined_population[j + 1] = temp;
            }
        }
    }


    /*
    for (int i = 0; i < (pop_size+num_crossover+num_mutation); i++) {
        cout<<"combined population : "<<combined_population[i].fitness<<endl ;
    }
      */
     //Menentukan populasi baru
    for (int k = 0; k < pop_size; k++) {
        p[k] = combined_population[k];
    }
   /* for(int k=0; k < pop_size; k++){
   cout<<"power"<<endl;
    for (int i = 0; i < n_hour; i++) {
        for (int j = 0; j < n_unit; j++) {
            cout << setprecision(4)<<power_output[i][j] << "\t";
        }
        cout << endl;
    }
   }
   */
}


void displayData_crossover(Offspring* offspring) {
	for (int k = 0; k <num_crossover; k++) {
        cout << "Offspring " << (k + 1) << "\n";

        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout << offspring[k].on_off[i][j] << '\t';
            }
            cout << "\n";
        }


       //cout << "Fitness 1: " << offspring[k].fitness <<endl;
    }

}


void mutation(ofstream &outputFile2,Population* p, OffspringMut* offspringmut){
//Melakukan seleksi induk untuk mutasi (roulette wheel)
    int selected_parents[100];
    int j, k, temp, start_index, end_index;
    float sum_fitness = 0.0;


    num_crossover=crossover_rate*pop_size;
    if (num_crossover % 2 ==1 ) {

    	num_crossover=num_crossover-1; // Make it even by decrementing by 1
	 }
    if (num_crossover == 0 || num_crossover == 1) {

    	num_crossover=2; // Make it even by decrementing by 1
	 }
    num_mutation=mutation_rate*pop_size;
    if (num_mutation % 2 == 0 ) {

    	num_mutation=1; // Make it even by decrementing by 1
	 }

    for (int i = 0; i < num_mutation; i++) {
        selected_parents_mut[i] = i % pop_size;
    }


    for (int k = 0; k < pop_size; k++) {
        sum_fitness = sum_fitness + p[k].fitness;

       // cout << "Sum Fitness Kromosom" << k << " = "   << setprecision(10)<<sum_fitness << endl;
    }



    int selected_count = 0;
    while (selected_count < num_mutation) {
        float spin = (rand() / (float)RAND_MAX) * sum_fitness;
      //  cout<<"spin="<<spin<<endl;
        float temp_sum = 0.0;
        int j = 0;

        while (temp_sum < spin && j < num_crossover) {
            temp_sum += p[j].fitness;
            j++;
        }

        selected_parents_mut[selected_count] = j-1;
        selected_count++;  // Increment the counter
        //cout << "spin: " << spin << ", temp_sum: " << temp_sum << ", j: " << j << endl;
    }

   /* cout << "selected parents=" << num_mutation << endl;
    for (int i = 0; i < num_mutation; i++) {
        cout << "index_parents=" << selected_parents_mut[i] << endl;
    }
    */
    outputFile2<<endl;
    outputFile2<<"MUTASI"<<endl;
    outputFile2<<"Induk Mutasi :"<<endl;
    for (int i = 0; i < num_mutation; i++) {
        outputFile2<< selected_parents_mut[i] +1<< endl;
    }

   /*
    for (int k = 0; k < num_mutation; k++) {
    	for (int i = 0; i < n_hour; i++) {
        for (int j = 0; j < n_unit; j++) {
            offspringmut[k].power_output[i][j] = p[selected_parents_mut[k]].power_output[i][j];
        }
     }
 	}
  */


//Mutasi Menggunakan inversion mutation
  for (int k = 0; k < num_mutation; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                offspringmut[k].on_off[i][j] = 0;
            }

        }
    }


      for (k = 0; k < num_mutation; k++) {
        start_index = rand() % n_hour;
        end_index = rand() % n_hour;
        int parent_mut = selected_parents_mut[k];
        if (start_index > end_index) {
            temp = start_index;
            start_index = end_index;
            end_index = temp;
        }

        // Copy induk ke offspring
        for (j = 0; j < n_unit; j++) {
            for (int h = 0; h < n_hour; h++) {
                offspringmut[k].on_off[h][j] = p[selected_parents_mut[k]].on_off[h][j];
            }
        }

        // Perform mutation
        for (j = 0; j < n_unit; j++) {
            int col_start = start_index;
            int col_end = end_index;

            for (int h = col_start; h <= col_end; h++) {
                if (offspringmut[k].on_off[h][j] == 0) {
                    offspringmut[k].on_off[h][j] = 1;
                } else {
                    offspringmut[k].on_off[h][j] = 0;
                }
            }
        }
    }



//Memastikan anak hasil mutasi memenuhi kendala

for (int k = 0; k < num_mutation; k++) {
 	for (int j = 0; j < n_unit; j++) {
        int time_on = 0;
        int time_off = min_off[j];  // Start with the minimum off time

        for (int i = 0; i < n_hour; i++) {
            if (offspringmut[k].on_off[i][j] == 1) {
                time_on++;
                time_off = 0;

                if (time_on >= min_on[j]) {
                    // Keep the unit on
                    offspringmut[k].on_off[i][j] = 1;
                } else if (i + 1 < n_hour) {
                    // Turn on the unit in the next hour
                    offspringmut[k].on_off[i + 1][j] = 1;
                }
            } else {
                time_off++;
                time_on = 0;

                if (time_off >= min_off[j]) {
                    // Turn off the unit
                    offspringmut[k].on_off[i][j] = 0;
                } else if (i + 1 < n_hour) {
                    // Turn off the unit in the next hour
                    offspringmut[k].on_off[i + 1][j] = 0;
                }
            }
        }
    }
}



    for (int k = 0; k < num_mutation; k++) {
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                offspringmut[k].yu[i][j] =power_output[i][j] * offspringmut[k].on_off[i][j];
            }
        }
    }
   /*
    // Ensure that the population satisfies the demand per hour
    for (int k = 0; k < num_mutation; k++) {
        int sum = 0;
        for (int i = 0; i < n_hour; i++) {
            sum = 0;
            for (int j = 0; j < n_unit; j++) {
                sum += offspringmut[k].yu[i][j];
            }
            if (sum < demand[i]) {
                for (int j = 0; j < n_unit; j++) {
                    if (offspringmut[k].on_off[i][j] == 0) {
                        offspringmut[k].on_off[i][j] = 1;
                    }
                }
            }
        }
    }
    */

    // Memastikan populasi memenuhi demand per jam

    for (int k = 0; k < num_mutation; k++) {
        int sum = 0;
        for (int i = 0; i < n_hour; i++) {
            sum = 0;
         // for (int l = 0; l < n_unit; l++) {
            for (int j = 0; j < n_unit; j++) {
                sum +=offspringmut[k].yu[i][j];
            }
            for (int j = 0; j < n_unit; j++) {
            	if (sum < demand[i]) {
                  int adjust = rand() % n_unit;
                    if (offspringmut[k].on_off[i][adjust] == 0) {
                        offspringmut[k].on_off[i][adjust] = 1;
                        sum += offspringmut[k].yu[i][j];
                    }

                }

            }

        }
    }

for (int k = 0; k < num_mutation; k++) {
    for (int j = 0; j < n_unit; j++) {
        int consecutive_off = 0; // Counter for consecutive hours unit is off

        for (int i = 0; i < n_hour; i++) {
            if (offspringmut[k].on_off[i][j] == 0) {
                consecutive_off++;
            } else {
                if (consecutive_off > 0 && consecutive_off < min_off[j]) {
                    // Turn on the unit for the previous consecutive_off hours
                    for (int x = i - consecutive_off; x < i; x++) {
                        offspringmut[k].on_off[x][j] = 1;
                    }
                }
                consecutive_off = 0; // Reset the counter
            }
        }

        // Check at the end in case the sequence ends with consecutive off hours
        if (consecutive_off > 0 && consecutive_off < min_off[j]) {
            // Turn on the unit for the previous consecutive_off hours
            for (int x = n_hour - consecutive_off; x < n_hour; x++) {
                offspringmut[k].on_off[x][j] = 1;
            }
        }
    }
}



    /*
     for (int k = 0; k < num_mutation; k++) {
        cout<<"mut offspring"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout<<offspringmut[k].on_off[i][j]<<"\t";
            }
        	cout<<endl;
        }
    }
    */

}

void calculate_mutation_fitness(ofstream &outputFile2,OffspringMut* offspringmut){


   int time_off;
   for (int k = 0; k < num_mutation; k++) {
        for (int j = 0; j < n_unit; j++) {
            for (int i = 0; i < n_hour; i++) {
            	offspringmut[k].startup_cost[i][j]=0;
            }
        }
     }


    for (int k = 0; k < num_mutation; k++) {
        for (int j = 0; j < n_unit; j++) {
            time_off = 0; // Reset the time_off counter for each unit

            for (int i = 0; i < n_hour; i++) {
                if (offspringmut[k].on_off[i][j] == 0) {
                    time_off++;

                    // Check the conditions to determine the appropriate startup cost
                    if (time_off >= min_off[j] && time_off <= cold_time[j]) {
                        offspringmut[k].startup_cost[i][j] = hot_startup[j];
                    } else if (time_off > cold_time[j]) {
                        offspringmut[k].startup_cost[i][j] = cold_startup[j];
                    } else {
                        offspringmut[k].startup_cost[i][j] = 0; // No startup cost if time_off < min_off[j]
                    }
                }
            }
        }
    }

for (int k = 0; k < num_mutation; k++) {
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
             	offspringmut[k].f_function[i][j]=a[j]*power_output[i][j]*power_output[i][j]+b[j]*power_output[i][j]+c[j];
               offspringmut[k].startup[i][j]=offspringmut[k].startup_cost[i][j];
               offspringmut[k].fitness_each[i][j]=offspringmut[k].f_function[i][j]+offspringmut[k].startup[i][j];
            }

        }
    }



     for (int k = 0; k < num_mutation; k++) {
        for (int i = 1; i < n_hour; i++) {
            for (int j = 1; j < n_unit; j++) {
            	offspringmut[k].fitness=offspringmut[k].fitness+offspringmut[k].fitness_each[i][j];
        		}
        }
    }

    float f_1,startup_1,fitness_1,add;
    for (int k = 0; k < num_mutation; k++) {
        for (int i = 0; i < n_hour; i++) {
        add+=offspringmut[k].fitness_each[i][0];
            for (int j = 0; j < n_unit; j++) {
              //	offspringmut[k].fitness=offspringmut[k].fitness+offspringmut[k].fitness_each[i][j];
                f_1 = a[0] * power_output[0][j] * power_output[0][j] + b[0] * power_output[0][j] + c[0];
      			 startup_1 = offspringmut[k].startup_cost[0][1] ;
      			 fitness_1 = f_1 + startup_1;
            }

        }
      offspringmut[k].fitness+=fitness_1;
   //   cout<<offspringmut[k].fitness<<endl;
    }

    outputFile2<<endl;

    outputFile2<<"ANAK HASIL MUTASI SETELAH MODIFIKASI"<<endl;
    for (int k = 0; k < num_mutation; k++) {
    	outputFile2<<"Anak "<<(k+1)<<":"<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                outputFile2<<offspringmut[k].on_off[i][j] <<"\t";
            }
            outputFile2<<endl;
        }
        outputFile2<<"Fitness : "<< offspringmut[k].fitness*1.05<<endl;
    }

    startup_1 = 0;
    fitness_1 = 0;
}


void combine_fitness(ofstream &outputFile2,/*int num_crossover,*/Population* p, Offspring* offspring,OffspringMut* offspringmut, Population* combined_population) {
   /* num_crossover=crossover_rate*pop_size;
    if (num_crossover % 2 ==1 ) {

    	num_crossover=num_crossover-1; // Make it even by decrementing by 1
	}
   //  cout<<"num cross: "<<num_crossover<<endl;
   // cout<<"num cross: "<<num_crossover<<endl;
   //  cout<<"num mut: "<<num_mutation<<endl;
    const int combined_size = pop_size + num_crossover + num_mutation;
    */

    // Copy individuals and fitness values from the initial population to the combined population
    for (int i = 0; i < pop_size; i++) {
        combined_population[i] = p[i];
    }

    /*
    // Menggabungkan anak crossover populasi awal
    for (int i = 0; i < num_crossover; i++) {
        combined_population[pop_size + i].fitness = offspring[i].fitness;
        for (int j = 0; j < n_hour; j++) {
            for (int k = 0; k < n_unit; k++) {
                combined_population[pop_size + i].on_off[j][k] = offspring[i].on_off[j][k];
                combined_population[pop_size + i].yu[j][k] = offspring[i].yu[j][k];
                combined_population[pop_size + i].fitness = offspring[i].fitness;
            }
        }
    }
          */
    //Menambahkan anak mutasi
    for (int i = 0; i < num_mutation; i++) {
        combined_population[selected_parents_mut[i]].fitness = offspringmut[i].fitness;
        for (int j = 0; j < n_hour; j++) {
            for (int k = 0; k < n_unit; k++) {
                combined_population[selected_parents_mut[i]].on_off[j][k] = offspringmut[i].on_off[j][k];
                combined_population[selected_parents_mut[i]].yu[j][k] = offspringmut[i].yu[j][k];
           //     combined_population[selected_parents_mut[i]].power_output[j][k] = power_output[j][k];

            }
        }
    }


    //Megurutkan fitness dari yang terkecil
    for (int i = 0; i < pop_size - 1; i++) {
        for (int j = 0; j < pop_size - i - 1; j++) {
            if (combined_population[j].fitness > combined_population[j + 1].fitness) {
                Population temp = combined_population[j];
                combined_population[j] = combined_population[j + 1];
                combined_population[j + 1] = temp;
            }
        }
    }


    outputFile2<<"Combined population : "<<endl ;
    for (int i = 0; i < pop_size; i++) {
        outputFile2<<combined_population[i].fitness*1.05<<endl ;
    }

    //Menentukan populasi baru
    for (int k = 0; k < pop_size; k++) {
        p[k] = combined_population[k];
    }
   /*
   for(int k=0; k < pop_size; k++){
   cout<<"power"<<endl;
    for (int i = 0; i < n_hour; i++) {
        for (int j = 0; j < n_unit; j++) {
            cout << setprecision(4)<<power_output[i][j] << "\t";
        }
        cout << endl;
    }
   } */
}





void displayData(const Population* p) {

  /* for (int k = 0; k < pop_size; k++) {
        cout << "Populasi " << (k + 1) << "\n";
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout << p[k].on_off[i][j] << '\t';
            }
            cout << "\n";
        }
    }

  */

    for (int j = 0; j < pop_size; j++) {
    	cout <<"Fitness Kromosom"<<(j+1)<<" = " << p[j].fitness << endl;
    }


}




void solusi_terbaik_fitness (ofstream &outputFile2,Population* p){
  /*
	ofstream outputFile("output.txt");

    // Check if the file was successfully opened
    if (!outputFile) {
        cerr << "Error opening output file." << endl;
       // return 0;
    }

    // Now, you can write to the output file using outputFile
    outputFile << "This will be saved in output.txt." << endl;
    outputFile << "You can write more lines here." << endl;

    // Don't forget to close the output file when done
    outputFile.close();
   */
   // return 0;
    /*  cout<<endl<<endl<<"Solusi Terbaik : "<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout << p[0].on_off[i][j]*power_output[i][j] << '\t';
            }
            cout << "\n";
        }
     */
     outputFile2 << "\nHASIL TERBAIK \n";
    	// Iterate through the results and write them to the file


         outputFile2 << "Power Output(i,j) pada Jam ke-i, Unit ke-j \n";
         outputFile2 << "\t|";
         for (int i = 0; i < n_unit; i++) {
       		outputFile2<<"Unit"<<(i+1)<<"\t";
         }
         outputFile2<<endl;
         //outputFile <<"Unit"<<(i+1)<<"\t|";
        	for (int j = 0; j < n_hour; j++) {
         	outputFile2<<"Jam "<<(j+1)<<"\t|";
         	for (int k = 0; k < n_unit; k++) {
            	outputFile2 << setprecision(5) << p[0].on_off[j][k]*power_output[j][k]  << '\t';
            }
            outputFile2 << "\n";
        	}

       fitness_final= p[0].fitness;
    //  cout << setprecision(13) <<p[0].fitness <<endl;


}

void solusi_terbaik_power_status (ofstream &outputFile2,Population* p){
   /*  cout<<endl<<endl<<"Power Output: "<<endl;
        for (int i = 0; i < n_hour; i++) {

            for (int j = 0; j < n_unit; j++) {
                cout <<setprecision(5) <<  power_output[i][j] << '\t';
            }
            cout << "\n";
        }
    */
    /*  cout<<endl<<endl<<"Status Unit: "<<endl;
        for (int i = 0; i < n_hour; i++) {
            for (int j = 0; j < n_unit; j++) {
                cout << p[0].on_off[i][j] << '\t';
            }
            cout << "\n";
        }
     */

     cout<<endl<<endl<<"Power Output pada Jam ke-i, Unit ke-j: "<<endl;
       cout<<"\t|";
     	 for (int i = 0; i < n_unit; i++) {
       	cout<<"Unit"<<(i+1)<<"\t|";
       }
       cout<<endl;
       for (int i = 0; i < n_unit+1; i++) {
       	cout<<"========";
       }
       cout<<endl;
        for (int i = 0; i < n_hour; i++) {
        		cout<<"Jam "<<(i+1)<<"\t|";
            for (int j = 0; j < n_unit; j++) {
                cout<<setprecision(5) << power_output[i][j]*p[0].on_off[i][j] << "\t";
            }
            cout << "\n";
        }

      // cout << "Fitness: " << p[0].fitness <<endl;


}



// Example function that uses the data
void processData() {


}


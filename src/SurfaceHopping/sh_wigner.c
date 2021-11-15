#include <stdlib.h>
#include <math.h>
#include "surface_hopping.h"
#include <stdio.h>

void sh_wigner_fill(struct Particle *ptr,
                    double q, double p, double std, 
                    long int npart, int dim){
    // Box-Muller generates two samples
    // you might want to pass in two samples
    // the loop is a tod odd because of the way 
    // we are setting up the particle class
    //for (int i=0;; i < npart; i++){
    //void *end;
    //for (end = &ptr[npart]; ptr != end; ptr++){
    //linked lists are not ideal for what we want to do
    struct Particle *part = ptr;
    while(part != NULL){
        for (int j=0; j < dim; j++){
                // i think jump conditions instructions are used in assembly
                // generate two from U[0,1]
                double x = (double)rand() / RAND_MAX;   
                double y = (double)rand() / RAND_MAX;   
                // https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
                // generate exp distribution
                // project onto cartesian co-ordinates
                // Box-Muller for independent distributions
                // the mean and variance are rather restrictive at the moment
                part->x[j] = (sqrt(- 2 * log(x)) * cos(2*M_PI*y))*std + q; 
                part->p[j] = (sqrt(- 2 * log(x)) * sin(2*M_PI*y))*std + p; 
        }
        part = part->next;
    }
}

// you should have a random library where you generate gaussian samples

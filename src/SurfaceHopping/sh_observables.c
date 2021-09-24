#include <stdlib.h>
#include <math.h>
#include "surface_hopping.h"



struct Observables *sh_observables_new(unsigned int npart, unsigned int dim){
  struct Observables *observables = malloc(sizeof(struct Observables));
  observables->mass_up = 0;
  observables->mass_down = 0;
  observables->npart = npart;
  observables->x_up = (double*)calloc(dim, sizeof(double));
  observables->x_down = (double*)calloc(dim, sizeof(double));
  observables->p_up = (double*)calloc(dim, sizeof(double)); // i do not know what i am doing - this approach may not work - try out
  observables->p_down = (double*)calloc(dim, sizeof(double));

  return observables;
}

void sh_observables_update(struct Observables *observables, struct Particle *particles, unsigned int dim){
  // can definitely better implementation for this mess
  observables->mass_up = 0;
  observables->mass_down = 0;
  observables->ke_up = 0;
  observables->ke_down = 0;
  observables->e_up = 0;
  observables->e_down= 0;
  for (int j=0; j<dim; j++){
    observables->x_up[j] = 0;
    observables->p_up[j] = 0;
    observables->x_down[j] = 0;
    observables->p_down[j] = 0;
  }
  
  struct Particle *part = particles;
  while(part != NULL){
    double ke_up = 0;
    double ke_down =0;
    if(part->state){
      observables->mass_up += 1;
      for (int j=0; j<dim; j++){
        ke_up += pow(part->p[j],2);
        observables->e_up += part->pot_new;
        observables->x_up[j] += part->x[j];
        observables->p_up[j] += part->p[j];
      }
    }
    else{
      observables->mass_down += 1;
      for (int j=0; j<dim; j++){
        ke_down += pow(part->p[j],2);
        observables->e_down += part->pot_new;
        observables->x_down[j] += part->x[j];
        observables->p_down[j] += part->p[j];
      }
    }
    observables->ke_up += 0.5 * ke_up;
    observables->ke_down += 0.5 * ke_down;
    part = part->next;
  }
  /* normalise */
  for (int i=0; i<dim; i++){
    observables->x_up[i]        /= observables->mass_up;
    observables->x_down[i]      /= observables->mass_down;
    observables->p_up[i]        /= observables->mass_up;
    observables->p_down[i]      /= observables->mass_down; // computing an equally weighted average
    observables->ke_up          /= observables->mass_up;
    observables->ke_down        /= observables->mass_down;
    observables->e_up           = observables->ke_up + observables->e_up/observables->mass_up;
    observables->e_down         = observables->ke_down + observables->e_down/observables->mass_down;
    observables->mass_up        /= (1. * observables->npart);
    observables->mass_down      /= (1. * observables->npart);
  }
}

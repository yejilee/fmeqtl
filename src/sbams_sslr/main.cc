using namespace std;

#include "controller.h"
#include <stdlib.h>

int main(int argc, char **argv){
  
  char grid_file[128];
  char data_file[128];
  char out_file[128];
  char gene_name[64];

  char prior_file[128];
  
  
  memset(gene_name,0,64);
  memset(grid_file,0,128);
  memset(data_file,0,128);
  memset(out_file,0,128);
  
  memset(prior_file,0,128);
  
  int prep = 0;

  int burnin = 25000;
  int rep = 50000;
  
  double abf_option = 0.5;
  int abf_prior_option = 1;

  int post_r2 = 0;
  
  
  double pes = 1.0;
  double lambda = 0.5;
  
  int run_option = 1;
  int meta_option = 0;
  
  for(int i=1;i<argc;i++){
     

    // required data files and additional info

    if(strcmp(argv[i], "-g")==0 || strcmp(argv[i], "-grid")==0){
      strcpy(grid_file,argv[++i]);
      continue;
    } 
    
    if(strcmp(argv[i], "-d")==0 || strcmp(argv[i], "-data")==0){
      strcpy(data_file,argv[++i]);
      continue;
    }

    // prior file
    if(strcmp(argv[i], "-prior")==0 || strcmp(argv[i], "-p")==0 ){
      strcpy(prior_file,argv[++i]);
      continue;
    }
    
    // output file
    if(strcmp(argv[i], "-o")==0){
      strcpy(out_file, argv[++i]);
      continue;
    }


    // mcmc details

    if(strcmp(argv[i], "-b")==0 || strcmp(argv[i], "-burnin")==0 ){
      burnin = atoi(argv[++i]);  
      continue;
    }

    if(strcmp(argv[i], "-r")==0 || strcmp(argv[i], "-rep")==0  ){
      rep = atoi(argv[++i]);
      continue;
    }

        

    // prior options

    if(strcmp(argv[i], "-ens")==0 ){
      pes = atof(argv[++i]);
      continue;
    }
    
    if(strcmp(argv[i],"-meta")==0){
      meta_option = 1;
      continue;
    }


    // abf option
    if(strcmp(argv[i],"-abf")==0){
      abf_option = atof(argv[++i]);
      continue;
    }


    

    
    
    
    // these are hidden options, not documented.

    // run options
    
    if(strcmp(argv[i], "-mcmc")==0 ){ // mcmc
      run_option = 1;
      continue;
    }



    if(strcmp(argv[i], "-scan")==0 ){ // single snp analysis
      run_option = 2;
      continue;
    }

    
    if(strcmp(argv[i], "-ga")==0 ){ // gene-level analysis
      run_option = 3;
      continue;
    }

    
    if(strcmp(argv[i], "-prep_hm")==0){ // prepare for hierarchical model
      run_option = 4;
      continue;
    }
    
    if(strcmp(argv[i], "-lambda")==0 ){
      lambda = atof(argv[++i]);
      continue;
    }

    if(strcmp(argv[i], "-n")==0 ){
      strcpy(gene_name, argv[++i]);
      continue;
    }

    if(strcmp(argv[i], "-post_r2")==0 ){
      post_r2 = 1;
      continue;
    }

    
    
    
    fprintf(stderr, "Error: unknown option \"%s\"\n",argv[i]);
    exit(1);
   
  }
  
  controller con(data_file,grid_file);
  con.set_outfile(out_file);
  con.set_meta_option(meta_option);
  con.set_gene(gene_name);
  con.set_sslr_option(abf_option);
  con.set_post_r2(post_r2);
  
  
  
 

  
  if(run_option == 4){
    con.prep_single_bf();
    return 1;
  }
  
  // all other options need prior
  
  if(strlen(prior_file)==0)
    con.set_prior(pes,lambda);
  else
    con.set_prior(prior_file);
  
  if(run_option == 1){
    con.run_mcmc(burnin,rep);
    return 1;
  }

  if(run_option == 2){
    con.scan();
    return 1;
  }

  if(run_option == 3){
    con.gene_level_analysis();
    return 1;
  }

}



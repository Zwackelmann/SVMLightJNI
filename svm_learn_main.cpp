/***********************************************************************/
/*                                                                     */
/*   svm_learn_main.c                                                  */
/*                                                                     */
/*   Command line interface to the learning module of the              */
/*   Support Vector Machine.                                           */
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 02.07.02                                                    */
/*                                                                     */
/*   Copyright (c) 2000  Thorsten Joachims - All rights reserved       */
/*                                                                     */
/*   This software is available for non-commercial use only. It must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/***********************************************************************/


/* uncomment, if you want to use svm-learn out of C++ */
/* extern "C" { */ 
# include "svm_common.h"
# include "svm_learn.h"
/*}*/ 

char docfile_[200];           /* file with training examples */
char modelfile_[200];         /* file for resulting classifier */
char restartfile[200];       /* file with initial alphas */

void   read_input_parameters(int, char **, char *, char *, char *, long *, 
			     LEARN_PARM *, KERNEL_PARM *);
void   wait_any_key();
void   print_help();


/*
int main (int argc, char* argv[])
{  
  DOC **docs;
  long totwords,totdoc,i;
  double *target;
  double *alpha_in=NULL;
  KERNEL_CACHE *kernel_cache;
  LEARN_PARM learn_parm;
  KERNEL_PARM kernel_parm;
  MODEL *model=(MODEL *)my_malloc(sizeof(MODEL));

  read_input_parameters(argc,argv,docfile_,modelfile_,restartfile,&verbosity,
			&learn_parm,&kernel_parm);
  read_documents(docfile_,&docs,&target,&totwords,&totdoc);
  if(restartfile[0]) alpha_in=read_alphas(restartfile,totdoc);

	FILE * dump = NULL;
	char* traindump = (char *) my_malloc(sizeof(char)*25);
	sprintf(traindump,"maintraindump%d.dat",1);
	
	int lengthcnt = 20;
	int namecnt=2;
  while((dump = fopen(traindump,"r+")) != NULL) {
		fclose(dump);
		printf("traindump is already there: %s\n",traindump);
		if (strlen(traindump) >= lengthcnt) {
			free(traindump);
			lengthcnt =+ 20; 
			traindump = (char *) my_malloc(sizeof(char)*lengthcnt);
		}
		sprintf(traindump,"maintraindump%d.dat",namecnt++);
	}
	
	printf("------------------------------ Writing traindump to file %s",traindump);
  if ((dump = fopen(traindump,"w")) == NULL) {
		perror("Doesnt work!\n");
		exit(1);
	}
	
	printf("\n|||||||||||||||||||||||||||||||||| dumping ..\n");
	long int z = 0;
	long int y = 0;
  fprintf(dump,"totaldocuments: %ld \n",totdoc);
	while(z<(totdoc)) {
    fprintf(dump,"(%ld) (QID: %ld) (CF: %.16g) (SID: %ld) ",docs[z]->docnum,docs[z]->queryid,docs[z]->costfactor,docs[z]->slackid);
		SVECTOR *v = docs[z]->fvec;
    fprintf(dump,"(NORM:%.32g) (UD:%s) (KID:%ld) (VL:%p) (F:%.32g) %.32g ",v->twonorm_sq,(v->userdefined == NULL ? "" : v->userdefined),v->kernel_id,v->next,v->factor,target[z]);
		if (v != NULL && v->words != NULL) {
			while ((v->words[y]).wnum) {
				fprintf(dump,"%ld:%.32g ",(v->words[y]).wnum, (v->words[y]).weight);
				y++;
			}
		} else 
				fprintf(dump, "NULL WORTE\n");
		fprintf(dump,"\n");
		y=0;
		z++;
  }

	
	fprintf(dump,"---------------------------------------------------\n");
	fprintf(dump,"kernel_type: %ld\n",kernel_parm.kernel_type);
	fprintf(dump,"poly_degree: %ld\n",kernel_parm.poly_degree);
	fprintf(dump,"rbf_gamma: %.32g\n",kernel_parm.rbf_gamma);
	fprintf(dump,"coef_lin: %.32g\n",kernel_parm.coef_lin);
	fprintf(dump,"coef_const: %.32g\n",kernel_parm.coef_const);
	fprintf(dump,"custom: %s\n",kernel_parm.custom);
	
	fprintf(dump,"type: %ld\n",learn_parm.type);
	fprintf(dump,"svm_c: %.32g\n",learn_parm.svm_c);
	fprintf(dump,"eps: %.32g\n",learn_parm.eps);
	fprintf(dump,"svm_costratio: %.32g\n",learn_parm.svm_costratio);
	fprintf(dump,"transduction_posratio: %.32g\n",learn_parm.transduction_posratio);
	fprintf(dump,"biased_hyperplane: %ld\n",learn_parm.biased_hyperplane);
	fprintf(dump,"svm_maxqpsize: %ld\n",learn_parm.svm_maxqpsize);
	fprintf(dump,"svm_newvarsinqp: %ld\n",learn_parm.svm_newvarsinqp);
	fprintf(dump,"epsilon_crit: %.32g\n",learn_parm.epsilon_crit);
	fprintf(dump,"epsilon_shrink: %.32g\n",learn_parm.epsilon_shrink);
	fprintf(dump,"svm_iter_to_shrink: %ld\n",learn_parm.svm_iter_to_shrink);
	fprintf(dump,"remove_inconsistent: %ld\n",learn_parm.remove_inconsistent);
	fprintf(dump,"skip_final_opt_check: %ld\n",learn_parm.skip_final_opt_check);
	fprintf(dump,"compute_loo: %ld\n",learn_parm.compute_loo);
	fprintf(dump,"rho: %.32g\n",learn_parm.rho);
	fprintf(dump,"xa_depth: %ld\n",learn_parm.xa_depth);
	fprintf(dump,"predfile: %s\n",learn_parm.predfile);
	fprintf(dump,"alphafile: %s\n",learn_parm.alphafile);
	fprintf(dump,"epsilon_const: %.32g\n",learn_parm.epsilon_const);
	fprintf(dump,"epsilon_a: %.32g\n",learn_parm.epsilon_a);
	fprintf(dump,"opt_precision: %.32g\n",learn_parm.opt_precision);
	fprintf(dump,"svm_c_steps: %ld\n",learn_parm.svm_c_steps);
	fprintf(dump,"svm_c_factor: %.32g\n",learn_parm.svm_c_factor);
	fprintf(dump,"svm_costratio_unlab: %.32g\n",learn_parm.svm_costratio_unlab);
	fprintf(dump,"svm_unlabbound: %.32g\n",learn_parm.svm_unlabbound);

	
  if(kernel_parm.kernel_type == LINEAR) { // don't need the cache
    kernel_cache=NULL;
  }
  else {
    // Always get a new kernel cache. It is not possible to use the
    // same cache for two different training runs
    kernel_cache=kernel_cache_init(totdoc,learn_parm.kernel_cache_size);
  }

  if(learn_parm.type == CLASSIFICATION) {
    svm_learn_classification(docs,target,totdoc,totwords,&learn_parm,
			     &kernel_parm,kernel_cache,model,alpha_in);
  }
  else if(learn_parm.type == REGRESSION) {
    svm_learn_regression(docs,target,totdoc,totwords,&learn_parm,
			 &kernel_parm,&kernel_cache,model);
  }
  else if(learn_parm.type == RANKING) {
    svm_learn_ranking(docs,target,totdoc,totwords,&learn_parm,
		      &kernel_parm,&kernel_cache,model);
  }
  else if(learn_parm.type == OPTIMIZATION) {
    svm_learn_optimization(docs,target,totdoc,totwords,&learn_parm,
			   &kernel_parm,kernel_cache,model,alpha_in);
  }
	
	fprintf(dump,"totwords: %ld\n",learn_parm.totwords);
	
	printf("|||||||||||||||||||||||||||||||||| z: %ld, totdoc: %ld\n",z,totdoc);
	
	fclose(dump);

  if(kernel_cache) {
    // Free the memory used for the cache.
    kernel_cache_cleanup(kernel_cache);
  }

  // Warning: The model contains references to the original data 'docs'.
  // If you want to free the original data, and only keep the model, you 
  // have to make a deep copy of 'model'.
  // deep_copy_of_model=copy_model(model);
  write_model(modelfile_,model);

  free(alpha_in);
  free_model(model,0);
  for(i=0;i<totdoc;i++) 
    free_example(docs[i],1);
  free(docs);
  free(target);

  return(0);
}
*/

/*---------------------------------------------------------------------------*/

/* void read_input_parameters(int argc,char *argv[],char *docfile_,char *modelfile,
			   char *restartfile,long *verbosity,
			   LEARN_PARM *learn_parm,KERNEL_PARM *kernel_parm)
{
  long i;
  char type[100];
  
  // set default
  strcpy (modelfile_, "svm_model");
  strcpy (learn_parm->predfile, "trans_predictions");
  strcpy (learn_parm->alphafile, "");
  strcpy (restartfile, "");
  (*verbosity)=1;
  learn_parm->biased_hyperplane=1;
  learn_parm->sharedslack=0;
  learn_parm->remove_inconsistent=0;
  learn_parm->skip_final_opt_check=0;
  learn_parm->svm_maxqpsize=10;
  learn_parm->svm_newvarsinqp=0;
  learn_parm->svm_iter_to_shrink=-9999;
  learn_parm->maxiter=100000;
  learn_parm->kernel_cache_size=40;
  learn_parm->svm_c=0.0;
  learn_parm->eps=0.1;
  learn_parm->transduction_posratio=-1.0;
  learn_parm->svm_costratio=1.0;
  learn_parm->svm_costratio_unlab=1.0;
  learn_parm->svm_unlabbound=1E-5;
  learn_parm->epsilon_crit=0.001;
  learn_parm->epsilon_a=1E-15;
  learn_parm->compute_loo=0;
  learn_parm->rho=1.0;
  learn_parm->xa_depth=0;
  kernel_parm->kernel_type=0;
  kernel_parm->poly_degree=3;
  kernel_parm->rbf_gamma=1.0;
  kernel_parm->coef_lin=1;
  kernel_parm->coef_const=1;
  strcpy(kernel_parm->custom,"empty");
  strcpy(type,"c");

  for(i=1;(i<argc) && ((argv[i])[0] == '-');i++) {
    switch ((argv[i])[1]) 
      { 
      case '?': print_help(); exit(0);
      case 'z': i++; strcpy(type,argv[i]); break;
      case 'v': i++; (*verbosity)=atol(argv[i]); break;
      case 'b': i++; learn_parm->biased_hyperplane=atol(argv[i]); break;
      case 'i': i++; learn_parm->remove_inconsistent=atol(argv[i]); break;
      case 'f': i++; learn_parm->skip_final_opt_check=!atol(argv[i]); break;
      case 'q': i++; learn_parm->svm_maxqpsize=atol(argv[i]); break;
      case 'n': i++; learn_parm->svm_newvarsinqp=atol(argv[i]); break;
      case '#': i++; learn_parm->maxiter=atol(argv[i]); break;
      case 'h': i++; learn_parm->svm_iter_to_shrink=atol(argv[i]); break;
      case 'm': i++; learn_parm->kernel_cache_size=atol(argv[i]); break;
      case 'c': i++; learn_parm->svm_c=atof(argv[i]); break;
      case 'w': i++; learn_parm->eps=atof(argv[i]); break;
      case 'p': i++; learn_parm->transduction_posratio=atof(argv[i]); break;
      case 'j': i++; learn_parm->svm_costratio=atof(argv[i]); break;
      case 'e': i++; learn_parm->epsilon_crit=atof(argv[i]); break;
      case 'o': i++; learn_parm->rho=atof(argv[i]); break;
      case 'k': i++; learn_parm->xa_depth=atol(argv[i]); break;
      case 'x': i++; learn_parm->compute_loo=atol(argv[i]); break;
      case 't': i++; kernel_parm->kernel_type=atol(argv[i]); break;
      case 'd': i++; kernel_parm->poly_degree=atol(argv[i]); break;
      case 'g': i++; kernel_parm->rbf_gamma=atof(argv[i]); break;
      case 's': i++; kernel_parm->coef_lin=atof(argv[i]); break;
      case 'r': i++; kernel_parm->coef_const=atof(argv[i]); break;
      case 'u': i++; strcpy(kernel_parm->custom,argv[i]); break;
      case 'l': i++; strcpy(learn_parm->predfile,argv[i]); break;
      case 'a': i++; strcpy(learn_parm->alphafile,argv[i]); break;
      case 'y': i++; strcpy(restartfile,argv[i]); break;
      default: printf("\nUnrecognized option %s!\n\n",argv[i]);
	       print_help();
	       exit(0);
      }
  }
  if(i>=argc) {
    printf("\nNot enough input parameters!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  strcpy (docfile_, argv[i]);
  if((i+1)<argc) {
    strcpy (modelfile_, argv[i+1]);
  }
  if(learn_parm->svm_iter_to_shrink == -9999) {
    if(kernel_parm->kernel_type == LINEAR) 
      learn_parm->svm_iter_to_shrink=2;
    else
      learn_parm->svm_iter_to_shrink=100;
  }
  if(strcmp(type,"c")==0) {
    learn_parm->type=CLASSIFICATION;
  }
  else if(strcmp(type,"r")==0) {
    learn_parm->type=REGRESSION;
  }
  else if(strcmp(type,"p")==0) {
    learn_parm->type=RANKING;
  }
  else if(strcmp(type,"o")==0) {
    learn_parm->type=OPTIMIZATION;
  }
  else if(strcmp(type,"s")==0) {
    learn_parm->type=OPTIMIZATION;
    learn_parm->sharedslack=1;
  }
  else {
    printf("\nUnknown type '%s': Valid types are 'c' (classification), 'r' regession, and 'p' preference ranking.\n",type);
    wait_any_key();
    print_help();
    exit(0);
  }    
  if((learn_parm->skip_final_opt_check) 
     && (kernel_parm->kernel_type == LINEAR)) {
    printf("\nIt does not make sense to skip the final optimality check for linear kernels.\n\n");
    learn_parm->skip_final_opt_check=0;
  }    
  if((learn_parm->skip_final_opt_check) 
     && (learn_parm->remove_inconsistent)) {
    printf("\nIt is necessary to do the final optimality check when removing inconsistent \nexamples.\n");
    wait_any_key();
    print_help();
    exit(0);
  }    
  if((learn_parm->svm_maxqpsize<2)) {
    printf("\nMaximum size of QP-subproblems not in valid range: %ld [2..]\n",learn_parm->svm_maxqpsize); 
    wait_any_key();
    print_help();
    exit(0);
  }
  if((learn_parm->svm_maxqpsize<learn_parm->svm_newvarsinqp)) {
    printf("\nMaximum size of QP-subproblems [%ld] must be larger than the number of\n",learn_parm->svm_maxqpsize); 
    printf("new variables [%ld] entering the working set in each iteration.\n",learn_parm->svm_newvarsinqp); 
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->svm_iter_to_shrink<1) {
    printf("\nMaximum number of iterations for shrinking not in valid range: %ld [1,..]\n",learn_parm->svm_iter_to_shrink);
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->svm_c<0) {
    printf("\nThe C parameter must be greater than zero!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->transduction_posratio>1) {
    printf("\nThe fraction of unlabeled examples to classify as positives must\n");
    printf("be less than 1.0 !!!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->svm_costratio<=0) {
    printf("\nThe COSTRATIO parameter must be greater than zero!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->epsilon_crit<=0) {
    printf("\nThe epsilon parameter must be greater than zero!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  if(learn_parm->rho<0) {
    printf("\nThe parameter rho for xi/alpha-estimates and leave-one-out pruning must\n");
    printf("be greater than zero (typically 1.0 or 2.0, see T. Joachims, Estimating the\n");
    printf("Generalization Performance of an SVM Efficiently, ICML, 2000.)!\n\n");
    wait_any_key();
    print_help();
    exit(0);
  }
  if((learn_parm->xa_depth<0) || (learn_parm->xa_depth>100)) {
    printf("\nThe parameter depth for ext. xi/alpha-estimates must be in [0..100] (zero\n");
    printf("for switching to the conventional xa/estimates described in T. Joachims,\n");
    printf("Estimating the Generalization Performance of an SVM Efficiently, ICML, 2000.)\n");
    wait_any_key();
    print_help();
    exit(0);
  }
}*/

void wait_any_key()
{
  printf("\n(more)\n");
  (void)getc(stdin);
}

/*void print_help()
{
  printf("\nSVM-light %s: Support Vector Machine, learning module     %s\n",VERSION,VERSION_DATE);
  copyright_notice();
  printf("   usage: svm_learn [options] example_file model_file\n\n");
  printf("Arguments:\n");
  printf("         example_file-> file with training data\n");
  printf("         model_file  -> file to store learned decision rule in\n");

  printf("General options:\n");
  printf("         -?          -> this help\n");
  printf("         -v [0..3]   -> verbosity level (default 1)\n");
  printf("Learning options:\n");
  printf("         -z {c,r,p}  -> select between classification (c), regression (r),\n");
  printf("                        and preference ranking (p) (default classification)\n");
  printf("         -c float    -> C: trade-off between training error\n");
  printf("                        and margin (default [avg. x*x]^-1)\n");
  printf("         -w [0..]    -> epsilon width of tube for regression\n");
  printf("                        (default 0.1)\n");
  printf("         -j float    -> Cost: cost-factor, by which training errors on\n");
  printf("                        positive examples outweight errors on negative\n");
  printf("                        examples (default 1) (see [4])\n");
  printf("         -b [0,1]    -> use biased hyperplane (i.e. x*w+b>0) instead\n");
  printf("                        of unbiased hyperplane (i.e. x*w>0) (default 1)\n");
  printf("         -i [0,1]    -> remove inconsistent training examples\n");
  printf("                        and retrain (default 0)\n");
  printf("Performance estimation options:\n");
  printf("         -x [0,1]    -> compute leave-one-out estimates (default 0)\n");
  printf("                        (see [5])\n");
  printf("         -o ]0..2]   -> value of rho for XiAlpha-estimator and for pruning\n");
  printf("                        leave-one-out computation (default 1.0) (see [2])\n");
  printf("         -k [0..100] -> search depth for extended XiAlpha-estimator \n");
  printf("                        (default 0)\n");
  printf("Transduction options (see [3]):\n");
  printf("         -p [0..1]   -> fraction of unlabeled examples to be classified\n");
  printf("                        into the positive class (default is the ratio of\n");
  printf("                        positive and negative examples in the training data)\n");
  printf("Kernel options:\n");
  printf("         -t int      -> type of kernel function:\n");
  printf("                        0: linear (default)\n");
  printf("                        1: polynomial (s a*b+c)^d\n");
  printf("                        2: radial basis function exp(-gamma ||a-b||^2)\n");
  printf("                        3: sigmoid tanh(s a*b + c)\n");
  printf("                        4: user defined kernel from kernel.h\n");
  printf("         -d int      -> parameter d in polynomial kernel\n");
  printf("         -g float    -> parameter gamma in rbf kernel\n");
  printf("         -s float    -> parameter s in sigmoid/poly kernel\n");
  printf("         -r float    -> parameter c in sigmoid/poly kernel\n");
  printf("         -u string   -> parameter of user defined kernel\n");
  printf("Optimization options (see [1]):\n");
  printf("         -q [2..]    -> maximum size of QP-subproblems (default 10)\n");
  printf("         -n [2..q]   -> number of new variables entering the working set\n");
  printf("                        in each iteration (default n = q). Set n<q to prevent\n");
  printf("                        zig-zagging.\n");
  printf("         -m [5..]    -> size of cache for kernel evaluations in MB (default 40)\n");
  printf("                        The larger the faster...\n");
  printf("         -e float    -> eps: Allow that error for termination criterion\n");
  printf("                        [y [w*x+b] - 1] >= eps (default 0.001)\n");
  printf("         -y [0,1]    -> restart the optimization from alpha values in file\n");
  printf("                        specified by -a option. (default 0)\n");
  printf("         -h [5..]    -> number of iterations a variable needs to be\n"); 
  printf("                        optimal before considered for shrinking (default 100)\n");
  printf("         -f [0,1]    -> do final optimality check for variables removed\n");
  printf("                        by shrinking. Although this test is usually \n");
  printf("                        positive, there is no guarantee that the optimum\n");
  printf("                        was found if the test is omitted. (default 1)\n");
  printf("         -y string   -> if option is given, reads alphas from file with given\n");
  printf("                        and uses them as starting point. (default 'disabled')\n");
  printf("         -# int      -> terminate optimization, if no progress after this\n");
  printf("                        number of iterations. (default 100000)\n");
  printf("Output options:\n");
  printf("         -l string   -> file to write predicted labels of unlabeled\n");
  printf("                        examples into after transductive learning\n");
  printf("         -a string   -> write all alphas to this file after learning\n");
  printf("                        (in the same order as in the training set)\n");
  wait_any_key();
  printf("\nMore details in:\n");
  printf("[1] T. Joachims, Making Large-Scale SVM Learning Practical. Advances in\n");
  printf("    Kernel Methods - Support Vector Learning, B. Sch�lkopf and C. Burges and\n");
  printf("    A. Smola (ed.), MIT Press, 1999.\n");
  printf("[2] T. Joachims, Estimating the Generalization performance of an SVM\n");
  printf("    Efficiently. International Conference on Machine Learning (ICML), 2000.\n");
  printf("[3] T. Joachims, Transductive Inference for Text Classification using Support\n");
  printf("    Vector Machines. International Conference on Machine Learning (ICML),\n");
  printf("    1999.\n");
  printf("[4] K. Morik, P. Brockhausen, and T. Joachims, Combining statistical learning\n");
  printf("    with a knowledge-based approach - A case study in intensive care  \n");
  printf("    monitoring. International Conference on Machine Learning (ICML), 1999.\n");
  printf("[5] T. Joachims, Learning to Classify Text Using Support Vector\n");
  printf("    Machines: Methods, Theory, and Algorithms. Dissertation, Kluwer,\n");
  printf("    2002.\n\n");
}*/



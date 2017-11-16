#include <iostream>
#include <math.h>
#include <random>

double scalar_gaussian_cost(float p, float mu, float sigma)
{
	double cost = pow((p-mu),2);
	return cost;
}
double gaussian_proposal(float start, float r)
{
	double end;
	end = start + r;
	return end;
}
int main()
{	
	int verbose = 1;
	double gaussian_sd = 5; // parameter for the cost function
	double* p; //Markov chain
	int nsteps = 5; // length of Markov chain
	p = new double[nsteps+1];
	p[0] = 10; //initialize Markov chain
	double step_size = 0.1; // step size
	//Create normal random number stream. One for each MCMC step.
	int N_seed = 3;
	std::default_random_engine generator(N_seed);
	std::normal_distribution<double> distribution (0.0,step_size);
	double* NR;
	NR = new double[nsteps];
	for(int i=0;i<nsteps;i++)
	{
		NR[i] = distribution(generator);
	}
	//Create uniform random number stream. One for each MCMC step.
	int U_seed = 2;
    std::mt19937 gen(U_seed); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
	double* UR;
	UR = new double[nsteps];
	for(int i=0;i<nsteps;i++)
	{
		UR[i] = dis(gen);
	}
	//iterate over steps
	for(int i=0;i<nsteps;i++)
	{
		double current_energy = scalar_gaussian_cost(p[i],0,gaussian_sd); // calculate current energy
		double proposal = gaussian_proposal(p[i],NR[i]); // propose a new parameter value
		double proposal_energy = scalar_gaussian_cost(proposal,0,gaussian_sd); // calculate energy of proposed value
		double dE = proposal_energy - current_energy; // calculate the energy difference
		if(dE<0)
		{
			//accept a favorable move with probability 1
			p[i+1] = proposal;
		}	
		else
		{
			//accept an unfavorable move with some probability, h (Metropolis criterion)
			float h = exp(-dE);
    		if(UR[i]<h) //Monte Carlo step to determine acceptance
    		{
    			p[i+1] = proposal;
    		}
    		else{p[i+1] = p[i];}
		}
		if(verbose==1)
		{
			std:: cout << "Step " <<i<<" parameter:" << p[i] << " energy: " << scalar_gaussian_cost(p[i],0,gaussian_sd) <<"\n";
			std:: cout << "Gaussian random number " << NR[i] << " Proposed value " << proposal << " Proposed energy: " << proposal_energy<<"\n";
			std:: cout << "dE " << dE << " acceptance probability = min(1, exp(-dE)) = min(1, " << exp(-dE) << ")" << "\n";
			std:: cout << "Uniform random number for this step: " << UR[i] << "\n";
			std:: cout << "Step " << i+1 << " parameter: " << p[i+1] << "\n";
			std::cout << "\n";
	 	}
	}
	return 0;
}
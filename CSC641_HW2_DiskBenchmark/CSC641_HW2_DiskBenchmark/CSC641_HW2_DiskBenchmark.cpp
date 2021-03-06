/* Disk Benchmark Program for Quantum Atlas III. The following information is provided by Dr. Jozo Dujmovic on the instructions handout.
xmax = 8057 cylinders
capacity = 9.1 GB
rotations per minute = 7200RPM
starting index = 1896
maximum time to stretch whole disk = 1.5455
*/
#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdlib.h>

#define WINPAUSE system("pause")

using namespace std;

// urn() is a random number generator that produces standard uniform random numbers.
// We want good quality of randomness but also a uniform distribution, or that each element
// has equal probability of being chosen.
double urn(void)
{
	static unsigned long r[18] = { 131071,43691,262657,649657,274177,
		524287,121369,61681,179951,513239,
		333667,909091,1777,8617,87211,
		174763,65537,0 },
		xmod = 1048573;
	static double rnmax = xmod;
	static int i = 17, j = 16, k = 0, n = 18;
	double rn;
	r[i] = (r[j] + r[k]) % xmod;
	rn = r[i] / rnmax;
	i = (i + 1) % n;
	j = (j + 1) % n;
	k = (k + 1) % n;
	return rn;
}


// Shuffler method ensures that there is no correlation between the numbers generated in the list produced in urn().
double shuffler(void)
{
	static int n = 200, firstcall = 1;
	static double table[200];
	double rnumber;
	int i, itable;
	if (firstcall)
	{
		for (i = 0; i<n; i++) table[i] = urn(); // populating array with uniform random numbers
		firstcall = 0;
	}
	itable = int(n * urn()); // Uniform selection from table
	rnumber = table[itable];
	table[itable] = urn(); // restore array element
	return rnumber;
}

int main()
{
	// Initializing constant values
	const int n = 200, xMax = 8057, xInit = 1686;
	const double tMax = 1.5455, c = 0.3197, r = 0.3868;

	double dq[n]; // dq representing Q components

				  // Using Shuffler Method for RNG
	for (int i = 0; i < n; i++) {
		dq[i] = shuffler();
	}
	//for (int i = 0; i < n; i++) {
	//	cout << dq[i] << endl;
	//}

	// dq should be filled with uniformly distributed random numbers
	// pick an initial position in the array call it "X".
	double X = dq[n - 1];
	double min_dist = fabs(dq[0] - X); // initialize minimum distance to be traveled
	int ind_track = 0;

	// variables for holding data in order to calculate average seek distance and time
	double ttl_seek_dist = 0;
	double ttl_seek_time = 0;
	double tX = 0;

	for (int j = 0; j < 100; j++) {
		// Loop through the array and find shortest distance, keep track of index to shortest distance
		for (int i = 0; i < n - 1; i++) {
			if (fabs(dq[i] - X) < min_dist) {
				min_dist = dq[i];
				ind_track = i; // ind_track represents index in array of the new position
			}
		}

		// Replace dq[i] with random value and X is set to i
		cout << endl << "dq[ind_track] " << dq[ind_track] << endl << "X: " << X;
		double dSeek = fabs(dq[ind_track] - X);

		if (dq[ind_track] > X) {
			tX = ((c*r*(min_dist*xMax - X * xMax)) / (pow((X*xMax - 1), 1 - r))) + tMax + c * pow((min_dist*xMax - 1), r);
		}
		else {
			tX = (tMax + c * pow((min_dist*xMax - 1), r));
		}

		X = dq[ind_track];
		dq[ind_track] = shuffler();

		// Compute seek distance and seek time
		double tSeek = (tMax * (sqrt(min_dist*xMax / xMax))); // theoretical value

		cout << endl << "theoretical time taken: " << tSeek << endl << "calculated time taken: " << tX << endl << "distance: " << dSeek * xMax << endl;
		ttl_seek_dist += dSeek;
		ttl_seek_time += tX;
	}

	// Compute average seek distance and time for n > 10000 disk accesses
	cout << endl << endl << "Average seek distance for 10000 trials: " << (ttl_seek_dist / 100)*xMax << endl;
	cout << "Average seek time for 10000 trials: " << ttl_seek_time / 100;

	cout << endl;
	WINPAUSE;
	return 0;
}

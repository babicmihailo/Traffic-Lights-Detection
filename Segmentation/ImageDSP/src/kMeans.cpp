#include "ColorSpaces.h"
#include "kMeans.h"
#include <list>
#include "SIFTLib.h"
#include "NoiseReduction.h"

using namespace ezsift;
using namespace std;
#define CONVERGENCE_THRESHOLD 1e-8

// Predefined list of colors for coloring separate segments
static const int NUM_COLORS = 16;
static const uchar RGBColors[NUM_COLORS][3] = { { 255, 0, 0 },{ 0, 255, 0 },{ 0, 0, 255 },{ 0, 255, 255 },{ 255, 255, 0 },{ 255, 0, 255 },{ 0, 0, 0 },{ 255, 255, 255 },{ 128, 0, 0 },{ 0, 128, 0 },{ 0, 0, 128 },{ 0, 128, 128 },{ 128, 128, 0 },{ 128, 0, 128 },{ 128, 128, 128 },{ 64, 128, 64 } };


// Calcilate Euclid distance between two points in n-dimensional space
static double getDistance(
	const vector<double>& a,
	const vector<double>& b
) {
	double temp = 0.0;
	auto it1 = a.begin();
	auto it2 = b.begin();
	for(; (it1 != a.end()) && (it2 != b.end()); it1++, it2++){
		temp += (*it1 - *it2) * (*it1 - *it2);
	}
	return sqrt(temp);
}

// Find nearest cluster for the given point
static int whichIsNearest(
	const vector<KMeansPoint>& centroids,
	KMeansPoint pt
) {
	double minDistance = 0;
	int minLabel = 0;
    for(size_t i = 0; i < centroids.size(); i++){
		double tempDistance = getDistance(centroids[i].feature, pt.feature);
		if(i == 0 || tempDistance < minDistance){
			minDistance = tempDistance;
			minLabel = i;
		}
	}
	return minLabel;
}

// Calculate kMeans
static vector<vector<int>> kMeans(
	const vector<KMeansPoint>& points,
	int nFeatures,
	int K
) {
	// Total number of points
	int nSamples = points.size();

	// Randomize initial centroids
	vector<KMeansPoint> centroids;
	for(int k = 0; k < K; k++){
		int rand_int = rand() % nSamples;
		centroids.push_back(points[rand_int]);
	}

	// Create empty vector for each cluster
	vector<vector<int> > cluster;
	for(int k = 0; k < K; k++){
		vector<int> vectemp;
		cluster.push_back(vectemp);
	}

	// Iteration counter
	int counter = 0;

	// Iteratively find better centroids
	while(true){

		// Clear each cluster
		for(int k = 0; k < K; k++){
			cluster[k].clear();
		}

		// Set convergence flag to TRUE
		bool converge = true;

		// For every sample, find which cluster it belongs to,
		// By comparing the distance between it and each clusters' centroid.
		for(int m = 0; m < nSamples; m++){
			int which = whichIsNearest(centroids, points[m]);
			cluster[which].push_back(m);
		}

		// For every cluster, re-calculate its centroid.
		for(int k = 0; k < K; k++){
			int clusterSize = cluster[k].size();

			vector<double> vectemp = vector<double>(nFeatures, 0);
			for(int i = 0; i < clusterSize; i++) {
				for(int j = 0; j < nFeatures; j++)
					vectemp[j] = 
						vectemp[j] + 
						points[cluster[k][i]].feature[j] / (double)clusterSize;
			}
			// If centroid position change is greater then
			// CONVERGENCE_THRESHOLD set convergence flag to false.
			if(
				getDistance(vectemp, centroids[k].feature) 
					> CONVERGENCE_THRESHOLD
			){
				converge = false;
			}



			centroids[k].feature = vectemp;
		}

		// If convergence achieved break the loop
		if(converge){
			break;
		}
		counter++;
	}

	// Return cluster vectors
	return cluster;
}

void IntensityBasedKMeans(
	uchar input[],
	int x_size,
	int y_size,
	int K
) {
	uchar* Y_buf = new uchar[x_size*y_size];
	char* U_buf = new char[x_size*y_size / 4];
	char* V_buf = new char[x_size*y_size / 4];
	const int numberOfFeatures = 1;


	RGBtoYUV420(input, x_size, y_size, Y_buf, U_buf, V_buf);

	//Smooth the image
	performGaussFilter(Y_buf, x_size, y_size, 5, 0.1);

	// Calculate feature vector for each point
	vector<KMeansPoint> points;
	for(int i = 0; i < x_size; i++){
		for(int j = 0; j < y_size; j++){
			KMeansPoint newPoint = { i, j };
			newPoint.feature.push_back(Y_buf[j * x_size + i]);

			points.push_back(newPoint);
		}
	}

	vector<vector<int>> clusters;
	clusters = kMeans(points, numberOfFeatures, K);

	// Paint each segment using predefined colors
	for(int k = 0; k < K; k++){
		for(int index : clusters[k]){
			int i = points[index].x;
			int j = points[index].y;
			input[3 * j * x_size + 3 * i] = RGBColors[k][0];
			input[3 * j * x_size + 3 * i + 1] = RGBColors[k][1];
			input[3 * j * x_size + 3 * i + 2] = RGBColors[k][2];
		}
	}

	delete[] Y_buf;
	delete[] U_buf;
	delete[] V_buf;
}

// Color Based kMeans segmentation
void ColorBasedKMeans(
	uchar input[],
	int x_size,
	int y_size,
	int K
) {
	const int numberOfFeatures = 3;

	// Calculate feature vector for each point
	vector<KMeansPoint> points;
	for(int i = 0; i < x_size; i++){
		for(int j = 0; j < y_size; j++){
			KMeansPoint newPoint = { i, j };
			newPoint.feature.push_back(input[j * 3 * x_size + i * 3]);
			newPoint.feature.push_back(input[j * 3 * x_size + i * 3 + 1]);
			newPoint.feature.push_back(input[j * 3 * x_size + i * 3 + 2]);

			points.push_back(newPoint);
		}
	}

	vector<vector<int>> clusters;
	clusters = kMeans(points, numberOfFeatures, K);

	// Paint each segment using predefined colors
	for(int k = 0; k < K; k++){
		for(int index : clusters[k]){
			int i = points[index].x;
			int j = points[index].y;
			input[3 * j * x_size + 3 * i] = RGBColors[k][0];
			input[3 * j * x_size + 3 * i + 1] = RGBColors[k][1];
			input[3 * j * x_size + 3 * i + 2] = RGBColors[k][2];
		}
	}

}

void IntensityPlusPositionBasedKMeans(
	uchar input[],
	int x_size,
	int y_size,
	int K
){
	// TODO
	uchar* Y_buf = new uchar[x_size*y_size];
	char* U_buf = new char[x_size*y_size / 4];
	char* V_buf = new char[x_size*y_size / 4];
    const int numberOfFeatures = 3;

    RGBtoYUV420(input, x_size, y_size, Y_buf, U_buf, V_buf);

    performGaussFilter(Y_buf, x_size, y_size, 5, 0.1);

    vector<KMeansPoint> points;
    for(int i = 0; i < x_size; i++){
        for(int j = 0; j < y_size; j++){

            KMeansPoint newPoint = { i, j };

            newPoint.feature.push_back(Y_buf[j * x_size + i]);
            newPoint.feature.push_back((double)i/x_size *255/10.0);
            newPoint.feature.push_back((double)j/y_size *255/10.0);

            points.push_back(newPoint);
        }
    }

    vector<vector<int>> clusters;
    clusters = kMeans(points, numberOfFeatures, K);


    for(int k = 0; k < K; k++){
        for(int index : clusters[k]){
            int i = points[index].x;
            int j = points[index].y;
            input[3 * j * x_size + 3 * i] = RGBColors[k][0];
            input[3 * j * x_size + 3 * i + 1] = RGBColors[k][1];
            input[3 * j * x_size + 3 * i + 2] = RGBColors[k][2];
        }
    }
	
	delete[] Y_buf;
	delete[] U_buf;
	delete[] V_buf;
}

// Performs quantization of vector elements and calculates histogram for a given vector
static inline vector<double> calculateHistogram(
	vector<uchar> values,
	int quants
) {
	vector<double> histogram(quants, 0);
	int quantSize = round(255.0 / quants);
	for(uchar& value : values){
		for(int i = 1; i <= quants; i++){
			if(value < i*quantSize){
				histogram[i - 1] += 1.0;
				break;
			}
		}
	}

	for(int i = 0; i < quants; i++){
		histogram[i] = histogram[i] / values.size();
	}

	return histogram;
}

void histogramBasedKMeans(
	uchar input[],
	int x_size,
	int y_size,
	int blockSize,
	int K
)  {
    // TODO

    const int numberOfFeatures = 1;

    uchar* Y_buf = new uchar[x_size*y_size];
    char* U_buf = new char[x_size*y_size / 4];
    char* V_buf = new char[x_size*y_size / 4];

    RGBtoYUV420(input, x_size, y_size, Y_buf, U_buf, V_buf);

    performGaussFilter(Y_buf, x_size, y_size, 5, 0.1);

    vector<uchar> values;
    calculateHistogram(values, 8);

    vector<KMeansPoint> points;
    for(int i = 0; i < x_size; i++){
        for(int j = 0; j < y_size; j++){
            KMeansPoint newPoint = { i, j };
            newPoint.feature.push_back(input[j * 3 * x_size + i * 3]);
            newPoint.feature.push_back(input[j * 3 * x_size + i * 3 + 1]);
            newPoint.feature.push_back(input[j * 3 * x_size + i * 3 + 2]);

            points.push_back(newPoint);
        }
    }

    vector<vector<int>> clusters;
    clusters = kMeans(points, numberOfFeatures, K);

    for(int k = 0; k < K; k++){
        for(int index : clusters[k]){
            int i = points[index].x;
            int j = points[index].y;
            input[3 * j * x_size + 3 * i] = RGBColors[k][0];
            input[3 * j * x_size + 3 * i + 1] = RGBColors[k][1];
            input[3 * j * x_size + 3 * i + 2] = RGBColors[k][2];
        }
    }


    delete[] Y_buf;
    delete[] U_buf;
    delete[] V_buf;


}

// Dodatni zadatak
void SIFTBasedKMeans(
	uchar input[],
	int x_size,
	int y_size,
	int blockSize,
	int K
) {
	// TODO
}

/* 
 * File:   KMeans.h
 * Author: pavel
 *
 * Created on 30 Январь 2013 г., 22:56
 */

#ifndef KMEANS_H
#define	KMEANS_H

#include<vector>

class KMeans {
public:
	KMeans( double** matrix, const int& a, const int& b, const int& c );
	KMeans(const KMeans& orig);
	virtual ~KMeans();
	
	void clusterize(int &step);

private:
	
	bool isEnd();

	// calculation dissimilarity between points with euclidean disatnce
    double getDistance(double a_1, double b_1,
                              double a_2, double b_2,
                              double a_3, double b_3);

    void updateCentroid();

    void outputData();
	
	/**
	 * кол-во кластеров
	 */
    int clustersCount;

    /**
	 * размерность данных
	 */
    int dimension;

    // the sum number of point
    int point_num;
    
    // all points extract from image
    double** point_set;

    // store centroid information B G R value
    double** centroids;

    double** copy_centroids;

    // store cluster points
    std::vector< std::vector<int> > clusters;
};

#endif	/* KMEANS_H */


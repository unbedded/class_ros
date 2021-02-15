#include <iostream>
#include <math.h>
#include <vector>
#include <stdio.h>
using namespace std;
double inverseSensorModel(double x, double y, double theta, double xi, double yi, double sensorData[]);

// Sensor characteristic: Min and Max ranges of the beams
double Zmax = 5000,
       Zmin = 170;
// Defining free cells(lfree), occupied cells(locc), unknown cells(l0) log odds values
double l0 = 0, locc = 0.4, lfree = -0.4;
// Grid dimensions
double gridWidth = 100, gridHeight = 100;
// Map dimensions
double mapWidth = 30000, mapHeight = 15000;
// Robot size with respect to the map
double robotXOffset = mapWidth / 5, robotYOffset = mapHeight / 3;
// Defining an l vector to store the log odds values of each cell
vector<vector<double>> l(mapWidth / gridWidth, vector<double>(mapHeight / gridHeight));

double distanceCalculate(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;

    dist = pow(x, 2) + pow(y, 2); //calculating Euclidean distance
    dist = sqrt(dist);

    return dist;
}

double inverseSensorModel_UDACITY(double x, double y, double theta, double xi, double yi, double sensorData[])
{
    //******************Code the Inverse Sensor Model Algorithm**********************//
    // Defining Sensor Characteristics
    double Zk, thetaK, sensorTheta;
    double minDelta = -1;
    double alpha = 200, beta = 20;

    //******************Compute r and phi**********************//
    double r = sqrt(pow(xi - x, 2) + pow(yi - y, 2));
    double phi = atan2(yi - y, xi - x) - theta;

    //Scaling Measurement to [-90 -37.5 -22.5 -7.5 7.5 22.5 37.5 90]
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            sensorTheta = -90 * (M_PI / 180);
        }
        else if (i == 1)
        {
            sensorTheta = -37.5 * (M_PI / 180);
        }
        else if (i == 6)
        {
            sensorTheta = 37.5 * (M_PI / 180);
        }
        else if (i == 7)
        {
            sensorTheta = 90 * (M_PI / 180);
        }
        else
        {
            sensorTheta = (-37.5 + (i - 1) * 15) * (M_PI / 180);
        }

        if (fabs(phi - sensorTheta) < minDelta || minDelta == -1)
        {
            Zk = sensorData[i];
            thetaK = sensorTheta;
            minDelta = fabs(phi - sensorTheta);
        }
    }

    //******************Evaluate the three cases**********************//
    if (r > min((double)Zmax, Zk + alpha / 2) || fabs(phi - thetaK) > beta / 2 || Zk > Zmax || Zk < Zmin)
    {
        return l0;
    }
    else if (Zk < Zmax && fabs(r - Zk) < alpha / 2)
    {
        return locc;
    }
    else if (r <= Zk)
    {
        return lfree;
    }
}

void occupancyGridMapping_UDACITY(double Robotx, double Roboty, double Robottheta, double sensorData[])
{
    for (int x = 0; x < mapWidth / gridWidth; x++)
    {
        for (int y = 0; y < mapHeight / gridHeight; y++)
        {
            double xi = x * gridWidth + gridWidth / 2 - robotXOffset;
            double yi = -(y * gridHeight + gridHeight / 2) + robotYOffset;
            if (sqrt(pow(xi - Robotx, 2) + pow(yi - Roboty, 2)) <= Zmax)
            {
                l[x][y] = l[x][y] + inverseSensorModel(Robotx, Roboty, Robottheta, xi, yi, sensorData) - l0;
            }
        }
    }
}

double inverseSensorModel(double x, double y, double theta, double xi, double yi, double sensorData[])
{
    //******************Code the Inverse Sensor Model Algorithm**********************//
    // Defining Sensor Characteristics
    double Zk, thetaK, sensorTheta;
    double minDelta = -1;
    double alpha = 200, beta = 20;

    //******************TODO: Compute r and phi**********************//
    //const double r = distanceCalculate(x, y, xi, yi);
    double r = sqrt(pow(xi - x, 2) + pow(yi - y, 2));
    const double phi = atan2(yi - y, xi - x) - theta;

    //Scaling Measurement to [-90 -37.5 -22.5 -7.5 7.5 22.5 37.5 90]
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            sensorTheta = -90 * (M_PI / 180);
        }
        else if (i == 1)
        {
            sensorTheta = -37.5 * (M_PI / 180);
        }
        else if (i == 6)
        {
            sensorTheta = 37.5 * (M_PI / 180);
        }
        else if (i == 7)
        {
            sensorTheta = 90 * (M_PI / 180);
        }
        else
        {
            sensorTheta = (-37.5 + (i - 1) * 15) * (M_PI / 180);
        }

        if (fabs(phi - sensorTheta) < minDelta || minDelta == -1)
        {
            Zk = sensorData[i];
            thetaK = sensorTheta;
            minDelta = fabs(phi - sensorTheta);
        }
    }

    //******************TODO: Evaluate the three cases**********************//
    if (Zk > Zmax || Zk < Zmin)
    { // OUT OF RANGE
        return l0;
    }
    else if ((r > min((double)Zmax, Zk + alpha / 2)) || (fabs(phi - thetaK) > beta / 2))
    {
        return l0;
    }
    else if ((Zk < Zmax) && (fabs(r - Zk) < alpha / 2))
    {
        return locc;
    }
    else if (r <= Zk)
    {
        return lfree;
    }
}

/*******************************
                     Delta=15000
                   <----------------->
                        ^ +Y
                  (0,0) |
           ^       +-------------------->c
           |       |    |(6000,5000) |
           |    <-------*----------------->
Delta=30000|   -X  |    |            |   +X
           |       |    |            |
           |       |    |            |
           |       |    |            |
           v R=150 +-----------------+
                   |    |
                   v    v -Y
                   r


*******************************/
void occupancyGridMapping(double Robotx, double Roboty, double Robottheta, double sensorData[])
{
    const int C = mapWidth / gridWidth;
    const int R = mapHeight / gridHeight;
    for (int c = 0; c < C; c++)
    {
        for (int r = 0; r < R; r++)
        {
            const double x_cMass = gridWidth * ((double)c + 0.5) - robotXOffset;
            const double y_cMass = -(gridHeight * ((double)r + 0.5)) + robotYOffset;
            const double delta = distanceCalculate(Robotx, Roboty, x_cMass, y_cMass);
            if ((delta <= Zmax) && (delta > Zmin))
            { // IN RANGE
                l[c][r] = l[c][r] + inverseSensorModel(Robotx, Roboty, Robottheta, x_cMass, y_cMass, sensorData) - l0;
            }
        }
    }
}

int main()
{
    double timeStamp;
    double measurementData[8];
    double robotX, robotY, robotTheta;

    FILE *posesFile = fopen("poses.txt", "r");
    FILE *measurementFile = fopen("measurement.txt", "r");

    // Scanning the files and retrieving measurement and poses at each timestamp
    while (fscanf(posesFile, "%lf %lf %lf %lf", &timeStamp, &robotX, &robotY, &robotTheta) != EOF)
    {
        fscanf(measurementFile, "%lf", &timeStamp);
        for (int i = 0; i < 8; i++)
        {
            fscanf(measurementFile, "%lf", &measurementData[i]);
        }
        occupancyGridMapping(robotX, robotY, (robotTheta / 10) * (M_PI / 180), measurementData);
    }

    // Displaying the map
    for (int x = 0; x < mapWidth / gridWidth; x++)
    {
        for (int y = 0; y < mapHeight / gridHeight; y++)
        {
            cout << l[x][y] << " ";
        }
    }

    return 0;
}

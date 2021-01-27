#include <iostream>
#include <vector>
#include <string>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <math.h>
#include <tuple>

using namespace std;
using namespace Eigen;

//const std::vector<float> measurements = { 1, 2, 3 };
const int N_size = 3;
const float measurements[N_size] = { 1, 2, 3 };
typedef tuple<MatrixXf, MatrixXf> state_covar_t;
const bool g_is_debug = true;

tuple<MatrixXf, MatrixXf> kalman_filter(MatrixXf x, MatrixXf P, MatrixXf u, MatrixXf F, MatrixXf H, MatrixXf R, MatrixXf I)
{
    // MISSSING PARAMETERS
    auto Qk = I * 0;
    MatrixXf Bk(2, 2);
    Bk(0,0) < 1, 0;
            0, 1;

    int n=0;
    for(auto m : measurements)
//  for (int n = 0; n < sizeof(measurements) / sizeof(measurements[0]); n++) 
/// for (int n = 0; n < N_size; n++) 
    {
        //float m = measurements[n];
        std::cout <<  n << ":" << m << std::endl;
        MatrixXf y(1, 1);
        MatrixXf S(1, 1);
        MatrixXf K(2, 1);
        MatrixXf z(1, 1);

        // Sensor update
        z << m;

        // **yk = zk - Hk\*xk** Innovation or measurement pre-fit residual 
        //y << z - H*x;
        y << z - (H * x); 

        // **Sk = Hk\*Pk_post\*Hk_trasp + Rk** Innovation (or pre-fit residual) covariance 
        //S << H * P * H.transpose() + R;
        S << H * P * H.transpose() + R;

        // **Kk = Pk_post\*Hk_trasp/Sk** Optimal Kalman gain 
        //K << P * H.transpose() * S.inverse();
        K << P * H.transpose() * S.inverse();

        // **xk_post = xk_prio + Kk\*yk** Updated (a posteriori) state estimate 
        //x << x + K*y;
        x << x + (K * y);
       
        // **Pk_post = (I-Kk\*Hk)\*Pk_prio** Updated (a posteriori) estimate covariance 
        //P << (I - K * H) * P;
        P << (I - (K * H)) * P;

        // xk_post = Fk*xk_prio + Bk*uk + wk - state transition eq
        x << (F * x) + u;

        // **P = F * P * F * T_trasp + Q** Predicted (a priori) estimate covariance 	
        P << F * P * F.transpose();

        n++;
    }
    return make_tuple(x, P);
}    
        
        
tuple<MatrixXf, MatrixXf> kalman_filter2(MatrixXf x, MatrixXf P, MatrixXf u, MatrixXf F, MatrixXf H, MatrixXf R, MatrixXf I)
{
    for (int n = 0; n < N_size; n++) {
        std::cout <<  n << " Z:" << measurements[n] << std::endl;
        MatrixXf y(1, 1);
        MatrixXf S(1, 1);
        MatrixXf K(2, 1);
        MatrixXf Z(1, 1);
        Z << measurements[n];

        y << Z - (H * x);
        S << H * P * H.transpose() + R;
        K << P * H.transpose() * S.inverse();
        x << x + (K * y);
        P << (I - (K * H)) * P;
        x << (F * x) + u;
        P << F * P * F.transpose();
    }

    return make_tuple(x, P);
}



int main_test1()
{

    MatrixXf x(2, 1);// Initial state (location and velocity) 
    x << 0,
    	 0; 
    MatrixXf P(2, 2);//Initial Uncertainty
    P << 100, 0, 
    	 0, 100; 
    MatrixXf u(2, 1);// External Motion
    u << 0,
    	 0; 
    MatrixXf F(2, 2);//Next State Function
    F << 1, 1,
    	 0, 1; 
    MatrixXf H(1, 2);//Measurement Function
    H << 1,
    	 0; 
    MatrixXf R(1, 1); //Measurement Uncertainty
    R << 1;
    MatrixXf I(2, 2);// Identity Matrix
    I << 1, 0,
    	 0, 1; 

    std::tie(x, P) = kalman_filter(x, P, u, F, H, R, I);
    std::cout << "x= " << x << endl;
    std::cout << "P= " << P << endl;

    return 0;
}

int main_test2()
{

    MatrixXf x(2, 1);// Initial state (location and velocity) 
    x << 0,
    	 0; 
    MatrixXf P(2, 2);//Initial Uncertainty
    P << 100, 0, 
    	 0, 100; 
    MatrixXf u(2, 1);// External Motion
    u << 0,
    	 0; 
    MatrixXf F(2, 2);//Next State Function
    F << 1, 1,
    	 0, 1; 
    MatrixXf H(1, 2);//Measurement Function
    H << 1,
    	 0; 
    MatrixXf R(1, 1); //Measurement Uncertainty
    R << 1;
    MatrixXf I(2, 2);// Identity Matrix
    I << 1, 0,
    	 0, 1; 

    std::tie(x, P) = kalman_filter2(x, P, u, F, H, R, I);
    std::cout << "x= " << x << endl;
    std::cout << "P= " << P << endl;

    return 0;
}

int main()
{
    main_test2();
    main_test1();
    return 0;
}
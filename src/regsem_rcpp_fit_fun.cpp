
#include <RcppArmadillo.h>

using namespace Rcpp;
using namespace arma;


//' Calculates the objective function values.
//'
//' @param ImpCov expected covariance matrix.
//' @param SampCov Sample covariance matrix.
//' @param type2 penalty type.
//' @param lambda penalty value.
//' @param gamma additional penalty for mcp and scad
//' @param pen_vec vector of penalized parameters.
//' @param pen_diff Vector of values to take deviation from.
//' @param e_alpha Alpha for elastic net
//'
// [[Rcpp::export]]
double rcpp_fit_fun(Rcpp::NumericMatrix ImpCov,
                 Rcpp::NumericMatrix SampCov,
                 int type2,
                 double lambda,
                 double gamma,
                 arma::vec pen_vec,
                 arma::vec pen_diff,
                 double e_alpha){
           //      int alpha,


double m;
m = ImpCov.nrow();
double fit;
double add;
arma::mat ImpCov2 = Rcpp::as <arma::mat>(ImpCov);
arma::mat SampCov2 = Rcpp::as<arma::mat>(SampCov);
//arma::mat Areg2 = Rcpp::as<arma::mat>(Areg);
//arma::double lambda2 = Rcpp::as<arma::int>(lambda);
//arma::vec pen_vec2 = Rcpp::as<arma::vec>(pen_vec);


//NumericMatrix A2;
//A2 = clone(A);
//NumericMatrix S2;
//S2 = clone(S);


  if (type2 == 0) {
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m);
  }
  else if (type2 == 1) {
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m  + 2*lambda * norm(pen_vec,1));
  }
  else if (type2 == 2) {
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m  + 2*lambda * norm(pen_vec,2));
  }
  else if (type2 == 3) {
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m  + 2*lambda * norm(pen_diff,1));
  }
  else if (type2 == 4) {
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m  +
      2*lambda * ((1-e_alpha)*norm(pen_vec,1) + e_alpha*norm(pen_vec,2)));
  }
  else if (type2 == 6) {
    add = 0;
    for (double i = 0; i < pen_vec.n_elem; i++) {
      if (pen_vec[i] <= lambda){
        add = add + lambda;
      }
      else if(pen_vec[i] > lambda){
        if (gamma*lambda - pen_vec[i] > 0) {
          add = add + (gamma*lambda - pen_vec[i])/((gamma - 1)*lambda) * lambda;
        }
        else {
          add = add + 0;
        }
      }
    }
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m + add);
  }
  else if (type2 == 7) {
    add = 0;
    for (double i = 0; i < pen_vec.n_elem; i++) {
      if (pen_vec[i] <= lambda*gamma){
        add = add + (lambda * (abs(pen_vec[i] - (pen_vec[i] - ((pen_vec[i]*pen_vec[i])/2*lambda*gamma)))));
      }
      else if(pen_vec[i] > lambda * gamma){
        add = add + pen_vec[i];
      }
    }
    fit = 0.5*(log(det(ImpCov2)) + trace(SampCov2 * (inv(ImpCov2))) - log(det(SampCov2))  - m + add);
  }
  else{
    fit = -9999999;
  }

//  List ret;
//  ret["fit"] = fit;
 // double fitt = fit;
 // return fit;
  //return fit;

  //return(std::cout << std::setprecision(5) << fit);
  return fit;
}


#ifndef __STAT_H
# define __STAT_H
#include "utils.h"
class Dstat {

public:
    Dstat(const data_t&);
    Dstat(const graph&, const nm_t&);
    void linefit(double, double, char, bool);
    void print() const;
    double mean(char) const ;
    double variance(char) const;
    double correlation() const;
    void freq(char);
private:
    data_t org;
    const data_t& od;
    data_t& cd;
    double sp;
    double cc;
    double avg;
    double stdv;
    data_t r_ccdf;
    data_t r_logs;
    data_t f;
    double squares_mean(char) const;
    double covariance() const;
    double slope() const;
    double crossing() const;
    void ccdf();
    void logs(char);
    void least_square();
    void filter(double, double, char, bool);
};

#endif

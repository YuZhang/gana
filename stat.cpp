#include "utils.h"

void Dstat::print() const
{
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        cout << it->first << " " << it->second << endl;
    }
}

void Dstat::ccdf()
{
    r_ccdf.clear();
    vector < double >xdata;
    double sum = 0;
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        xdata.push_back(it->first);
        sum += it->second;
    }
    sort(xdata.begin(), xdata.end());
    vector < double >::iterator vit, vend;
    vend = xdata.end();
    double c = 0;
    for (vit = xdata.begin(); vit != vend; ++vit) {
        r_ccdf[*vit] = 1 - c / sum;
	data_t::const_iterator dit = cd.find(*vit);
        c += dit->second;
    }
    cd = r_ccdf;
}
void Dstat::logs(char axis)
{
    r_logs.clear();
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        if (axis == 'x')
            r_logs[log(it->first)] = it->second;
        else if (axis == 'y')
            r_logs[it->first] = log(it->second);
        else if (axis == 'n')
            r_logs[it->first] = it->second;
	else {
            r_logs[log(it->first)] = log(it->second);
        }
    }
    cd = r_logs;
}
void Dstat::filter(double st, double ed, char ax, bool is_ccdf)
{
    if (is_ccdf) 
        ccdf();
    data_t tmp(cd);
    data_t::iterator it, end;
    end = tmp.end();
    for (it = tmp.begin(); it != end; ++it) {
        if (it->first > ed or it->first < st) {
            tmp.erase(it);
        }
    }
    cd = tmp;
    logs(ax);
}

double Dstat::mean(char axis) const
{
    double sum = 0;
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        sum += axis == 'x' ? it->first : it->second;
    }
    return sum / cd.size();
}

double Dstat::squares_mean(char axis) const
{
    double sum = 0;
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        if (axis == 'x')
            sum += pow(it->first, 2);
        else if (axis == 'y')
            sum += pow(it->second, 2);
        else {
            sum += it->first * it->second;
        }
    }
    return sum / cd.size();
}
double Dstat::variance(char axis) const
{
    return squares_mean(axis) - pow(mean(axis), 2);
}

double Dstat::covariance() const 
{
    return squares_mean('z') - (mean('x') * mean('y'));
}

double Dstat::correlation() const
{
    return sqrt(pow(covariance(), 2) /
                (variance('x') * variance('y')));
}

double Dstat::slope() const
{
    return covariance() / variance('x');
}

double Dstat::crossing() const
{
    return exp(mean('y') - (slope() * mean('x')));
}

void Dstat::least_square()
{
    double vx = variance('x');
    double vy = variance('y');
    double cv = covariance();
    sp = cv / vx;
    cc = sqrt(cv * cv / (vx * vy));
}

void Dstat::freq(char axis)
{
    double d;
    data_t::const_iterator it, end;
    end = cd.end();
    for (it = cd.begin(); it != end; ++it) {
        d = axis == 'x' ? it->first : it->second;
	f[d] +=1;
    }
    cd = f;
}

Dstat::Dstat(const data_t& D) : od(D), cd(org)
{
    org = D;
}

Dstat::Dstat(const graph& G, const nm_t& nm) : od(org), cd(org)
{
    graph::node_iterator it, end;
    end = G.nodes_end();
    for (it = G.nodes_begin(); it != end; ++it) {
        org[it->id()]= nm[*it];
    }
}

void Dstat::linefit(double st, double ed, char axis, bool is_ccdf)
{
    filter(st, ed, axis, is_ccdf);  
    least_square();
    if (is_ccdf)
       sp -= 1;
    cout << "linefit: " << sp << " " << cc << endl;
}

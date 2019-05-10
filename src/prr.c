/*
 * GQRM
 * Copyright (C) 2019-2025 Chaofan Ma <chaofanma@hotmail.com>
 *
 * This file is part of GQRM.
 *
 * GQRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GQRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GQRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "prr.h"

#define M_SQRT1_2    0.7071067811

/* total bits to be sent */
static const double BITS = 200;

/* transmit power level (in dBm) */
static const double pt = -15;

/* path loss exponent */
static const double ple = 4;

/* data rate (in kbps) */
static const double dr = 250;

/* noise bandwidth (in kHz) */
static const double nb = 50;

/* standard deviation due to multipath effects */
static const double std = 5;

/* average path loss at reference distance (in dBm) */
static const double pl0 = 20;

/* noise floor (in dBm) */
static const double nf = -115;

/* reference distance (in m) */
static const double d0 = 1;

/* PRR constraint */
double PRR_CONSTRAINT = 0.95;

static double snr(const double, const double);
static double q_func(const double);
static double ber(const double, const double);

/* @fn snr
 * Compute the average SNR at distance d with 
 * set transmit power to pt.
 */
static double 
snr(const double pt, const double d) {
    return pt - pl0 - 10 * ple * log10(d / d0) - nf;
}

/* @fn q_func
 * Implementation of the Q-function.
 */
static double 
q_func(const double value) {
    return 1.0 - 0.5 * erfc(-value * M_SQRT1_2);
}

/* @fn ber
 * Compute the bit error rate at distance d with
 * transmit power set to pt.
 */
static double 
ber(const double pt, const double d) {
    return q_func(sqrt(2 * snr(pt, d) * nb / dr));
}

/* @fn prr
 * Compute the packet reception rate at distance d
 * with transmit power set to pt.
 */
double 
prr(const double pt, const double d) {
    double p = - pt;
    return pow(1.0 - ber(p, d), 8 * BITS);
}

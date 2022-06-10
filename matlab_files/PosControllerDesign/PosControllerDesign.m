
clear; clc;
set(cstprefs.tbxprefs,'FrequencyUnits','Hz')

% EC90 flat 260W
r= 1;
Kt = 0.231; 
Im = 0.000506;
Fm = 0.001; 
Kv = Kt;
Ra = 0.844;
La = 0.00107;

s = tf([1 0],1);

M = 1/((s*Im + Fm)*s) ;

 
M = minreal(M); 
% M = zpk(M) to visualuze in form (s+z_1)(s+p_1)
sisotool(M);

%% Discrete PD Controller

s = tf([1 0],1);
PD = 25.641*(s+11.7)/(s+1200);
PD_discrete = c2d(PD, 1/2000, 'tustin');

%         19.78 z - 19.67
% PD(z) = ---------------
%         z - 0.5385

%% Discretize PID controller high disturbance du2y

s = tf([1 0],1);
PID_high_dist = (5.206*(s+13.82)*(s+0.7657))/(s*(s+338.3));
PID_high_dist_discrete = c2d(PID_high_dist, 1/2000, 'tustin');
%                    20.58 z^2 - 41.06 z + 20.48
%  PID_low_dist(z) = ---------------------------
%                      z^2 - 1.667 z + 0.6667

%% Discretize PID controller low disturbance du2y

s = tf([1 0],1);
PID_low_dist = (24.638*(s+4.679)*(s+5.607))/(s*(s+800));
PID_low_dist_discrete = c2d(PID_low_dist, 1/2000, 'tustin');
%                    4.818 z^2 - 9.6 z + 4.783
%  PID_high_dist(z) = -------------------------
%                      z^2 - 1.844 z + 0.844

%% Discretize PI Controller, not required for homework

s = tf([1 0],1);
PI = 0.55772*(s+0.03231)/s;
PI_discretize = c2d(PI, 1/2000, 'tustin');

%% Control and Saturation

F = minreal(PD / (1 + PD*M));
 
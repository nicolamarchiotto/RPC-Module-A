
clear; clc;
set(cstprefs.tbxprefs,'FrequencyUnits','Hz')


% %faulhaber 2237-24crx
% r = 103;
% Kt = 31*1e-3*r; 
% Im = 3.0000e-07*r^2;
% Fm = 0.01; 
% Kv = 1/(2*pi*301/60)*r;
% Ra = 15.7;
% La = 590e-6;

%maxon
% r= 1;
% Kt = 0.0705*r; 
% Im = 0.0003*r^2;
% Fm = 0.00001*r^2; 
% Kv = 1/(2*pi*135/60)*r;
% Ra = 0.343;
% La = 0.000264;

% EC90 flat 260W
r= 1;
Kt = 0.231; 
Im = 0.000506;
Fm = 0.001; 
Kv = Kt;
Ra = 0.844;
La = 0.00107;

s = tf([1 0],1);

% fw_path = Kt / ((s*La + Ra) * (s*Im + Fm));
% fb_path = Kv;

M = 1/((s*Im + Fm)*s) 

%%sys = feedback(fw_path,fb_path) * (1/s);
 
% M = fw_path / ( 1 + fw_path*fb_path) * (1/s)
M = minreal(M); 
M = zpk(M)
sisotool(M);
%% Discretize the Controller
% s = tf([1 0],1);
C = 28.029*(s+11.7)/(s+1200)
PD = c2d(C, 1/2000, 'tustin')
%% Discretize PID controller
s = tf([1 0],1);

PID = (24.638*(s+4.679)*(s+5.607))/(s*(s+800))
% PID = (3.8973*(s+4.679)*(s+5.691))/(s*(s+200))
% PID_discrete = c2d(PID, 1/2000, 'tustin')
PID_discrete = c2d(PID, 1/2000, 'zoh')
% PID_discrete = c2d(PID, 1/2000, 'foh')
% PID_discrete = c2d(PID, 1/2000, 'impulse')
% PID_discrete = c2d(PID, 1/2000, 'matched')
% PID_discrete = c2d(PID, 1/2000, 'least-squared')
% PID_discrete = c2d(PID, 1/2000, 'damped')
%% Discretize PI Controller
s = tf([1 0],1);

PI = 0.55772*(s+0.03231)/s
PI_discretize = c2d(PI, 1/2000, 'tustin')
%% Control and Saturation
F = PD / (1 + PD*M)
 

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
 
fw_path = Kt / ((s*La + Ra) * (s*Im + Fm));
fb_path = Kv;

%%sys = feedback(fw_path,fb_path) * (1/s);
 
M = fw_path / ( 1 + fw_path*fb_path) * (1/s)
M = minreal(M); 
M = zpk(M)
sisotool(M)
%% Discretize the Controller
C = (950.66*(s+118.3))/(s+14420);
C_discrete = c2d(C, 1/2000, 'tustin')
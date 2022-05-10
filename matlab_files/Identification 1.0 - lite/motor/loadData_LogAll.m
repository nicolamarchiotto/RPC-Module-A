DATA = textscan(fid,'%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');

D = cell2struct(DATA(1,1),'A',1);
t = struct2array(D);

D = cell2struct(DATA(1,2),'A',1);
current = struct2array(D);

D = cell2struct(DATA(1,3),'A',1);
CurrentCommand = struct2array(D);

D = cell2struct(DATA(1,4),'A',1);
theta_m = struct2array(D);

D = cell2struct(DATA(1,5),'A',1);
diff_theta_m = struct2array(D);

D = cell2struct(DATA(1,6),'A',1);
dtheta_m  = struct2array(D);

D = cell2struct(DATA(1,7),'A',1);
theta_e = struct2array(D);

D = cell2struct(DATA(1,8),'A',1);
diff_theta_e = struct2array(D);

D = cell2struct(DATA(1,9),'A',1);
dtheta_e = struct2array(D);

D = cell2struct(DATA(1,10),'A',1);
tau = struct2array(D);

ts = mean(diff(t));

ddtheta_e = [diff(dtheta_e); 0]/ts;
ddtheta_m = [diff(dtheta_m); 0]/ts;

half_sampling_freq = 1/(2.*ts);
cut_off_freq = 10; % [Hz]

[B,A] = butter(8,cut_off_freq/half_sampling_freq);

%current = filter(B,A, current);
tau_m = filter(B,A, tau);
%theta_e = filter(B,A, theta_e);
%dtheta_e = filter(B,A, dtheta_e);
%ddtheta_e = filter(B,A, ddtheta_e);
theta_m = filter(B,A, theta_m);
dtheta_m = filter(B,A, dtheta_m);
ddtheta_m = filter(B,A, ddtheta_m);

tmp = find(t>startTime);
start_idx = tmp(1);
tmp = find(t>endTime);
if numel(tmp)==0
    end_idx = length(t);
else
    end_idx = tmp(1);
end
 
t = t(start_idx:end_idx);
current = current(start_idx:end_idx);
tau = tau(start_idx:end_idx);
theta_m = theta_m(start_idx:end_idx);
dtheta_m = dtheta_m(start_idx:end_idx);
ddtheta_m = ddtheta_m(start_idx:end_idx);
theta_e = theta_e(start_idx:end_idx);
dtheta_e = dtheta_e(start_idx:end_idx);
ddtheta_e = ddtheta_e(start_idx:end_idx);

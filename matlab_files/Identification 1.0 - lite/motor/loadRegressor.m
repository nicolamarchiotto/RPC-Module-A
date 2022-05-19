[dtheta_m_pos dtheta_m_neg sign_dtheta_m_pos...
    sign_dtheta_m_neg] = frictionModes(dtheta_m);

offset = ones(size(theta_m));

y = tau_m; 

% phi = [ddtheta_m dtheta_m sign(dtheta_m)];

phi = [ddtheta_m dtheta_m sign(dtheta_m) -9.81*sin(theta_m)];


% phi = [ddtheta_m dtheta_m sign(dtheta_m) -9.81*sin(theta_m) -offset];

% phi = [ddtheta_e sin(theta_m) dtheta_m_pos dtheta_m_neg sign_dtheta_m_pos sign_dtheta_m_neg];
% phi = [ddtheta_e dtheta_e];


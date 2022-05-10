clear
close all

addpath('..')

Y = [];
Phi = [];
lambdas = [];
sigmas_lambda = [];
sigmas_exp = [];

filepath = './';
files = dir([filepath '*.csv']);
startTime = 0.5;
endTime = 120;


% 
% filepath = '../../../Log/SEA/';
% files = dir([filepath 'PDInj_SEAHandTransparency1.csv']);
% startTime = 2.5;
% endTime = 26;

for i=1:length(files)
    
    files(i).name
    fid = fopen([filepath files(i).name]);
    
    loadData_LogSheep
    fclose(fid);
    loadRegressor;
        
%     % normalization
%     kn = 1/max(y);
%     y = kn*y;
%     phi = kn*phi;
    
    
    % identificazione
    [lambda sigma_exp Phi Y var_lambda] = LSIdentification(phi,y, 3, 1);
    var_lambda;
    
    data(i).y = Y;
    data(i).phi = Phi;
    data(i).sigma = sigma_exp;
    
    sigma_lambda = diag(sqrt(abs(var_lambda)));
    
    lambda
    %sigma_lambda
    %sigma_exp
    
    lambdas = [lambdas lambda];
    sigmas_exp = [sigmas_exp sigma_exp];
    sigmas_lambda = [sigmas_lambda sigma_lambda];
    

    pause
    clf
    
end

%%

% [lambdaML var_lambdaML] = MLIdentification(data, 3, 1);
% 
% lambdaML
% 
% 
% figure(10)
% subplot(2,1,1)
% hist(lambdas(1,:)')
% subplot(2,1,2)
% plot(lambdas(1,:),sigmas_lambda(1,:),'.',lambdas(1,:),sigmas_exp,'r.')
% 
% figure(11)
% subplot(2,1,1)
% hist(lambdas(2,:)')
% subplot(2,1,2)
% plot(lambdas(2,:),sigmas_lambda(2,:),'.',lambdas(2,:),sigmas_exp,'r.')
% 
% figure(12)
% subplot(2,1,1)
% hist(lambdas(3,:)')
% subplot(2,1,2)
% plot(lambdas(3,:),sigma_lambda(3,:),'.',lambdas(3,:),sigmas_exp,'r.');
% 
% figure(13)
% subplot(2,1,1)
% hist(lambdas(4,:)')
% subplot(2,1,2)
% plot(lambdas(4,:),sigma_lambda(4,:),'.',lambdas(4,:),sigmas_exp,'r.');


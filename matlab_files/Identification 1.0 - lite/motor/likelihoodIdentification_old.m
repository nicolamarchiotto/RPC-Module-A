clear
close all

addpath('..')

Y = [];
Phi = [];
lambdas = [];
sigmas = [];

% filepath = '../../../Log/SEL/';
% files = dir([filepath 'PDInj_LinkHandTransparency1.csv']);
% startTime = 2.5;
% endTime = 14.3;

filepath = 'data/';
files = dir([filepath 'PDinj*.csv']);
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
    
    loadData
    loadRegressor;
    
    
    % normalization
    kn = 1/max(y);
    y = kn*y;
    phi = kn*phi;
    
    
    % identificazione
    [lambda sigma Phi Y] = LSIdentification(phi,y, 3, 1);
    
    data(i).y = Y;
    data(i).phi = Phi;
    data(i).sigma = sigma;
    
    lambda
    sigma
    
    lambdas = [lambdas lambda];
    sigmas = [sigmas sigma];
    
    fclose(fid);
    pause
    clf
    
end

%%

figure(10)
subplot(2,1,1)
hist(lambdas(1,:)')
subplot(2,1,2)
plot(lambdas(1,:),sigmas,'.')

figure(11)
subplot(2,1,1)
hist(lambdas(2,:)')
subplot(2,1,2)
plot(lambdas(2,:),sigmas,'.')

figure(12)
subplot(2,1,1)
hist(lambdas(3,:)')
subplot(2,1,2)
plot(lambdas(3,:),sigmas,'.')

[lambdaML var_lambdaML] = MLIdentification(data, 3, 3);

lambdaML
s = sqrt(abs(var_lambdaML))
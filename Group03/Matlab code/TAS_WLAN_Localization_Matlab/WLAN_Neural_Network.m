%% WLAN Neural Network

% Load the data sets
load('Neural_Network_Data')

%% Setup of Neural Network
n = size(new_list,2);

net = feedforwardnet(37);

% Neural Inputs

% The first 37 rows of new_list contain the signal strengths at a 
% distinct position.  The coordinates of the corresponding x and ypositions
% are located in rows 38 and 39.

% Here the signal strengths input are stored
x=new_list(1:37,1:n);

% Neural Targets
% The targets per signal strength vector are stored
t=new_list(38:39,1:n);

% Train Neural Network
net = train(net,x,t);

%% Training of Neural Network

% Matrix to store predicted path that neural network is to guess
predicted_path = zeros(2,403);

% Defines the number of guesses to be made
iter = 200;

% Simulation of the number of guesses
for i = 1 : iter;
    predicted_path(:,i) = sim(net,list(1:37,i));
end

%% Plot  Results

figure

hold on

resolution = 0.05;

% Plots the actual path and the resulting predicted path
for i = 1 : iter;
    scatter(list(38,i)/resolution,list(39,i)/resolution,'.')
    scatter(predicted_path(1,i)/resolution,predicted_path(2,i)/resolution,'d')
end

% Plots lines that connect the guesses with the corresponding actual path
for ii = 1 : iter
    plot([list(38,ii)/resolution predicted_path(1,ii)/resolution],[list(39,ii)/resolution predicted_path(2,ii)/resolution]);
end

title('Predicted Path compared with Actual Path')

hold off



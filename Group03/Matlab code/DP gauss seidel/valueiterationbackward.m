% This script starts the asynchronous value iteration whit the gauss seidel 
% method. The states are chosen in counterclockwise order. To start this the
% script init.m must have been called in the past or one of the appropriate
% saved data files been loaded. One iteration takes about 30 minutes on a
% 3Ghz Intel i5 Haswell Low Voltage.

valueiterations=1; % set the number of desired iterations here
tic
for valiter=1:valueiterations
    valiter
    'step1'
    backsavepolicy
    toc
    'step2'
    backsavepolicy3
    toc
    'step3'
    backsavepolicy2
    toc
    'step4'
    backsavepolicy4
    toc
end
simulate;
currentiter=currentiter+valueiterations
toc
save rueckwaerts3-7
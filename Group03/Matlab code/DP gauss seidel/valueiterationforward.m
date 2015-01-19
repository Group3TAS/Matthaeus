% This script starts the asynchronous value iteration whit the gauss seidel 
% method. The states are chosen in clockwise order. To start this the
% script init.m must have been called in the past or one of the appropriate
% saved data files been loaded. One iteration takes about 30 minutes on a
% 3Ghz Intel i5 Haswell Low Voltage.

valueiterations=3; % set the number of desired iterations here
tic
for valiter=1:valueiterations
    valiter
    'step1'
    savepolicy
    toc
    'step2'
    savepolicy4
    toc
    'step3'
    savepolicy2
    toc
    'step4'
    savepolicy3
    toc
end
simulate;
currentiter=currentiter+valueiterations
toc
save rueckwaerts3-7
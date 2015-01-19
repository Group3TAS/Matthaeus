% Initializes the algorithm.
% Warning: This script calls the clear all function. If it is accidentially
% called the previous state of the workspace can be found in initbackup.mat

save initbackup
clear all

global value
global policy
global legal
global consts

legal=imread('rueckwaerts.pgm');

consts.xstart=8;
consts.ystart=4;
consts.xend=25;
consts.yend=22;

consts.DX=0.05; %%%% 0.05
consts.DY=0.05; %%%% 0.05
consts.DTHETA=pi/18; %%%% /18

consts.rangex=consts.xstart+consts.DX/2:consts.DY:consts.xend-consts.DX/2;
consts.nx=length(consts.rangex);
consts.rangey=consts.ystart+consts.DY/2:consts.DY:consts.yend-consts.DY/2;
consts.ny=length(consts.rangey);
consts.rangetheta=0:consts.DTHETA:2*pi-0.01;
consts.ntheta=length(consts.rangetheta);

consts.alpha=0.99;

value=zeros(consts.nx,consts.ny,consts.ntheta);
value(100:220,100:250,:)=20*ones(length(100:220),length(100:250),36);

value(:,1:1,:)=10*ones(length(1:340),length(1:1),36);
value(:,360:360,:)=10*ones(length(1:340),length(360:360),36);
value(1:1,:,:)=10*ones(length(1:1),length(1:360),36);
value(340:340,:,:)=10*ones(length(340:340),length(1:360),36);

policy=zeros(consts.nx,consts.ny,consts.ntheta);

currentiter=0;


% This script takes the three dimensional policy matrix and reshapes it into a two
% dimensional matrix. This matrix is then stored as .csv file. This .csv
% should then be copied into the catkin workspace folder by the user and renamed
% into pol.txt. Once this is done the car is good to go :)

nx=consts.nx;
ny=consts.ny;
ntheta=consts.ntheta;

shapedpolicy=zeros(nx*ntheta,ny);

        for thetai=1:ntheta
            shapedpolicy(((thetai-1)*nx+1):((thetai)*nx),:)=policy(:,:,thetai);         
        end
csvwrite('pol.csv',shapedpolicy)
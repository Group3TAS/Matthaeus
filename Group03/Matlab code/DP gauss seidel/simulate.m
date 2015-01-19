% simulates the car with the current policy

T=50;               % set simulation time here
dt=0.4;             % set time between sensor readings here
dtsim=0.01;         % set time discretization of simulation here

x0=10.5;            % set starting x here
y0=10;              % set starting y here
theta0=-pi/2-0.2;   % set starting theta here



simfactor=dt/dtsim;
l=length(0:dt:T);

x=zeros(l*simfactor,1);
y=zeros(l*simfactor,1);
theta=zeros(l*simfactor,1);
v=zeros(l,1);
radius=zeros(l,1);

x(1)=x0;
y(1)=y0;
theta(1)=theta0;

i=1;
ti=1;

for t=0:dt:T 
    [ v(i),radius(i) ] = lookuppolicy( x(ti),y(ti),theta(ti));
        
    for simi=1:simfactor
        [ dx, dy, dtheta ] = dynamic( x(ti),y(ti),theta(ti),v(i),radius(i),dtsim );

        x(ti+1)=x(ti)+dx;
        y(ti+1)=y(ti)+dy;
        theta(ti+1)=theta(ti)+dtheta;
        ti=ti+1;
    end
    i=i+1;
end
hold on
plot(x,y);
hold on
plotmap;


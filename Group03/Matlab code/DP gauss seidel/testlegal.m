% Displays the legal places

x=[];
y=[];
cost=[];

i=1;
for xi=1:0.1:30
    for yi=1:0.1:30        
        x(i)=xi;
        y(i)=yi;
        [lx,ly]=size(legal);
        xsim=lx-round(yi/0.05);
        ysim=round(xi/0.05);
        [ cost(i) ] =legal(xsim,ysim);
        i=i+1;       
    end
end

plot3(x,y,cost,'.','MarkerSize',2)
hold on
scatter([10 10],[10 12])

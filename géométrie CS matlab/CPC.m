% test equation param CPC ferme these veynandt
thetaC=45*pi/180; % angle max d'acceptance (rad)
r=48/2; % rayon du tube
thetaMin=pi/2+thetaC;
thetaMax=3*pi/2-thetaC;
theta=[0:0.1:thetaMax];
Mtheta=theta;
% Mtheta2=theta;
u=length(theta);
for i=1:u
    if theta(i)>thetaMin
        Mtheta(i)= (thetaMin+theta(i)-cos(theta(i)-thetaC))/(1+sin(theta(i)-thetaC));
    end
end
figure
x=r*(sin(theta)-Mtheta.*cos(theta));
y=r*(-cos(theta)-Mtheta.*sin(theta));
% x2=r*(sin(theta)-Mtheta2.*cos(theta));
% y2=r*(-cos(theta)-Mtheta2.*sin(theta));
plot(x,y)
hold on
plot(-x,y)
theta_tube=[0:0.1:2*pi];
xtube=r*cos(theta_tube);
ytube=r*sin(theta_tube);
plot(xtube,ytube);
% plot(x2,y2)
% for k=1:(u-1)
%     dx(k)=x(k+1)-x(k);
%     dy(k)=y(k+1)-y(k);
%     df(k)=dy(k)./dx(k);
% end
% hold on
% plot(x(1:u-1),df)


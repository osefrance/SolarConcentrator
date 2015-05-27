function Sah=soleil_ah(n,t,Phi)
% Calcul de la trajectoire du soleil h=hauteur, a=azimut
% J jour, M mois, phi latitude du lieu, t heure solaire
% n numero du jour [1:365]
delt=delta(n); % declinaison du soleil (en radian)
phi=Phi*pi/180; % latitude terrestre du lieu
omega=15*(t-12); % angle solaire
omega=omega*pi/180; % en radian
h=asin(sin(phi)*sin(delt)+cos(phi)*cos(delt)*cos(omega)); % hauteur
a=asin(cos(delt)*sin(omega)./cos(h));  
%r=atand(cos(a)/tan(h))
Sah=[a; h];
end


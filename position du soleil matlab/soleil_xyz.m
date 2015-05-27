function S=soleil_xyz(n,t,Phi)
% Calcul de la trajectoire du soleil en coord cartesiennes
% J jour, M mois, phi latitude du lieu, t heure solaire
% n numero du jour
% nb de jours a ajouter pour chaque mois passe
% mois=[0 31 59 90 120 151 181 212 243 273 304 334];
% n=mois(M)+J;
%eps=23.45*pi/180; % inclinaison equateur/ecliptique (en radian)
delt=delta(n); % declinaison du soleil (en radian)

omega=15*(t-12); % angle solaire
omega=omega*pi/180; % en radian
phi=Phi*pi/180; % latitude terrestre du lieu
Zsol=sin(delt)*sin(phi)+cos(delt)*cos(phi)*cos(omega); % =sin(h)
Xsol=-sin(delt)*cos(phi)+cos(delt)*sin(phi)*cos(omega); 
Ysol=cos(delt)*sin(omega);

S=[Xsol; Ysol; Zsol];
end

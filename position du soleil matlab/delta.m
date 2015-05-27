function d = delta(n)
% calcul de delta declinaison du soleil (en radian)
% methode simplifiee 
% valeur par defaut a 0 (equinoxes)
% J jour, M mois, n numero du jour
% % nb de jours a ajouter pour chaque mois passe
% mois=[0 31 59 90 120 151 181 212 243 273 304 334];
% n=mois(M)+J;
l=0.01707*(n-80); %0.0172 vitesse angulaire de rotation moyenne de la Terre (rad/jour)
eps=23.45*pi/180; % inclinaison equateur/ecliptique (en radian)
d=asin(sin(eps)*sin(l)); % declinaison du soleil (en radian)
%d2=asin(sin(eps)*sin(0.0172*(n-(81-2*sin(0.0172*(n-2))))))
end
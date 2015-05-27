function h=heure_legale(n,t,long)
% heure legale moyenne a partir de l'heure solaire vraie
% long longitude en degres decimaux (negative a l'est de greenwich)
% n numero du jour [1:365]
% t heure solaire

% correction de l'equation du temps (formule simple)
% irregularites dans la rotation de la Terre
va=2*pi/365; % vitesse angulaire de la Terre (rad/jour)
l=va*(n-81);
E=-9.87*sin(2*l)+7.53*cos(l)+1.5*sin(l); % en minutes decimales

% correction de longitude 
Long=4*long; % en minutes décimales

% ajouter aussi heure d'ete +2 - heure d'hiver +1 (valeurs pour 2015)
if n<88 || n>297
    u=1;
else u=2;
end

h=t+u+(E+Long)/60;
end

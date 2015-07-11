% parametrage miroir 
% angle d'incidence solaire (thtrans et thi)
% orientation N-S
% jour 269 (alternatiba paris 26/09) à midi
S=soleil_ah(269,15,45); % azimut S(1) et hauteur S(2) solaire
thtrans=atan(sin(S(1))/tan(S(2)))
thi=asin(cos(S(1))*cos(S(2)));
% 2 cadre
n=10; % nb facettes par cadre
lm=10; % largeur facette miroir
dx=14.3; % intervalle centres deux facettes consécutives
% centre facette m1 = 14.3
H=150; % hauteur absorbeur (cm)
i=-n:n; % indice facette
xi=dx*i; % abscisse centre facette i
thini=atan(xi/H); % angle entree CPC facette i
alphai=(thini+thtrans)/2; % angle d'inclinaison facette i
figure
hold on
plot(i,thini*180/pi)
plot(i,alphai*180/pi)
% largeur faisceau réflechi intercepté par l'horizontale facette i
li=(cos(alphai)+xi/H.*sin(alphai))*lm;
% en tenant compte en plus du diametre apparant du soleil
gamma=16/60*pi/180; %16 min d'angle
li2=li+2*H*tan(gamma)./(cos(thi).*(cos(thini).^2));
figure
hold on
plot(i,li)
plot(i,li2)
% ajouter les effets d'ombrage!!



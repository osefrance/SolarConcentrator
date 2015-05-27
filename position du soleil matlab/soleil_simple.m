% Calcul de la trajectoire du soleil et trace

%--- CHOISIR UNE DATE ---%
J=1; % jour 
M=7; % mois
n=jour(J,M); % calcul le numero du jour
% printemps equinoxe 21.03=80 
% ete solstice 21.06=172
% automne equinoxe 21.09=264 
% hiver solstice 21.12=355

%--- CHOISIR UN LIEU ---%
phi=47.6; % latitude terrestre du lieu

%--- COURSE DU SOLEIL ---%
% X positif vers le sud - Y positif vers l'est
t=(9:0.5:15); % heure solaire du lieu (1 pt par 1/2h)
stemp=soleil_xyz(n,t,phi);
Xsol=stemp(1,:);
Ysol=stemp(2,:);
Zsol=stemp(3,:);
figure
hold on
stem3(Xsol,Ysol,Zsol)

% % calculer les 4 dates importantes en même temps
% n=[80 172 264 355]; 
% Xsols=zeros(length(n),length(t));
% Ysols=zeros(length(n),length(t));
% Zsols=zeros(length(n),length(t));
% color=['r' 'b' 'g' 'k'];
% saison={'printemps' 'ete' 'automne' 'hiver'};
% figure
% for s=1:4
%     stemp=soleil_xyz(n(s),t,phi);
%     Xsols(s,:)=stemp(1,:);
%     Ysols(s,:)=stemp(2,:);
%     Zsols(s,:)=stemp(3,:);
%     hold on
%     stem3(Xsols(s,:),Ysols(s,:),Zsols(s,:),'DisplayName',saison{s},'Color',color(s))
% end
% xlabel('Nord-Sud');
% ylabel('Est-Ouest');
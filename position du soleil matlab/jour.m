function n=jour(J,M)

% nb de jours a ajouter pour chaque mois passe
mois=[0 31 59 90 120 151 181 212 243 273 304 334];
n=mois(M)+J;

end

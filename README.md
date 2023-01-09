# loup-garou-c
Loup garou de thiercelieux en C


README : 

Le loup de garou se lance  : 


Le principe du jeu consiste à trouver qui sont les imposteurs (loups garous) et les éliminer. Pour gagner les loups garous eux devront "tuer" les autres villageois.
De jour 1 personne est désigné par les membres du villages pour être éliminé. La nuit les loups garous désignent 1 personne à assassiner. 
Les loups garous sont de simples villageois le jour, ils disposent donc des mêmes accès et participent aux discussions comme les autres.

Les loups garous disposent d'un chat "privé" consultable et affichable la nuit uniquement.

Le maître du jeu (host) peut consulter l'état, le rôle, et le pseudo de tout les joueurs.

les villageois disposent d'un simple chat et dorment la nuit et ne sont donc pas témoins de ce qu'il s'y passe et s'y dit.

La répartition est de plus ou moins 1/3 de loup garou par partie.

Nous conseillons de lancer une partie à partir de 8 joueurs.

## Comment jouer au jeu

Il faut cloner ce repo dans un dossier sur votre PC
Il faut ensuite compiler le code à l'aide de make (en cas d'échec vérifier que les dossiers objects et executables sont bien existant, sinon vous pouvez les créer)

Pour créer une partie il suffit de lancer la commande ./executables/serveur.exe
Le programme affichera un numéro de partie qui servira au client pour se connecter

Pour rejoindre la partie, il faut executer la commande ./executables/client.exe
Il suffit ensuite d'entrer le numéro de partie et son nom de joueur.

Bon jeu !
TODO:
- Pour le moment seul le global state permet en etat inactif de ne pas prendre en compte les messages des modules, ils peuvent cependant au moment de l'activation etre en active_5 :(
- De l'interface il faudra donc initialiser les modules individuellement
- Notions de groupe, position dans l'espace pour le comportement de groupe.
- QUID de la déconnexion d'un module ?
-  

**Comportement individuels :**

PHASE 0:

Question macro:
    - Combien de couleur pour la fonction ?
    - Qui choisit la fonction ?
    
- Si INACTIVE -> ACTIVE_0
    - comportement fixe [array quand meme]
    - couleurs random dans une gamme fixée dependant de la phase
    - QUID du spot lumineux ?

- Si ACTIVE_0 -> INACTIVE
    - comportement fixe [array quand meme]
    - couleurs -> comportement de base 
    - QUID du spot lumineux ?
 ==> garder en memoire les couleurs de la phase 0
 
- Si ACTIVE_0 -> ACTIVE_5
    - comportement fixe [array quand meme]
    - couleurs random dans une gamme fixée dependant de la phase
- Si ACTIVE_5 -> ACTIVE_5
    - comportement aleatoire

- Si ACTIVE_5 -> INACTIVE
    - comportement fixe [array quand meme]
    - couleurs ?? inactivation totale ? comportement de base ?? 


Doit-on vérifier l'état d'avant ? oui pour ACTIVE_5 -> ACTIVE_5 et ACTIVE_0 -> ACTIVE_5 
Doit-on vérifier le temps ? Nope 

- Changement de phase:
    > PHASE 0 -> PHASE 1
    - regarder dans la BDD si + de n modules connectés (% si certains modules perdent la connexion ?) sont en status ACTIVE_5
    - Si OK:
       - passage phase 2 dans le global state
       - Comportement de transition PUIS comportement de phase 2 -> comment envoyer deux signaux a la suite ?
       - meme logique que pour la phase 0
    > PHASE 1 -> PHASE 2 
    - regarder dans la BDD si 100% des  modules connectés sont en ACTIVE_5
    - meme problematique que pour la transition 0 -> 1 
    

PHASE 2:  
    - Comportements globaux : 
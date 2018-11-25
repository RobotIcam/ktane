# ktane
## Explication du code du module Morse

Le code du module morse fonctionne sur deux fonctions intriquées. La première fonction, LightMorse, actionne la diodes de manière à 
retranscrire le mot en code Morse. Durant les temps d'attente (entre deux actionnement de diodes), la fonction en appelle une deuxième 
(PotentiometerReader) durant un temps donné, qui va vérifier en premier lieu si le bouton est appuyé, auquel cas il va vérifier la valeur 
du potentiomètre. Si le potentiomètre correspond au mot, le programme allume la diode verte (fonction success)

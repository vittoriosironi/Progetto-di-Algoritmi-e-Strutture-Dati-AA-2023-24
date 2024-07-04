## Lunghezza stringhe
Le stringhe sono lunghe fino a 255 caratteri -> in realtà si può fare molto di meno
# Gli ordini vengono smaltiti per ordine di preparazione o di istante di tempo in cui ordinati?


# Tabella HAS - consigli
La dimensione ottimale di una hash table è un parametro critico che influisce direttamente sulle prestazioni dell'operazione di inserimento, ricerca e cancellazione. Per hash table non ad indirizzamento aperto (quindi che utilizzano tecniche come chaining), la dimensione ottimale dipende da vari fattori tra cui il numero atteso di chiavi (n), il carico massimo desiderato (\(\alpha\)), e la funzione di hash utilizzata.

### Passaggi per determinare la dimensione ottimale:

1. **Stima del numero di chiavi (n):**
   - È importante avere una stima del numero massimo di chiavi che verranno inserite nella tabella hash. Anche se le chiavi sono "possibilmente infinite", puoi usare una stima pratica basata sul caso d'uso specifico.

2. **Scelta del carico massimo (\(\alpha\)):**
   - Il carico (\(\alpha\)) è definito come \( \alpha = \frac{n}{m} \), dove \( n \) è il numero di chiavi e \( m \) è la dimensione della tabella hash.
   - Per hash table con chaining, un carico compreso tra 0.5 e 1.0 è generalmente considerato efficiente. Un valore troppo alto di \(\alpha\) aumenterà la lunghezza media delle catene, peggiorando le prestazioni, mentre un valore troppo basso sprecherà spazio.

3. **Calcolo della dimensione della tabella hash (m):**
   - Utilizza la formula \( m = \frac{n}{\alpha} \) per determinare la dimensione della tabella hash.
   - Scegli \( m \) come un numero primo per ridurre la probabilità di collisioni e migliorare la distribuzione delle chiavi.

### Esempio pratico:

Supponiamo che ti aspetti di avere circa 10.000 chiavi e desideri mantenere un carico massimo di 0.75.

1. Numero atteso di chiavi (\( n \)) = 10.000
2. Carico massimo desiderato (\( \alpha \)) = 0.75
3. Calcola la dimensione della tabella hash (\( m \)):
   \[
   m = \frac{n}{\alpha} = \frac{10.000}{0.75} \approx 13.333
   \]
4. Scegli un numero primo vicino a 13.333, ad esempio 13.337.

Quindi, la dimensione ottimale della tua hash table dovrebbe essere 13.337.

### Consigli pratici:
- **Monitoraggio e ridimensionamento dinamico:** È utile implementare una strategia per monitorare il carico della tabella hash e ridimensionarla dinamicamente (es. raddoppiare la dimensione e reinserire le chiavi) quando il carico supera una certa soglia.
- **Funzione di hash efficiente:** Assicurati di utilizzare una buona funzione di hash per garantire una distribuzione uniforme delle chiavi e minimizzare le collisioni.

Seguendo questi passaggi, puoi determinare una dimensione ottimale per la tua hash table in modo da garantire prestazioni efficienti.
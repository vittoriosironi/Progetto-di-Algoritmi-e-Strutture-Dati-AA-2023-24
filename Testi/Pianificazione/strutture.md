## Ingrediente
Rappresentano quelli necessari per le ricette (NON I LOTTI)
- Nome
- Quantità
- Puntatore-Ricetta

## Ricetta
- Nome (UNIVOCO PER LA PASTICCERIA, se viene inserita una ricetta con lo stesso nome viene ignorato il comando)
- Ingredienti utilizzati (puntatori ad ingredienti)
- Peso (numero intero, indica i grammi)

## Magazzino
Caratterizzato da n lotti (lotti[n]) di ingredienti che riforniscono il magazzino secondo una pianificazione stabilita dal fornitore.
### Lotto
- Nome-Ingrediente (UNIVOCO PER IL MAGAZZINO) \
Sottostruttura, ordinata per data di scadenza crescente (lista)
    - Quantità
    - Data di scadenza (indicata dal numero dell'istante di tempo a partire dal quale il lotto è scaduto)

## Ordine PREPARATI
Per preparare gli ordini, gli ingredienti necessari vengono prelevati dal magazzino privilegiando sempre i lotti con la scadenza più prossima. Ad ogni rifornimento la pasticceria valuta se è possibile, con gli ingredienti ricevuti, preparare ordini attualmente in attesa. Se sì li prepara in questo istante di tempo. Gli ordini in attesa vengono smaltiti in ordine cronologico di arrivo dell'ordine
- Puntatore-Ricetta (se rimuovo la ricetta ma ci sono ordini ancora da spedire, allora la ricetta non viene spedita, potremmo anche tenere semplicemente il nome tanto ho l'hash table)
- Numero elementi ordinati
- Istante di tempo ordine
- Istante di tempo preparazione

## Ordini IN ATTESA
Creo una coda parte che mi segnala solo gli ordini in attesa
- Puntatore-Ricetta (se rimuovo la ricetta ma ci sono ordini ancora da spedire, allora la ricetta non viene spedita, potremmo anche tenere semplicemente il nome tanto ho l'hash table)
- Numero elementi ordinati
- Istante di tempo ordine
- Istante di tempo preparazione


# Idee - Miscellanous
Noto già che sarà necessario accedere continuamente alla lista delle ricetta, quindi per trovare la ricett più velocemente possiamo usare una `hash table`. \
Anche per i lotti per quando faccio i rifornimenti uso una `hash table` e per ogni posizione di questa c'è una `lista figlia` che ha i lotti in ordine di data di scadenza. \
Per gli ordini possiamo usare una `coda` così da tenerli in ordine cronologico.
Una volta capito quali sono quelli da evadere, di ordini, uso un algoritmo di ordinamento per ordinarli a seconda del peso in un'altra struttura dati tipo un'`array` che contiene i puntatori alla coda. \
Per gli ordini in attesa uso una `coda`, quando l'ordine potrà essere preparato viene letto e spostato nella coda degli ordini preparati. Mi salvo il puntatore del primo ordine che rimane in attesa così poi la coda la riporto lì
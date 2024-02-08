1. **Was ist Image Light?**
   
   ImageLight ist ein Tool dass es ermöglicht die Leuchdichte aus Bildern einer kalibrierten Kamera zu extrahieren und diese in einem Falschfarbenbild darzustellen.
   Imagelight verwendet für die approximation ein künstliches neuronales Netz, dass mithilfe von RGB-Werten die Leuchtdichte berrechnet.
2. **Verwendung**

   Das Programm lässt sich per batch Skript starten, dieses startet einen Server der das KNN dass die Bilder approxiemiert. Der Nutzer muss in dem CMD fenster zuerst die Adresse des
   Servers eingeben (Bei lokaler Nutzung muss die Addresse "127.0.0.1" verwendet werden, diese wird automatisch verwendet wenn Sie anfangs mit "y" Antworten). Das Skript startet neben dem Server auch ein Client.
   In diesen Client kann per Drag & Drop das zu konvertierende Bild gezogen werden, hierbei ist allerdings wichtig dass dieses Abmessungen von 2352 x 1568 Pixel hat. Unter dem Bild sind 2 Reiter, der zweite zeigt das Falschfarbenbild.
   In dem Bild lassen sich Fenster aufziehen, neben diesen Fenstern lässt sich eine ID und die durchschnittliche Leuchtdichte im Fenster Ablesen. Die Genauigkeit dieser Approximation ist direkt abhänigh von 
   der Menge, Qualität und Diversität der Kallibrierungsdaten.
3. **Benötigte Ausstattung**

   - Kamera mit Festbrennweiten Objektiv optimalerweise Fisheye
   - Stativ
   - Raum der abgedunkelt werden kann um Messfehler durch sich änderen Sonnenstand auszuschließen
   - Beamer oder Farbkarte   
5. **Kallibrierung**

   Für die Kallibrierung ist es wichtig möglichst viele Tupel von RGB Werten und der dazugehörigen Leuchtdichte zu sammeln. Die RGB-Werte müssen in dezimalschreibweise und im Linear RGB Colorspace sein.
   ImageLight kann diese Daten liefern indem Sie in den Einstellungen den "Calibration mode" aktivieren. In diesem Modus sendet das Programm keine Daten an das KNN. Nun müssen Sie erstmal jede menge Daten sammeln.
   Dies lässt sich am besten in einem Raum mit Beamer machen indem Sie volle Kontrolle über die Beleuchtung haben. Auf dem Beamer können Sie nun mehrfarbige Testbilder anzeigen. Stellen Sie ein Stativ
   rechttwinkig zu Leinwand auf. Auf diesem Staiv montieren Sie nun abwechseln die Kamera und ein Leuchtdichte Messgerät. Mit der Kamera machen Sie nun ein Bild, wichtig dabei ist dass sie dieses im Manuell Modus machen.
   (Stellen Sie alle Korrekturfunktionen wie z.B. Auto-Iso, Auto-Weißabgleich, Automatische-BeleuchtungsKorrektur aus. Die einzigen Korrekturen die Aktiviert bleiben dürfen ist der Auto-Focus und eine Vinietten Korektur. Stellen Sie den ISO Wert
   , die Blende und die Belichtungszeit auf Konstante Werte. Die Werte müssen so gewählt werden, dass das Bild im Relevanten Messberreich weder Unter- noch Überbelichtet ist.)
   Nun Tauschen sie die Kamera gegen ein Lechtdichtemessgerät aus. Bewegen Sie das Stativ nicht. Verstellen die die Höhe so dass die Linse des Messgeräts auf der selben Höhe ist wie die derr Kamera. Es ist Wichtig dass die Bilder aus dem selben Winkel gemacht werden,
    da dies die Leuchtdichte Werte drastisch verändern kann. Machen sie nun Messungen mit dem Lechtdichtemessgerät an markanten Stellen im Bild. Notieren sie sich die Messwerte anschließen so, dass Sie diese anschließend wieder den Punkten zuordnen können.
   Laden Sie nun die eben gemachten Bilder auf ihren Pc. Öffnen sie die Bilder in ImageLight im Kallibrierungsmodus. Ziehen Sie nun Messboxen über die Punkte im Bild zu denen Sie zuvor die Messdaten mit dem Candelamessgerät aufgenommen haben.
   Exportieren Sie nun die Messpunkte als CSV [Dartei -> Export as CSV]. Ordnen Sie nun den Messpunkten die Candela Werte zu und Speichern sie die Dartei sicher. Wiederholen Sie den Prozess bis Sie mindestens 100 Einträge haben, varieren sie Dabei die Helligkeit des Beamers und die Helligkeit der Raumbeleuchtung.
   Die Menge der benötigten Datensäze hängt stark von Ihrem Anwendungsfall ab. Für uns waren Messungen Zwischen 50 und 1300 Candela wichtig, die ersten zufiedenstellenden Ergebnisse haben wir ab 280 Einträgen erhalten.
   Wichtig ist dass bei den Messungen möglichts alle Farben die anschließend Gemessen werden sollen im Tainingsdatenset sein sollten. Und dies in meheren Helligkeiten. Die Farben sollten gleichmäßig verteilt sein, da es sonst zu einem Bais kommen kann.
   Kopieren Sie nun alle ihre Tainings-Datensätze in die Dartei "data.xlsx". Löschen Sie allerdings vorher die Beispieldaten. Starten sie nun das Batch Skript "train.bat". Das Netzwerk sollte nun mit dem Training beginnen.
   Das Training kann je nach Menge der Daten 5-20min Dauern. Nach dem Ende des Trainings werden ihnen Performace Parameter zu dem Netzwerk gezeigt. Je nach Qualiät und Quantität der Daten variiert die Performace stark.
   Wenn sich Ihr Datensatz zu unserem in in diesen Beiden Parametern zu unserem stark unterschiedet kann es sein dass Sie die Hyperparameter des KNN an ihren Datensatz anpassen müssen. Diese könne sie in der "main.py" Datei ändern.
   Besonders Interessant währen hierbei für Sie vermutlich die Werte im LearnRateScheduler, wenn ihr trainig sofort bendet wird und eine schlechte Performance aufweißt dann sollten Sie kleinere Zahlen wählen, wenn das Training sehr lange dauert dann können Sie größere Zahlen wählen.
   

# El balance del confort y la eficiencia energética
## Listado de códigos utilizados y su función

### RecibeDisositivoIoT.py

Archivo receptor de los datos de temperatura y humedad enviados a través de un Broker MQTT. Su funcionalidad es que además de recibir, también manda y ejecutar determinadas instrucciones en una base de datos de MySQL, almacenando de igual forma los datos recibidos en un principio. Todo esto es realizado también por medio del Broker.

### CódigoBD.txt

Instrucciones utilizadas en MySQL para generar la base de datos utilizada, que llevará por nombre "iot".

### Reto_IoT_v2021.2.twb

Archivo generado en Tableau para la obtención de gráficos utilizados para la representación de los resultados obtenidos (promedios de temperatura y humedad en un cierto tiempo)

### Gráficas_IoT.pptx

Gráfico combinado obtenido a partir de los promedios de temperatura y humedad captados por el sensor. A pesar de realizar lo mismo que "Reto_IoT_v2021.2.twb", se permite tener una visualización sin necesidad de utilizar Tableau.


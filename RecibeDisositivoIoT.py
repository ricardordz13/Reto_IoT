import json
import paho.mqtt.client as mqtt
import random
import mysql.connector as mysql
import time
import datetime
from datetime import datetime

db = mysql.connect(
    host='localhost',
    user='root',
    passwd='',
    database = 'iot'
    )
mycursor = db.cursor(buffered=True)

mycursor.execute("USE iot")

global cont
cont = 0
global counter
counter = 0
global sumHum
sumHum = 0
global sumTemp
sumTemp = 0

# Función que calcula el promedio de los datos de humedad y temperatura
def prom(sumTemp, sumHum):
    promedioTemp = sumTemp / 10
    promedioHum = sumHum / 10
    # Fecha es hora
    # fecha = time.strftime("%X")
    fecha = (datetime.today().strftime('%H%M%S'))
    ahorro = 1000 - ((promedioTemp * 10) + (promedioHum * 10))
    sqls = f'INSERT INTO resultado (Fecha, PromedioTemp, PromedioHumedad, Tiempo, Ahorro) VALUES ("{fecha}","{promedioTemp}","{promedioHum}","60 s","{ahorro}")'
    print('Comando ejecutado: ',sqls)
    mycursor.execute(sqls)
    
    sqls = f'INSERT INTO genera (dispositivoID, Fecha) VALUES ("0123456789","{fecha}")'
    print('Comando ejecutado: ',sqls)
    mycursor.execute(sqls)
    
    db.commit()
    
# Callback Function on Connection with MQTT Server
# Función Callback que se ejecuta cuando se conectó con el servidor MQTT
def on_connect(client, userdata, flags, rc):
    print ("Connected with Code :" +str(rc))
    client.subscribe("A01284398")

def on_message(client, userdata, msg):
    global cont
    global counter
    global sumTemp
    global sumHum
    # print the message received from the subscribed topic
    topic = msg.topic
    m_decode = str(msg.payload.decode("utf-8","ignore"))
    m_in = json.loads(m_decode)
    print()
    print('------------Llegada de mensaje en servidor-------')
    print('Tópico: ', topic)
    print(type(m_decode),' ' , m_decode)
    print(type(m_in), ' ' , m_in)
    print('Dispositivo: ', type(m_in['dispositivo']) ,' ',m_in['dispositivo'])
    print('       Tipo: ', type(m_in['tipo'])        ,' ',m_in['tipo'])
    print('       Dato: ', type(m_in['dato'])        ,' ',m_in['dato'])
    #print ("Recibido--->", str(msg.payload) )
    dispositivo = m_in['dispositivo']
    
    fecha = time.strftime("%x")
    hora = time.strftime("%X")
    tipo = m_in['tipo']
    dato = m_in['dato']
    
    if tipo == 'temperatura':
        sumTemp += dato
    else:
        sumHum += dato        
    
    if cont == 0:
        sqls = f'INSERT INTO dispositivo (dispositivoID, Domicilio, tipoSensor, tipoDispositivo) VALUES ("0123456789", "713", "TH", "S")'
        print('Comando ejecutado: ',sqls)
        mycursor.execute(sqls)
        db.commit()
        cont += 1

    sqls = f'INSERT INTO datalogging (Fecha, Hora, Tipo, Dato, dispositivoID) VALUES ("{fecha}","{hora}","{tipo}","{dato}","0123456789")'
    print('Comando ejecutado: ',sqls)
    mycursor.execute(sqls)
        
    db.commit()

    counter += 1
    if counter == 20:
        counter = 0
        prom(sumTemp, sumHum)
        sumTemp = 0
        sumHum = 0
        
# Generamos el cliente y las funciones para recibir mensajes y
# cuando se genera la conexión.
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
# Hacemos la conexión al Broker
client.connect("broker.mqtt-dashboard.com", port=1883)
client.loop_start()
time.sleep(10)
opc = ''
while opc == '' or True:
    #envia_dispositivo()
    print('Espera unos segundos...')
    time.sleep(10)
    # opc = input('Teclea enter para continuar, otro para salir...')
       
# al salir paramos el loop y nos desconectamos
client.loop_stop()
client.disconnect()
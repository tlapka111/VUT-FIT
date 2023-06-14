import sys
import socket
import json

#Globalní proměnné
###############################################################

PORT = 80
SERVER = 'api.openweathermap.org'
API_KEY = sys.argv[1]
CITY = sys.argv[2]
CITY = CITY.lower()
ID = sys.argv[3]
LAT = sys.argv[4]
LON = sys.argv[5]

###############################################################

#funkce vytvoří socket, naváže spojení s cilovým serverem, odešle požadavek
#GET a přijme odpověd od serveru
def data_from_socket(id, gps):
    try:
        tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except OSError:
        print ("Error occurred during creating socket.")
        exit(1)
    try:
        tcp_socket.connect((SERVER, PORT))
        if id == True:
            tcp_socket.sendall(str.encode("GET /data/2.5/weather?id=" + ID + "&APPID=" + API_KEY +  "&units=metric HTTP/1.0\r\n\r\n"))
        elif gps == True:
            tcp_socket.sendall(str.encode("GET /data/2.5/weather?lat=" + LAT + "&lon=" + LON + "&APPID=" + API_KEY + "&units=metric HTTP/1.0\r\n\r\n"))
        else:
            tcp_socket.sendall(str.encode("GET /data/2.5/weather?q=" + CITY + "&APPID=" + API_KEY + "&units=metric HTTP/1.0\r\n\r\n"))
        raw_data = tcp_socket.recv(4096)
        return raw_data
    except OSError:
        print ("Error occurred during communication with the server.")
        exit(1)
    finally:
        tcp_socket.close()
        
#zpracovaní příchozých dat ve formátu JSON a uložení do slovníku
def json_parser(weather_data):
    weather_json = json.loads(weather_data)
    json_data = dict(
        city = weather_json.get('name'),
        description = weather_json.get('weather')[0].get('description'),
        temperature = weather_json.get('main').get('temp'),
        humidity = weather_json.get('main').get('humidity'),
        pressure = weather_json.get('main').get('pressure'),
        wind_speed = (round((weather_json.get('wind').get('speed')*3.6), 2)),
        wind_degree = weather_json.get('wind').get('deg')
    )
    return json_data

#výpis uložených dat o počasí na STDOUT 
def json_output(json_data):
    print ('{}'.format(json_data['city']))
    print ('{}'.format(json_data['description']))
    print ('temp: {}°C'.format(json_data['temperature']))
    print ('humidity: {}%'.format(json_data['humidity']))
    print ('pressure: {}hPa'.format(json_data['pressure'])) 
    print ('wind-speed: {}km/h'.format(json_data['wind_speed']))
    print ('wind-deg: {}'.format(json_data['wind_degree']))

#zpracovaní vstupních argumentů
def args_parser():
    if len(sys.argv[1]) != 0 and len(sys.argv[2]) != 0 and len(sys.argv[3]) == 0 and len(sys.argv[4]) == 0 and len(sys.argv[5]) == 0:
        id_city = False
        gps = False
    elif len(sys.argv[1]) != 0 and len(sys.argv[2]) == 0 and len(sys.argv[3]) != 0 and len(sys.argv[4]) == 0 and len(sys.argv[5]) == 0:
        id_city = True
        gps = False
    elif len(sys.argv[1]) != 0 and len(sys.argv[2]) == 0 and len(sys.argv[3]) == 0 and len(sys.argv[4]) != 0 and len(sys.argv[5]) != 0:
        id_city = False
        gps = True
    else:
        print ("Bad arguments given.")
        exit(1)
    return id_city, gps


def main():
    args = args_parser()
    data = data_from_socket(args[0],args[1]).decode('utf-8')
    http_header = data.split("\r\n\r\n")[0] #http hlavička
    http_code = http_header.split('\n',1)[0] # první řádek http hlavičky s návratovým kódem
    weather_data = data.split("\r\n\r\n")[1] #json data
    if "HTTP/1.1 200 OK" in http_code :
        json_output(json_parser(weather_data))   
    else:
        print (http_code) #vytiskne chybovou hlášku z http hlavičky
        
main()
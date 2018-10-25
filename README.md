# ESP32-WeatherStation


# -> Create AP on Manjaro Linux:

## Cria a interface com addr físico <addr> parecido com o do atual WiFi
###### "sudo iw dev wlp3s0 interface add wlp3s0_ap type managed addr 5c:c9:d3:92:56:fc"
## Ativa a interface. Se buscar em ifconfig aparece
###### "sudo ifconfig wlp3s0_ap up"
## create_ap <interface_new> <wifi_interface> <ssid> <password>
###### "sudo create_ap wlp3s0_ap wlp3s0 apbeck 12345679"


# -> Ao desconectar e conectar de novo, caso não der certo:

## Desativa a interface. Se buscar em ifconfig não aparece mais
###### "sudo ifconfig wlp3s0_ap down"
## Ativa a interface. Se buscar em ifconfig aparece
###### "sudo ifconfig wlp3s0_ap up"
## create_ap <interface_new> <wifi_interface> <ssid> <password>
###### "sudo create_ap wlp3s0_ap wlp3s0 apbeck 12345679"

# esp8266-2wc

Create AP on Manjaro Linux:

<!-- testes -->
# "sudo iw dev wlp3s0 interface add wlp3s0_ap type managed addr 5c:c9:d3:92:56:fc"	->	cria a interface com addr físico <addr>
# "sudo ifconfig wlp3s0_ap up"	->	Ativa a interface. Se buscar em ifconfig aparece
# "sudo create_ap wlp3s0_ap wlp3s0 apbeck 12345679"	->	create_ap <interface_new> <wifi_interface> <ssid> <password>


Ao desconectar e conectar de novo, caso não der certo:

# "sudo ifconfig wlp3s0_ap down"	->	Desativa a interface. Se buscar em ifconfig não aparece mais
# "sudo ifconfig wlp3s0_ap up"	->	Ativa a interface. Se buscar em ifconfig aparece
# "sudo create_ap wlp3s0_ap wlp3s0 apbeck 12345679"	->	create_ap <interface_new> <wifi_interface> <ssid> <password>

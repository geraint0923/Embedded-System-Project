iptables -I FORWARD -o br-lan -j ACCEPT
iptables -I FORWARD -o tun0 -j ACCEPT
iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -j MASQUERADE

#./route_init.sh

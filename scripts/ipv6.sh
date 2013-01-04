ip tunnel add sit1 mode sit remote 59.66.4.50 local 59.66.133.36
ifconfig sit1 up
ifconfig sit1 add 2001:da8:200:900e:0:5efe:59.66.133.36/64
ip route add 2001::/3 via 2001:da8:200:900e::1 metric 1
route add -net 59.66.0.0/16 gw 59.66.133.1
route add -net 166.111.0.0/16 gw 59.66.133.1
#openvpn --config /etc/openvpn/gerainty/geraint-oy6.ovpn

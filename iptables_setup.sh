sudo iptables -N TRAFFIC_ACC_IN
sudo iptables -N TRAFFIC_ACC_OUT
sudo iptables -I INPUT -j TRAFFIC_ACC_IN
sudo iptables -I OUTPUT -j TRAFFIC_ACC_OUT
sudo iptables -A TRAFFIC_ACC_IN -p tcp
sudo iptables -A TRAFFIC_ACC_IN -p udp
sudo iptables -A TRAFFIC_ACC_IN -p icmp
sudo iptables -A TRAFFIC_ACC_OUT -p tcp
sudo iptables -A TRAFFIC_ACC_OUT -p udp
sudo iptables -A TRAFFIC_ACC_OUT -p icmp
